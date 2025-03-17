#ifndef ARENA_H_
#define ARENA_H_

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* buffer;
    size_t alloc_offset;
    size_t capacity;
} arena;

#define KB(x) (x * 1024)

#ifndef ARENA_DEFAULT_CAPACITY
#    define ARENA_DEFAULT_CAPACITY KB(8)
#endif // !ARENA_DEFAULT_CAPACITY

void arena_init(arena* a, size_t capacity) {
    *a = (arena){(char*)malloc(capacity), 0, capacity};
}

void* arena_alloc(arena* a, size_t size, size_t align) {
    if (a->buffer == NULL) {
        arena_init(a, ARENA_DEFAULT_CAPACITY);
    }

    assert(align != 0 && (align & (align - 1)) == 0);

    uintptr_t alloc_res = (uintptr_t)a->buffer + a->alloc_offset;
    uintptr_t padding = (~alloc_res + 1) & (align - 1);
    alloc_res += padding;

    assert(alloc_res + size <= (uintptr_t)a->buffer + a->capacity);

    a->alloc_offset += size + padding;

    memset((void*)alloc_res, 0, size);
    return (void*)alloc_res;
}

void arena_free(arena* a) {
    free(a);
}

void arena_reset(arena* a) {
    a->alloc_offset = 0;
}

#endif
