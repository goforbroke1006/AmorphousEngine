---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/30/23 1:51 AM
---

require 'Core/Space'
require 'Core/Vector3'
require 'Core/Quaternion'

Transform = {
    position = Vector3:new(0.0, 0.0, 0.0),
    rotation = Quaternion:new(0.0, 0.0, 0.0, 0.0),
    localScale = Vector3:new(1.0, 1.0, 1.0),
}

function Transform:new()
    tr = {
        position = Vector3:new(0.0, 0.0, 0.0),
        rotation = Quaternion:new(0.0, 0.0, 0.0, 0.0),
        localScale = Vector3:new(1.0, 1.0, 1.0),
    }

    self.__index = self
    setmetatable(tr, self)

    return tr
end

--- Moves the transform in the direction and distance of translation.
--- https://docs.unity3d.com/ScriptReference/Transform.Translate.html
function Transform:Translate(translation --[[Vector3]], relativeTo)
    --if (type(translation) ~= "Vector3") then
    --    error("translation has incompatible type " .. type(translation))
    --end

    if translation:magnitude() == 0.0 then
        -- zero movement has no impact
        return
    end

    relativeTo = relativeTo or Space.Self

    if (relativeTo == Space.Self) then
        translation = self.rotation * translation
    end

    self.position.x = self.position.x + translation.x
    self.position.y = self.position.y + translation.y
    self.position.z = self.position.z + translation.z
end

--- Use Transform.Rotate to rotate GameObjects in a variety of ways.
--- The rotation is often provided as an Euler angle and not a Quaternion.
--- https://docs.unity3d.com/ScriptReference/Transform.Rotate.html
function Transform:Rotate(eulers --[[Vector3]], relativeTo)
    eulers = eulers or Vector3.zero
    relativeTo = relativeTo or Space.Self

    local current = self.rotation.eulerAngles
    current.x = current.x + eulers.x
    current.y = current.y + eulers.y
    current.z = current.z + eulers.z

    self.rotation = Quaternion.Euler(current.x, current.y, current.z)
end

--- Rotates the transform so the forward vector points at /target/'s current position.
--- https://docs.unity3d.com/ScriptReference/Transform.LookAt.html
function Transform:LookAt(target --[[Transform]], worldUp --[[Vector3]])
    worldUp = worldUp or Vector3.up
    -- TODO: implement me

    local forwardVector = (target.position - self.position):Normalize()

    local rotAxis = Vector3.Cross(Vector3.forward, forwardVector)
    local dot = Vector3.Dot(Vector3.forward, forwardVector)

    local q = Quaternion:new(
            rotAxis.x,
            rotAxis.y,
            rotAxis.z,
            dot + 1
    );

    q:Normalize()

    self.rotation = q;
end
