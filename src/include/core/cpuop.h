#ifndef INCLUDE_CORE_CPUOP_H
#define INCLUDE_CORE_CPUOP_H

	/* Place all CPU Opcode information here including
	 * Opcode length and cycles, mnemonic chars 
	 * May also be able to place cpu functions in here
	 * if made correctly! */
	
	#include "types.h"
	
	/* Note: cb_cycle contains 16 items because cb values repeat
	 * themselves every line. So to get the proper index just use
	 * opcode_number & 0x0F as the index to the array */

	extern const u8 op_width[256];
	extern const u8 op_cycle[256];
	extern const u8 cb_cycle[16];

	extern const char* op_name[256];
	extern const char* cb_name[256];

#endif
