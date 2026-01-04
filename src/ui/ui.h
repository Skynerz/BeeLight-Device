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
#include <cstdint>
#include <string>
using String = std::string;  // todo
#endif

/* Exported defines and constants --------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
void ui_init();

// Setters
void setDirectionIcon(const uint8_t *iconData, size_t iconSize);

void setConnected(const bool connected);

#endif /* UI_H */
