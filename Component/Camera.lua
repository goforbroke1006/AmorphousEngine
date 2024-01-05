---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/2/24 11:03 PM
---

require 'Core/LuaBehaviour'
require 'Core/Color'

Camera = LuaBehaviour:new()

Camera.backgroundColor = Color:new()

function Camera:Start()
    Debug.Log("Camera :: Start");
end

function Camera:Update()
    Debug.Log("Camera :: Update");
end
