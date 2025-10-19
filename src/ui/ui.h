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
using String = std::string; //todo
#endif

/* Exported defines and constants --------------------------------------------*/

/* Exported typedef ----------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
void ui_init();


// Setters
void setTime(const String time);

void setEta(const String eta);

void setEda(const String eda);

void setArrivingTime(const String arrivingTime);

void setDirection(const String direction);

void setDirectionDistance(const String direction);

void setDirectionIcon(const uint8_t *iconData, size_t iconSize);

void setConnected(const bool connected);

void clearData();

#endif /* UI_H */
