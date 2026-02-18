#ifndef COMMISSIONSCREENS_HPP
#define COMMISSIONSCREENS_HPP

#include "ui/AbstractScreen.hpp"
#include "ui/commissionning/CommissionBleConfigScreen.hpp"

class CommissionStartScreen : public AbstractScreen {
   public:
    CommissionStartScreen() : AbstractScreen("CommissionStartScreen") {
    }
    void populate() override;
};

class CommissionFinishScreen : public AbstractScreen {
   public:
    CommissionFinishScreen() : AbstractScreen("CommissionFinishScreen") {
    }
    void populate() override;
};
#endif /* COMMISSIONSCREENS_HPP */
