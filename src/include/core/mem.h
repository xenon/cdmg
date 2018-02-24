#ifndef INCLUDE_CORE_MEM_H
#define INCLUDE_CORE_MEM_H

#include "debug.h"	
#include "types.h"
/*#include <stdlib.h>*/

struct bootrom;
struct cart;

struct mem {
	struct bootrom* boot;
	struct cart* cart;
	/* u8 c_rom_bank; */
	/* for the cart possibly use virtual functions for cart reading
	 * as in use a function pointer. This simplifies MBC additions */

/* THIS SHOULD PROBABLY GO IN THE CART...?
	u8 *c_ram; 
	u8 c_ram_bank;
	size_t c_ram_size; */
	u8 joypad[8];
	bool joypad_select_direction;

	bool dma;
	u16 dma_counter;
	u16 dma_source;
	
	u8 byte[0xFFFF]; /* 0x000-0x8000 0x8000-0xFFFF */
};

enum {
	ADR_INT_VBLANK  = 0x0040,
	ADR_INT_LCDSTAT = 0x0048,
	ADR_INT_TIMER   = 0x0050,
	ADR_INT_SERIAL  = 0x0058,
	ADR_INT_JOYPAD  = 0x0060,

	ADR_ECHORAM = 0xE000,
	
	ADR_OAM = 0xFE00,

	ADR_UNUSABLE = 0xFEA0,
	
	ADR_JOYPAD = 0xFF00,

	ADR_SERIAL_TD = 0xFF01,
	ADR_SERIAL_TC = 0xFF02,
	
	ADR_TIMER_DIV  = 0xFF04,
	ADR_TIMER_TIMA = 0xFF05,
	ADR_TIMER_TMA  = 0xFF06,
	ADR_TIMER_TMC  = 0xFF07,

	ADR_APU_1_SW     = 0xFF10,
	ADR_APU_1_LWP    = 0xFF11,
	ADR_APU_1_VE     = 0xFF12,
	ADR_APU_1_FLO    = 0xFF13,
	ADR_APU_1_FHI    = 0xFF14,
	ADR_APU_2_LWP    = 0xFF16,
	ADR_APU_2_VE     = 0xFF17,
	ADR_APU_2_FLO    = 0xFF18,
	ADR_APU_2_FHI    = 0xFF19,
	ADR_APU_3_ON     = 0xFF1A,
	ADR_APU_3_L      = 0xFF1B,
	ADR_APU_3_SOL    = 0xFF1C,
	ADR_APU_3_FLO    = 0xFF1D,
	ADR_APU_3_FHI    = 0xFF1E,
	ADR_APU_4_L      = 0xFF20,
	ADR_APU_4_VE     = 0xFF21,
	ADR_APU_4_POLYC  = 0xFF22,
	ADR_APU_4_COUNT  = 0xFF23,
	ADR_APU_CCONTROL = 0xFF24,
	ADR_APU_SOT      = 0xFF25,
	ADR_APU_ON       = 0xFF26,
	
	ADR_APU_WPRAM = 0xFF30,
	
	
	ADR_LCD_C    = 0xFF40,
	ADR_LCD_STAT = 0xFF41,
	ADR_LCD_SCX  = 0xFF42,
	ADR_LCD_SCY  = 0xFF43,
	ADR_LCD_LY   = 0xFF44,
	ADR_LCD_LYC  = 0xFF45,
	ADR_DMA_OAM  = 0xFF46,
	ADR_BGP      = 0xFF47,
	ADR_OBP0     = 0xFF48,
	ADR_OBP1     = 0xFF49,
	ADR_LCD_WY   = 0xFF4A,
	ADR_LCD_WX   = 0xFF4B,
	
	ADR_IF = 0xFF0F,
	
	ADR_ZEROPAGE = 0xFF80,
	
	ADR_IE = 0xFFFF,
};

extern struct mem* new_mem(struct cart* cart, struct bootrom* boot);
extern void free_mem(struct mem* this);
extern void reset_mem(struct mem* this);

extern u8 mem_rb(struct mem* this, u16 address);
extern u16 mem_rw(struct mem* this, u16 address);
extern void mem_wb(struct mem* this, u16 address, u8 data);
extern void mem_ww(struct mem* this, u16 address, u16 data);

static inline void
mem_dma
(struct mem* this)
{
	if (this->dma) {
		this->byte[ADR_OAM + this->dma_counter] = this->byte[this->dma_source + this->dma_counter];
		++(this->dma_counter);
		if (this->dma_counter == 160)
			this->dma = false;
	}
}

#endif
