#ifndef INCLUDE_CORE_CART_H
#define INCLUDE_CORE_CART_H

#include "debug.h"	
#include "types.h"
#include <stdlib.h>
	
enum cart_type {
	ROM =       0x00,
	ROM_R =     0x08,
	ROM_RB =    0x09,
	MBC1 =      0x01,
	MBC1_R =    0x02,
	MBC1_RB =   0x03,
	MBC2 =      0x05,
	MBC2_B =    0x06,
	MBC3 =      0x11,
	MBC3_BT =   0x0F,
	MBC3_RBT =  0x10,
	MBC4 =      0x15,
	MBC4_R =    0x16,
	MBC4_RB =   0x1B,
	MBC5 =      0x19,
	MBC5_R =    0x1A,
	MBC5_RB =   0x1B,
	MBC5_P =    0x1C,
	MBC5_RP =   0x1D,
	MBC5_RBP =  0x1E,
	MBC6 =      0x20,
	MBC7_RBPS = 0x22,
	MMM01 =     0x0B,
	MMM01_R =   0x0C,
	MMM01_RB =  0x0D,
	CAMERA =    0xFC,
	TAMA5 =     0xFD,
	HUC1_RB =   0xFF,
	HUC3 =      0xFE
};

struct cart {
	u8 *byte;
	size_t size;
	enum cart_type type;

	/* Header information */
	char title[17];
	u8 cgb_flag, sgb_flag, destination_code, version, header_checksum;
	u32 size_rom, size_ram;
};

/* Possibly include read/write functions, overriding a function pointer
 * based on if it is MBC1, MBC2, Standard etc... */
extern struct cart* new_cart(const char *path);
extern void free_cart(struct cart* this);
extern void cart_reset(struct cart* this);
	
extern bool cart_checksum(struct cart* this);
extern bool cart_checksum_header(struct cart* this);

#endif
