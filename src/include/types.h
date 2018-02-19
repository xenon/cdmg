#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

/* Type definition for portability, ease of use and readability.
 * If using C99 or newer stdint.h types are used.
 * Otherwise types are defined as in C89. */

#if __STDC_VERSION__ >= 199901L
	#include <stdint.h>
	typedef int8_t 			s8;
	typedef uint8_t 		u8;
	typedef int16_t 		s16;
	typedef uint16_t 		u16;
	typedef int32_t 		s32;
	typedef uint32_t 		u32;

	#include <stdbool.h>
		
#else
	typedef signed char 	s8;
	typedef unsigned char 	u8;
	typedef signed short	s16;
	typedef unsigned short	u16;
	typedef signed long 	s32;
	typedef unsigned long	u32;

	#define bool char
	#define true 1
	#define false 0

#endif

#endif
