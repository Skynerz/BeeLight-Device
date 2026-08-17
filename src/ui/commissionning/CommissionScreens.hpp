#ifndef COMMISSIONSCREENS_HPP
#define COMMISSIONSCREENS_HPP

#include "ui/AbstractScreen.hpp"
#include "ui/UserInputScreen.hpp"

class CommissionStartScreen : public AbstractScreen {
   public:
    CommissionStartScreen() : AbstractScreen("CommissionStartScreen") {
    }
    void populate() override;

   private:
    static void onNextButtonClicked(lv_event_t* event);
};

class CommissionDeviceNameConfigScreen : public UserInputScreen {
   public:
    CommissionDeviceNameConfigScreen() : UserInputScreen("CommissionDeviceNameConfigScreen") {
    }
};

class CommissionConnWaitingScreen : public AbstractScreen {
   public:
    CommissionConnWaitingScreen()
        : AbstractScreen("CommissionDeviceNameConfigScreen"), is_advertising_started_m(false) {
    }
    void populate() override;
    void onTimerEvent() override;

   private:
    bool is_advertising_started_m;
    void displayPinCode();
};

class CommissionFinishScreen : public AbstractScreen {
   public:
    CommissionFinishScreen() : AbstractScreen("CommissionFinishScreen") {
    }
    void populate() override;
};
#endif /* COMMISSIONSCREENS_HPP */
