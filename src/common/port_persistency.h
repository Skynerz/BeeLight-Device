#ifndef PORT_PERSISTENCY_H
#define PORT_PERSISTENCY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

typedef enum {
    NONE = 0,
    // Commissionning
    COMMISSIONNING_STATUS,
    //BLE config
    DEVICE_NAME,
    PASSWORD,

    KEY_COUNT
} Key_t;

/**
 * @brief Initialize the persistency module
 */
void persistency_init(void);

/**
 * @brief Deinitialize the persistency module
 */
void persistency_deinit(void);

/**
 * @brief Write a value to persistency
 * @param key The key to write
 * @param value Pointer to the value to write (must be allocated by caller)
 * @return 1 if the key was written successfully, 0 otherwise
 */
uint8_t persistency_write(const Key_t key, const void* value);

/**
 * @brief Read a value from persistency
 * @param key The key to read
 * @param value Output parameter, pointer to the read value (must be allocated by caller)
 * @param len Output parameter, length of the read value
 * @return 1 if the key was found and read successfully, 0 otherwise
 */
uint8_t persistency_read(const Key_t key, void** value, uint8_t* len);

#ifdef __cplusplus
}
#endif

#endif
