/**
 * @file   Dashboard.hpp
 * @author Tom Coulon
 * @brief  User Interface header file
 */

#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP
/* Includes ------------------------------------------------------------------*/
#ifndef SIMULATOR
#include <Arduino.h>
#else
#include <cstdint>
#include <string>
using String = std::string;  // todo
#endif

#include "AbstractScreen.hpp"
#include "DirectionIconWidget.hpp"

/* Exported defines and constants --------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/

// Setters
void setDirectionIcon(const uint8_t *iconData, size_t iconSize);

void setConnected(const bool connected);

class Dashboard : public AbstractScreen {
   public:
    Dashboard() : AbstractScreen("Dashboard"), directionIcon_m(obj()) {
    }
    void populate() override;
    void onTimerEvent() override;
    void onPostPopulate() override;
    void onScreenLoaded() override;

   private:
    DirectionIconWidget directionIcon_m;
    uint8_t tick_m{0};
    static void updateNextInstructionIcon(lv_event_t *event = nullptr);
};

#endif /* DASHBOARD_HPP */
