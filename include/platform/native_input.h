#ifndef PLATFORM_NATIVE_INPUT_H
#define PLATFORM_NATIVE_INPUT_H

#include <macros.h>

#if defined(__PS3__) || defined(__CELLOS_LV2__)
#ifndef SDL_SCANCODE_X
#define SDL_SCANCODE_X             101
#endif
#ifndef SDL_SCANCODE_V
#define SDL_SCANCODE_V             102
#endif
#ifndef SDL_SCANCODE_Z
#define SDL_SCANCODE_Z             103
#endif
#ifndef SDL_SCANCODE_C
#define SDL_SCANCODE_C             104
#endif
#ifndef SDL_SCANCODE_LSHIFT
#define SDL_SCANCODE_LSHIFT        105
#endif
#ifndef SDL_SCANCODE_LCTRL
#define SDL_SCANCODE_LCTRL         106
#endif
#ifndef SDL_SCANCODE_LEFTBRACKET
#define SDL_SCANCODE_LEFTBRACKET   107
#endif
#ifndef SDL_SCANCODE_RSHIFT
#define SDL_SCANCODE_RSHIFT        108
#endif
#ifndef SDL_SCANCODE_RCTRL
#define SDL_SCANCODE_RCTRL         109
#endif
#ifndef SDL_SCANCODE_RIGHTBRACKET
#define SDL_SCANCODE_RIGHTBRACKET  110
#endif
#ifndef SDL_SCANCODE_UP
#define SDL_SCANCODE_UP            111
#endif
#ifndef SDL_SCANCODE_DOWN
#define SDL_SCANCODE_DOWN          112
#endif
#ifndef SDL_SCANCODE_LEFT
#define SDL_SCANCODE_LEFT          113
#endif
#ifndef SDL_SCANCODE_RIGHT
#define SDL_SCANCODE_RIGHT         114
#endif
#ifndef SDL_SCANCODE_SPACE
#define SDL_SCANCODE_SPACE         115
#endif
#ifndef SDL_SCANCODE_RETURN
#define SDL_SCANCODE_RETURN        116
#endif
#ifndef SDL_SCANCODE_RALT
#define SDL_SCANCODE_RALT          117
#endif
#ifndef SDL_SCANCODE_LALT
#define SDL_SCANCODE_LALT          118
#endif
#ifndef SDL_SCANCODE_F1
#define SDL_SCANCODE_F1            119
#endif
#ifndef SDL_SCANCODE_F2
#define SDL_SCANCODE_F2            120
#endif
#ifndef SDL_SCANCODE_F3
#define SDL_SCANCODE_F3            121
#endif
#ifndef SDL_SCANCODE_F4
#define SDL_SCANCODE_F4            122
#endif
#ifndef SDL_SCANCODE_F5
#define SDL_SCANCODE_F5            123
#endif
#ifndef SDL_SCANCODE_F6
#define SDL_SCANCODE_F6            124
#endif
#ifndef SDL_SCANCODE_F7
#define SDL_SCANCODE_F7            125
#endif
#ifndef SDL_SCANCODE_F8
#define SDL_SCANCODE_F8            126
#endif
#ifndef SDL_SCANCODE_F9
#define SDL_SCANCODE_F9            127
#endif
#ifndef SDL_SCANCODE_F10
#define SDL_SCANCODE_F10           128
#endif
#ifndef SDL_SCANCODE_F11
#define SDL_SCANCODE_F11           129
#endif
#ifndef SDL_SCANCODE_F12
#define SDL_SCANCODE_F12           130
#endif
#endif

#define PLATFORM_INPUT_PAD_COUNT 4

struct PlatformInputPadSnapshot
{
	u8 status;
	u8 id;
	u8 buttons[2];
	u8 analog[4];
	u8 connected;
	u8 reserved[3];
};

int Platform_InputInit(void);
void Platform_InputShutdown(void);
void Platform_InputUpdate(void);
void Platform_InputControllerAdded(int deviceIndex);
void Platform_InputControllerRemoved(int instanceId);
int Platform_InputCycleKeyboardController(void);
int Platform_InputCycleGamepadController(void);

void Platform_InputPadInit(int slot, unsigned char *padData);
int Platform_InputPadGetState(int port);
void Platform_InputPadVibrate(int port, unsigned char *table, int len);
int Platform_InputCapturePadSnapshots(struct PlatformInputPadSnapshot *dst, int count);
int Platform_InputInstallPadSnapshots(const struct PlatformInputPadSnapshot *src, int count);
void Platform_InputClearInstalledPadSnapshots(void);
int Platform_InputGetStateSize(void);
int Platform_InputCaptureState(void *dst, int dstSize);
int Platform_InputRestoreState(const void *src, int srcSize);

#endif
