---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/14/24 12:32 PM
---

--- https://docs.unity3d.com/ScriptReference/Object.html
Object = {
    __instanceID = 0,
    name = ""
}

function Object:new(instanceID --[[integer]], name --[[string]])
    name = name or ""
    obj = {
        __instanceID = instanceID,
        name = name
    }

    self.__index = self
    setmetatable(obj, self)

    return obj
end

--- https://docs.unity3d.com/ScriptReference/Object.GetInstanceID.html
--- int Returns the instance ID of the object.
function Object:GetInstanceID()
    return self.__instanceID;
end

--- https://docs.unity3d.com/ScriptReference/Object.ToString.html
--- string The name returned by ToString.
function Object:ToString()
    -- TODO: implement me
    return ""
end

--- https://docs.unity3d.com/ScriptReference/Object.Destroy.html
--- Removes a GameObject, component or asset.
--- obj     The object to destroy.
--- t       The optional amount of time to delay before destroying the object.
Object.Destroy = function(obj --[[Object]], t)
    t = t or 0.0

    if obj == nil then
        return
    end

    -- TODO: implement me
end

--- https://docs.unity3d.com/ScriptReference/Object.Instantiate.html
Object.Instantiate = function(original --[[Object]], arg1, arg2, arg3)
    if original == nil then
        Debug.LogError("'original' should not be nil")
        return
    end

    -- public static Object Instantiate(Object original);
    if arg1 == nil and arg2 == nil and arg3 == nil then
        -- TODO: implement me
    end

    -- public static Object Instantiate(Object original, Transform parent);
    if arg1:IsA("Transform") and arg2 == nil and arg3 == nil then
        -- TODO: implement me
    end

    -- public static Object Instantiate(Object original, Transform parent, bool instantiateInWorldSpace);
    if arg1:IsA("Transform") and type(arg2) == "bool" and arg3 == nil then
        -- TODO: implement me
    end

    -- public static Object Instantiate(Object original, Vector3 position, Quaternion rotation);
    if arg1:IsA("Vector3") and arg2:IsA("Quaternion") and arg3 == nil then
        -- TODO: implement me
    end

    -- public static Object Instantiate(Object original, Vector3 position, Quaternion rotation, Transform parent);
    if arg1:IsA("Vector3") and arg2:IsA("Quaternion") and arg3:IsA("Transform") then
        -- TODO: implement me
    end

    Debug.LogError("Unexpected signature")
end
