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

class CommissionBleConfigScreen : public UserInputScreen {
   public:
    CommissionBleConfigScreen() : UserInputScreen("CommissionBleConfigScreen") {
    }
};

class CommissionFinishScreen : public AbstractScreen {
   public:
    CommissionFinishScreen() : AbstractScreen("CommissionFinishScreen") {
    }
    void populate() override;
};
#endif /* COMMISSIONSCREENS_HPP */
