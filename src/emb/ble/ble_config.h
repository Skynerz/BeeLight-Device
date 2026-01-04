/**
 * @file   ble_config.h
 * @author Tom Coulon
 * @brief  BLE configuration header file
 */

#ifndef BLE_CONFIG_H
#define BLE_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>
#include <BLEAdvertising.h>

/* Exported defines and constants --------------------------------------------*/

// Generic Data Service
#define SERVICE_UUID_GENERIC "0000AAAA-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_TIME "0000AAA1-0000-1000-8000-00805F9B34FB"

// Navigation Data Service
#define SERVICE_UUID_NAVIGATION "0000BBBB-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_ETA "0000BBB1-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_EDA "0000BBB2-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_ARRIVING_TIME "0000BBB3-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_INSTRUCTION "0000BBB4-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_INSTRUCTION_DISTANCE "0000BBB5-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_INSTRUCTION_ICON "0000BBB6-0000-1000-8000-00805F9B34FB"
#define CHARAC_UUID_INSTRUCTION_META "0000BBB7-0000-1000-8000-00805F9B34FB"

static String DEVICE_NAME = "BeeLight";
#if defined(CONFIG_BLUEDROID_ENABLED)
static uint16_t DEVICE_APPEARANCE = ESP_BLE_APPEARANCE_OUTDOOR_SPORTS_LOCATION_AND_NAV;
#elif defined(CONFIG_NIMBLE_ENABLED)
static uint16_t DEVICE_APPEARANCE = 0x1442;
#endif

/* Exported typedef ----------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
void ble_init(void);

#endif /* BLE_CONFIG_H */
