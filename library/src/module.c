#include <malloc.h>
#include <stdint.h>

// refer to:
// https://replit.com/@SamTheDev/Linked-List#list.h

// Module Backend
struct _ax_module {
    int enabled;
    void* user;

    // TODO data

    struct _ax_module* next;  // next module
};

// Append Module List (takes in: list, new module, returns: module's id)
uint32_t _ax_module_append(struct _ax_module** list, struct _ax_module* module) {
    if (list == NULL)
        return 0;

    if (*list == NULL) {
        *list = module;
        return 0;
    }

    uint32_t id = 1;
    struct _ax_module* last = *list;
    while (last->next != NULL) {
        last = last->next;
        id++;
    }

    last->next = module;
    return id;
}

// Insert Module into List (takes in: list, new module, place to insert; 0 first, returns: success/error code)
int _ax_module_insert(struct _ax_module** list, struct _ax_module* module, uint32_t id);

// Get Module by ID (takes in: list, id to return, returns: module pointer)
struct _ax_module* _ax_module_get(struct _ax_module* list, uint32_t id);

// Pop First Module (takes in: list, returns: module pointer)
struct _ax_module* _ax_module_pop_front(struct _ax_module** list);

// Pop Last Module [SLOW] (takes in: list, returns: module pointer)
struct _ax_module* _ax_module_pop_back(struct _ax_module** list);

// Pop Module (takes in: list, id to pop, returns: module pointer)
struct _ax_module* _ax_module_pop(struct _ax_module** list, uint32_t id);