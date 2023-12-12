// Physics.cpp
#include "Physics.h"

namespace Physics
{
    AABB::AABB() : minBounds(glm::vec3(0.0f)), maxBounds(glm::vec3(0.0f)) {}

    AABB::AABB(const glm::vec3& minBounds, const glm::vec3& maxBounds)
        : minBounds(minBounds), maxBounds(maxBounds) {}

    const glm::vec3& AABB::GetMinBounds() const
    {
        return minBounds;
    }

    const glm::vec3& AABB::GetMaxBounds() const
    {
        return maxBounds;
    }

    AABB CalculateAABB(const std::vector<glm::vec3>& vertices)
    {
        if (vertices.empty())
        {
            // Return an invalid AABB if there are no vertices
            return AABB();
        }

        glm::vec3 minBounds = vertices[0];
        glm::vec3 maxBounds = vertices[0];

        for (const glm::vec3& vertex : vertices)
        {
            minBounds = glm::min(minBounds, vertex);
            maxBounds = glm::max(maxBounds, vertex);
        }

        return AABB(minBounds, maxBounds);
    }

    bool CheckCollision(const AABB& box1, const AABB& box2)
    {
        // Check for collision along each axis
        bool collisionX = box1.GetMaxBounds().x >= box2.GetMinBounds().x && box2.GetMaxBounds().x >= box1.GetMinBounds().x;
        bool collisionY = box1.GetMaxBounds().y >= box2.GetMinBounds().y && box2.GetMaxBounds().y >= box1.GetMinBounds().y;
        bool collisionZ = box1.GetMaxBounds().z >= box2.GetMinBounds().z && box2.GetMaxBounds().z >= box1.GetMinBounds().z;

        // If there is overlap along all axes, boxes are colliding
        return collisionX && collisionY && collisionZ;
    }
}
