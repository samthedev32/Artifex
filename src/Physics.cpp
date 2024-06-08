#include <Physics.hpp>

#include <algorithm>
#include <cmath>

namespace Physics {
    Engine::Engine(const vec<2> &partitionSize) : m_partitionSize(partitionSize) {
    }

    Engine::~Engine() {
    }

    void Engine::process(Transform &t) {
        const auto lastPos = t.Position;

        t.Position += t.Velocity;

        if (t.Velocity.length() != 0) {
            Partition::Location last = getPos(lastPos);
            Partition::Location current = getPos(t.Position);

            // TODO: get entity id
            flecs::id_t id = 0;
            if (last != current) {
                m_partition[last].erase(id);
                m_partition[current].insert(id);
            }
        }
    }

    Partition::Location Engine::getPos(const vec<2> &Point) const {
        return Point / m_partitionSize;
    }

    bool Engine::collision(const vec<2> &a_center, const vec<2> &a_size, const vec<2> &b_center, const vec<2> &b_size,
                           vec<2> *amount) {
        vec<2> half[2] = {a_size / 2, b_size / 2};
        vec<2> dist = a_center - b_center;

        vec<2, bool> overlap = {
            abs(dist->x) < half[0]->x + half[1]->x,
            abs(dist->y) < half[0]->y + half[1]->y
        };

        if (amount) {
            (*amount)->x = abs(dist->x);
            (*amount)->y = abs(dist->y);
        }

        return overlap->x && overlap->y;
    }

    bool Engine::collision(const vec<2> &a_center, const vec<2> &a_size, const vec<2> &b_center, float b_radius) {
        // Find the distance between the circle center and the rectangle center
        vec<2> dist = a_center - b_center;

        // Calculate the half-width and half-height of the rectangle
        vec<2> half = a_size / 2;

        // Closest point on the rectangle to the circle
        const vec<2> closest = {
            std::clamp(dist->x, -half->width, half->width),
            std::clamp(dist->y, -half->height, half->height)
        };

        // Calculate the distance from the closest point on the rectangle to the circle center
        vec<2> displacement = closest - dist;
        float distance = std::sqrt(displacement->x * displacement->x + displacement->y * displacement->y);

        // Check if the distance is less than the circle radius
        return distance < b_radius;
    }
}
