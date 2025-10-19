#include "mainwindow.hpp"
#include <QDebug>
#include <QIntValidator>

MainWindow::MainWindow(QObject* parent) : QObject(parent)
{
    w = std::make_shared<QMainWindow>();
    setupUi(w.get());
    w->show();

    connect(quitButton, &QPushButton::clicked, [this]() {
        qDebug() << "Bye!";
        w->close();
    });

    connect(connectButton, &QPushButton::clicked, &client, &BeelightClient::connectToDevice);
    connect(&client, &BeelightClient::connectStateChanged, this, &MainWindow::updateConnectButtonState);
    remaining_distance->setValidator(new QIntValidator(0, 100000, this));
    connect(w_remaining_distance_button, &QPushButton::clicked, this, &MainWindow::onWriteRemainingDistanceClicked);
}

void MainWindow::onWriteRemainingDistanceClicked() {
    QString value = remaining_distance->text();
    unsigned int distance = value.toUInt();
    client.writeVar("remaining_distance", (uint8_t*)&distance, sizeof(distance));
}

void MainWindow::updateConnectButtonState(BeelightClient::ConnectState state)
{
    switch (state)
    {
    case BeelightClient::DISCONNECTED:
        connectButton->setText("Connect");
        break;
    case BeelightClient::CONNECTING:
        connectButton->setText("Connecting...");
        break;
    case BeelightClient::CONNECTED:
        connectButton->setText("Disconnect");
        break;
    }
}
