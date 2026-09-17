#ifndef BeelightCom_emb_HPP
#define BeelightCom_emb_HPP

#include "com/BeelightCom.hpp"

class BeelightCom_Impl : public BeelightCom {
public:
    void init() override;
    void uninit() override;
    void start_advertising(void) override;
    void stop_advertising(void) override;
    bool is_connected() override;
    bool get_pin(uint32_t&) override;
};

#endif
