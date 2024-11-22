#include <Artifex/ecs/freelist.h>

#include <stdlib.h>
#include <assert.h>

void tlComponent_free(tlComponent *store) {
  if (!store) return;

  if (store->data && store->size) free(store->data);
  store->data = NULL, store->size = 0, store->length = 0;

  tlFreelist_free(&store->freeList);
}



tlID tlFreelist_pop(tlFreelist *list) {
  if (!list->data || !list->size || !list->count)
    return TL_NONE;

  // TODO asserts

  list->count--;
  tlID id = list->data[list->count];

  // shrink freelist
  if (list->size > FREELIST_MIN_SIZE && (float)list->count <= (float)list->size * FREELIST_SHRINK_TRIGGER) {
    size_t newSize = list->size * FREELIST_SHRINK_AMOUNT;
    if (newSize < FREELIST_MIN_SIZE) newSize = FREELIST_MIN_SIZE;
    tlID *newPtr = realloc(list->data, sizeof(tlID) * newSize);
    if (newPtr) list->size = newSize, list->data = newPtr;
  }
  
  return id;
}

uint8_t tlFreelist_push(tlFreelist *list, tlID id) {
  // allocate new buffer if necessary
  if (!list->data || !list->size) {
    list->size = FREELIST_MIN_SIZE, list->count = 0;
    if (!(list->data = malloc(sizeof(tlID) * list->size))) return 1;
  }

  // leaving it could become a memory leak
  assert(list->count <= list->size);

  // expand list
  if (list->count == list->size) {
    size_t newSize = list->size * 2; // TODO macro
    tlID *newPtr = realloc(list->data, sizeof(tlID) * newSize);
    if (!newPtr) return 1;
    list->size = newSize, list->data = newPtr;
  }

  // Add to list
  list->data[list->count] = id;
  list->count++;
  return 0;
}
