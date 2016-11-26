#include "devices/dma.h"

static const uint8_t const dma_page_ports[8] = {0x87, 0x83, 0x81, 0x82, 0x8F, 0x8B, 0x89, 0x8A};
static const uint8_t const dma_addr_ports[8] = {0x00, 0x02, 0x04, 0x06, 0xC0, 0xC4, 0xC8, 0xCC};
const uint8_t const dma_count_ports[8] = {0x01, 0x03, 0x05, 0x07, 0xC2, 0xC6, 0xCA, 0xCE};

#define DMA0_PAGE_PORT	0x87
#define DMA0_ADDR_PORT	0x00
#define DMA0_CONT_PORT	0x01
#define DMA1_PAGE_PORT	0x83
#define DMA1_ADDR_PORT	0x02
#define DMA1_CONT_PORT	0x03
#define DMA2_PAGE_PORT	0x81
#define DMA2_ADDR_PORT	0x04
#define DMA2_CONT_PORT	0x05
#define DMA3_PAGE_PORT	0x82
#define DMA3_ADDR_PORT	0x06
#define DMA3_CONT_PORT	0x07
#define DMA4_PAGE_PORT	0x8F
#define DMA4_ADDR_PORT	0xC0
#define DMA4_CONT_PORT	0xC2
#define DMA5_PAGE_PORT	0x8B
#define DMA5_ADDR_PORT	0xC4
#define DMA5_CONT_PORT	0xC6
#define DMA6_PAGE_PORT	0x89
#define DMA6_ADDR_PORT	0xC8
#define DMA6_CONT_PORT	0xCA
#define DMA7_PAGE_PORT	0x8A
#define DMA7_ADDR_PORT	0xCC
#define DMA7_CONT_PORT	0xCE

void dma_init(uint8_t dma_chan, uint8_t mode, uint8_t page, uint16_t offset, uint16_t length)
{
	mode |= dma_chan;

	// Disable interrupts

	PAUSE_DMA(dma_chan);
	CLEAR_DMA(dma_chan);
	outb(DMA_MODE_PORT(dma_chan), mode);
	outb(dma_addr_ports[dma_chan], offset&0xFF);
	outb(dma_addr_ports[dma_chan], offset>>8);
	outb(dma_page_ports[dma_chan], page);
	outb(dma_count_ports[dma_chan], length&0xFF);
	outb(dma_count_ports[dma_chan], length>>8);
	RESUME_DMA(dma_chan);
	
	// Enable interrupts
}
