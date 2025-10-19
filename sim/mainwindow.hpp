#include "ui_mainwindow.h"
#include <QObject>
#include <QThread>
#include <memory>
#include "beelight_client.hpp"

class CurrentTimeUpdater : public QThread
{
    Q_OBJECT
public:
    explicit CurrentTimeUpdater(QObject *parent = nullptr) : QThread(parent) { }
public:
    void run();
signals:
     void currentTimeUpdated(const QString &currentTime);
};

class MainWindow : public QObject, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QObject* parent = nullptr);

public slots:
    void updateConnectButtonState(BeelightClient::ConnectState state);
    void updateCurrentTime(const QString &currentTime);
    void onWriteCurrentTimeClicked();
    void onWriteEDAClicked();
    void onReadEDAClicked();
    void onWriteETAClicked();
    void onReadETAClicked();
    void onWriteArrivingTimeClicked();
    void onReadArrivingTimeClicked();
    void onWriteNextInstructionClicked();
    void onReadNextInstructionlicked();
    void onWriteRemainingDistanceBeforeNextInstructionClicked();
    void onReadRemainingDistanceBeforeNextInstructionlicked();
private:
    std::shared_ptr<QMainWindow> w;
    BeelightClient client{this};
    CurrentTimeUpdater timeUpdater_m{this};
};
