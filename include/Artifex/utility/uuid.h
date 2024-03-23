#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef uint64_t uuid_t;

// Generate New UUID
uuid_t uuid_generate();

// Convert String (xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx) to UUID
// uuid_t uuid_to_uuid(const char *str);

// // Convert UUID to String (xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx)
// const char *uuid_to_str(uuid_t uuid);

#ifdef __cplusplus
}
#endif