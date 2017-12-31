#ifndef INCLUDE_CORE_MEMFN_H
#define INCLUDE_CORE_MEMFN_H

	#include "debug.h"
	#include "types.h"

	/* INSTRUCTION STRUCT DEFINES */
	#define lobyte b.x
	#define hibyte b.y

	union conversion {
		struct { u8 x, y; } b;
		u16 word;
	};

#endif
