// Physics.h
#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Physics
{
    class AABB
    {
    public:
        AABB();  // Default constructor

        AABB(const glm::vec3& minBounds, const glm::vec3& maxBounds);

        const glm::vec3& GetMinBounds() const;
        const glm::vec3& GetMaxBounds() const;

    private:
        glm::vec3 minBounds;
        glm::vec3 maxBounds;
    };

    // Function to calculate AABB from vertices
    AABB CalculateAABB(const std::vector<glm::vec3>& vertices);

    // Function to check collision between two AABBs
    bool CheckCollision(const AABB& box1, const AABB& box2);
}
