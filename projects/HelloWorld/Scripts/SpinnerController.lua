---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/29/23 2:44 PM
---

require 'Core/LuaBehaviour'
require 'Core/Time'

SpinnerController = LuaBehaviour:new()

SpinnerController.rotationSpeed = 0.0

function SpinnerController:Start()
    Debug.Log("SpinnerController :: Start");

    self.angleY = 0.0

    if (self.rotationSpeed == 0.0) then
        Debug.LogWarning('rotationSpeed should not be equals zero')
    end
end

function SpinnerController:Update()
    Debug.Log("SpinnerController :: Update");

    self.angleY = self.angleY + self.rotationSpeed * Time.deltaTime
    self.angleY = self.angleY % 180.0

    Debug.Log("Angle Y " .. self.angleY);

    self.transform.rotation = Quaternion.Euler(0.0, self.angleY, 0.0);
end
