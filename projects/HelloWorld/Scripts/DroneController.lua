---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/29/23 2:44 PM
---

require 'Core/LuaBehaviour'

DroneController = LuaBehaviour:new()

DroneController.motionSpeed = 0.0
DroneController.targetTr = Transform

function DroneController:Start()
    Debug.Log("DroneController :: Start");

    if (self.motionSpeed == 0.0) then
        Debug.LogWarning('motionSpeed should not be equals zero')
    end
    if (self.targetTr == nil) then
        Debug.LogError('targetTr is required')
    else
        Debug.Log("targetTr = " .. self.targetTr.position:ToString())
    end
end

function DroneController:Update()
    Debug.Log("DroneController :: Update");

    --Debug.Log("targetTr = " .. self.targetTr.position:ToString())
    self.transform:LookAt(self.targetTr.position);

    local move = Vector3.right * self.motionSpeed * Time.deltaTime
    print('  next move ' .. move.x .. ' ' .. move.y .. ' ' .. move.z)

    self.transform:Translate(move);

    local distance = self.transform.position:Distance(self.targetTr.position);
    Debug.Log("Distance to target " .. distance);
end