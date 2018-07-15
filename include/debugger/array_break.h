#ifndef INCLUDE_DEBUGGER_ARRAY_BREAKPOINT_H_
#define INCLUDE_DEBUGGER_ARRAY_BREAKPOINT_H_

	#include <stdlib.h>
	
	#include "debug.h"
	#include "types.h"
	
	#include "debugger/array.h"

	/* Breakpoint and Condition Structs */
	struct brk {
		u16 address;
		bool expires;
	};

	extern struct arr_brk* arr_brk_init(size_t size);
	extern void arr_brk_free(struct arr_brk *arr);
	extern void arr_brk_add(struct arr_brk *arr, u16 bp, bool expires);
	extern void arr_brk_remove(struct arr_brk *arr, u16 bp);
	extern void arr_brk_clear(struct arr_brk *arr);

/*	extern struct arr_cnd* arr_cnd_init(size_t size);
	extern void arr_cnd_free(struct arr_cnd *arr);
	extern void arr_cnd_add(struct arr_cnd *arr, u16 bp, bool once);
	extern void arr_cnd_clear(struct arr_cnd *arr);
*/
#endif
