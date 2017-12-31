#include "core/mem.h"
#include "core/bootrom.h"
#include "core/cart.h"
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
		memcpy((&this->byte[0]), &(this->boot->byte[0]), 256);
		return;
	}
	
	this->byte[0xFF04] = 0x00;
	this->byte[0xFF05] = 0x00;
	this->byte[0xFF06] = 0x00;
	this->byte[0xFF07] = 0x00;
	this->byte[0xFF10] = 0x80;
	this->byte[0xFF11] = 0xBF;
	this->byte[0xFF12] = 0xF3;
	this->byte[0xFF14] = 0xBF;
	this->byte[0xFF16] = 0x3F;
	this->byte[0xFF17] = 0x00;
	this->byte[0xFF19] = 0xBF;
	this->byte[0xFF1A] = 0x7F;
	this->byte[0xFF1B] = 0xFF;
	this->byte[0xFF1C] = 0x9F;
	this->byte[0xFF1E] = 0xBF;
	this->byte[0xFF20] = 0xFF;
	this->byte[0xFF21] = 0x00;
	this->byte[0xFF22] = 0x00;
	this->byte[0xFF23] = 0xBF;
	this->byte[0xFF24] = 0x77;
	this->byte[0xFF25] = 0xF3;
	this->byte[0xFF26] = 0xF1;
	this->byte[0xFF40] = 0x91;
	this->byte[0xFF42] = 0x00;
	this->byte[0xFF43] = 0x00;
	this->byte[0xFF45] = 0x00;
	this->byte[0xFF47] = 0xFC;
	this->byte[0xFF48] = 0xFF;
	this->byte[0xFF49] = 0xFF;
	this->byte[0xFF4A] = 0x00;
	this->byte[0xFF4B] = 0x00;
	this->byte[0xFFFF] = 0x00;
}

u8
mem_rb
(struct mem* this, u16 address)
{
	ASSERT(this);
	return 0x00;
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
}

void
mem_ww
(struct mem* this, u16 address, u16 data)
{
	ASSERT(this);
}
