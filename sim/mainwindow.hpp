#include "ui_mainwindow.h"
#include <QObject>
#include <memory>
#include "beelight_client.hpp"

class MainWindow : public QObject, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(QObject* parent = nullptr);

public slots:
    void updateConnectButtonState(BeelightClient::ConnectState state);
    void onWriteRemainingDistanceClicked();
private:
    std::shared_ptr<QMainWindow> w;
    BeelightClient client{this};
};
