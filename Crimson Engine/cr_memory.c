#include "crimson.h"

void *cr_malloc(size_t size)
{
	void *ptr = malloc(size);

	if (ptr != NULL) {
		return ptr;
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: FATAL ERROR, COULD NOT MALLOC MEMORY. EXITING.\n");
#endif
		crimson.exitcode = 1;
		return NULL;
	}
}

void *cr_calloc(size_t nitems, size_t size)
{
	void *ptr = calloc(nitems, size);

	if (ptr != NULL) {
		return ptr;
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: FATAL ERROR, COULD NOT CALLOC MEMORY. EXITING.\n");
#endif
		crimson.exitcode = 1;
		return NULL;
	}
}

void *cr_realloc(void *ptr, size_t size)
{
	void *new_ptr = realloc(ptr, size);

	if (new_ptr != NULL) {
		ptr = new_ptr;
		free(ptr);
	}
	else {
#if CR_DEBUG
		printf("Crimson Debug: FATAL ERROR, COULD NOT REALLOC MEMORY. EXITING.\n");
#endif
		crimson.exitcode = 1;
		return NULL;
	}

	return ptr;
}

void cr_free(void *ptr)
{
	free(ptr);
}
