//int mystrcmp(const char *s1, const char *s2);
#if 10
/* Prototypes for __malloc_hook, __free_hook */
#include <malloc.h>

/* Prototypes for our hooks.  */
static void my_init_hook(void);
static void *my_malloc_hook(size_t, const void *);
static void my_free_hook(void*, const void *);

static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void*, const void *);

static void my_init(void)
{
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}

static void my_exit(void)
{
	/* Restore all old hooks */
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
}

static void *my_malloc_hook(size_t size, const void *caller)
{
#if 10
	{
		printf("malloc4 (%u) \n", (unsigned int) size);
		void * ret = __builtin_return_address(1);
		printf("ret address [%p]\n", ret);
		void * caller = __builtin_frame_address(0);
		printf("call address [%p]\n", caller);
	}
#endif
	void *result;
	/* Restore all old hooks */
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	/* Call recursively */
	result = malloc(size);
	/* Save underlying hooks */
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	/* printf might call malloc, so protect it too. */
	printf("malloc3 (%u) returns %p\n", (unsigned int) size, result);
	/* Restore our own hooks */
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;

	return result;
}

static void my_free_hook(void *ptr, const void *caller)
{
	/* Restore all old hooks */
	__malloc_hook = old_malloc_hook;
	__free_hook = old_free_hook;
	/* Call recursively */
	free(ptr);
	/* Save underlying hooks */
	old_malloc_hook = __malloc_hook;
	old_free_hook = __free_hook;
	/* printf might call free, so protect it too. */
	printf("freed pointer %p\n", ptr);
	/* Restore our own hooks */
	__malloc_hook = my_malloc_hook;
	__free_hook = my_free_hook;
}
#endif
