---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/30/23 10:44 AM
---

require 'Core/Transform'
require 'Core/GameObject'

require 'Core/Debug'
require 'Core/Time'
require 'Core/Space'

LuaBehaviour = {
    --__type_name = "",
    --enabled = true,
    --gameObject = GameObject,
    --transform = Transform,
}

function LuaBehaviour:new()
    local lb = {
        __type_name = "",
        __type_filepath = "",
        enabled = true,
        gameObject = GameObject:new(),
        transform = Transform,
    }
    lb.transform = lb.gameObject.transform

    self.__index = self
    setmetatable(lb, self)

    return lb
end

function LuaBehaviour:IsA(tableName --[[string]])
    return "LuaBehaviour" == tableName
end

function LuaBehaviour:GetComponent(typeArg)
    return self.gameObject:GetComponent(typeArg)
end

--- https://docs.unity3d.com/ScriptReference/Component.GetComponents.html
function LuaBehaviour:GetComponents()
    return __all_components[self.gameObject.__instanceID]
end

function LuaBehaviour.__make_clone(origin --[[LuaBehaviour]])
    local component = LuaBehaviour:new()

    for k, v in pairs(origin) do
        component[k] = v
    end

    return setmetatable(component, getmetatable(origin))
end
