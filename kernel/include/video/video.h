#ifndef __VIDEO_VIDEO_H__
#define __VIDEO_VIDEO_H__

#include <stdint.h>
#include "video/vbe.h"
#include "video/device.h"

extern video_device_t* video_device;

void video_init(void);
void vbe_init(vbe_controller_t* controller, vbe_mode_t* mode);

#endif