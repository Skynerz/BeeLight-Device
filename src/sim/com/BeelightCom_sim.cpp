#include "BeelightCom_sim.hpp"
#include "model/NavigationModel.hpp"
#include <ctime>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "Frame.hpp"

void BeelightCom_sim::init()
{
    logger_m.info("BeelightCom_sim init");

    //serverInit();
    setSimulation(true);

    // todo bind ?
    timer_m = lv_timer_create(&step, 1000, this);

    // first call
    step(timer_m);
}

void BeelightCom_sim::serverInit()
{
    logger_m.info("server init");
    socket_m = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    serverAddress_m.sin_family = AF_INET;
    serverAddress_m.sin_port = htons(PORT);
    serverAddress_m.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(socket_m, (struct sockaddr *)&serverAddress_m,
             sizeof(serverAddress_m)))
    {
        logger_m.error("Bind error: " + std::string(strerror(errno)));
    }

    int reuse = 1;
    if (setsockopt(socket_m, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0)
    {
        logger_m.error("setsockopt error: " + std::string(strerror(errno)));
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
    int peer;
    int sockLen = sizeof(serverAddress_m);
    FD_ZERO(&rfds);
    FD_SET(socket_m, &rfds);
    FD_SET(STDIN_FILENO, &rfds);

    tv.tv_usec = 1000 * 1000; // 200ms
    tv.tv_sec = 0;

    if (!clientConnected)
    {
        int recVal = select(socket_m + 1, &rfds, NULL, NULL, &tv);
        if (recVal == -1)
        {
            logger_m.error("select error");
            // Error
            close(socket_m);
            clientConnected = false;
        }
        else if (recVal > 0)
        {
            peer = accept(socket_m, NULL, NULL);
            if (peer < 0)
            {
                logger_m.error("accept error: " + std::string(strerror(errno)));
            }
            else
            {
                logger_m.info("Client connected !");
                clientConnected = true;
            }
        }
    }

    if (clientConnected)
    {
        ssize_t len = recvfrom(peer, &pkt, sizeof(CmdFrame), 0, (sockaddr *)&serverAddress_m, (socklen_t *)&sockLen);
        if (len < 0)
        {
            // Failed to read packet
            logger_m.error("Error reading packet" + len);
        }
        else if (len == 0) {
            logger_m.error("socket closed");
            clientConnected = false;
        }
        else
        {
            logger_m.debug("read "+std::to_string(len));
        }
    }
}

void step(lv_timer_t *timer)
{
    /*Use the user_data*/
    BeelightCom_sim *user_data = (BeelightCom_sim *)lv_timer_get_user_data(timer);
    if (user_data)
    {
        auto logger = user_data->getLogger();
        auto model = NavigationModel::instance();

        //user_data->serverStep();

        if (user_data->isSimulationEnabled())
        {
            // current time update
            std::time_t t = std::time(0); // Get the time now
            std::tm *now = std::localtime(&t);
            std::string currentTime = std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min);
            model->setCurrentTime(currentTime);
        }
    }
}
