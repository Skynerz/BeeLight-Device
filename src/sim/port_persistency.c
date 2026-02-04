#include "common/port_persistency.h"

#define PORT_PERSISTENCY_C
#include <stdio.h>
#include <string.h>

#include "port_persistency_int.h"

void persistency_init() {
    if (config_file == NULL) {
        if ((config_file = fopen(PERSISTENCY_FILE_NAME, "r"))) {
            fclose(config_file);
            config_file = fopen(PERSISTENCY_FILE_NAME, "r+");
        }
        else {
            //file doesn't exist, create it
            config_file = fopen(PERSISTENCY_FILE_NAME, "w+");
        }
    }

    persistency_int_load_config_file();
}

void persistency_deinit() {
    if (config_file != NULL) {
        fclose(config_file);
        config_file = NULL;
        persistency_int_reset_config();
    }
}

uint8_t persistency_read(const Key_t key, void** value, uint8_t* len) {
    uint8_t ret = 0;
    *value      = NULL;
    *len        = 0;
    if ((value != NULL) && (len != NULL)) {
        Config_entry_t* entry = persistency_int_get(key);
        if (entry != NULL) {
            *value = &entry->value;
            *len   = entry->len;
            ret    = 1;
        } else {
            printf("persistency_read: key %d not found\n", key);
        }
    }
    return ret;
}

uint8_t persistency_write(const Key_t key, const void* value) {
    uint8_t ret = 1;
    Config_entry_t* entry         = persistency_int_get(key);
    Parser_config_t* parser_entry = persistency_int_get_parser_entry_by_key(key);
    if (parser_entry != NULL) {
        if (entry != NULL) {
            if (parser_entry != NULL) {
                switch (parser_entry->type) {
                    case ENTRY_TYPE_UINT8:
                        entry->value.u8 = *(uint8_t*) value;
                        break;
                    case ENTRY_TYPE_UINT32:
                        entry->value.u32 = *(uint32_t*) value;
                        break;
                    case ENTRY_TYPE_STRING:
                        strncpy(entry->value.str, (char*) value, strlen((char*) value));
                        break;
                    default:
                        // ignored
                        ret = 0;
                        break;
                }
            } else {
                ret = 0;
                printf("persistency_write: key %d unknown\n", key);
            }
        } else {
            ret = persistency_int_add_by_key(key, value);
        }

        // Rewrite the entire file
        persistency_int_write_config_file();
    } else {
        ret = 0;
        printf("persistency_write: parser entry for key %d not found\n", key);
    }
    return ret;
}
