#include "debug.h"
#include "types.h"

#include "core/cart.h"
#include "core/cartinfo.h"
#include "core/system.h"
#include "backend/backend.h"
#include "backend/event.h"
#include "backend/window.h"

#include <stdlib.h>
#include <stdio.h>

/* Emulation */
struct window* cdmg_window;
struct system* cdmg_system;

/* Flags */
bool bootrom = false;
int bootrom_arg = -1;
bool header_only = false;

int
header_printout
(char* path)
{
	struct cart* cdmg_cart;
	cdmg_cart = new_cart(path);
	if (cdmg_cart == NULL)
		return -1;

	/* TODO: Print cart checksums */
	printf("%s\nTYPE: %s\nREGION: %s\nROM: %d\nRAM: %d\nCGB: %s\nSGB: %s\n",
		   cdmg_cart->title,
		   cart_type_name(cdmg_cart->type),
		   cart_region_code(cdmg_cart->destination_code),
		   cdmg_cart->size_rom,
		   cdmg_cart->size_ram,
		   cart_cgb_flag(cdmg_cart->cgb_flag),
		   cart_sgb_flag(cdmg_cart->sgb_flag));
	return 0;
}

int
main
(int argc, char* argv[])
{
	int i;
	if (argc < 2) {
		printf("Please specify a game rom.\n");
		goto err_before_alloc;
	}

	/* Parse arguments */
	if (argc > 2) {
		for (i = 1; i < argc - 1; ++i) {
			/* Sanity check */
			if (argv[i][0] != '-' || argv[i][1] == '\0') {
				printf("Argument %d is malformed.\n", i);
				goto err_before_alloc;
			}

			switch (argv[i][1]) {
			/* Header only */
			case 'h':
				header_only = true;
				continue;
			case 'b':
				bootrom = true;
				/* Read a bootrom here */
				continue;
			}
		}
	}

	/* Startup loading */
	if (header_only == true) {
		if (header_printout(argv[argc-1]) == 0)
			return 0;
		else
			goto err_before_alloc;
	}

	if (init_backend() == false)
		goto err_before_alloc;

	cdmg_window = new_window(WINDOW_STD_WIDTH, WINDOW_STD_HEIGHT, "CDMG");
	if (cdmg_window == NULL)
		goto err_at_window;

	printf("Window object created\n");

	cdmg_system = new_system(argv[argc-1], NULL);
	if (cdmg_system == NULL)
		goto err_at_system;

	printf("System object created\n");

	enum event_type e;
	while (e != NOTIFY_QUIT) {
		e = event_key_get();
		if (e != NONE)
			printf("%d ", e);
	}
	
	system_step(cdmg_system);

	/* TODO: Fix cart_checksum functions in system/cart.c 
	if(checksum_system(cdmg_system)) printf("Cart checksum success.\n");
	else printf("Cart checksum failure.\n");
	*/

	free_system(cdmg_system);
	free_window(cdmg_window);
	exit_backend();
	return 0;

err_at_system:
	free_window(cdmg_window);
err_at_window:
	exit_backend();
err_before_alloc:
	printf("CDMG fatal error, exiting.\n");
	return -1;
}
