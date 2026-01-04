/**
 * @file   ui.h
 * @author Tom Coulon
 * @brief  User Interface header file
 */

#ifndef UI_H
#define UI_H
/* Includes ------------------------------------------------------------------*/
#ifndef SIMULATOR
#include <Arduino.h>
#else
#include <string>
#include <cstdint>
#include <AbstractScreen.hpp>
#include "DirectionIconWidget.hpp"

using String = std::string; //todo
#endif

/* Exported defines and constants --------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/

// Setters
void setDirectionIcon(const uint8_t *iconData, size_t iconSize);

void setConnected(const bool connected);

class Dashboard : public AbstractScreen
{
public:
    using AbstractScreen::AbstractScreen;
    void populate() override;
    void onTimerEvent() override;

private:
    DirectionIconWidget directionIcon_m;
    void updateNextInstructionIcon(lv_event_t *event = nullptr);
};

#endif /* UI_H */
