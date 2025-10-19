#ifndef BEELIGHTCOM_SIM_HPP
#define BEELIGHTCOM_SIM_HPP

#include <lvgl.h>
#include <functional>
#include <map>
//TODO windows
#include <sys/socket.h>
#include <netinet/in.h>
#include "com/BeelightCom.hpp"
#include "Frame.hpp"

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

    ~BeelightCom_sim() override {
        uninit();
    }
    void init() override;
    void uninit() override;
    void serverInit();
    void serverStep();

    void setSimulation(bool enable) {
        simulationEnabled_m = enable;
    }
    bool isSimulationEnabled() const {
        return simulationEnabled_m;
    }

private:
    void processPacket(const CmdFrame &pkt, int peerFd);
    void initReadCommand();
    void processReadCommand(const CmdFrame &inPkt, CmdFrame &outPkt);
    void initWriteCommand();
    void processWriteCommand(const CmdFrame &inPkt, CmdFrame &outPkt);
    int socket_m;
    int peer_m;
    sockaddr_in serverAddress_m;
    bool clientConnected{false};
    lv_timer_t *timer_m;
    bool simulationEnabled_m{false};
    BeeLog logger_m{"BeelightCom_sim"};

    std::map<std::string, std::function<std::string()>> readCb_m;
    std::map<std::string, std::function<void(std::string)>> writeCb_m;
};

void step(lv_timer_t *timer);

#endif
