#ifndef INCLUDE_CORE_BITFN_H
#define INCLUDE_CORE_BITFN_H

	#define BIT_VAL(x, val) (x & val ? 1 : 0)
	#define VAL_SET0(x, val) do { x &= (0xFF - val); } while(0)
	#define VAL_SET1(x, val) do { x |= val; } while(0)
	#define VAL_FLIP(x, val) do { x ^= val; } while(0)

#endif
