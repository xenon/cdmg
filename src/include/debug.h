#ifndef INCLUDE_DEBUG_H
#define INCLUDE_DEBUG_H

/* Use the macro function ASSERT(exp) whenever necessary.
* The assertion is safely removed if not debugging. */

#if CDMG_DEBUG
	#include <assert.h>
	#define ASSERT(exp) do { assert(exp); } while(0)
#else
	/* ((void)0) is effectively a NOP or No Operation. */
	#define ASSERT(exp) ((void)0)
#endif

#endif
