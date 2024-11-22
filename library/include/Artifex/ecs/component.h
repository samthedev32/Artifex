/**
 * @brief Component Store for Typeless (ECS)
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Artifex/ecs/freelist.h>

// Component Store Config
// TODO

// Component Store for Typeless (ECS)
typedef struct _tl_component {
  tlID size, length; // size of buffer, length of elements
  void *data;        // data array

  tlFreelist freeList;
} tlComponent;

// Free Component Store
void tlComponent_free(tlComponent *store);

// Add value to Component Store
tlID tlComponentAdd(tlComponent *store, *value);



// Pop Component
tlID tlComponent_pop(tlFreelist list);

// // Push item to Freelist
// uint8_t tlFreelist_push(tlFreelist list, tlID id);

#ifdef __cplusplus
}
#endif
