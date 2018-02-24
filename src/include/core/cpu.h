#ifndef INCLUDE_CORE_CPU_H
#define INCLUDE_CORE_CPU_H

#include "debug.h"	
#include "types.h"

struct mem;

struct instruction {
	u8 opcode;
	union {
		union {
			struct { u8 x, y; } u;
			struct { s8 x, y; } s;
		} b;
		u16 word;
	} o;
};

union result {
	struct { u8 w, x, y, z; } b;
	struct { u16 x, y; } w;
	u32 words;
};

struct cpu {
	struct mem* mem;

	struct instruction instr;
	union result res;
	u16 reg_pc, reg_sp;

	union {
		u16 xy;
		struct {
			u8 x, y;
		} s;
	} pair[4];

	/*struct {
		u8 a, f, b, c, d, e, h, l;
		} back;*/

	bool halt, halt_bug, hang, ime, ime_pending, stop;
};

extern struct cpu* new_cpu(struct mem* mem);
extern void free_cpu(struct cpu* this);

/* TODO SAVESTATE IMPLEMENTATION
	extern bool loadstate_cpu();
	extern savestate_cpu() */
extern void reset_cpu(struct cpu* this);
extern void cpu_run(struct cpu* this, u32 times);
extern void cpu_step(struct cpu* this);

#endif
