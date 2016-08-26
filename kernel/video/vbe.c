#include "video/vbe.h"

const char* vbe_memory_models[VBE_MAX_MEM_MODEL] =
{
	"text",
	"CGA graphics",
	"Hercules graphics",
	"Planar",
	"Packed",
	"Non-chain 4,256 color",
	"Direct Color",
	"YUV",
};