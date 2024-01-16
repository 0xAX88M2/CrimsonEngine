#ifndef CR_MEMORY_H
#define CR_MEMORY_H

void *cr_malloc(size_t size);
void *cr_calloc(size_t nitems, size_t size);
void *cr_realloc(void *ptr, size_t size);

void cr_free(void *ptr);

#endif // !CR_MEMORY_H
