---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/2/24 11:03 PM
---

require 'Core/LuaBehaviour'
require 'Core/Color'

Light = {}

function Light:new()
    local instance = LuaBehaviour:new()
    instance.color = Color.yellow

    setmetatable(instance, self)
    self.__index = self
    return instance
end

function Light:Start()
    -- Debug.Log("Light :: Start");
end

function Light:Update()
    -- Debug.Log("Light :: Update");
end
