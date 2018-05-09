#include "core/ppu.h"

/* PPU Modes */
enum {
	MODE_HBLANK = 0,
	MODE_VBLANK = 1,
	MODE_OAM = 2,
	MODE_TRANSFER = 3
};

/* LCD Control Bits */
enum {
	LCDC_BG_DISPLAY      = 1,
	LCDC_OBJ_ENABLE      = 1 << 1,
	LCDC_OBJ_SIZE        = 1 << 2,
	LCDC_TILEMAP_SELECT  = 1 << 3,
	LCDC_TILEDATA_SELECT = 1 << 4,
	LCDC_WINDOW_ENABLE   = 1 << 5,
	LCDC_WINDOW_SELECT   = 1 << 6,
	LCDC_DISPLAY_ENABLE  = 1 << 7
};

/* STAT Bits */
enum {
	STAT_MODE_LO         = 1,
	STAT_MODE_HI         = 1 << 1,
	STAT_COINCIDENCE     = 1 << 2,
	STAT_INT0_HBLANK     = 1 << 3,
	STAT_INT1_VBLANK     = 1 << 4,
	STAT_INT2_OAM        = 1 << 5,
	STAT_INT_COINCIDENCE = 1 << 6
};

/* Color Names */
enum {
	COLOR_WHITE = 0,
	COLOR_LGRAY = 1,
	COLOR_DGRAY = 2,
	COLOR_BLACK = 3
};

/* OAM Attributes */
enum {
	ATTRIBUTE_PALETTE        = 1 << 4,
	ATTRIBUTE_XFLIP          = 1 << 5,
	ATTRIBUTE_YFLIP          = 1 << 6,
	ATTRIBUTE_OBJBG_PRIORITY = 1 << 7
};

struct ppu*
new_ppu
(void)
{
	
}

void
free_ppu
(struct ppu* this)
{
	
}

void
reset_ppu
(struct ppu* this)
{
	
}

void
ppu_step
(struct ppu* this)
{
	
}
