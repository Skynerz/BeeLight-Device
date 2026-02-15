#ifndef PORT_PERSISTENCY_INT_H
#define PORT_PERSISTENCY_INT_H

#include "common/port_persistency.h"
#include <stdio.h>

typedef struct {
    Key_t key;
    size_t len;
    union {
        uint8_t u8;
        uint32_t u32;
        char str[64];
    } value;
} Config_entry_t;

typedef enum {
    ENTRY_TYPE_NONE = 0,
    ENTRY_TYPE_UINT8,
    ENTRY_TYPE_UINT32,
    ENTRY_TYPE_STRING,
} Entry_type_t;

typedef struct {
    char key_str[32];
    Key_t key_enum;
    Entry_type_t type;
    uint8_t type_size;
} Parser_config_t;

#ifndef PORT_PERSISTENCY_C
#define GLOBAL 
#else
#define GLOBAL extern
#endif
GLOBAL FILE* config_file;

#define PERSISTENCY_FILE_NAME (const char*)"./beelight-device.conf"

void persistency_int_load_config_file(void);
void persistency_int_write_config_file(void);
void persistency_int_reset_config(void);

Parser_config_t* persistency_int_get_parser_entry_by_key(const Key_t key);
Parser_config_t* persistency_int_get_parser_entry_by_name(const char* name);

uint8_t persistency_int_add_by_name(const char* key, const char* value);
uint8_t persistency_int_add_by_key(const Key_t key, const void* value);
Config_entry_t* persistency_int_get(const Key_t key);

#endif
