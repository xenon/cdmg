#ifndef INCLUDE_CORE_BOOTROM_H
#define INCLUDE_CORE_BOOTROM_H

	#include "debug.h"
	#include "types.h"
	#include "memfn.h"

	struct bootrom {
		union conversion con;
		u8 byte[256];
	};

	extern struct bootrom* new_bootrom(const char* path);
	extern void free_bootrom(struct bootrom* this);
	
	static inline u8
	bootrom_rb
	(struct bootrom* this, u16 address)
	{
		return this->byte[address];
	}

	static inline u16
	bootrom_rw
	(struct bootrom* this, u16 address)
	{
		this->con.lobyte = this->byte[address];
		this->con.hibyte = this->byte[address + 1];
		return this->con.word;
	}
	
	static inline void
	bootrom_wb
	(struct bootrom* this, u16 address, u8 data)
	{
		this->byte[address] = data;
	}
	
	static inline void
	bootrom_ww
	(struct bootrom* this, u16 address, u16 data)
	{
		this->con.word = data;
		this->byte[address] = this->con.lobyte;
		this->byte[address + 1] = this->con.hibyte;
	}

#endif
