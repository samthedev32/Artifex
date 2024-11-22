/**
 * @brief Freelist for Typeless (ECS)
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <Artifex/ecs/typeless.h>

#include <stddef.h>

// Freelist for Typeless (ECS)
typedef struct _tl_freelist {
  tlID *data;
  size_t size, count;
} tlFreelist;

// Free Freelist
void tlFreelist_free(tlFreelist *list);

// Pop first item from Freelist
tlID tlFreelist_pop(tlFreelist *list);

// Push item to Freelist
uint8_t tlFreelist_push(tlFreelist *list, tlID id);

#ifdef __cplusplus
}
#endif
