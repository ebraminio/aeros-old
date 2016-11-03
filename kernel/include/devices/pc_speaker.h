#ifndef __DEVICES_PC_SPEAKER_H__
#define __DEVICES_PC_SPEAKER_H__

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


void pc_speaker_init(void);
void play_sound(uint32_t freq);
void shutup(void);

#endif
