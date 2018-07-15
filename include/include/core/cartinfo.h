#ifndef INCLUDE_CORE_CARTINFO_H
#define INCLUDE_CORE_CARTINFO_H

#include "types.h"

extern const char* cart_type_name(u16 index);
extern const char* cart_cgb_flag(u8 flag);
extern const char* cart_sgb_flag(u8 flag);
extern const char* cart_region_code(u8 region);

#endif
