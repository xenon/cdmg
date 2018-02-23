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

	u8 byte[0xFFFF]; /* 0x000-0x8000 0x8000-0xFFFF */
};

enum {
	ADR_INT_VBLANK  = 0x0040,
	ADR_INT_LCDSTAT = 0x0048,
	ADR_INT_TIMER   = 0x0050,
	ADR_INT_SERIAL  = 0x0058,
	ADR_INT_JOYPAD  = 0x0060,

	ADR_JOYPAD = 0xFF00,

	ADR_LCD_C    = 0xFF40,
	ADR_LCD_LY   = 0xFF44,
	ADR_LCD_LYC  = 0xFF45,
	ADR_LCD_STAT = 0xFF41,
	ADR_LCD_SCY  = 0xFF42,
	ADR_LCD_SCX  = 0xFF43,
	ADR_LCD_WY   = 0xFF4A,
	ADR_LCD_WX   = 0xFF4B,

	ADR_DMA_OAM = 0xFF46,
	
	ADR_TIMER_DIV  = 0xFF04,
	ADR_TIMER_TIMA = 0xFF05,
	ADR_TIMER_TMA  = 0xFF06,
	ADR_TIMER_TMC  = 0xFF07,
	
	ADR_IF = 0xFF0F,
	ADR_IE = 0xFFFF,
};

extern struct mem* new_mem(struct cart* cart, struct bootrom* boot);
extern void free_mem(struct mem* this);
extern void reset_mem(struct mem* this);

extern u8 mem_rb(struct mem* this, u16 address);
extern u16 mem_rw(struct mem* this, u16 address);
extern void mem_wb(struct mem* this, u16 address, u8 data);
extern void mem_ww(struct mem* this, u16 address, u16 data);

#endif
