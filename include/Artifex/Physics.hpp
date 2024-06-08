#pragma once

#include <types/vec.hpp>
#include <flecs.h>

#include <set>
#include <map>
#include <variant>

namespace Physics {
    namespace Partition {
        typedef vec<2, uint32_t> Location;
        typedef std::set<flecs::id_t> EntityList;
    }

    struct Transform {
        uint32_t id;
        vec<2> Position, Size, Velocity;
    };

    struct Rect {
        vec<2> Position, Velocity;
    };

    struct Properties {
        bool hasCollider = false; // has collider, like walls or even the main player
        bool isStatic = false; // static objects can't move
        // TODO more properties
    };

    struct Object {
        std::variant<Rect, Transform> body;

        Properties properties;
    };

    // TODO collision table
    // TODO areas (circles), triggers

    // 2D Physics Engine
    class Engine {
    public:
        explicit Engine(const vec<2> &partitionSize = 10.0f);

        ~Engine();

        // Process Entity
        void process(Transform &t);

    private:
        [[nodiscard]] Partition::Location getPos(const vec<2> &Point) const;

        // Collision Detection between Rect and Rect
        static bool collision(const vec<2> &a_center, const vec<2> &a_size, const vec<2> &b_center,
                              const vec<2> &b_size,
                              vec<2> *amount = nullptr);

        // Collision Detection between Rect and Circle
        static bool collision(const vec<2> &a_center, const vec<2> &a_size, const vec<2> &b_center, float b_radius);

        const vec<2> m_partitionSize;
        std::map<Partition::Location, Partition::EntityList> m_partition{};
    };
}
