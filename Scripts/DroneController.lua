---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/29/23 2:44 PM
---

DroneController = LuaBehaviour

function DroneController:Start()
    Debug.Log("DroneController :: Start");

    self.rotationSpeed = 1.0
    self.angleY = 0.0
    self.targetObj = Transform;
end

function DroneController:Update()
    Debug.Log("DroneController :: Update");

    self.angleY = self.angleY + Time.deltaTime * self.rotationSpeed
    self.transform:Translate(Vector3.right);
    self.transform:Rotate(0.0, self.angleY, 0.0, Space.Self);
    self.transform:LookAt(self.targetObj, Space.Self);
end
