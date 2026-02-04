#include "port_persistency_int.h"

#include <stdlib.h>
#include <string.h>

static Config_entry_t config[KEY_COUNT];  // Assuming a maximum of 10 config
static int entry_count                 = 0;
static Parser_config_t parser_config[] = {
#ifndef TESTING
    {"COMMISSIONNING_STATUS", COMMISSIONNING_STATUS, ENTRY_TYPE_UINT8, sizeof(uint8_t)},
    {"DEVICE_NAME", DEVICE_NAME, ENTRY_TYPE_STRING, 0},
    {"PASSWORD", PASSWORD, ENTRY_TYPE_STRING, 0},
#else
    {"TEST_UINT8", 0, ENTRY_TYPE_UINT8, sizeof(uint8_t)},
    {"TEST_UINT32", 1, ENTRY_TYPE_UINT32, sizeof(uint32_t)},
    {"TEST_STRING", 2, ENTRY_TYPE_STRING, 0},
#endif
};

void persistency_int_load_config_file(void) {
    char buffer[128];
    persistency_int_reset_config();
    if (config_file != NULL) {
        while (!feof(config_file)) {
            fgets(buffer, sizeof(buffer), config_file);
            char* delimiter_pos = strchr(buffer, '=');
            if (delimiter_pos != NULL) {
                *delimiter_pos        = '\0';
                const char* key_str   = buffer;
                const char* value_str = delimiter_pos + 1;

                // Remove newline character from value_str
                char* newline_pos = strchr(value_str, '\n');
                if (newline_pos != NULL) {
                    *newline_pos = '\0';
                }

                persistency_int_add_by_name(key_str, value_str);
            }
        }
    } else {
        printf("persistency_int_load_config_file: config file not opened !\n");
    }
}

void persistency_int_reset_config(void) {
    memset(config, 0, sizeof(config));
    entry_count = 0;
}

void persistency_int_write_config_file(void) {
    Parser_config_t* parser_entry;
    char buffer[128];
    if (config_file) {
        // Rewrite the entire file
        fseek(config_file, 0, SEEK_SET);
        for (int i = 0; i < entry_count; i++) {
            parser_entry = persistency_int_get_parser_entry_by_key(config[i].key);
            if (parser_entry != NULL) {
                switch (parser_entry->type) {
                    case ENTRY_TYPE_UINT8:
                        snprintf(buffer, sizeof(buffer), "%s=%u\n", parser_entry->key_str, config[i].value.u8);
                        break;
                    case ENTRY_TYPE_UINT32:
                        snprintf(buffer, sizeof(buffer), "%s=%u\n", parser_entry->key_str, config[i].value.u32);
                        break;
                    case ENTRY_TYPE_STRING:
                        snprintf(buffer, sizeof(buffer), "%s=%s\n", parser_entry->key_str, config[i].value.str);
                        break;
                    default:
                        // ignored
                        break;
                }
                fwrite(buffer, 1, strlen(buffer), config_file);
                fflush(config_file);
            }
        }
    }
}

Parser_config_t* persistency_int_get_parser_entry_by_key(const Key_t key) {
    for (int i = 0; i < sizeof(parser_config) / sizeof(Parser_config_t); i++) {
        if (parser_config[i].key_enum == key) {
            return &parser_config[i];
        }
    }
    return NULL;
}

Parser_config_t* persistency_int_get_parser_entry_by_name(const char* name) {
    for (int i = 0; i < sizeof(parser_config) / sizeof(Parser_config_t); i++) {
        if (strcmp(name, parser_config[i].key_str) == 0) {
            return &parser_config[i];
        }
    }
    return NULL;
}

uint8_t persistency_int_add_by_name(const char* key, const char* value) {
    uint8_t ret                   = 1;
    Parser_config_t* parser_entry = persistency_int_get_parser_entry_by_name(key);

    if (parser_entry != NULL) {
        switch (parser_entry->type) {
            case ENTRY_TYPE_UINT8: {
                uint8_t* pValue              = (uint8_t*) value;
                uint8_t val                  = *pValue - '0';
                config[entry_count].key      = parser_entry->key_enum;
                config[entry_count].len      = parser_entry->type_size;
                config[entry_count].value.u8 = val;
                entry_count++;
                break;
            }
            case ENTRY_TYPE_UINT32: {
                uint32_t val                  = (uint32_t) atoi(value);
                config[entry_count].key       = parser_entry->key_enum;
                config[entry_count].len       = parser_entry->type_size;
                config[entry_count].value.u32 = val;
                entry_count++;
                break;
            }
            case ENTRY_TYPE_STRING:
                config[entry_count].key = parser_entry->key_enum;
                config[entry_count].len = strlen(value);
                strncpy(config[entry_count].value.str, value, config[entry_count].len);
                entry_count++;
                break;
            default:
                // ignored
                ret = 0;
                break;
        }
    } else {
        ret = 0;
    }
    return ret;
}

uint8_t persistency_int_add_by_key(const Key_t key, const void* value) {
    uint8_t ret                   = 1;
    Parser_config_t* parser_entry = persistency_int_get_parser_entry_by_key(key);
    if (parser_entry != NULL) {
        switch (parser_entry->type) {
            case ENTRY_TYPE_UINT8: {
                uint8_t val                  = *(uint8_t*) value;
                config[entry_count].key      = parser_entry->key_enum;
                config[entry_count].len      = parser_entry->type_size;
                config[entry_count].value.u8 = val;
                entry_count++;
                break;
            }
            case ENTRY_TYPE_UINT32: {
                uint32_t val                  = *(uint32_t*) value;
                config[entry_count].key       = parser_entry->key_enum;
                config[entry_count].len       = parser_entry->type_size;
                config[entry_count].value.u32 = val;
                entry_count++;
                break;
            }
            case ENTRY_TYPE_STRING:
                config[entry_count].key = parser_entry->key_enum;
                config[entry_count].len = strlen((char*) value);
                strncpy(config[entry_count].value.str, (char*) value, config[entry_count].len);
                entry_count++;
                break;
            default:
                // ignored
                ret = 0;
                break;
        }
    } else {
        ret = 0;
    }
    return ret;
}

Config_entry_t* persistency_int_get(const Key_t key) {
    for (int i = 0; i < entry_count; i++) {
        if (config[i].key == key) {
            return &config[i];
        }
    }
    return NULL;
}
