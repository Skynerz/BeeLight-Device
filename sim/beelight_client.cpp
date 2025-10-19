#include "beelight_client.hpp"
#include <qthread.h>

void BeelightClient::connectToDevice()
{
    qDebug() << "socket state " << QString::number(socket.state());
    if (connect_mtx.try_lock())
    {
        if (isConnected())
        {
            disconnect();
        }
        else
        {
            qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");

            emit connectStateChanged(CONNECTING);
            socket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
            socket.connectToHost(INET_ADDR, PORT);
            if (socket.waitForConnected())
            {
                qint64 bufferSize = socket.readBufferSize();
                if (bufferSize > sizeof(data))
                {
                    qDebug() << "Buffer size is " + QString::number(bufferSize);
                }
                if (hello())
                {
                    qDebug() << "Connected!";
                    emit connectStateChanged(CONNECTED);
                    rxEnabled = true;
                    rx_thread = std::thread(&BeelightClient::rxTask, this);
                }
                else
                {
                    qDebug() << "Connect error";
                    emit connectStateChanged(DISCONNECTED);
                }
            }
        }
        connect_mtx.unlock();
    }
    else
    {
        qDebug() << "Connection in progress";
    }
}

void BeelightClient::disconnect()
{
    qDebug() << "Disconnecting";
    CmdFrame closeFrame{
        .type = 1,
        .cmd = CMD_CLOSE,
        .len = 0,
    };
    //close RX first !
    rxEnabled = false;
    rx_thread.join();
    sendCommand(closeFrame);
    socket.disconnectFromHost();
    emit connectStateChanged(DISCONNECTED);
}

bool BeelightClient::hello()
{
    CmdFrame helloFrame{
        .type = 1,
        .cmd = CMD_HELLO,
        .len = 5,
    };
    sprintf(helloFrame.data.write.varName, "Hello", 5);
    sendCommand(helloFrame);
    if (!socket.waitForReadyRead(5000))
    {
        qDebug() << "No response from device";
        return false;
    }
    qint64 len = socket.read(data, sizeof(CmdFrame));
    CmdFrame* frame = (CmdFrame *)data;
    QString tok(frame->data.read.varName);
    qDebug() << "Hello response: " + tok;   
    if ((frame->type == 0) && (frame->cmd == CMD_HELLO) && (frame->status == 1) && tok.startsWith("World"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void BeelightClient::sendCommand(const CmdFrame &frame)
{
    socket.write((const char *)&frame, frame.len + 4); // tx+cmd+len+status+data
}

bool BeelightClient::writeVar(const QString &varName, const QString& value)
{
    uint16_t len = value.length();
    if (!isConnected() || len > sizeof(CmdFrame::data.write.data))
    {
        return false;
    }
    CmdFrame frame{0};
    frame.type = 1;
    frame.cmd = CMD_WRITE;
    frame.len = len + sizeof(CmdFrame::data.write.varName);
    
    strncpy(frame.data.write.varName, varName.toStdString().c_str(), sizeof(CmdFrame::data.write.varName));
    strncpy((char*) frame.data.write.data, value.toStdString().c_str(), sizeof(CmdFrame::data.write.data));
    sendCommand(frame);

    CmdFrame response{};
    getAnswer(CMD_WRITE, response);
    if (response.status == 1 && response.cmd == CMD_WRITE && response.type == 0)
    {
        return true;
    }
    return false;
}

bool BeelightClient::readVar(const QString &varName, QString& value)
{
    CmdFrame frame{};
    frame.type = 1;
    frame.cmd = CMD_READ;
    frame.len = varName.length();
    strncpy(frame.data.read.varName, varName.toStdString().c_str(), sizeof(CmdFrame::data.read.varName));
    sendCommand(frame);

    CmdFrame response{};
    getAnswer(CMD_READ, response);
    if(response.status == 1 && response.cmd == CMD_READ && response.type != 0)
    {
        value = QString::fromUtf8((char*)response.data.data, response.len);
        return true;
    }

    return false;
}

bool BeelightClient::getAnswer(Command cmd, CmdFrame &frame)
{
    int8_t retries = 5;
    bool found = false;
    do
    {
        for (auto it = rxQueue.begin(); it != rxQueue.end(); ++it)
        {
            if (it->second.cmd == cmd)
            {
                frame = it->second;
                rxQueue.erase(it);
                found = true;
                break;
            }
        }
        if (!found)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } while (retries-- > 0 && !found);
    return found;
}

void BeelightClient::rxTask()
{
    while (rxEnabled)
    {
        if (socket.waitForReadyRead(1000))
        {
            qint64 n = socket.read(data, sizeof(data));
            if (n > 0)
            {
                // Process incoming data
                CmdFrame *frame = (CmdFrame *)data;
                if (frame->type == 0)
                { // response
                    rxQueue.emplace_back(RxEntry(std::chrono::system_clock::now(), *frame));
                }
            }
        }
        if (!rxQueue.empty())
        {
            for (auto it = rxQueue.begin(); it != rxQueue.end();)
            {
                auto now = std::chrono::system_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->first);
                if (duration.count() > 10)
                { // timeout 10 seconds
                    it = rxQueue.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }
}
