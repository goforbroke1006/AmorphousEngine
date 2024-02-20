---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/19/24 11:23 PM
---

require "Core/Vector3"

BoxCollider = {}

function BoxCollider:new()
    local instance = LuaBehaviour:new()
    instance.center = Vector3.zero
    instance.size = Vector3:new(1.0, 1.0, 1.0)

    setmetatable(instance, self)
    self.__index = self
    return instance
end

function BoxCollider:Start()
    -- Debug.Log("Camera :: Start");
end

function BoxCollider:Update()
    -- Debug.Log("Camera :: Update");
end
