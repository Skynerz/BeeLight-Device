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

void persistency_init(void);
void persistency_deinit(void);
uint8_t persistency_write(const Key_t key, const void* value);
uint8_t persistency_read(const Key_t key, void** value, uint8_t* len);

#ifdef __cplusplus
}
#endif

#endif
