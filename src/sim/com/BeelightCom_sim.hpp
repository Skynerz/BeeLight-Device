#ifndef BEELIGHTCOM_SIM_HPP
#define BEELIGHTCOM_SIM_HPP

#include <lvgl.h>

#include <functional>
#include <map>
#if __WIN32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include "Frame.hpp"
#include "com/BeelightCom.hpp"

class BeelightCom_sim : public BeelightCom {
   public:
    static BeelightCom_sim *instance() {
        static BeelightCom_sim *instance = nullptr;
        if (instance == nullptr) {
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
    void simulationStep();

   private:
    void processPacket(const CmdFrame &pkt, int peerFd);
    void initReadCommand();
    void processReadCommand(const CmdFrame &inPkt, CmdFrame &outPkt);
    void initWriteCommand();
    void processWriteCommand(const CmdFrame &inPkt, CmdFrame &outPkt);
    int peer_m;
#if __WIN32__
    SOCKET socket_m;
#else
    int socket_m;
    sockaddr_in serverAddress_m;
#endif
    bool clientConnected{false};
    lv_timer_t *timer_m;
    bool simulationEnabled_m{false};
    BeeLog logger_m{"BeelightCom_sim"};

    std::map<std::string, std::function<std::string()>> readCb_m;
    std::map<std::string, std::function<void(std::string)>> writeCb_m;
};

void step(lv_timer_t *timer);

#endif
