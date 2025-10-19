#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <mutex>
#include <thread>
#include "Frame.hpp"

class BeelightClient : public QObject
{
    Q_OBJECT
public:
    BeelightClient(QObject* parent = nullptr) : QObject(nullptr) {

    }
    ~BeelightClient() = default;

    void connectToDevice();
    void disconnect();
    bool isConnected() const {
        return socket.state() == QAbstractSocket::ConnectedState;
    }
    bool hello();
    bool writeVar(const QString& varName, const QString& value);
    bool readVar(const QString& varName, QString& value);

    enum ConnectState {
        DISCONNECTED,
        CONNECTING,
        CONNECTED
    };

signals:
    void connectStateChanged(ConnectState connectState);

private:
    std::mutex connect_mtx;
    std::thread rx_thread;
    QTcpSocket socket{this};
    char data[512];
    void sendCommand(const CmdFrame &frame);
    bool getAnswer(Command cmd, CmdFrame &frame);
    void rxTask();
    bool rxEnabled{true};

    using RxEntry = std::pair<std::chrono::time_point<std::chrono::system_clock>, CmdFrame>; //a optimiser
    std::vector<RxEntry> rxQueue;
};

#endif // CLIENT_HPP
