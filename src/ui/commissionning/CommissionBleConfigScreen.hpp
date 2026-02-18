#ifndef COMMISSION_BLECONFIGSCREEN_HPP
#define COMMISSION_BLECONFIGSCREEN_HPP

#include "ui/commissionning/CommissionScreens.hpp"

class CommissionBleConfigScreen : public AbstractScreen {
   public:
    CommissionBleConfigScreen() : AbstractScreen("CommissionBleConfigScreen") {
    }
    void populate() override;
};

#endif /* COMMISSION_BLECONFIGSCREEN_HPP */