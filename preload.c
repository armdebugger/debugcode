#if 10
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <execinfo.h>

// __attribute__ ((visibility("default")))
static void StackTrace(void)
{
	#define MAX_STACK_TRACE 100
	int j, nptrs;
	void *buffer[MAX_STACK_TRACE];
	char **strings;

	nptrs = backtrace(buffer, MAX_STACK_TRACE);
	//backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO);
	strings = backtrace_symbols(buffer, nptrs);
	if(strings == NULL)
	{
		return;
	}

	for(j = 0; j < nptrs; j++)
		fprintf(stderr, "%d: %s\n", j, strings[j]);

	free(strings);
}

struct layout
{
	void *next;
	void *return_address;
};

void * __stackdump_known_base;
__attribute__((no_instrument_function))
void stackdump(void)
{
	void *top_frame = __builtin_frame_address(0);
	void *top_stack = ({ void *__dummy; &__dummy; });
	int cnt = 0;
	int array_count = 10;

	struct layout *current = (struct layout *)top_frame;

	while(cnt < array_count)
	{
		if((void *)current < top_stack || (__stackdump_known_base!=NULL && (void *)current > __stackdump_known_base))
			break;

		fprintf(stderr, "ret_addr %d: %p\n", cnt, current->return_address);
		current = (struct layout *)current->next;

		if(((unsigned long) current) & 1)
			break;

		cnt ++;
	}
}

int hook_running = 0;
void *malloc(size_t size)
{
	static void* (*real_malloc)(size_t)=NULL;

	if(real_malloc==NULL)
	{
		real_malloc = dlsym(RTLD_NEXT, "malloc");

		if(NULL == real_malloc)
		{
			fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
		}
		else
			fprintf(stderr, "real_malloc = %p\n", real_malloc);
	}

	void *p = NULL;
	p = real_malloc(size);

	if(!hook_running)
	{
		hook_running = 1;
		fprintf(stderr, "malloc(%d) = ", (int)size);
		fprintf(stderr, "%p\n", p);
		stackdump();
		StackTrace();
		hook_running = 0;
	}

	return p;
}

void freeX(void *p)
{
	fprintf(stderr, "free %p\n", p);
}
#endif