#include "backend/thread.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

struct thread {
	SDL_Thread* thd;
	int (*fun)(void);
	int ret;
	bool called;
	bool done;
};

static int
dispatch_thread
(void* structure)
{
	return ((struct thread*)structure)->fun();
}

struct thread*
new_thread
(int (*function)(void), const char* name)
{
	struct thread* this = malloc(sizeof(struct thread));
	if (this == NULL)
		goto err_at_alloc;

	this->fun = function;
	this->called = false;
	this->done = false;
	
	this->thd = SDL_CreateThread(dispatch_thread, name, (void*)this);
	if (this->thd == NULL)
		goto err_at_thd;
	
	return this;

err_at_thd:
	free(this);
err_at_alloc:
	return NULL;
}

void
free_thread
(struct thread* this)
{
	ASSERT(this);
	if (this->done == false)
		SDL_WaitThread(this->thd, &this->ret);
	free(this);
}

int
wait_thread
(struct thread* this)
{
	ASSERT(this);
	if (this->done == true)
		return this->ret;
	
	if (this->called == true)
		ASSERT(false); /* Already waiting for thread to finish! */

	this->called = true;
	SDL_WaitThread(this->thd, &this->ret);
	this->done = true;
	return this->ret;
}
