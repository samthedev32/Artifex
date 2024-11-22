/**
 * @brief Typeless - Entity-Component System for Artifex
 *
 * @author SamTheDev
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Typeless Entity-Component System
typedef struct _Typeless *Typeless;

// ID Type
typedef uint32_t tlID;

#define TL_ID_STORE(id) ((tlID)(id) >> 0x18)
#define TL_ID_VALUE(id) ((tlID)(id) & 0xFFFFFF)

#define TL_NONE 0

// ---- Basic Control
 
// Initialize ECS
uint8_t tlInitialize(Typeless *ecs);

// Destroy ECS
void tlDestroy(Typeless *ecs);

// ---- Type Control

// Preserve component store
uint8_t tlPreserve(Typeless ecs, uint8_t byteLength);

// Create new value
tlID tlAddValue(Typeless ecs, void *value, uint8_t byteLength);

// Delete value
uint8_t tlDelValue(Typeless ecs, tlID id);

// Get pointer to component
void *tlGetValue(Typeless ecs, tlID id);

// ---- Entity Management

// Create new entity
tlID tlAddEntity(Typeless ecs, uint8_t components);

// Delete entity
uint8_t tlDelEntity(Typeless ecs, tlID id);

// Get Entity Data
uint8_t tlGetEntity(Typeless ecs, tlID id, tlID **data, uint8_t *size);

#ifdef __cplusplus
}
#endif
