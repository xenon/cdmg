#ifndef INCLUDE_CONSTANTS_H
#define INCLUDE_CONSTANTS_H

#define GB_CLOCK_FREQ  4194304

#define GB_CYCLES_FRAME 69905 /* We lose 4 cycles here... */

#define GB_TIMER_FREQ0 4096
#define GB_TIMER_FREQ1 262144
#define GB_TIMER_FREQ2 65536
#define GB_TIMER_FREQ3 16384

enum {
	ADR_INT_VBLANK  = 0x0040,
	ADR_INT_LCDSTAT = 0x0048,
	ADR_INT_TIMER   = 0x0050,
	ADR_INT_SERIAL  = 0x0058,
	ADR_INT_JOYPAD  = 0x0060,

	ADR_ENTRY_POINT   = 0x0100,
	ADR_CART_TITLE    = 0x0134,
	ADR_CART_CGB_FLAG = 0x0143,
	ADR_CART_LICN     = 0x0144,
	ADR_CART_SGB_FLAG = 0x0146,
	ADR_CART_TYPE     = 0x0147,
	ADR_CART_ROM_SIZE = 0x0148,
	ADR_CART_RAM_SIZE = 0x0149,
	ADR_CART_DESTINAT = 0x014A,
	ADR_CART_OLD_LICN = 0x014B,
	ADR_CART_MASK_VER = 0x014C,
	ADR_CART_CHK_HEAD = 0x014D,
	ADR_CART_CHK_GLOB = 0x014E,

	ADR_ECHORAM = 0xE000,

	ADR_OAM = 0xFE00,

	ADR_UNUSABLE = 0xFEA0,

	ADR_JOYPAD = 0xFF00,

	ADR_SERIAL_TD = 0xFF01,
	ADR_SERIAL_TC = 0xFF02,

	ADR_TIMER_DIV  = 0xFF04,
	ADR_TIMER_TIMA = 0xFF05,
	ADR_TIMER_TMA  = 0xFF06,
	ADR_TIMER_TMC  = 0xFF07,

	ADR_APU_1_SW     = 0xFF10,
	ADR_APU_1_LWP    = 0xFF11,
	ADR_APU_1_VE     = 0xFF12,
	ADR_APU_1_FLO    = 0xFF13,
	ADR_APU_1_FHI    = 0xFF14,
	ADR_APU_2_LWP    = 0xFF16,
	ADR_APU_2_VE     = 0xFF17,
	ADR_APU_2_FLO    = 0xFF18,
	ADR_APU_2_FHI    = 0xFF19,
	ADR_APU_3_ON     = 0xFF1A,
	ADR_APU_3_L      = 0xFF1B,
	ADR_APU_3_SOL    = 0xFF1C,
	ADR_APU_3_FLO    = 0xFF1D,
	ADR_APU_3_FHI    = 0xFF1E,
	ADR_APU_4_L      = 0xFF20,
	ADR_APU_4_VE     = 0xFF21,
	ADR_APU_4_POLYC  = 0xFF22,
	ADR_APU_4_COUNT  = 0xFF23,
	ADR_APU_CCONTROL = 0xFF24,
	ADR_APU_SOT      = 0xFF25,
	ADR_APU_ON       = 0xFF26,

	ADR_APU_WPRAM = 0xFF30,

	ADR_LCD_C    = 0xFF40,
	/* LCD CONTROL BITS:
	   7 - Enable
	   6 - Window Tilemap address
	   5 - Window enable
	   4 - BG and Window Tile Data
	   3 - BG Tilemap address
	   2 - OBJ Size
	   1 - OBJ Enable
	   0 - BG Enable */
	ADR_LCD_STAT = 0xFF41,
	/* LCD STATUS BITS:
	   6 - LYC=LY Interrupt
	   5 - Mode 2 OAM Interrupt
	   4 - Mode 1 V-Blank Interrupt
	   3 - Mode 0 H-Blank Interrupt
	   2 - LYC=LY Flag
	   1-0 - Mode */
	ADR_LCD_SCY  = 0xFF42,
	ADR_LCD_SCX  = 0xFF43,
	ADR_LCD_LY   = 0xFF44, /* Read only */
	ADR_LCD_LYC  = 0xFF45,
	ADR_DMA_OAM  = 0xFF46,
	ADR_BGP      = 0xFF47,
	/* COLOR PALETTE BITS: where 00=white, 01=lgrey, 10=dgrey, 11=black
	   6-7 - Color for 11
	   4-5 - Color for 10
	   2-3 - Color for 01
	   0-1 - Color for 00 */
	ADR_OBP0     = 0xFF48,
	ADR_OBP1     = 0xFF49,
	ADR_LCD_WY   = 0xFF4A,
	ADR_LCD_WX   = 0xFF4B,

	ADR_IF = 0xFF0F,

	ADR_ZEROPAGE = 0xFF80,

	ADR_IE = 0xFFFF
};

#endif
