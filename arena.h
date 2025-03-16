#ifndef ARENA_H_
#define ARENA_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *buffer;
  uint64_t current_offset;
  uint64_t buffer_size;
} Arena;

void arena_init(Arena *arena, uint64_t buffer_size);
void *arena_alloc(Arena *arena, uint64_t num_of_elem, uint64_t elem_size, uint64_t align_size);
void arena_reset_pointer(Arena *arena);
void arena_delete(Arena *arena);

Arena arena = {};

#endif // ARENA_H_

#ifdef ARENA_IMPLEMENTATION

void arena_init(Arena *arena, uint64_t buffer_size)
{
  *arena = (Arena) {
    .buffer = malloc(buffer_size),
    .buffer_size = buffer_size,
    .current_offset = 0,
  };
}

void *arena_alloc(Arena *arena, uint64_t num_of_elem, uint64_t elem_size, uint64_t align_size)
{
  if (align_size == 0 || (align_size & (align_size - 1)) != 0) return NULL;

  uintptr_t allocation_size = num_of_elem * elem_size;
  if (allocation_size < elem_size) return NULL;

  uintptr_t total_offset = (uintptr_t)arena->current_offset + (uintptr_t)arena->buffer;
  uint64_t padding = (~total_offset+1) & (align_size-1); // total_offset % align_size

  total_offset += padding;
  if (total_offset + allocation_size > (uintptr_t)arena->buffer_size + (uintptr_t)arena->buffer) return NULL;

  arena->current_offset += (padding + allocation_size);
  memset((void*) total_offset, 0, allocation_size);
  return (void*) total_offset;
}

void arena_reset_pointer(Arena *arena)
{
  arena->current_offset = 0;
  memset(arena->buffer, 0, arena->buffer_size);
}

void arena_delete(Arena *arena)
{
  arena->current_offset = 0;
  free(arena->buffer);
}

#endif // ARENA_IMPLEMENTATION
