---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/29/23 2:44 PM
---

require "Core/LuaBehaviour"

DroneController = LuaBehaviour:new()

DroneController.motionSpeed = 0.0

function DroneController:Start()
    if (self.motionSpeed == 0.0) then
        Debug.LogWarning("motionSpeed should not be equals zero")
    end
end

function DroneController:Update()
    if Input.GetButtonDown("Fire1") then
        Debug.Log("Fire!!!")
    end

    local horMove = Input.GetAxis("Horizontal");
    if horMove ~= 0.0 then
        local move = Vector3:new(horMove, 0.0, 0.0)
        self.transform:Translate(move * self.motionSpeed * Time.deltaTime);
    end

    local verMove = Input.GetAxis("Vertical")
    if verMove ~= 0.0 then
        local move = Vector3:new(0.0, 0.0, verMove)
        self.transform:Translate(move * self.motionSpeed * Time.deltaTime);
    end
end
