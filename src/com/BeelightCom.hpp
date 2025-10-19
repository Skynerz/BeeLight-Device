#ifndef BEELIGHTCOM_HPP
#define BEELIGHTCOM_HPP

#include "BeeLog.hpp"

class BeelightCom
{
public:
    virtual ~BeelightCom() = default;
    virtual void init() = 0;
    virtual void uninit() = 0;

    BeeLog *getLogger() { return &log_m; }

protected:
    BeeLog log_m{"BeelightCom"};
};

#endif
