#ifndef INCLUDE_BACKEND_THREAD_H
#define INCLUDE_BACKEND_THREAD_H

#include "debug.h"
#include "types.h"

struct thread;

extern struct thread* new_thread(int (*function)(void), const char* name);
extern void free_thread(struct thread*);
extern int wait_thread(struct thread*);

#endif
