#ifndef __DEVICES_DMA_H__
#define __DEVICES_DMA_H__

#include <sys/io.h>

#define DMA_IS_16B(n) (3<n)

#define DMA_MASK_PORT(n) (DMA_IS_16B(dma_chan) ? 0xD4 : 0x0A)
#define DMA_CLEAR_PORT(n) (DMA_IS_16B(dma_chan) ? 0xD8 : 0x0C)
#define DMA_MODE_PORT(n) (DMA_IS_16B(dma_chan) ? 0xD6 : 0x0B)

#define PAUSE_DMA(n) outb(DMA_MASK_PORT(n), 4|n)
#define RESUME_DMA(n) outb(DMA_MASK_PORT(n), n)
#define CLEAR_DMA(n) outb(DMA_CLEAR_PORT(n), 0)

void dma_init(uint8_t dma_chan, uint8_t mode, uint8_t page, uint16_t offset, uint16_t length);
uint16_t _dma_completed(uint8_t dma_chan);

#endif