---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/20/24 10:52 AM
---

require "Core/Vector3"

PhysicsSystem = {}

function PhysicsSystem.hasCollision(collider1 --[[BoxCollider]], collider2 --[[BoxCollider]])
    if collider1:IsA("BoxCollider") and collider2:IsA("BoxCollider") then
        local vertices = PhysicsSystem.getBoxColliderVertices(collider2)
        for _, vertex in pairs(vertices) do
            if PhysicsSystem.isPointInsideBoxCollider(collider1, vertex) then
                return true
            end
        end

        return false
    end

    Debug.LogError("Now method supports just BoxCollider+BoxCollider")

    return false
end

function PhysicsSystem.getBoxColliderVertices(collider --[[BoxCollider]])
    local boxPos = collider.transform.position + collider.center

    local minX = boxPos.x - collider.size.x / 2.0
    local maxX = boxPos.x + collider.size.x / 2.0

    local minY = boxPos.y - collider.size.y / 2.0
    local maxY = boxPos.y + collider.size.y / 2.0

    local minZ = boxPos.z - collider.size.z / 2.0
    local maxZ = boxPos.z + collider.size.z / 2.0

    local vertices = {
        -- top square
        [0] = Vector3:new(minX, maxY, minZ),
        [1] = Vector3:new(minX, maxY, maxZ),
        [2] = Vector3:new(maxX, maxY, maxZ),
        [3] = Vector3:new(maxX, maxY, minZ),

        -- bottom square
        [4] = Vector3:new(minX, minY, minZ),
        [5] = Vector3:new(minX, minY, maxZ),
        [6] = Vector3:new(maxX, minY, maxZ),
        [7] = Vector3:new(maxX, minY, minZ),
    }

    return vertices
end

function PhysicsSystem.isPointInsideBoxCollider(collider --[[BoxCollider]], point --[[Vector3]])
    -- https://developer.mozilla.org/en-US/docs/Games/Techniques/3D_collision_detection#point_vs._aabb

    local boxPos = collider.transform.position + collider.center

    local minX = boxPos.x - collider.size.x / 2.0
    local maxX = boxPos.x + collider.size.x / 2.0

    local minY = boxPos.y - collider.size.y / 2.0
    local maxY = boxPos.y + collider.size.y / 2.0

    local minZ = boxPos.z - collider.size.z / 2.0
    local maxZ = boxPos.z + collider.size.z / 2.0

    return point.x >= minX and point.x <= maxX
            and point.y >= minY and point.y <= maxY
            and point.z >= minZ and point.z <= maxZ
end