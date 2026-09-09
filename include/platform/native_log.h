#ifndef NATIVE_LOG_H
#define NATIVE_LOG_H

#include <macros.h>

int Platform_LogSetPath(const char *path);
const char *Platform_LogGetPath(void);
void Platform_LogInit(const char *appName);
void Platform_LogShutdown(void);
void Platform_LogFlush(void);
void Platform_Log(const char *fmt, ...) CTR_PRINTF_FORMAT(1, 2);
void Platform_LogWarn(const char *fmt, ...) CTR_PRINTF_FORMAT(1, 2);
void Platform_LogError(const char *fmt, ...) CTR_PRINTF_FORMAT(1, 2);

#if defined(__PS3__) || defined(__CELLOS_LV2__)
#define printf Platform_Log
#endif

#endif
