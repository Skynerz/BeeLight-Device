#include "BeelightCom_sim.hpp"
#include "model/NavigationModel.hpp"
#include <ctime>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <poll.h>

void BeelightCom_sim::init()
{
    logger_m.info("BeelightCom_sim init");

    initReadCommand();
    initWriteCommand();
    serverInit();
    setSimulation(true);

    // todo bind ?
    timer_m = lv_timer_create(&step, 1000, this);

    // first call
    step(timer_m);
}

void BeelightCom_sim::uninit()
{
    logger_m.info("BeelightCom_sim uninit");
    lv_timer_delete(timer_m);
    close(socket_m);
}

void BeelightCom_sim::serverInit()
{
    socket_m = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddress_m.sin_family = AF_INET;
    serverAddress_m.sin_port = htons(PORT);
    serverAddress_m.sin_addr.s_addr = inet_addr(INET_ADDR);

    if (bind(socket_m, (struct sockaddr *)&serverAddress_m,
             sizeof(serverAddress_m)))
    {
        logger_m.error("Bind error: " + std::string(strerror(errno)));
    }

    int reuse = 1;
    if (setsockopt(socket_m, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
    {
        logger_m.error("SO_REUSEADDR setsockopt error: " + std::string(strerror(errno)));
    }

    if (setsockopt(socket_m, SOL_SOCKET, SO_KEEPALIVE, &reuse, sizeof(reuse)) != 0)
    {
        logger_m.error("SO_KEEPALIVE setsockopt error: " + std::string(strerror(errno)));
    }

    if (listen(socket_m, 32) < 0)
    {
        logger_m.error("listen error: " + std::string(strerror(errno)));
        close(socket_m);
    }
}

void BeelightCom_sim::serverStep()
{
    CmdFrame pkt;
    struct timeval tv;
    fd_set rfds;
    int sockLen = sizeof(serverAddress_m);
    FD_ZERO(&rfds);
    FD_SET(socket_m, &rfds);
    FD_SET(STDIN_FILENO, &rfds);
    struct pollfd pollFd;
    pollFd.events = POLLIN;
    int ret;

    tv.tv_usec = 1000 * 200; // 200ms
    tv.tv_sec = 0;

    if (!clientConnected)
    {
        int recVal = select(socket_m + 1, &rfds, NULL, NULL, &tv);
        if (recVal == -1)
        {
            logger_m.error("select error");
            // Error
            close(peer_m);
            clientConnected = false;
        }
        else if (recVal > 0)
        {
            peer_m = accept(socket_m, NULL, NULL);
            if (peer_m < 0)
            {
                logger_m.error("accept error: " + std::string(strerror(errno)));
            }
            else
            {
                logger_m.info("Client connected !");
                clientConnected = true;
                setSimulation(false);
            }
        }
    }

    if (clientConnected)
    {
        pollFd.fd = peer_m;
        ret = poll(&pollFd, 1, 100); // 1 second for timeout
        switch (ret)
        {
        case -1:
            logger_m.error("poll error");
            clientConnected = false;
            close(peer_m);
            break;
        case 0:
            // logger_m.debug("poll timeout");
            break;
        default:
        {
            ssize_t len = recvfrom(peer_m, &pkt, sizeof(CmdFrame), 0, (sockaddr *)&serverAddress_m, (socklen_t *)&sockLen);
            if (len < 0)
            {
                // Failed to read packet
                logger_m.error("Error reading packet" + len);
            }
            else if (len == 0)
            {
                logger_m.error("socket closed");
                clientConnected = false;
            }
            else
            {
                logger_m.debug("read " + std::to_string(len) + " bytes");
                processPacket(pkt, peer_m);
            }
        }
        break;
        }
    }
}

void BeelightCom_sim::processPacket(const CmdFrame &pkt, int peerFd)
{
    CmdFrame response{
        .type = 0,
        .cmd = pkt.cmd,
        .len = 0,
        .status = 0,
    };
    if (pkt.type == 1)
    {
        switch (pkt.cmd)
        {
        case CMD_HELLO:
            if (strncmp((const char *)&pkt.data, "Hello", pkt.len) == 0)
            {
                logger_m.info("Received CMD_HELLO");
                response.len = 5;
                response.status = 1;
                snprintf((char *)&response.data.read.varName, 6, "World");
                write(peerFd, &response, response.len + 4);
            }
            break;
        case CMD_CLOSE:
            logger_m.info("Received CMD_CLOSE");
            clientConnected = false;
            close(peerFd);
            setSimulation(true);
            break;
        case CMD_READ:
            processReadCommand(pkt, response);
            write(peerFd, &response, response.len + 4);
            break;
        case CMD_WRITE:
            processWriteCommand(pkt, response);
            write(peerFd, &response, response.len + 4);
            break;
        default:
            logger_m.info("Received unknown command: " + std::to_string(pkt.cmd));
            break;
        }
    }
}

void BeelightCom_sim::initReadCommand()
{
    readCb_m["CurrentTime"] = []()
    {
        return NavigationModel::instance()->getCurrentTime();
    };
    readCb_m["EstTimeBeforeArrival"] = []()
    {
        return NavigationModel::instance()->getEstTimeBeforeArrival();
    };
    readCb_m["EstDistanceBeforeArrival"] = []()
    {
        return NavigationModel::instance()->getEstDistanceBeforeArrival();
    };
    readCb_m["ArrivingTime"] = []()
    {
        return NavigationModel::instance()->getArrivingTime();
    };
    readCb_m["NextInstruction"] = []()
    {
        return NavigationModel::instance()->getNextInstruction();
    };
    readCb_m["RemainingDistanceBeforeNextInstruction"] = []()
    {
        return NavigationModel::instance()->getRemainingDistanceBeforeNextInstruction();
    };
}

void BeelightCom_sim::processReadCommand(const CmdFrame &inPkt, CmdFrame &outPkt)
{
    std::string varName = std::string(inPkt.data.read.varName);
    if (readCb_m.find(varName) != readCb_m.end())
    {
        std::string value = readCb_m[varName]();
        outPkt.status = 1;
        outPkt.len = value.length();
        strncpy((char *)outPkt.data.data, value.c_str(), sizeof(outPkt.data.data));
    }
    else
    {
        logger_m.error("Unknown read variable: " + varName);
        outPkt.status = 0; // not found
        outPkt.len = 0;
    }
}

void BeelightCom_sim::initWriteCommand()
{
    writeCb_m["CurrentTime"] = [this](std::string value)
    {
        logger_m.debug("Setting CurrentTime to " + value);
        NavigationModel::instance()->setCurrentTime(value);
    };
    writeCb_m["EstTimeBeforeArrival"] = [this](std::string value)
    {
        logger_m.debug("Setting EstTimeBeforeArrival to " + value);
        NavigationModel::instance()->setEstTimeBeforeArrival(value);
    };
    writeCb_m["EstDistanceBeforeArrival"] = [this](std::string value)
    {
        logger_m.debug("Setting EstDistanceBeforeArrival to " + value);
        NavigationModel::instance()->setEstDistanceBeforeArrival(value);
    };
    writeCb_m["ArrivingTime"] = [this](std::string value)
    {
        logger_m.debug("Setting ArrivingTime to " + value);
        NavigationModel::instance()->setArrivingTime(value);
    };
    writeCb_m["NextInstruction"] = [this](std::string value)
    {
        logger_m.debug("Setting NextInstruction to " + value);
        NavigationModel::instance()->setNextInstruction(value);
    };
    writeCb_m["RemainingDistanceBeforeNextInstruction"] = [this](std::string value)
    {
        logger_m.debug("Setting RemainingDistanceBeforeNextInstruction to " + value);
        NavigationModel::instance()->setRemainingDistanceBeforeNextInstruction(value);
    };
}

void BeelightCom_sim::processWriteCommand(const CmdFrame &inPkt, CmdFrame &outPkt)
{
    std::string varName = std::string(inPkt.data.write.varName);
    char c_value[256] = {0};
    strncpy(c_value, (const char *)inPkt.data.write.data, inPkt.len - sizeof(inPkt.data.write.varName));
    std::string value = std::string(c_value);
    if (writeCb_m.find(varName) != writeCb_m.end())
    {
        writeCb_m[varName](value);
        outPkt.status = 1;
        outPkt.len = 0;
    }
    else
    {
        logger_m.error("Unknown write variable: " + varName);
        outPkt.status = 0; // not found
        outPkt.len = 0;
    }
}

void step(lv_timer_t *timer)
{
    static bool init = true;
    const std::string directions[] = {"droite", "gauche", "tout droit", "arrive"};
    static int dirIndex = 0;
    /*Use the user_data*/
    BeelightCom_sim *user_data = (BeelightCom_sim *)lv_timer_get_user_data(timer);
    if (user_data)
    {
        auto logger = user_data->getLogger();
        auto model = NavigationModel::instance();

        user_data->serverStep();

        if (user_data->isSimulationEnabled())
        {
            if (init)
            {
                model->setNextInstruction(directions[dirIndex]);
                init = false;
            }
            else
            {
                // current time update
                std::time_t t = std::time(0); // Get the time now
                std::tm *now = std::localtime(&t);
                static uint8_t i = 0;
                std::string currentTime = std::to_string(now->tm_hour) + ":" + std::to_string(i++); // std::to_string(now->tm_min);
                model->setCurrentTime(currentTime);

                // ETA update
                static int eta = 20;
                model->setEstTimeBeforeArrival(std::to_string(eta) + " sec");
                eta = (eta > 0 ? eta - 1 : 20);

                // EDA update
                static int eda = 2000;
                model->setEstDistanceBeforeArrival(std::to_string(eda) + " m");
                eda = (eda > 0 ? eda - 100 : 2000);

                static int dni = 500;
                model->setRemainingDistanceBeforeNextInstruction(std::to_string(dni) + " m");
                dni -= 100;
                if (dni <= 0)
                {
                    dirIndex = (dirIndex + 1) % 4;
                    model->setNextInstruction(directions[dirIndex]);
                    dni = 500;
                }
            }
        }
    }
}
