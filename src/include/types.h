#ifndef INCLUDE_TYPES_H
#define INCLUDE_TYPES_H

	/* Type definition for portability, ease of use and readability.
	 * s = signed, u = unsigned, 8 = 8 bits, 16 = 16 bits, 32 = 32 bits.
	 * If using C99 or newer stdint.h types are used. 
	 * Otherwise types are defined as in C89. */

	/* The function main should always return an int and not one of these! */

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
	
		#define bool int
		#define true 1
		#define false 0

	#endif

#endif
