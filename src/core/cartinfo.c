#include "core/cartinfo.h"

static const char* type_name[32] = {
	"ROM ONLY",
	"MBC1",
	"MBC1 RAM",
	"MBC1 RAM BATTERY",
	"MBC2",
	"MBC2 BATTERY",
	"ROM RAM",
	"ROM RAM BATTERY",
	"MMM01",
	"MMM01 RAM",
	"MMM01 RAM BATTERY",
	"MBC3 BATTERY TIMER",
	"MBC3 RAM BATTERY TIMER",
	"MBC3",
	"MBC3 RAM",
	"MBC3 RAM BATTERY",
	"MBC4",
	"MBC4 RAM",
	"MBC4 RAM BATTERY",
	"MBC5",
	"MBC5 RAM",
	"MBC5 RAM BATTERY",
	"MBC5 RUMBLE",
	"MBC5 RAM RUMBLE",
	"MBC5 RAM BATTERY RUMBLE",
	"MBC6",
	"MBC7 RAM BATTERY RUMBLE SENSOR",
	"CAMERA",
	"TAMA5",
	"HUC3",
	"HUC1 RAM BATTERY",
	"UNDEFINED"
};

static const char* cgb_flag[3] = {
	"UNDEFINED",
	"CGB SUPPORTED",
	"CGB ONLY"
};

static const char* sgb_flag[2] = {
	"SGB DISABLED",
	"SGB SUPPORTED"
};

static const char* region_code[3] = {
	"JAPANESE OR UNDEFINED",
	"NON-JAPANESE",
	"UNDEFINED"
};

const char* cart_type_name(u16 index)
{
	switch (index) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
		return type_name[index];
	case 0x05:
	case 0x06:
		return type_name[index - 1];
	case 0x08:
	case 0x09:
		return type_name[index - 2];
	case 0x0B:
	case 0x0C:
	case 0x0D:
		return type_name[index - 3];
	case 0x0F:
	case 0x10:
	case 0x11:
	case 0x12:
	case 0x13:
		return type_name[index - 4];
	case 0x15:
	case 0x16:
	case 0x17:
		return type_name[index - 5];
	case 0x19:
	case 0x1A:
	case 0x1B:
	case 0x1C:
	case 0x1D:
	case 0x1E:
		return type_name[index - 6];
	case 0x20:
		return type_name[index - 7];
	case 0x22:
		return type_name[index - 8];
	case 0xFC:
	case 0xFD:
	case 0xFE:
	case 0xFF:
		return type_name[27 + (index - 0xFC)];
	default:
		return type_name[31];
	}
}

const char* cart_cgb_flag(u8 flag)
{
	if (flag == 0x80) {
		return cgb_flag[1];
	} else if (flag == 0xC0) {
		return cgb_flag[2];
	} else {
		return cgb_flag[0];
	}
}

const char* cart_sgb_flag(u8 flag)
{
	if (flag == 0x03)
		return sgb_flag[1];
	else
		return sgb_flag[0];
}

const char* cart_region_code(u8 region)
{
	if (region > 1)
		return region_code[2];
	else
		return region_code[region];
}
