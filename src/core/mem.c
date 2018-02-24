#include "core/mem.h"
#include "core/bitfn.h"
#include "core/bootrom.h"
#include "core/cart.h"
#include "core/joypad.h"
#include <stdlib.h>
#include <string.h>

struct mem*
new_mem
(struct cart* cart, struct bootrom* boot)
{
	struct mem* this = malloc(sizeof(struct mem));
	this->cart = cart;
	this->boot = boot;
	reset_mem(this);
	return this;
}

void
free_mem
(struct mem* this)
{
	ASSERT(this);
/*	free(this->c_ram); */
	free(this);
}

void
reset_mem
(struct mem* this)
{
	ASSERT(this);

	/* Use the bootrom if we have one */
	if (this->boot != NULL) {
		memcpy((&this->byte[0]), &(this->boot->byte[0]), BOOTROM_SIZE);
		return;
	}

	this->joypad_select_direction = true;
	this->joypad[0] = 0;
	this->joypad[1] = 0;
	this->joypad[2] = 0;
	this->joypad[3] = 0;
	this->joypad[4] = 0;
	this->joypad[5] = 0;
	this->joypad[6] = 0;
	this->joypad[7] = 0;
	
	this->dma = false;
	
	this->byte[ADR_TIMER_DIV]  = 0x00;
	this->byte[ADR_TIMER_TIMA] = 0x00;
	this->byte[ADR_TIMER_TMA]  = 0x00;
	this->byte[ADR_TIMER_TMC]  = 0x00;

	this->byte[ADR_APU_1_SW]     = 0x80;
	this->byte[ADR_APU_1_LWP]    = 0xBF;
	this->byte[ADR_APU_1_VE]     = 0xF3;
	this->byte[ADR_APU_1_FHI]    = 0xBF;
	this->byte[ADR_APU_2_LWP]    = 0x3F;
	this->byte[ADR_APU_2_VE]     = 0x00;
	this->byte[ADR_APU_2_FHI]    = 0xBF;
	this->byte[ADR_APU_3_ON]     = 0x7F;
	this->byte[ADR_APU_3_L]      = 0xFF;
	this->byte[ADR_APU_3_SOL]    = 0x9F;
	this->byte[ADR_APU_3_FHI]    = 0xBF;
	this->byte[ADR_APU_4_L]      = 0xFF;
	this->byte[ADR_APU_4_VE]     = 0x00;
	this->byte[ADR_APU_4_POLYC]  = 0x00;
	this->byte[ADR_APU_4_COUNT]  = 0xBF;
	this->byte[ADR_APU_CCONTROL] = 0x77;
	this->byte[ADR_APU_SOT]      = 0xF3;
	this->byte[ADR_APU_ON]       = 0xF1;
	
	this->byte[ADR_LCD_C]    = 0x91;
	this->byte[ADR_LCD_STAT] = 0x00;
	this->byte[ADR_LCD_SCY]  = 0x00;
	this->byte[ADR_LCD_SCX]  = 0x00;
	this->byte[ADR_LCD_LY]   = 0x00;
	this->byte[ADR_LCD_LYC]  = 0x00;
	this->byte[ADR_BGP]      = 0xFC;
	this->byte[ADR_OBP0]     = 0xFF;
	this->byte[ADR_OBP1]     = 0xFF;
	this->byte[ADR_LCD_WY]   = 0x00;
	this->byte[ADR_LCD_WX]   = 0x00;
	
	this->byte[ADR_IE] = 0x00;
}

u8
mem_rb
(struct mem* this, u16 address)
{
	ASSERT(this);
	
	switch (address & 0xF000) {
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
		return this->cart->byte[address];
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
		/* DEPENDS ON BANK CONTROLLER? */
	default:
		return this->byte[address];
	}
}

u16
mem_rw
(struct mem* this, u16 address)
{
	ASSERT(this);
	return 0x0000;
}

void
mem_wb
(struct mem* this, u16 address, u8 data)
{
	ASSERT(this);
	switch (address & 0xF000) {
	case 0x0000:
	case 0x1000:
	case 0x2000:
	case 0x3000:
	case 0x4000:
	case 0x5000:
	case 0x6000:
	case 0x7000:
	default:
		if (address == ADR_JOYPAD) {
			if (BIT_VAL(data, 4)) {
				joypad_select_direction(this);
			} else if (BIT_VAL(data, 5)) {
				joypad_select_button(this);
			}
			return;
		} else if (address == ADR_DMA_OAM) {
			this->dma = true;
			this->dma_counter = 0;
			this->dma_source = (data << 8);
		}
	}

}

void
mem_ww
(struct mem* this, u16 address, u16 data)
{
	ASSERT(this);
}
