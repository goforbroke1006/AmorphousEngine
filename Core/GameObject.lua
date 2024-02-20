---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/31/23 11:10 AM
---

require 'Core/Object'
require 'Core/Transform'

GameObject = {} -- ToObject:new(0)

GameObject.__instanceID = 0
GameObject.name = ""
GameObject.transform = Transform:new()
GameObject.__components = {}

function GameObject:new(__instanceID --[[integer]], name --[[string]])
    __instanceID = __instanceID or -1
    name = name or ""

    local gameObj = Object:new(__instanceID)
    gameObj.__instanceID = __instanceID
    gameObj.name = name
    gameObj.transform = Transform:new()
    gameObj.__components = {}

    self.__index = self
    setmetatable(gameObj, self)

    return gameObj
end

--- https://docs.unity3d.com/ScriptReference/Object.GetInstanceID.html
--- int Returns the instance ID of the object.
function GameObject:GetInstanceID()
    return self.__instanceID;
end

function GameObject:IsA(className --[[string]])
    return className == "GameObject" or className == "Object"
end

function GameObject:AddComponent(name --[[string]])
    -- TODO: implement me
    return nil
end

--- https://docs.unity3d.com/ScriptReference/GameObject.GetComponent.html
function GameObject:GetComponent(typeArg)
    if type(typeArg) ~= "table" then
        Debug.LogError("Incompatible component ref type " .. type(typeArg) .. ", table required")
        return nil
    end

    local typeName = getTableName(typeArg)

    return __all_components[self.__instanceID][typeName]
end

function GameObject.Find(name --[[string]])
    -- TODO: implement me
    return nil
end
