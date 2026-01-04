#include "beelog_wrapper.h"

#include <stdarg.h>

#include <mutex>

#include "BeeLog.hpp"

static char message[256];
std::mutex msg_mtx;

void beelog_print(const unsigned char logLevel, const char *tag, const char *format, ...) {
    va_list args;
    /* Initialize the va_list structure */
    va_start(args, format);

#ifndef WIN32
    std::lock_guard<std::mutex> msg_lock(msg_mtx);
#endif
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    switch (logLevel) {
        default:
        case BEELOG_LEVEL_DEBUG:
            BeeLog::debug(tag, message);
            break;
        case BEELOG_LEVEL_INFO:
            BeeLog::info(tag, message);
            break;
        case BEELOG_LEVEL_WARN:
            BeeLog::warn(tag, message);
            break;
        case BEELOG_LEVEL_ERROR:
            BeeLog::error(tag, message);
            break;
    }
}
