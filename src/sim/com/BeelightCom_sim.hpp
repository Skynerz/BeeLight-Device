#ifndef BEELIGHTCOM_SIM_HPP
#define BEELIGHTCOM_SIM_HPP

#include <lvgl.h>
//TODO windows
#include <sys/socket.h>
#include <netinet/in.h>
#include "com/BeelightCom.hpp"

class BeelightCom_sim : public BeelightCom
{
public:
    static BeelightCom_sim *instance()
    {
        static BeelightCom_sim *instance = nullptr;
        if (instance == nullptr)
        {
            instance = new BeelightCom_sim();
        }
        return instance;
    }
    void init() override;
    void serverInit();
    void serverStep();

    void setSimulation(bool enable) {
        simulationEnabled_m = enable;
    }
    bool isSimulationEnabled() const {
        return simulationEnabled_m;
    }

private:
    int socket_m;
    sockaddr_in serverAddress_m;
    bool clientConnected{false};
    lv_timer_t *timer_m;
    bool simulationEnabled_m{false};
    BeeLog logger_m{"BeelightCom_sim"};
};

void step(lv_timer_t *timer);

#endif
