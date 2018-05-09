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
	return this;

err_at_cpu:
	free_mem(this->mem);
err_at_mem:
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
	/*audio_reset(system->audio);*/
	reset_cpu(this->cpu);
	reset_mem(this->mem);
	/*ppu_reset(system->ppu);*/
}

void
step_system
(struct system* this)
{
	cpu_run(this->cpu, 256);
}

bool
checksum_system
(struct system* this)
{
	return cart_checksum_header(this->cart);
}
