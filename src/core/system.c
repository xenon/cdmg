#include "core/system.h"
#include "backend/window.h"
#include "core/bootrom.h"
#include "core/cart.h"
#include "core/cpu.h"
#include "core/mem.h"
#include "core/ppu.h"
#include "core/timer.h"
#include "constants.h"
#include <stdlib.h>

struct system {
	s32 cycles;
	/*struct apu* apu;*/
	struct bootrom* bootrom;
	struct cart* cart;
	struct cpu* cpu;
	struct mem* mem;
	struct ppu* ppu;
	struct timer* timer;

	struct window* ref_window;
};

struct system*
new_system
(const char* cart_path, const char* bootrom_path, struct window* attached_window)
{
	struct system* this = malloc(sizeof(struct system));

	this->timer = new_timer();
	if (this->timer == NULL)
		goto err_at_timer;

	if (bootrom_path == NULL)
		this->bootrom = NULL;
	else {
		this->bootrom = new_bootrom(bootrom_path);
		if (this->bootrom == NULL)
			goto err_at_bootrom;
	}

	this->cart = new_cart(cart_path);
	if (this->cart == NULL)
		goto err_at_cart;
	
	this->mem = new_mem(this->cart, this->bootrom);
	if (this->mem == NULL)
		goto err_at_mem;
	
	this->cpu = new_cpu(this->mem);
	if (this->cpu == NULL)
		goto err_at_cpu;

	this->cycles = 0;
	this->ref_window = attached_window;

	return this;

err_at_cpu:
	free_mem(this->mem);
err_at_mem:
	free_cart(this->cart);
err_at_cart:
	free_bootrom(this->bootrom);
err_at_bootrom:
	free_timer(this->timer);
err_at_timer:
	return NULL;
}

void
free_system
(struct system* this)
{
	free_cpu(this->cpu);
	free_mem(this->mem);
	free_cart(this->cart);
	free_bootrom(this->bootrom);
	free(this);
}

void
reset_system
(struct system* this)
{
	this->cycles = 0;
	/*reset_apu(this->apu);*/
	reset_cpu(this->cpu);
	reset_mem(this->mem);
	reset_ppu(this->ppu);
}

void
system_step
(struct system* this)
{
	s32 opcode_cycles;
	s32 update_cycles = 0;

	while (update_cycles < GB_CYCLES_FRAME) {
		opcode_cycles = cpu_step(this->cpu);
		update_cycles += opcode_cycles;
		timer_step(this->timer, this->mem, opcode_cycles);
		ppu_step(this->ppu, opcode_cycles);
		/* interrupt step */
	}
	window_draw(this->ref_window);
}

bool
system_checksum
(struct system* this)
{
	return cart_checksum_header(this->cart);
}
