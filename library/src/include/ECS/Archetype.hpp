#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "ID.hpp"

namespace Artifex::ECS {

// Component Structure
struct Type {
    std::unordered_set<ID> type{};

    ID id();
    explicit operator ID();
};

// Edges of archetype
struct ArchetypeEdge {
    ID add{};
    ID remove{};
};

struct Archetype {
    Archetype() = delete;

    Type type{};
    std::vector<Column> columns;  // Component values

    std::unordered_map<ID, ArchetypeEdge> edges;  // Map of neighbour archetypes
};

struct ArchetypeRecord {
    ID column;
};

typedef std::unordered_map<ID, ArchetypeRecord> ArchetypeMap;  // Map of Archetype Records

}