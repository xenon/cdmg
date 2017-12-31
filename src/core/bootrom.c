#include "core/bootrom.h"
#include <stdio.h>
#include <stdlib.h>

struct bootrom*
new_bootrom
(const char* path)
{
	ASSERT(path);

	FILE* file = fopen(path, "rb");
	if (file == NULL)
		goto err_at_file;

	fseek(file, 0, SEEK_END);
	if (ftell(file) < 256)
		goto err_at_filesize;
	fseek(file, 0, SEEK_SET);
	
	struct bootrom* this = malloc(sizeof(struct bootrom));
	if (fread(&this->byte[0], 1, 256, file) != 256)
		goto err_at_filedata;
	
	fclose(file);	
	return this;

err_at_filedata:
	free(this);
err_at_filesize:
	fclose(file);
err_at_file:
	return NULL;
}

void
free_bootrom
(struct bootrom* this)
{
	free(this);
}

