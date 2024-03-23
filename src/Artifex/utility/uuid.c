#include <Artifex/utility/uuid.h>

#include <math.h>
#include <stdlib.h>

// TODO: proper UUID generator
uuid_t uuid_generate() { return rand() % UINT64_MAX; }

// uuid_t uuid_to_uuid(const char *str) {}

// const char *uuid_to_str(uuid_t uuid) {}