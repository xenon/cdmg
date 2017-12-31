#ifndef INCLUDE_DEBUGGER_ARRAY_H_
#define INCLUDE_DEBUGGER_ARRAY_H_

	#include <stdlib.h>
	
	#include "debug.h"
	#include "error.h"
	#include "types.h"

	struct array {
		u16 *item;
		u16 max, min;
		size_t size;
		size_t capacity;
	};
	
	struct array *array_create();
	/* error_code array_double(struct array *a); */
	bool array_contains(struct array *a, u16 item);
	error_code array_delete(struct array *a);
	error_code array_add(struct array *a, u16 item);
	error_code array_remove(struct array *a, u16 item);


#endif
