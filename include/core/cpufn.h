#ifndef INCLUDE_CORE_CPUFN_H
#define INCLUDE_CORE_CPUFN_H
	
#include "debug.h"
#include "types.h"
#include "core/bitfn.h"
#include "core/cpu.h"
#include "core/mem.h"

/* CPU STRUCT DEFINES */
#define reg_af pair[0].xy
#define reg_a  pair[0].s.x
#define reg_f  pair[0].s.y
#define reg_bc pair[1].xy
#define reg_b  pair[1].s.x
#define reg_c  pair[1].s.y
#define reg_de pair[2].xy
#define reg_d  pair[2].s.x
#define reg_e  pair[2].s.y
#define reg_hl pair[3].xy
#define reg_h  pair[3].s.x
#define reg_l  pair[3].s.y
#define reg_back_a back.a
#define reg_back_f back.f
#define reg_back_b back.b
#define reg_back_c back.c
#define reg_back_d back.d
#define reg_back_e back.e
#define reg_back_h back.h
#define reg_back_l back.l

/* INSTRUCTION STRUCT DEFINES */
#define ubyte2 o.b.u.x
#define ubyte3 o.b.u.y
#define sbyte2 o.b.s.x
#define sbyte3 o.b.s.y
#define word   o.word

/* RESULT UNION DEFINES */
#define byte1 b.w
#define byte2 b.x
#define byte3 b.y
#define byte4 b.z
#define word1 w.x
#define word2 w.y

/* FLAG VALUES FOR BIT OPERATIONS */
#define FLAG_C 0x10
#define FLAG_H 0x20
#define FLAG_N 0x40
#define FLAG_Z 0x80

/* SPECIAL */
static inline void
stop
(struct cpu* this)
{
	this->stop = true;
}

static inline void
halt
(struct cpu* this)
{
	this->halt = true;
	if (!(this->ime) && ((mem_rb(this->mem, ADR_IF) & mem_rb(this->mem, ADR_IE) & 0x1F) != 0)) {
		this->halt = false;
		this->halt_bug = true;
	}
}

/* JUMP AND CALL */
static inline void
call_a16
(struct cpu* this)
{
	this->reg_sp -= 2;
	mem_ww(this->mem, this->reg_sp, this->reg_pc);
	this->reg_pc = this->instr.word;
}

static inline bool
call_f
(struct cpu* this, bool expression)
{
	if (expression) {
		this->reg_sp -= 2;
		mem_ww(this->mem, this->reg_sp, this->reg_pc);
		this->reg_pc = this->instr.word;
		return true;
	}
	return false;
}

static inline bool
jmp_a
(struct cpu* this, bool expression)
{
	if (expression) {
		this->reg_pc = this->instr.opcode;
		return true;
	}
	return false;
}

static inline bool
jmp_f
(struct cpu* this, bool expression)
{
	if (expression) {
		this->reg_pc += this->instr.sbyte2;
		return true;
	}
	return false;
}

static inline void
jmp_hl
(struct cpu* this)
{
	this->reg_pc = this->reg_hl;
}

static inline void
ret
(struct cpu* this)
{
	this->reg_pc = mem_rw(this->mem, this->reg_sp);
	this->reg_sp += 2;
}

static inline void
reti
(struct cpu* this)
{
	this->reg_pc = mem_rw(this->mem, this->reg_sp);
	this->reg_sp += 2;
	this->ime = true;
}

static inline bool
ret_f
(struct cpu* this, bool expression)
{
	if (expression) {
		this->reg_pc = mem_rw(this->mem, this->reg_sp);
		this->reg_sp += 2;
		return true;
	}
	return false;
}

static inline void
rst
(struct cpu* this, u16 value)
{
	this->reg_sp -= 2;
	mem_ww(this->mem, this->reg_sp, this->reg_pc);
	this->reg_pc = value;
}

/* 16BIT LOAD, STORE, MOVE */
static inline u16
pop
(struct cpu* this)
{
	this->reg_sp += 2;
	return mem_rw(this->mem, this->reg_sp - 2);
}

static inline void
push
(struct cpu* this, u16 value)
{
	mem_ww(this->mem, this->reg_sp, value);
	this->reg_sp -= 2;
}

static inline void
ld_hl_sp
(struct cpu* this)
{	
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	this->res.words = this->reg_sp + this->instr.sbyte2;
	if (this->res.word2 == 0)
		VAL_SET0(this->reg_f, FLAG_C);
	else
		VAL_SET1(this->reg_f, FLAG_C);

	if ((this->res.word1 & 0x0F) + (this->instr.sbyte2 & 0x0F) > 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);

	this->reg_hl = this->res.word1;
}

/* 8BIT ARITHMETIC AND LOGICAL */
static inline u8
inc
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	if ((value & 0x0F) == 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);
	
	++value;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
	
	return value;
}

static inline u8
dec
(struct cpu* this, u8 value)
{
	VAL_SET1(this->reg_f, FLAG_N);
	if (value & 0x0F)
		VAL_SET0(this->reg_f, FLAG_H);
	else
		VAL_SET1(this->reg_f, FLAG_H);
	
	--value;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
	
	return value;
}

static inline void
daa
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.word1 = this->reg_a;
	if (this->reg_f & FLAG_N) {
		if (this->reg_f & FLAG_H)
			this->res.word1 = (this->res.word1 - 6) & 0x0F;
		if (this->reg_f & FLAG_C)
			this->res.word1 -= 0x60;
	} else {
		if (this->reg_f & FLAG_H || (this->res.word1 & 0x0F) > 9)
			this->res.word1 += 0x06;
		if (this->reg_f & FLAG_C || this->res.word1 > 0x9F)
			this->res.word1 += 0x60;
	}

	if (this->res.word1 > 0xFF)
		VAL_SET1(this->reg_f, FLAG_C);
	
	if (this->res.word1 == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	this->reg_a = this->res.byte1;
}

static inline void
cpl
(struct cpu* this)
{
	VAL_SET1(this->reg_f, FLAG_N);
	VAL_SET1(this->reg_f, FLAG_H);
	this->reg_a = ~this->reg_a;
}

static inline void
scf
(struct cpu* this)
{	
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	VAL_SET1(this->reg_f, FLAG_C);
}

static inline void
ccf
(struct cpu* this)
{	
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	VAL_FLIP(this->reg_f, FLAG_C);
}

static inline void
add
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	this->res.word1 = this->reg_a + value;
	if (this->res.byte2)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if (this->res.byte1 == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	if ((this->res.byte1 & 0x0F) + (value & 0x0F) > 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);
	
	this->reg_a = this->res.byte1;
}

static inline void
adc
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);	
	this->res.word1 = this->reg_a + value + BIT_VAL(this->reg_f, FLAG_C);
	if (this->res.byte2)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if (this->res.byte1 == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	if ((this->res.byte1 & 0x0F) + 
	    ((value + BIT_VAL(this->reg_f, FLAG_C)) & 0x0F) > 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);
	
	this->reg_a = this->res.byte1;
}

static inline void
sub
(struct cpu* this, u8 value)
{
	VAL_SET1(this->reg_f, FLAG_N);
	if (this->reg_a < value)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if ((this->reg_a & 0x0F) < (value & 0x0F))
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);

	this->reg_a -= value;
	if (this->reg_a == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
}

static inline void
sbc
(struct cpu* this, u8 value)
{
	VAL_SET1(this->reg_f, FLAG_N);
	value = value + BIT_VAL(this->reg_f, FLAG_C);
	if (this->reg_a < value)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if ((this->reg_a & 0x0F) < (value & 0x0F))
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);

	this->reg_a -= value;
	if (this->reg_a == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
}

static inline void
and
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET1(this->reg_f, FLAG_H);
	VAL_SET0(this->reg_f, FLAG_C);
	this->reg_a &= value;
	if (this->reg_a == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
}

static inline void
xor
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	VAL_SET0(this->reg_f, FLAG_C);
	this->reg_a ^= value;
	if (this->reg_a == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
}

static inline void
or
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	VAL_SET0(this->reg_f, FLAG_C);
	this->reg_a |= value;
	if (this->reg_a == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);
}

static inline void
cp
(struct cpu* this, u8 value)
{
	VAL_SET1(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_C);
	if (this->reg_a == value) {
		VAL_SET1(this->reg_f, FLAG_Z);
	} else {
		VAL_SET0(this->reg_f, FLAG_Z);
		if (this->reg_a < value)
			VAL_SET1(this->reg_f, FLAG_C);
	}
	
	if ((this->reg_a & 0x0F) < (value & 0x0F))
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);
}

/* 16BIT ARITHMETIC AND LOGICAL */
static inline void
add_hl
(struct cpu* this, u16 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	this->res.words = this->reg_hl + value;
	if (this->res.word2 > 0)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if ((this->res.word1 & 0x0F) + (value & 0x0F) > 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);
}

static inline void
add_sp
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	this->res.words = this->reg_sp + this->instr.sbyte2;
	if (this->res.word2 > 0)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if ((this->res.word1 & 0x0F) + (this->instr.sbyte2 & 0x0F) > 0x0F)
		VAL_SET1(this->reg_f, FLAG_H);
	else
		VAL_SET0(this->reg_f, FLAG_H);

	this->reg_sp = this->res.word1;
}

/* 8BIT ROTATIONS AND SHIFTS */
static inline void
rlca
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = BIT_VAL(this->reg_a, 0x80);
	if (this->res.byte1 == 0)
		VAL_SET0(this->reg_f, FLAG_C);
	else
		VAL_SET1(this->reg_f, FLAG_C);

	this->reg_a <<= 1;
	this->reg_a += this->res.byte1;
}

static inline void
rrca
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = this->reg_a & 0x01;
	if (this->res.byte1 == 0)
		VAL_SET0(this->reg_f, FLAG_C);
	else
		VAL_SET1(this->reg_f, FLAG_C);

	this->reg_a >>= 1;
	if (this->res.byte1)
		this->reg_a |= 0x80;
}

static inline void
rla
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = BIT_VAL(this->reg_f, FLAG_C);
	if (this->reg_a & 0x80)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	this->reg_a <<= 1;
	this->reg_a += this->res.byte1;
}

static inline void
rra
(struct cpu* this)
{
	VAL_SET0(this->reg_f, FLAG_Z);
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = 0x80 * BIT_VAL(this->reg_f, FLAG_C);
	if (this->reg_a & 0x01)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	this->reg_a >>= 1;
	this->reg_a += this->res.byte1;
}

/* CB PREFIX */
static inline u8
rlc
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = (value & 0x80) >> 7;
	if (value & 0x80)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);
	
	value <<= 1;
	value += this->res.byte1;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
rrc
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = (value & 0x01);
	value >>= 1;
	if (this->res.byte1) {
		VAL_SET1(this->reg_f, FLAG_C);
		value |= 0x80;
	} else
		VAL_SET0(this->reg_f, FLAG_C);

	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
rl
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	this->res.byte1 = BIT_VAL(this->reg_f, FLAG_C);
	if (value & 0x80)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	value <<= 1;
	value += this->res.byte1;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
rr
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	value >>= 1;
	if (this->reg_f & FLAG_C)
		value |= 0x80;
	
	if (value & 0x01)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);

	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
sla
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	if (value & 0x80)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);
	
	value <<= 1;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
sra
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	if (value & 0x01)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);
	
	value = (value & 0x80) | (value >> 1);
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
srl
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	if (value & 0x01)
		VAL_SET1(this->reg_f, FLAG_C);
	else
		VAL_SET0(this->reg_f, FLAG_C);
	
	value >>= 1;
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline u8
swap
(struct cpu* this, u8 value)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	VAL_SET0(this->reg_f, FLAG_C);
	value = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
	if (value == 0)
		VAL_SET1(this->reg_f, FLAG_Z);
	else
		VAL_SET0(this->reg_f, FLAG_Z);

	return value;
}

static inline void
bit
(struct cpu* this, u8 value1, u8 bit)
{
	VAL_SET0(this->reg_f, FLAG_N);
	VAL_SET0(this->reg_f, FLAG_H);
	if (value1 & (1 << bit))
		VAL_SET1(this->reg_f, FLAG_C);
	else	
		VAL_SET0(this->reg_f, FLAG_C);
}

static inline u8
res
(u8 value, u8 bit)
{
	return value & ~(1 << bit);
}

static inline u8
set
(u8 value, u8 bit)
{
	return value | (1 << bit);
}

#endif
