#include "core/cart.h"
#include <stdio.h>
#include <stdlib.h>

static void
cart_loadheader
(struct cart* this)
{
	ASSERT(this);	
	ASSERT(this->byte);
	ASSERT(this->size);

	for (int i = 0x0134; i <= 0x0143; ++i)
		this->title[i - 0x0134] = this->byte[i];
	this->title[16] = '\0';

	this->cgb_flag = this->byte[0x0143];
	this->sgb_flag = this->byte[0x0146];
	this->type = this->byte[0x0147];
	
	switch (this->byte[0x0148]) {
	case 0x52:
		this->size_rom = 1179648;
		break;
	case 0x53:
		this->size_rom = 1310720;
		break;
	case 0x54:
		this->size_rom = 1572864;
		break;
	default:
		this->size_rom = 32768 * (1 << this->byte[0x148]);
		break;
	}

	switch (this->byte[0x0149]) {
	case 0x00:
		this->size_ram = 0;
		break;
	case 0x01:
		this->size_ram = 2048;
		break;
	case 0x02:
		this->size_ram = 8192;
		break;
	case 0x03:
		this->size_ram = 32768;
		break;
	case 0x04:
		this->size_ram = 131072;
		break;
	case 0x05:
		this->size_ram = 65536;
		break;
	}
	
	this->destination_code = this->byte[0x014A];
	this->version = this->byte[0x014C];
	this->header_checksum = this->byte[0x014D];
}

struct cart*
new_cart
(const char* path)
{	
	ASSERT(path);

	FILE* file = fopen(path, "rb");
	if (file == NULL)
		goto err_at_file;

	struct cart* this = malloc(sizeof(struct cart));
	fseek(file, 0, SEEK_END);
	this->size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	/* ADD CHECK FOR MINIMUM CART SIZE HERE ? */
	if (this->size == 0)
		goto err_at_filesize;
	
	this->byte = malloc(this->size);
	if (this->byte == NULL ||
	    fread(&this->byte[0], 1, this->size, file) != this->size)
		goto err_at_filedata;
	
	fclose(file);
	cart_loadheader(this);
	return this;

err_at_filedata:
	free(this->byte);
err_at_filesize:
	free(this);
err_at_file:
	fclose(file);
	return NULL;
}

void
free_cart
(struct cart* this)
{
	ASSERT(this);

	free(this->byte);
	free(this);
}

bool
cart_checksum
(struct cart* this)
{
	ASSERT(this);
	ASSERT(this->byte);
	ASSERT(this->size);
	
	return true;
}

bool
cart_checksum_header
(struct cart* this)
{
	int i = 0;
	ASSERT(this);
	ASSERT(this->byte);
	ASSERT(this->size);
	ASSERT(this->header_checksum);
	for (i = 0x0134; i < 0x014D; i -= this->byte[i] - 1);
	if (this->byte[0x014D] - i == 0)
		return true;
	return false;
}
/* Concerning read and write functions:
 * The memory unit accesses the cartridge memory
 * primarily through reads.
 * Though certain rom controllers support
 * interpretation of writes as bank switches
 * along with a host of other things...
 * How do I best implement this behaviour? 
 * -> Switch case, one function call
 * -> Function pointer */
