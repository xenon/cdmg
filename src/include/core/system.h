#ifndef INCLUDE_CORE_SYSTEM_H
#define INCLUDE_CORE_SYSTEM_H

#include "debug.h"	
#include "types.h"

struct system;

extern struct system* new_system(const char* cart_path, const char* bootrom_path);
extern void free_system(struct system* this);
extern void reset_system(struct system* this);

extern void system_step(struct system* this);
extern bool system_checksum(struct system* this);
#endif
