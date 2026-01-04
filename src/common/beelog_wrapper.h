#ifndef BEELOG_WRAPPER_H
#define BEELOG_WRAPPER_H

#ifdef __cplusplus
extern "C" {
#endif

#define BEELOG_LEVEL_DEBUG (unsigned char) 0
#define BEELOG_LEVEL_INFO (unsigned char) 1
#define BEELOG_LEVEL_WARN (unsigned char) 2
#define BEELOG_LEVEL_ERROR (unsigned char) 3

void beelog_print(const unsigned char logLevel, const char *tag, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
