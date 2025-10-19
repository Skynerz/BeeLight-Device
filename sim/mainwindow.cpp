#include "mainwindow.hpp"
#include <QDebug>
#include <QIntValidator>
#include <QTime>

MainWindow::MainWindow(QObject *parent) : QObject(parent)
{
    w = std::make_shared<QMainWindow>();
    setupUi(w.get());
    w->show();

    connect(quitButton, &QPushButton::clicked, [this]()
            {
        qDebug() << "Bye!";
        w->close(); });

    connect(&client, &BeelightClient::connectStateChanged, this, &MainWindow::updateConnectButtonState);
    connect(connectButton, &QPushButton::clicked, &client, &BeelightClient::connectToDevice);
    connect(w_currentTime_button, &QPushButton::clicked, this, &MainWindow::onWriteCurrentTimeClicked);
    eda->setValidator(new QIntValidator(0, 100000, this));
    connect(w_eda_button, &QPushButton::clicked, this, &MainWindow::onWriteEDAClicked);
    connect(r_eta_button, &QPushButton::clicked, this, &MainWindow::onReadEDAClicked);
    eta->setValidator(new QIntValidator(0, 100000, this));
    connect(w_eta_button, &QPushButton::clicked, this, &MainWindow::onWriteETAClicked);
    connect(r_eta_button, &QPushButton::clicked, this, &MainWindow::onReadETAClicked);
    connect(w_atime_button, &QPushButton::clicked, this, &MainWindow::onWriteArrivingTimeClicked);
    connect(r_atime_button, &QPushButton::clicked, this, &MainWindow::onReadArrivingTimeClicked);
    nextInstruction->setValidator(new QIntValidator(0, 100000, this));
    connect(w_nextInstruction_button, &QPushButton::clicked, this, &MainWindow::onWriteNextInstructionClicked);
    connect(r_nextInstruction_button, &QPushButton::clicked, this, &MainWindow::onReadNextInstructionlicked);
    rdbni->setValidator(new QIntValidator(0, 100000, this));
    connect(w_rdbni_button, &QPushButton::clicked, this, &MainWindow::onWriteRemainingDistanceBeforeNextInstructionClicked);
    connect(r_rdbni_button, &QPushButton::clicked, this, &MainWindow::onReadRemainingDistanceBeforeNextInstructionlicked);

    connect(&timeUpdater_m, &CurrentTimeUpdater::currentTimeUpdated, this, &MainWindow::updateCurrentTime);
    timeUpdater_m.start();
}

void MainWindow::updateCurrentTime(const QString &currentTime)
{
    if (currentTimeLabel->text().contains(":"))
    {
        QString tmp(currentTime);
        tmp = tmp.replace(":", " ");
        currentTimeLabel->setText(tmp);
    }
    else
    {
        currentTimeLabel->setText(currentTime);
    }
}

void MainWindow::onWriteCurrentTimeClicked()
{
    QString value = currentTimeLabel->text();
    value = value.replace(" ", ":");
    client.writeVar("CurrentTime", value);
}

void MainWindow::onWriteEDAClicked()
{
    QString value = eda->text();
    client.writeVar("EstDistanceBeforeArrival", value + " m");
}

void MainWindow::onReadEDAClicked()
{
    QString value;
    client.readVar("EstDistanceBeforeArrival", value);
    eda->setText(value);
}

void MainWindow::onWriteETAClicked()
{
    QString value = eta->text();
    client.writeVar("EstTimeBeforeArrival", value + " s");
}

void MainWindow::onReadETAClicked()
{
    QString value;
    client.readVar("EstTimeBeforeArrival", value);
    eda->setText(value);
}

void MainWindow::onWriteArrivingTimeClicked()
{
    QString value = arrivingTime->text();
    client.writeVar("EstTArrivingTimeBeforeArrival", value);
}

void MainWindow::onReadArrivingTimeClicked()
{
    QString value;
    client.readVar("EstTArrivingTimeBeforeArrival", value);
    arrivingTime->setText(value);
}

void MainWindow::onWriteNextInstructionClicked()
{
    QString value = nextInstruction->text();
    client.writeVar("NextInstruction", value);
}

void MainWindow::onReadNextInstructionlicked()
{
    QString value;
    client.readVar("NextInstruction", value);
    nextInstruction->setText(value);
}

void MainWindow::onWriteRemainingDistanceBeforeNextInstructionClicked()
{
    QString value = rdbni->text();
    client.writeVar("RemainingDistanceBeforeNextInstruction", value);
}

void MainWindow::onReadRemainingDistanceBeforeNextInstructionlicked()
{
    QString value;
    client.readVar("RemainingDistanceBeforeNextInstruction", value);
    rdbni->setText(value);
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

void CurrentTimeUpdater::run()
{
    while (1)
    {
        emit currentTimeUpdated(QTime::currentTime().toString("hh:mm"));
        sleep(1);
    }
}
