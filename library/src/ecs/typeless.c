#include <Artifex/ecs/typeless.h>

#include <Artifex/ecs/freelist.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <stdio.h>

// Config
#define START_COMPONENTS 1 // larger value may save some initial loading time
#define ENTITY_START_SIZE 32

struct _tl_component {
  tlID size, length;         // size of buffer, length of elements
  void *data;                // data array

  // TODO locks
  tlFreelist freeList;
};

struct _tl_entity {
  size_t size;
  tlID *data;

  // TODO state & indicator variables
};

struct _Typeless {
  // Component Store
  uint8_t components; // number of component stores (from byte 1 onwards)
  struct _tl_component *component;

  // TODO list:
  // thread-safety:
  //   - paralell reads
  //   - lock on write (per-list locks)
  // archetypes

  // Archetype Store
  // uint8_t archetypes;
  // struct _tl_archetype *archetypes;

  // Entity Store
  uint8_t size, count;
  struct _tl_entity *entity;
  tlFreelist freeList;
};

uint8_t tlInitialize(Typeless *ecs) {
  if (!ecs) return 1;

  if (!(*ecs = malloc(sizeof(struct _Typeless)))) return 1;

  // init component store
  (*ecs)->components = 0;
  (*ecs)->component = NULL;

  // init entity store
  (*ecs)->size = 0, (*ecs)->count = 0, (*ecs)->entity = NULL;
  memset(&(*ecs)->freeList, 0, sizeof(struct _tl_freelist));

  return 0;
}

void tlDestroy(Typeless *ecs) {
  if (!ecs || !*ecs) return;

  // free component storage
  if ((*ecs)->component && (*ecs)->components) {
    for (uint8_t i = 0; i < (*ecs)->components; i++) {
      struct _tl_component *c = &(*ecs)->component[i];
      if (c->data && c->size) free(c->data);
      tlFreelist_free(&c->freeList);
    }

    free((*ecs)->component);
  }

  // free entity store
  if ((*ecs)->entity && (*ecs)->size) {
    // for (size_t i = 0; i < (*ecs)->count;
    // TODO
  }

  tlFreelist_free(&(*ecs)->freeList);

  free(*ecs);
  *ecs = NULL;
}

uint8_t tlPreserve(Typeless ecs, uint8_t byteLength) {
  if (!ecs || !byteLength) return 1;

  if (!ecs->components || !ecs->component) {
    ecs->components = ENTITY_START_SIZE;
    assert(ecs->component = calloc(ecs->components, sizeof(struct _tl_component)));
    // unrecoverable error; fail rather than return with no solution
    // TODO could be changed to a return later
  }

  if (ecs->components >= byteLength) return 0;

  struct _tl_component *newPtr = realloc(ecs->component, sizeof(struct _tl_component) * byteLength);
  if (!newPtr) return 1;
  memset(newPtr, 0, byteLength);

  ecs->components = byteLength;
  ecs->component = newPtr;

  return 0;
}

tlID tlAddValue(Typeless ecs, void *value, uint8_t byteLength) {
  if (!ecs || !value || !byteLength) return TL_NONE;
  if (tlPreserve(ecs, byteLength)) return TL_NONE;

  struct _tl_component *c = &ecs->component[byteLength-1];
  tlID id = tlFreelist_pop(&c->freeList);

  // data is contignous
  if (id == TL_NONE) {
    id = (uint32_t)(byteLength-1) << 0x18; // set store id

    // expand array
    if (c->length == c->size) {
      size_t newSize = c->size * 2;
      tlID *newPtr = realloc(c->data, sizeof(tlID) * newSize);
      if (!newPtr) return TL_NONE;
      c->size = newSize, c->data = newPtr; // TODO
    }

    // get new ID
    assert(TL_ID_STORE(c->length) == 0); // check if first 8 bits are unused
    
    id += TL_ID_VALUE(c->length);
    c->length++;
  }

  return id;
}

uint8_t tlDelValue(Typeless ecs, tlID id) {
  if (!ecs || id == TL_NONE) return 1;

  uint8_t store = TL_ID_STORE(id);
  tlID n = TL_ID_VALUE(id);

  if (store > ecs->components) return 1;

  struct _tl_component *c = &ecs->component[store];

  if (n == c->length) c->length--;
  else tlFreelist_push(&c->freeList, id);

  // fun fact: i wrote this fn while watching starship flight 6

  return 0;
}

void *tlGetValue(Typeless ecs, tlID id) {
  if (!ecs || id == TL_NONE) return NULL;

  uint8_t store = id >> 0x18;
  tlID n = id & 0xFFFFFF;

  if (store > ecs->components) return NULL;
  struct _tl_component *c = &ecs->component[store];

  return NULL; // TODO
}

tlID tlAddEntity(Typeless ecs, uint8_t components) {
  if (!ecs) return TL_NONE;

  if (!ecs->size || !ecs->entity) {
    ecs->size = ENTITY_START_SIZE, ecs->count = 0;
    assert(ecs->entity = calloc(ecs->size, sizeof(struct _tl_entity)));
    // unrecoverable error; fail rather than return with no solution
    // TODO could be changed to a return later
  }

  tlID id = tlFreelist_pop(&ecs->freeList);

  if (id == TL_NONE) {
    // expand array
    if (ecs->count == ecs->size) {
      size_t newSize = ecs->size * 2;
      struct _tl_entity *newPtr = realloc(ecs->entity, sizeof(struct _tl_entity) * newSize);
      if (!newPtr) return TL_NONE;
      ecs->size = newSize, ecs->entity = newPtr; // TODO
    }

    ecs->count++;
    id = ecs->count;
  }

  ecs->entity[id-1].size = components;
  ecs->entity[id-1].data = components == 0 ? NULL : calloc(components, sizeof(tlID));

  return id;
}

uint8_t tlDelEntity(Typeless ecs, tlID id) {
  if (!ecs || !ecs->size || !ecs->entity || id == TL_NONE) return 1;

  if (id > ecs->count) return 1;

  const struct _tl_entity *e = &ecs->entity[id];

  if (e->size && e->data) free(e->data);

  if (id == ecs->count) ecs->count--;
  else tlFreelist_push(&ecs->freeList, id);

  // TODO set a (maybe dynamic) border from which shrinking will be done

  return 0;
}

uint8_t tlGetEntity(Typeless ecs, tlID id, tlID **data, uint8_t *size) {
  if (!ecs || !ecs->size || !ecs->entity || id == TL_NONE || !data || !size) return 1;

  if (id > ecs->count) return 1;

  const struct _tl_entity *e = &ecs->entity[id];
  *data = e->data;
  *size = e->size;
  return 0;
}

// tlID tlTypeCounter(Typeless ecs) {
//   // TODO
// }

// tlID tlRegisterType(Typeless ecs, tlID typeID, uint16_t byteLength) {
//   // TODO
// }

// uint8_t tlTypeInfo(Typeless ecs, tlID typeID, uint16_t *bytelentgh,
//                    tlID *elements);

// tlID tlNew(Typeless ecs, tlID typeID, void *value);

// uint8_t tlDel(Typeless ecs, tlID typeID, tlID id);

// uint8_t tlSet(Typeless ecs, tlID typeID, tlID id, void *value);

// void *tlGet(Typeless ecs, tlID typeID, tlID id);

// inline const tlID tlHashString(const char key[TLECS_KEY_LENGTH]) {
//   uint64_t hash = 1;
//   for (int i = 0; i < TLECS_KEY_LENGTH; i++) {
//     if (key[i] == 0)
//       return hash;

//     hash *= key[i];
//     hash %= 4294967296; // limit of tlID
//   }

//   return hash;
// }
