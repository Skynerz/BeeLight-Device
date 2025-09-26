#include "beelight_client.hpp"

#define SERVER "localhost"

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
            emit connectStateChanged(CONNECTING);
            socket.connectToHost(SERVER, PORT);
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
                    rx_thread = std::thread(&BeelightClient::rxTask, this);
                }
                else {
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
    socket.disconnectFromHost();
    rxEnabled = false;
    rx_thread.join();
    emit connectStateChanged(DISCONNECTED);
}

bool BeelightClient::hello()
{
    socket.write("Hello");
    if(!socket.waitForReadyRead(5000)) {
        qDebug() << "No response from device";
        return false;
    }
    socket.read(data, sizeof(data));
    if(QString(data).startsWith("World")) {
        return true;
    }
    else {
        return false;
    }
}

void BeelightClient::sendCommand(const CmdFrame &frame)
{
    socket.write((const char *)&frame, frame.len + 4); // tx+cmd+len+status+data
}

bool BeelightClient::writeVar(const QString& varName, const uint8_t *data, uint16_t len)
{
    if (!isConnected() || len > sizeof(CmdFrame::data.write.data))
    {
        return false;
    }
    CmdFrame frame{};
    frame.tx = 1;
    frame.cmd = CMD_WRITE;
    frame.len = len;
    strncpy(frame.data.write.varName, varName.toStdString().c_str(), sizeof(CmdFrame::data.write.varName));
    memcpy(frame.data.write.data, data, len);
    sendCommand(frame);

    CmdFrame response{};
    getAnswer(CMD_WRITE, response);
    return true;
}

bool BeelightClient::readVar(const QString& varName, uint8_t* data, uint16_t& len) {
    CmdFrame frame{};
    frame.tx = 1;
    frame.cmd = CMD_READ;
    frame.len = 0;
    strncpy(frame.data.read.varName, varName.toStdString().c_str(), sizeof(CmdFrame::data.read.varName));
    sendCommand(frame);
    // if (!socket.waitForReadyRead(5000)) {
    //     qDebug() << "No response from device";
    //     return false;
    // }
    // qint64 n = socket.read((char*)&frame, sizeof(frame));
    // if (n < 3) {
    //     qDebug() << "Invalid response from device";
    //     return false;
    // }
    // if (frame.tx != 0 || frame.cmd != CMD_READ) {
    //     qDebug() << "Invalid response from device";
    //     return false;
    // }
    // if (frame.len > sizeof(frame.data)) {
    //     qDebug() << "Invalid response length from device";
    //     return false;
    // }
    // if (len < frame.len) {
    //     qDebug() << "Buffer too small";
    //     return false;
    // }
    // memcpy(data, frame.data.data, frame.len);
    // len = frame.len;
    return true;
}

bool BeelightClient::getAnswer(Command cmd, CmdFrame &frame) {
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
        if(!found) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } while (retries-- > 0 && !found);
    return found;
}

void BeelightClient::rxTask() {
    while(rxEnabled) {
        if (socket.waitForReadyRead(1000)) {
            qint64 n = socket.read(data, sizeof(data));
            if (n > 0) {
                // Process incoming data
                CmdFrame* frame = (CmdFrame*)data;
                if (frame->tx == 0) { // response
                    rxQueue.emplace_back(RxEntry(std::chrono::system_clock::now(), *frame));
                }
            }
        }
        for(auto it = rxQueue.begin(); it != rxQueue.end(); ) {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - it->first);
            if (duration.count() > 10) { // timeout 10 seconds
                it = rxQueue.erase(it);
            }
            else {
                ++it;
            }
        }
    }
}