#ifndef INCLUDE_CORE_PPU_H
#define INCLUDE_CORE_PPU_H

#include "debug.h"
#include "types.h"

struct ppu {
	
};

extern struct ppu* new_ppu();
extern void free_ppu(struct ppu* this);
extern void reset_ppu(struct ppu* this);

extern void ppu_step(struct ppu* this, s32 cycles);

#endif
