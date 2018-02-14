#include "debug.h"
#include "types.h"

#include "core/system.h"
#include "media/frontend.h"
#include "media/event.h"
#include "media/window.h"

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

	if (init_libraries() == false)
		goto err_before_alloc;

	cdmg_window = new_window(window_std_width, window_std_height, "CDMG");
	if (cdmg_window == NULL)
		goto err_at_window;

	printf("Window object created\n");

	cdmg_system = new_system(argv[0], "");
	if (cdmg_system == NULL)
		goto err_at_system;

	printf("System object created\n");

	enum event_type e;
	while (e != QUIT) {
		e = event_key_get();
		if (e != NONE)
			printf("%d ", e);
	}

	free_system(cdmg_system);
	free_window(cdmg_window);
	exit_libraries();
	return 0;

err_at_system:
	free_window(cdmg_window);
err_at_window:
	exit_libraries();
err_before_alloc:
	return -1;
}
