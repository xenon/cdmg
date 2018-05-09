#include "debug.h"
#include "types.h"

#include "core/system.h"
#include "backend/backend.h"
#include "backend/event.h"
#include "backend/window.h"

#include <stdlib.h>
#include <stdio.h>

struct window* cdmg_window;
struct system* cdmg_system;

int
main
(int argc, char* argv[])
{
	if (argc < 2) {
		printf("Please specify a game rom.\n");
		goto err_before_alloc;
	}

	if (init_backend() == false)
		goto err_before_alloc;

	cdmg_window = new_window(WINDOW_STD_WIDTH, WINDOW_STD_HEIGHT, "CDMG");
	if (cdmg_window == NULL)
		goto err_at_window;

	printf("Window object created\n");

	cdmg_system = new_system(argv[1], NULL);
	if (cdmg_system == NULL)
		goto err_at_system;

	printf("System object created\n");

	/*
	enum event_type e;
	while (e != NOTIFY_QUIT) {
		e = event_key_get();
		if (e != NONE)
			printf("%d ", e);
			}*/
	
	step_system(cdmg_system);

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
	printf("CDMG Fatal Error, exiting.\n");
	return -1;
}
