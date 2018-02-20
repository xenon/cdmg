/* TODO
 * 1) IS the CB function 'bit' supposed to set anything or is it void?
 * 2) Make cpu_run work based on cycles and make system give it the cycles per frame
 * 3) 4194304 Mod 60 = 4 (i.e. each frame divides into 69,905 cycles with overall remainder of 4cycles per 60 frames) */

#include "core/cpu.h"
#include "core/cpufn.h"
#include "core/cpuop.h"
#include "core/mem.h"
#include <stdlib.h>

#ifdef CDMG_DEBUG
#include <stdio.h>
#endif

struct cpu*
new_cpu
(struct mem* mem)
{
	struct cpu* this = malloc(sizeof(struct cpu));
	this->mem = mem;
	reset_cpu(this);
	return this;
}

void
free_cpu
(struct cpu* this)
{
	free(this);
}

void
reset_cpu
(struct cpu* this)
{
	ASSERT(this);

	this->reg_a = 0x01;
	this->reg_f = 0xB0;
	this->reg_b = 0x00;
	this->reg_c = 0x13;
	this->reg_d = 0x00;
	this->reg_e = 0xD8;
	this->reg_hl = 0x014D;
	this->reg_sp = 0xFFFE;
	this->reg_pc = 0x0100;
	
	this->halt = false;
	this->hang = false;
	this->ime = false;
	this->ime_pending = false;
	this->stop = false;
}

void
cpu_interrupt
(struct cpu* this, u16 address)
{
	this->instr.word = address;
	call_a16(this);
	/* INCREMENT CYCLES FOR CALL A16 HERE */
}

void
cpu_step
(struct cpu* this)
{
	cpu_run(this, 1);
}

void
cpu_run
(struct cpu* this, u32 times)
{
	u32 cycles = 0;
	ASSERT(this);
	ASSERT(this->mem);

	while(cycles < times) {
	if (this->hang)
		break;
		
	this->instr.opcode = mem_rb(this->mem, this->reg_pc);
	switch (op_width[this->instr.opcode]) {
	case 3: this->instr.ubyte3 = mem_rb(this->mem, this->reg_pc + 2);
	case 2: this->instr.ubyte2 = mem_rb(this->mem, this->reg_pc + 1);
	default:
		cycles += op_cycle[this->instr.opcode];

	#ifdef CDMG_DEBUG
	if (this->instr.opcode != 0xCB)
		printf("%s at %04x, cycle %d\n", this->instr.opcode, op_name[this->instr.opcode], this->reg_pc, cycles);
	else
		printf("%s at %04x, cycle %d\n", cb_name[this->instr.ubyte2], this->reg_pc, cycles);
	#endif
	
		this->reg_pc += op_width[this->instr.opcode];
	}

	if (this->ime_pending) {
		this->ime = true;
		this->ime_pending = false;
	}
	
	
	switch (this->instr.opcode) {	
	/* SPECIAL */
	default: /* Undefined opcode */
		this->hang = true;
		continue;
	case 0x00:
		continue;
	case 0x10:
		stop(this);
		continue;
	case 0x76:
		halt(this);
		continue;
	case 0xF3:
		this->ime = false;
		continue;
	case 0xFB:
		this->ime_pending = true;
		continue;

	/* JUMP AND CALL */
	case 0x18:
		this->reg_pc += this->instr.sbyte2;
		continue;
	case 0xC3:
		this->reg_pc = this->instr.word;
		continue;
	case 0x20:
		if (jmp_f(this, !(this->reg_f & FLAG_Z)))
			cycles += 4;
		continue;
	case 0x28:
		if (jmp_f(this, this->reg_f & FLAG_Z))
			cycles += 4;
		continue;
	case 0x30:
		if (jmp_f(this, !(this->reg_f & FLAG_C)))
			cycles += 4;
		continue;
	case 0x38:
		if (jmp_f(this, this->reg_f & FLAG_C))
			cycles += 4;
		continue;
	case 0xC0:
		if (ret_f(this, !(this->reg_f & FLAG_Z)))
			cycles += 12;
		continue;
	case 0xC8:
		if (ret_f(this, this->reg_f & FLAG_Z))
			cycles += 12;
		continue;
	case 0xD0:
		if (ret_f(this, !(this->reg_f & FLAG_C)))
			cycles += 12;
		continue;
	case 0xD8:
		if (ret_f(this, this->reg_f & FLAG_C))
			cycles += 12;
		continue;
	case 0xC2:
		if (jmp_a(this, !(this->reg_f & FLAG_Z)))
			cycles += 4;
		continue;
	case 0xCA:
		if (jmp_a(this, this->reg_f & FLAG_Z))
			cycles += 4;
		continue;
	case 0xD2:
		if (jmp_a(this, !(this->reg_f & FLAG_C)))
			cycles += 4;
		continue;
	case 0xDA:
		if (jmp_a(this, this->reg_f & FLAG_C))
			cycles += 4;
		continue;
	case 0xC4:
		if (call_f(this, !(this->reg_f & FLAG_Z)))
			cycles += 12;
		continue;
	case 0xCC:
		if (call_f(this, this->reg_f & FLAG_Z))
			cycles += 12;
		continue;
	case 0xD4:
		if (call_f(this, !(this->reg_f & FLAG_C)))
			cycles += 12;
		continue;
	case 0xDC:
		if (call_f(this, this->reg_f & FLAG_C))
			cycles += 12;
		continue;
	case 0xC9:
		ret(this);
		continue;
	case 0xCD:
		call_a16(this);
		continue;
	case 0xD9:
		reti(this);
		continue;
	case 0xE9:
		jmp_hl(this);
		continue;
	case 0xC7:
		rst(this, 0x0000);
		continue;
	case 0xCF:
		rst(this, 0x0008);
		continue;
	case 0xD7:
		rst(this, 0x0010);
		continue;
	case 0xDF:
		rst(this, 0x0018);
		continue;
	case 0xE7:
		rst(this, 0x0020);
		continue;
	case 0xEF:
		rst(this, 0x0028);
		continue;
	case 0xF7:
		rst(this, 0x0030);
		continue;
	case 0xFF:
		rst(this, 0x0038);
		continue;
	
	/* 8BIT LOAD, STORE, MOVE */
	case 0x02:
		mem_wb(this->mem, this->reg_bc, this->reg_a);
		continue;
	case 0x12:
		mem_wb(this->mem, this->reg_de, this->reg_a);
		continue;
	case 0x22:
		mem_wb(this->mem, this->reg_hl, this->reg_a);
		this->reg_hl += 1;
		continue;
	case 0x32:
		mem_wb(this->mem, this->reg_hl, this->reg_a);
		this->reg_hl -= 1;
		continue;
	case 0x0A:
		this->reg_a = mem_rb(this->mem, this->reg_bc);
		continue;
	case 0x1A:
		this->reg_a = mem_rb(this->mem, this->reg_de);
		continue;
	case 0x2A:
		this->reg_a = mem_rb(this->mem, this->reg_hl);
		this->reg_hl += 1;
		continue;
	case 0x3A:
		this->reg_a = mem_rb(this->mem, this->reg_hl);
		this->reg_hl -= 1;
		continue;
	case 0x06:
		this->reg_b = this->instr.ubyte2;
		continue;
	case 0x0E:
		this->reg_c = this->instr.ubyte2;
		continue;
	case 0x16:
		this->reg_d = this->instr.ubyte2;
		continue;
	case 0x1E:
		this->reg_e = this->instr.ubyte2;
		continue;
	case 0x26:
		this->reg_h = this->instr.ubyte2;
		continue;
	case 0x2E:
		this->reg_l = this->instr.ubyte2;
		continue;
	case 0x36:
		mem_wb(this->mem, this->reg_hl, this->instr.ubyte2);
		continue;
	case 0x3E:
		this->reg_a = this->instr.ubyte2;
		continue;
	case 0x40:
		this->reg_b = this->reg_b;
		continue;
	case 0x41:
		this->reg_b = this->reg_c;
		continue;	
	case 0x42:
		this->reg_b = this->reg_d;
		continue;
	case 0x43:
		this->reg_b = this->reg_e;
		continue;
	case 0x44:
		this->reg_b = this->reg_h;
		continue;
	case 0x45:
		this->reg_b = this->reg_l;
		continue;
	case 0x46:
		this->reg_b = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x47:
		this->reg_b = this->reg_a;
		continue;
	case 0x48:
		this->reg_c = this->reg_b;
		continue;
	case 0x49:
		this->reg_c = this->reg_c;
		continue;	
	case 0x4A:
		this->reg_c = this->reg_d;
		continue;
	case 0x4B:
		this->reg_c = this->reg_e;
		continue;
	case 0x4C:
		this->reg_c = this->reg_h;
		continue;
	case 0x4D:
		this->reg_c = this->reg_l;
		continue;
	case 0x4E:
		this->reg_c = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x4F:
		this->reg_c = this->reg_a;
		continue;
	case 0x50:
		this->reg_d = this->reg_b;
		continue;
	case 0x51:
		this->reg_d = this->reg_c;
		continue;	
	case 0x52:
		this->reg_d = this->reg_d;
		continue;
	case 0x53:
		this->reg_d = this->reg_e;
		continue;
	case 0x54:
		this->reg_d = this->reg_h;
		continue;
	case 0x55:
		this->reg_d = this->reg_l;
		continue;
	case 0x56:
		this->reg_d = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x57:
		this->reg_d = this->reg_a;
		continue;
	case 0x58:
		this->reg_e = this->reg_b;
		continue;
	case 0x59:
		this->reg_e = this->reg_c;
		continue;	
	case 0x5A:
		this->reg_e = this->reg_d;
		continue;
	case 0x5B:
		this->reg_e = this->reg_e;
		continue;
	case 0x5C:
		this->reg_e = this->reg_h;
		continue;
	case 0x5D:
		this->reg_e = this->reg_l;
		continue;
	case 0x5E:
		this->reg_e = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x5F:
		this->reg_e = this->reg_a;
		continue;
	case 0x60:
		this->reg_h = this->reg_b;
		continue;
	case 0x61:
		this->reg_h = this->reg_c;
		continue;	
	case 0x62:
		this->reg_h = this->reg_d;
		continue;
	case 0x63:
		this->reg_h = this->reg_e;
		continue;
	case 0x64:
		this->reg_h = this->reg_h;
		continue;
	case 0x65:
		this->reg_h = this->reg_l;
		continue;
	case 0x66:
		this->reg_h = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x67:
		this->reg_h = this->reg_a;
		continue;
	case 0x68:
		this->reg_l = this->reg_b;
		continue;
	case 0x69:
		this->reg_l = this->reg_c;
		continue;	
	case 0x6A:
		this->reg_l = this->reg_d;
		continue;
	case 0x6B:
		this->reg_l = this->reg_e;
		continue;
	case 0x6C:
		this->reg_l = this->reg_h;
		continue;
	case 0x6D:
		this->reg_l = this->reg_l;
		continue;
	case 0x6E:
		this->reg_l = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x6F:
		this->reg_l = this->reg_a;
		continue;
	case 0x70:
		mem_wb(this->mem, this->reg_hl, this->reg_b);
		continue;
	case 0x71:
		mem_wb(this->mem, this->reg_hl, this->reg_c);
		continue;
	case 0x72:
		mem_wb(this->mem, this->reg_hl, this->reg_d);
		continue;	
	case 0x73:
		mem_wb(this->mem, this->reg_hl, this->reg_e);
		continue;
	case 0x74:
		mem_wb(this->mem, this->reg_hl, this->reg_h);
		continue;
	case 0x75:
		mem_wb(this->mem, this->reg_hl, this->reg_l);
		continue;
	case 0x77:
		mem_wb(this->mem, this->reg_hl, this->reg_a);
		continue;
	case 0x78:
		this->reg_a = this->reg_b;
		continue;
	case 0x79:
		this->reg_a = this->reg_c;
		continue;	
	case 0x7A:
		this->reg_a = this->reg_d;
		continue;
	case 0x7B:
		this->reg_a = this->reg_e;
		continue;
	case 0x7C:
		this->reg_a = this->reg_h;
		continue;
	case 0x7D:
		this->reg_a = this->reg_l;
		continue;
	case 0x7E:
		this->reg_a = mem_rb(this->mem, this->reg_hl);
		continue;
	case 0x7F:
		this->reg_a = this->reg_a;
		continue;
	case 0xE0:
		mem_wb(this->mem, (this->instr.ubyte2 + 0xFF00), this->reg_a);
		continue;
	case 0xF0:
		this->reg_a = mem_rb(this->mem, (this->instr.ubyte2 + 0xFF00));
		continue;
	case 0xE2:
		mem_wb(this->mem, (this->reg_c + 0xFF00), this->reg_a);
		continue;
	case 0xF2:
		this->reg_a = mem_rb(this->mem, (this->reg_c + 0xFF00));
		continue;
	case 0xEA:
		mem_wb(this->mem, this->instr.word, this->reg_a);
		continue;
	case 0xFA:
		this->reg_a = mem_rb(this->mem, this->instr.word);
		continue;

	/* 16BIT LOAD, STORE, MOVE */
	case 0x01:
		this->reg_bc = this->instr.word;
		continue;
	case 0x08:
		mem_ww(this->mem, this->instr.word, this->reg_sp);
		continue;
	case 0x11:
		this->reg_de = this->instr.word;
		continue;
	case 0x21:
		this->reg_hl = this->instr.word;
		continue;
	case 0x31:
		this->reg_sp = this->instr.word;
		continue;
	case 0xC1:
		this->reg_bc = pop(this);
		continue;
	case 0xC5:
		push(this, this->reg_bc);
		continue;
	case 0xD1:
		this->reg_de = pop(this);
		continue;
	case 0xD5:
		push(this, this->reg_de);
		continue;
	case 0xE1:
		this->reg_hl = pop(this);
		continue;
	case 0xE5:
		push(this, this->reg_hl);
		continue;
	case 0xF1:
		this->reg_af = pop(this);
		this->reg_f &= 0xF0;
		continue;
	case 0xF5:
		push(this, this->reg_af);
		continue;
	case 0xF8:
		ld_hl_sp(this);
		continue;
	case 0xF9:
		this->reg_sp = this->reg_hl;
		continue;

	/* 8BIT ARITHMETIC AND LOGICAL */
	case 0x04:
		this->reg_b = inc(this, this->reg_b);
		continue;
	case 0x0C:
		this->reg_c = inc(this, this->reg_c);
		continue;
	case 0x14:
		this->reg_d = inc(this, this->reg_d);
		continue;
	case 0x1C:
		this->reg_e = inc(this, this->reg_e);
		continue;
	case 0x24:
		this->reg_h = inc(this, this->reg_h);
		continue;
	case 0x2C:
		this->reg_l = inc(this, this->reg_l);
		continue;
	case 0x3C:
		this->reg_a = inc(this, this->reg_a);
		continue;
	case 0x05:
		this->reg_b = dec(this, this->reg_b);
		continue;
	case 0x0D:
		this->reg_c = dec(this, this->reg_c);
		continue;
	case 0x15:
		this->reg_d = dec(this, this->reg_d);
		continue;
	case 0x1D:
		this->reg_e = dec(this, this->reg_e);
		continue;
	case 0x25:
		this->reg_h = dec(this, this->reg_h);
		continue;
	case 0x2D:
		this->reg_l = dec(this, this->reg_l);
		continue;
	case 0x3D:
		this->reg_a = dec(this, this->reg_a);
		continue;
	case 0x34:
		mem_wb(this->mem, this->reg_hl, inc(this, this->reg_hl));
		continue;
	case 0x35:
		mem_wb(this->mem, this->reg_hl, dec(this, this->reg_hl));
		continue;
	case 0x27:
		daa(this);
		continue;
	case 0x2F:
		cpl(this);
		continue;
	case 0x37:
		scf(this);
		continue;
	case 0x3F:
		ccf(this);
		continue;
	case 0x80:
		add(this, this->reg_b);
		continue;
	case 0x81:
		add(this, this->reg_c);
		continue;
	case 0x82:
		add(this, this->reg_d);
		continue;
	case 0x83:
		add(this, this->reg_e);
		continue;
	case 0x84:
		add(this, this->reg_h);
		continue;
	case 0x85:
		add(this, this->reg_l);
		continue;
	case 0x86:
		add(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0x87:
		add(this, this->reg_a);
		continue;
	case 0xC6:
		add(this, this->instr.ubyte2);
		continue;
	case 0x88:
		adc(this, this->reg_b);
		continue;
	case 0x89:
		adc(this, this->reg_c);
		continue;
	case 0x8A:
		adc(this, this->reg_d);
		continue;
	case 0x8B:
		adc(this, this->reg_e);
		continue;
	case 0x8C:
		adc(this, this->reg_h);
		continue;
	case 0x8D:
		adc(this, this->reg_l);
		continue;
	case 0x8E:
		adc(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0x8F:
		adc(this, this->reg_a);
		continue;
	case 0xCE:
		adc(this,this->instr.ubyte2);
		continue;
	case 0x90:
		sub(this, this->reg_b);
		continue;
	case 0x91:
		sub(this, this->reg_c);
		continue;
	case 0x92:
		sub(this, this->reg_d);
		continue;
	case 0x93:
		sub(this, this->reg_e);
		continue;
	case 0x94:
		sub(this, this->reg_h);
		continue;
	case 0x95:
		sub(this, this->reg_l);
		continue;
	case 0x96:
		sub(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0x97:
		sub(this, this->reg_a);
		continue;
	case 0xD6:
		sub(this, this->instr.ubyte2);
		continue;
	case 0x98:
		sbc(this, this->reg_b);
		continue;
	case 0x99:
		sbc(this, this->reg_c);
		continue;
	case 0x9A:
		sbc(this, this->reg_d);
		continue;
	case 0x9B:
		sbc(this, this->reg_e);
		continue;
	case 0x9C:
		sbc(this, this->reg_h);
		continue;
	case 0x9D:
		sbc(this, this->reg_l);
		continue;
	case 0x9E:
		sbc(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0x9F:
		sbc(this, this->reg_a);
		continue;
	case 0xDE:
		sbc(this, this->instr.ubyte2);
		continue;
	case 0xA0:
		and(this, this->reg_b);
		continue;
	case 0xA1:
		and(this, this->reg_c);
		continue;
	case 0xA2:
		and(this, this->reg_d);
		continue;
	case 0xA3:
		and(this, this->reg_e);
		continue;
	case 0xA4:
		and(this, this->reg_h);
		continue;
	case 0xA5:
		and(this, this->reg_l);
		continue;
	case 0xA6:
		and(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0xA7:
		and(this, this->reg_a);
		continue;
	case 0xE6:
		and(this, this->instr.ubyte2);
		continue;
	case 0xA8:
		xor(this, this->reg_b);
		continue;
	case 0xA9:
		xor(this, this->reg_c);
		continue;
	case 0xAA:
		xor(this, this->reg_d);
		continue;
	case 0xAB:
		xor(this, this->reg_e);
		continue;
	case 0xAC:
		xor(this, this->reg_h);
		continue;
	case 0xAD:
		xor(this, this->reg_l);
		continue;
	case 0xAE:
		xor(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0xAF:
		xor(this, this->reg_a);
		continue;
	case 0xEE:
		xor(this, this->instr.ubyte2);
		continue;
	case 0xB0:
		or(this, this->reg_b);
		continue;
	case 0xB1:
		or(this, this->reg_c);
		continue;
	case 0xB2:
		or(this, this->reg_d);
		continue;
	case 0xB3:
		or(this, this->reg_e);
		continue;
	case 0xB4:
		or(this, this->reg_h);
		continue;
	case 0xB5:
		or(this, this->reg_l);
		continue;
	case 0xB6:
		or(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0xB7:
		or(this, this->reg_a);
		continue;
	case 0xF6:
		or(this, this->instr.ubyte2);
		continue;
	case 0xB8:
		cp(this, this->reg_b);
		continue;
	case 0xB9:
		cp(this, this->reg_c);
		continue;
	case 0xBA:
		cp(this, this->reg_d);
		continue;
	case 0xBB:
		cp(this, this->reg_e);
		continue;
	case 0xBC:
		cp(this, this->reg_h);
		continue;
	case 0xBD:
		cp(this, this->reg_l);
		continue;
	case 0xBE:
		cp(this, mem_rb(this->mem, this->reg_hl));
		continue;
	case 0xBF:
		cp(this, this->reg_a);
		continue;
	case 0xFE:
		cp(this, this->instr.ubyte2);
		continue;
	
	/* 16BIT ARITHMETIC AND LOGICAL */
	case 0x03:
		this->reg_bc += 1;
		continue;
	case 0x13:
		this->reg_de += 1;
		continue;
	case 0x23:
		this->reg_hl += 1;
		continue;
	case 0x33:
		this->reg_sp += 1;
		continue;
	case 0x0B:
		this->reg_bc -= 1;
		continue;
	case 0x1B:
		this->reg_de -= 1;
		continue;
	case 0x2B:
		this->reg_hl -= 1;
		continue;
	case 0x3B:
		this->reg_sp -= 1;
		continue;
	case 0x09:
		add_hl(this, this->reg_bc);
		continue;
	case 0x19:
		add_hl(this, this->reg_de);
		continue;
	case 0x29:
		add_hl(this, this->reg_hl);
		continue;
	case 0x39:
		add_hl(this, this->reg_sp);
		continue;
	case 0xE8:
		add_sp(this);
		continue;

	/* 8BIT ROTATIONS AND SHIFTS */
	case 0x07:
		rlca(this);
		continue;
	case 0x0F:
		rrca(this);
		continue;
	case 0x17:
		rla(this);
		continue;
	case 0x1F:
		rra(this);
		continue;

	/* CB INSTRUCTIONS */
	case 0xCB:
		cycles += cb_cycle[this->instr.ubyte2 & 0x0F];
		switch (this->instr.ubyte2) {
		case 0x00:
			this->reg_b = rlc(this, this->reg_b);
			continue;
		case 0x01:
			this->reg_c = rlc(this, this->reg_c);
			continue;
		case 0x02:
			this->reg_d = rlc(this, this->reg_d);
			continue;
		case 0x03:
			this->reg_e = rlc(this, this->reg_e);
			continue;
		case 0x04:
			this->reg_h = rlc(this, this->reg_h);
			continue;
		case 0x05:
			this->reg_l = rlc(this, this->reg_l);
			continue;
		case 0x06:
			mem_wb(this->mem, this->reg_hl, rlc(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x07:
			this->reg_a = rlc(this, this->reg_a);
			continue;
		case 0x08:
			this->reg_b = rlc(this, this->reg_b);
			continue;
		case 0x09:
			this->reg_c = rlc(this, this->reg_c);
			continue;
		case 0x0A:
			this->reg_d = rrc(this, this->reg_d);
			continue;
		case 0x0B:
			this->reg_e = rrc(this, this->reg_e);
			continue;
		case 0x0C:
			this->reg_h = rrc(this, this->reg_h);
			continue;
		case 0x0D:
			this->reg_l = rrc(this, this->reg_l);
			continue;
		case 0x0E:
			mem_wb(this->mem, this->reg_hl, rrc(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x0F:
			this->reg_a = rrc(this, this->reg_a);
			continue;
		case 0x10:
			this->reg_b = rl(this, this->reg_b);
			continue;
		case 0x11:
			this->reg_c = rl(this, this->reg_c);
			continue;
		case 0x12:
			this->reg_d = rl(this, this->reg_d);
			continue;
		case 0x13:
			this->reg_e = rl(this, this->reg_e);
			continue;
		case 0x14:
			this->reg_h = rl(this, this->reg_h);
			continue;
		case 0x15:
			this->reg_l = rl(this, this->reg_l);
			continue;
		case 0x16:
			mem_wb(this->mem, this->reg_hl, rl(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x17:
			this->reg_a = rl(this, this->reg_a);
			continue;
		case 0x18:
			this->reg_b = rr(this, this->reg_b);
			continue;
		case 0x19:
			this->reg_c = rr(this, this->reg_c);
			continue;
		case 0x1A:
			this->reg_d = rr(this, this->reg_d);
			continue;
		case 0x1B:
			this->reg_e = rr(this, this->reg_e);
			continue;
		case 0x1C:
			this->reg_h = rr(this, this->reg_h);
			continue;
		case 0x1D:
			this->reg_l = rr(this, this->reg_l);
			continue;
		case 0x1E:
			mem_wb(this->mem, this->reg_hl, rr(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x1F:
			this->reg_a = rr(this, this->reg_a);
			continue;
		case 0x20:
			this->reg_b = sla(this, this->reg_b);
			continue;
		case 0x21:
			this->reg_c = sla(this, this->reg_c);
			continue;
		case 0x22:
			this->reg_d = sla(this, this->reg_d);
			continue;
		case 0x23:
			this->reg_e = sla(this, this->reg_e);
			continue;
		case 0x24:
			this->reg_h = sla(this, this->reg_h);
			continue;
		case 0x25:
			this->reg_l = sla(this, this->reg_l);
			continue;
		case 0x26:
			mem_wb(this->mem, this->reg_hl, sla(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x27:
			this->reg_a = sla(this, this->reg_a);
			continue;
		case 0x28:
			this->reg_b = sra(this, this->reg_b);
			continue;
		case 0x29:
			this->reg_c = sra(this, this->reg_c);
			continue;
		case 0x2A:
			this->reg_d = sra(this, this->reg_d);
			continue;
		case 0x2B:
			this->reg_e = sra(this, this->reg_e);
			continue;
		case 0x2C:
			this->reg_h = sra(this, this->reg_h);
			continue;
		case 0x2D:
			this->reg_l = sra(this, this->reg_l);
			continue;
		case 0x2E:
			mem_wb(this->mem, this->reg_hl, sra(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x2F:
			this->reg_a = sra(this, this->reg_a);
			continue;
		case 0x30:
			this->reg_b = swap(this, this->reg_b);
			continue;
		case 0x31:
			this->reg_c = swap(this, this->reg_c);
			continue;
		case 0x32:
			this->reg_d = swap(this, this->reg_d);
			continue;
		case 0x33:
			this->reg_e = swap(this, this->reg_e);
			continue;
		case 0x34:
			this->reg_h = swap(this, this->reg_h);
			continue;
		case 0x35:
			this->reg_l = swap(this, this->reg_l);
			continue;
		case 0x36:
			mem_wb(this->mem, this->reg_hl, swap(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x37:
			this->reg_a = swap(this, this->reg_a);
			continue;
		case 0x38:
			this->reg_b = srl(this, this->reg_b);
			continue;
		case 0x39:
			this->reg_c = srl(this, this->reg_c);
			continue;
		case 0x3A:
			this->reg_d = srl(this, this->reg_d);
			continue;
		case 0x3B:
			this->reg_e = srl(this, this->reg_e);
			continue;
		case 0x3C:
			this->reg_h = srl(this, this->reg_h);
			continue;
		case 0x3D:
			this->reg_l = srl(this, this->reg_l);
			continue;
		case 0x3E:
			mem_wb(this->mem, this->reg_hl, srl(this, mem_rb(this->mem, this->reg_hl)));
			continue;
		case 0x3F:
			this->reg_a = srl(this, this->reg_a);
			continue;
		case 0x40:
			bit(this, this->reg_b, 0);
			continue;
		case 0x41:
			bit(this, this->reg_c, 0);
			continue;
		case 0x42:
			bit(this, this->reg_d, 0);
			continue;
		case 0x43:
			bit(this, this->reg_e, 0);
			continue;
		case 0x44:
			bit(this, this->reg_h, 0);
			continue;
		case 0x45:
			bit(this, this->reg_l, 0);
			continue;
		case 0x46:
			bit(this, mem_rb(this->mem, this->reg_hl), 0);
			continue;
		case 0x47:
			bit(this, this->reg_a, 0);
			continue;
		case 0x48:
			bit(this, this->reg_b, 1);
			continue;
		case 0x49:
			bit(this, this->reg_c, 1);
			continue;
		case 0x4A:
			bit(this, this->reg_d, 1);
			continue;
		case 0x4B:
			bit(this, this->reg_e, 1);
			continue;
		case 0x4C:
			bit(this, this->reg_h, 1);
			continue;
		case 0x4D:
			bit(this, this->reg_l, 1);
			continue;
		case 0x4E:
			bit(this, mem_rb(this->mem, this->reg_hl), 1);
			continue;
		case 0x4F:
			bit(this, this->reg_a, 1);
			continue;
		case 0x50:
			bit(this, this->reg_b, 2);
			continue;
		case 0x51:
			bit(this, this->reg_c, 2);
			continue;
		case 0x52:
			bit(this, this->reg_d, 2);
			continue;
		case 0x53:
			bit(this, this->reg_e, 2);
			continue;
		case 0x54:
			bit(this, this->reg_h, 2);
			continue;
		case 0x55:
			bit(this, this->reg_l, 2);
			continue;
		case 0x56:
			bit(this, mem_rb(this->mem, this->reg_hl), 2);
			continue;
		case 0x57:
			bit(this, this->reg_a, 2);
			continue;
		case 0x58:
			bit(this, this->reg_b, 3);
			continue;
		case 0x59:
			bit(this, this->reg_c, 3);
			continue;
		case 0x5A:
			bit(this, this->reg_d, 3);
			continue;
		case 0x5B:
			bit(this, this->reg_e, 3);
			continue;
		case 0x5C:
			bit(this, this->reg_h, 3);
			continue;
		case 0x5D:
			bit(this, this->reg_l, 3);
			continue;
		case 0x5E:
			bit(this, mem_rb(this->mem, this->reg_hl), 3);
			continue;
		case 0x5F:
			bit(this, this->reg_a, 3);
			continue;
		case 0x60:
			bit(this, this->reg_b, 4);
			continue;
		case 0x61:
			bit(this, this->reg_c, 4);
			continue;
		case 0x62:
			bit(this, this->reg_d, 4);
			continue;
		case 0x63:
			bit(this, this->reg_e, 4);
			continue;
		case 0x64:
			bit(this, this->reg_h, 4);
			continue;
		case 0x65:
			bit(this, this->reg_l, 4);
			continue;
		case 0x66:
			bit(this, mem_rb(this->mem, this->reg_hl), 4);
			continue;
		case 0x67:
			bit(this, this->reg_a, 4);
			continue;
		case 0x68:
			bit(this, this->reg_b, 5);
			continue;
		case 0x69:
			bit(this, this->reg_c, 5);
			continue;
		case 0x6A:
			bit(this, this->reg_d, 5);
			continue;
		case 0x6B:
			bit(this, this->reg_e, 5);
			continue;
		case 0x6C:
			bit(this, this->reg_h, 5);
			continue;
		case 0x6D:
			bit(this, this->reg_l, 5);
			continue;
		case 0x6E:
			bit(this, mem_rb(this->mem, this->reg_hl), 5);
			continue;
		case 0x6F:
			bit(this, this->reg_a, 5);
			continue;
		case 0x70:
			bit(this, this->reg_b, 6);
			continue;
		case 0x71:
			bit(this, this->reg_c, 6);
			continue;
		case 0x72:
			bit(this, this->reg_d, 6);
			continue;
		case 0x73:
			bit(this, this->reg_e, 6);
			continue;
		case 0x74:
			bit(this, this->reg_h, 6);
			continue;
		case 0x75:
			bit(this, this->reg_l, 6);
			continue;
		case 0x76:
			bit(this, mem_rb(this->mem, this->reg_hl), 6);
			continue;
		case 0x77:
			bit(this, this->reg_a, 6);
			continue;
		case 0x78:
			bit(this, this->reg_b, 7);
			continue;
		case 0x79:
			bit(this, this->reg_c, 7);
			continue;
		case 0x7A:
			bit(this, this->reg_d, 7);
			continue;
		case 0x7B:
			bit(this, this->reg_e, 7);
			continue;
		case 0x7C:
			bit(this, this->reg_h, 7);
			continue;
		case 0x7D:
			bit(this, this->reg_l, 7);
			continue;
		case 0x7E:
			bit(this, mem_rb(this->mem, this->reg_hl), 7);
			continue;
		case 0x7F:
			bit(this, this->reg_a, 7);
			continue;
		case 0x80:
			this->reg_b = res(this->reg_b, 0);
			continue;
		case 0x81:
			this->reg_c = res(this->reg_c, 0);
			continue;
		case 0x82:
			this->reg_d = res(this->reg_d, 0);
			continue;
		case 0x83:
			this->reg_e = res(this->reg_e, 0);
			continue;
		case 0x84:
			this->reg_h = res(this->reg_h, 0);
			continue;
		case 0x85:
			this->reg_l = res(this->reg_l, 0);
			continue;
		case 0x86:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 0));
			continue;
		case 0x87:
			this->reg_a = res(this->reg_a, 0);
			continue;
		case 0x88:
			this->reg_b = res(this->reg_b, 1);
			continue;
		case 0x89:
			this->reg_c = res(this->reg_c, 1);
			continue;
		case 0x8A:
			this->reg_d = res(this->reg_d, 1);
			continue;
		case 0x8B:
			this->reg_e = res(this->reg_e, 1);
			continue;
		case 0x8C:
			this->reg_h = res(this->reg_h, 1);
			continue;
		case 0x8D:
			this->reg_l = res(this->reg_l, 1);
			continue;
		case 0x8E:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 1));
			continue;
		case 0x8F:
			this->reg_a = res(this->reg_a, 1);
			continue;
		case 0x90:
			this->reg_b = res(this->reg_b, 2);
			continue;
		case 0x91:
			this->reg_c = res(this->reg_c, 2);
			continue;
		case 0x92:
			this->reg_d = res(this->reg_d, 2);
			continue;
		case 0x93:
			this->reg_e = res(this->reg_e, 2);
			continue;
		case 0x94:
			this->reg_h = res(this->reg_h, 2);
			continue;
		case 0x95:
			this->reg_l = res(this->reg_l, 2);
			continue;
		case 0x96:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 2));
			continue;
		case 0x97:
			this->reg_a = res(this->reg_a, 2);
			continue;
		case 0x98:
			this->reg_b = res(this->reg_b, 3);
			continue;
		case 0x99:
			this->reg_c = res(this->reg_c, 3);
			continue;
		case 0x9A:
			this->reg_d = res(this->reg_d, 3);
			continue;
		case 0x9B:
			this->reg_e = res(this->reg_e, 3);
			continue;
		case 0x9C:
			this->reg_h = res(this->reg_h, 3);
			continue;
		case 0x9D:
			this->reg_l = res(this->reg_l, 3);
			continue;
		case 0x9E:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 3));
			continue;
		case 0x9F:
			this->reg_a = res(this->reg_a, 3);
			continue;
		case 0xA0:
			this->reg_b = res(this->reg_b, 4);
			continue;
		case 0xA1:
			this->reg_c = res(this->reg_c, 4);
			continue;
		case 0xA2:
			this->reg_d = res(this->reg_d, 4);
			continue;
		case 0xA3:
			this->reg_e = res(this->reg_e, 4);
			continue;
		case 0xA4:
			this->reg_h = res(this->reg_h, 4);
			continue;
		case 0xA5:
			this->reg_l = res(this->reg_l, 4);
			continue;
		case 0xA6:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 4));
			continue;
		case 0xA7:
			this->reg_a = res(this->reg_a, 4);
			continue;
		case 0xA8:
			this->reg_b = res(this->reg_b, 5);
			continue;
		case 0xA9:
			this->reg_c = res(this->reg_c, 5);
			continue;
		case 0xAA:
			this->reg_d = res(this->reg_d, 5);
			continue;
		case 0xAB:
			this->reg_e = res(this->reg_e, 5);
			continue;
		case 0xAC:
			this->reg_h = res(this->reg_h, 5);
			continue;
		case 0xAD:
			this->reg_l = res(this->reg_l, 5);
			continue;
		case 0xAE:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 5));
			continue;
		case 0xAF:
			this->reg_a = res(this->reg_a, 5);
			continue;
		case 0xB0:
			this->reg_b = res(this->reg_b, 6);
			continue;
		case 0xB1:
			this->reg_c = res(this->reg_c, 6);
			continue;
		case 0xB2:
			this->reg_d = res(this->reg_d, 6);
			continue;
		case 0xB3:
			this->reg_e = res(this->reg_e, 6);
			continue;
		case 0xB4:
			this->reg_h = res(this->reg_h, 6);
			continue;
		case 0xB5:
			this->reg_l = res(this->reg_l, 6);
			continue;
		case 0xB6:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 6));
			continue;
		case 0xB7:
			this->reg_a = res(this->reg_a, 6);
			continue;
		case 0xB8:
			this->reg_b = res(this->reg_b, 7);
			continue;
		case 0xB9:
			this->reg_c = res(this->reg_c, 7);
			continue;
		case 0xBA:
			this->reg_d = res(this->reg_d, 7);
			continue;
		case 0xBB:
			this->reg_e = res(this->reg_e, 7);
			continue;
		case 0xBC:
			this->reg_h = res(this->reg_h, 7);
			continue;
		case 0xBD:
			this->reg_l = res(this->reg_l, 7);
			continue;
		case 0xBE:
			mem_wb(this->mem, this->reg_hl, res(mem_rb(this->mem, this->reg_hl), 7));
			continue;
		case 0xBF:
			this->reg_a = res(this->reg_a, 7);
			continue;
		case 0xC0:
			this->reg_b = set(this->reg_b, 0);
			continue;
		case 0xC1:
			this->reg_c = set(this->reg_c, 0);
			continue;
		case 0xC2:
			this->reg_d = set(this->reg_d, 0);
			continue;
		case 0xC3:
			this->reg_e = set(this->reg_e, 0);
			continue;
		case 0xC4:
			this->reg_h = set(this->reg_h, 0);
			continue;
		case 0xC5:
			this->reg_l = set(this->reg_l, 0);
			continue;
		case 0xC6:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 0));
			continue;
		case 0xC7:
			this->reg_a = set(this->reg_a, 0);
			continue;
		case 0xC8:
			this->reg_b = set(this->reg_b, 1);
			continue;
		case 0xC9:
			this->reg_c = set(this->reg_c, 1);
			continue;
		case 0xCA:
			this->reg_d = set(this->reg_d, 1);
			continue;
		case 0xCB:
			this->reg_e = set(this->reg_e, 1);
			continue;
		case 0xCC:
			this->reg_h = set(this->reg_h, 1);
			continue;
		case 0xCD:
			this->reg_l = set(this->reg_l, 1);
			continue;
		case 0xCE:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 1));
			continue;
		case 0xCF:
			this->reg_a = set(this->reg_a, 1);
			continue;
		case 0xD0:
			this->reg_b = set(this->reg_b, 2);
			continue;
		case 0xD1:
			this->reg_c = set(this->reg_c, 2);
			continue;
		case 0xD2:
			this->reg_d = set(this->reg_d, 2);
			continue;
		case 0xD3:
			this->reg_e = set(this->reg_e, 2);
			continue;
		case 0xD4:
			this->reg_h = set(this->reg_h, 2);
			continue;
		case 0xD5:
			this->reg_l = set(this->reg_l, 2);
			continue;
		case 0xD6:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 2));
			continue;
		case 0xD7:
			this->reg_a = set(this->reg_a, 2);
			continue;
		case 0xD8:
			this->reg_b = set(this->reg_b, 3);
			continue;
		case 0xD9:
			this->reg_c = set(this->reg_c, 3);
			continue;
		case 0xDA:
			this->reg_d = set(this->reg_d, 3);
			continue;
		case 0xDB:
			this->reg_e = set(this->reg_e, 3);
			continue;
		case 0xDC:
			this->reg_h = set(this->reg_h, 3);
			continue;
		case 0xDD:
			this->reg_l = set(this->reg_l, 3);
			continue;
		case 0xDE:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 3));
			continue;
		case 0xDF:
			this->reg_a = set(this->reg_a, 3);
			continue;
		case 0xE0:
			this->reg_b = set(this->reg_b, 4);
			continue;
		case 0xE1:
			this->reg_c = set(this->reg_c, 4);
			continue;
		case 0xE2:
			this->reg_d = set(this->reg_d, 4);
			continue;
		case 0xE3:
			this->reg_e = set(this->reg_e, 4);
			continue;
		case 0xE4:
			this->reg_h = set(this->reg_h, 4);
			continue;
		case 0xE5:
			this->reg_l = set(this->reg_l, 4);
			continue;
		case 0xE6:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 4));
			continue;
		case 0xE7:
			this->reg_a = set(this->reg_a, 4);
			continue;
		case 0xE8:
			this->reg_b = set(this->reg_b, 5);
			continue;
		case 0xE9:
			this->reg_c = set(this->reg_c, 5);
			continue;
		case 0xEA:
			this->reg_d = set(this->reg_d, 5);
			continue;
		case 0xEB:
			this->reg_e = set(this->reg_e, 5);
			continue;
		case 0xEC:
			this->reg_h = set(this->reg_h, 5);
			continue;
		case 0xED:
			this->reg_l = set(this->reg_l, 5);
			continue;
		case 0xEE:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 5));
			continue;
		case 0xEF:
			this->reg_a = set(this->reg_a, 5);
			continue;
		case 0xF0:
			this->reg_b = set(this->reg_b, 6);
			continue;
		case 0xF1:
			this->reg_c = set(this->reg_c, 6);
			continue;
		case 0xF2:
			this->reg_d = set(this->reg_d, 6);
			continue;
		case 0xF3:
			this->reg_e = set(this->reg_e, 6);
			continue;
		case 0xF4:
			this->reg_h = set(this->reg_h, 6);
			continue;
		case 0xF5:
			this->reg_l = set(this->reg_l, 6);
			continue;
		case 0xF6:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 6));
			continue;
		case 0xF7:
			this->reg_a = set(this->reg_a, 6);
			continue;
		case 0xF8:
			this->reg_b = set(this->reg_b, 7);
			continue;
		case 0xF9:
			this->reg_c = set(this->reg_c, 7);
			continue;
		case 0xFA:
			this->reg_d = set(this->reg_d, 7);
			continue;
		case 0xFB:
			this->reg_e = set(this->reg_e, 7);
			continue;
		case 0xFC:
			this->reg_h = set(this->reg_h, 7);
			continue;
		case 0xFD:
			this->reg_l = set(this->reg_l, 7);
			continue;
		case 0xFE:
			mem_wb(this->mem, this->reg_hl, set(mem_rb(this->mem, this->reg_hl), 7));
			continue;
		case 0xFF:
			this->reg_a = set(this->reg_a, 7);
			continue;
		}
	}
		/* IF THE FOR LOOP REACHES THE END IT WILL QUIT
		 * THIS ALLOWS HALT TO EFFECTIVELY DO NOTHING WITHOUT LOOPING
		 * OPCODES that use 'break' will reach this point and terminate the loop */
		break;
	}
}
