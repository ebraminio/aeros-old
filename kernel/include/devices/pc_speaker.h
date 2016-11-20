#ifndef __DEVICES_PC_SPEAKER_H__
#define __DEVICES_PC_SPEAKER_H__

#include <stdint.h>
#include <stdbool.h>

#define DO		261
#define DO_S	277
#define RE		294
#define RE_S	311
#define MI		330
#define FA		349
#define FA_S	370
#define SOL		392
#define SOL_S	415
#define LA		440
#define LA_S	466
#define SI		494

void enable_sound(bool enable);
void play_sound(uint32_t freq, uint16_t duration);

#endif