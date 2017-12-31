#include "core/system.h"
#include "core/bootrom.h"
#include "core/cart.h"
#include "core/cpu.h"
#include "core/mem.h"
#include <stdlib.h>

struct system {
	/*struct apu* apu;*/
	struct bootrom* bootrom;
	struct cart* cart;
	struct cpu* cpu;
	struct mem* mem;
	/*struct ppu* ppu;*/
};

struct system*
new_system
(const char* cart_path, const char* bootrom_path)
{
	struct system* this = malloc(sizeof(struct system));
	this->bootrom = new_bootrom(bootrom_path);
	if (this->bootrom == NULL)
		goto err_at_bootrom;
	this->cart = new_cart(cart_path);
	if (this->cart == NULL)
		goto err_at_cart;
	this->cpu = new_cpu(NULL); /* fix me to take in a memory struct */
	if (this->cpu == NULL)
		goto err_at_cpu;
	this->mem = new_mem(this->cart, this->bootrom);
	if (this->mem == NULL)
		goto err_at_mem;
	return this;

err_at_mem:
	free_cpu(this->cpu);
err_at_cpu:
	free_cart(this->cart);
err_at_cart:
	free_bootrom(this->bootrom);
err_at_bootrom:
	return NULL;
}

void
free_system
(struct system* this)
{
	free_bootrom(this->bootrom);
	free_cart(this->cart);
	free_cpu(this->cpu);
	free_mem(this->mem);
	free(this);
}

void
reset_system
(struct system* this)
{
	/*audio_reset(system->audio);*/
	reset_cpu(this->cpu);
	reset_mem(this->mem);
	/*ppu_reset(system->ppu);*/
}
