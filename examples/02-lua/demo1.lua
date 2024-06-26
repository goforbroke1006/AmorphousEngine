---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/20/24 1:26 AM
---

package.path = package.path .. ';./projects/01-HelloWorld/?.lua'

require 'Core'
require 'Component'

__all_game_objects[0] = GameObject:new(0, 'Application System')
__all_game_objects[0].transform.position:Set(0.000000, 0.000000, 0.000000)
__all_game_objects[0].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[0].transform.localScale:Set(0.000000, 0.000000, 0.000000)

__all_game_objects[1] = GameObject:new(1, 'Main Camera')
__all_game_objects[1].transform.position:Set(0.000000, 100.000000, -100.000000)
__all_game_objects[1].transform.rotation:Set(-0.382683, 0.000000, 0.000000, 0.923880)

__all_game_objects[2] = GameObject:new(2, 'Light 1')
__all_game_objects[2].transform.position:Set(0.000000, 500.000000, 0.000000)
__all_game_objects[2].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)

__all_game_objects[3] = GameObject:new(3, 'Barrel center')
__all_game_objects[3].transform.position:Set(0.000000, 0.000000, 0.000000)
__all_game_objects[3].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[3].transform.localScale:Set(1.500000, 1.500000, 1.500000)

__all_game_objects[4] = GameObject:new(4, 'Barrel front')
__all_game_objects[4].transform.position:Set(0.000000, 0.000000, 50.000000)
__all_game_objects[4].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[4].transform.localScale:Set(1.500000, 1.500000, 1.500000)

__all_game_objects[5] = GameObject:new(5, 'Barrel back')
__all_game_objects[5].transform.position:Set(0.000000, 0.000000, -50.000000)
__all_game_objects[5].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[5].transform.localScale:Set(1.500000, 1.500000, 1.500000)

__all_game_objects[6] = GameObject:new(6, 'Barrel right')
__all_game_objects[6].transform.position:Set(50.000000, 0.000000, 0.000000)
__all_game_objects[6].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[6].transform.localScale:Set(1.500000, 1.500000, 1.500000)

__all_game_objects[7] = GameObject:new(7, 'Barrel left')
__all_game_objects[7].transform.position:Set(-50.000000, 0.000000, 0.000000)
__all_game_objects[7].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[7].transform.localScale:Set(1.500000, 1.500000, 1.500000)

__all_game_objects[8] = GameObject:new(8, 'Drone 1')
__all_game_objects[8].transform.position:Set(1.000000, 0.000000, 50.000000)
__all_game_objects[8].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__all_game_objects[8].transform.localScale:Set(0.250000, 0.250000, 0.250000)

require 'Scripts/ApplicationSystem'

function ApplicationSystem:new()
    instance = LuaBehaviour:new()
    setmetatable(instance, self)
    self.__index = self
    return instance
end

require 'Scripts/DroneController'

function DroneController:new()
    instance = LuaBehaviour:new()
    setmetatable(instance, self)
    self.__index = self
    return instance
end

require 'Scripts/SpinnerController'

function SpinnerController:new()
    instance = LuaBehaviour:new()
    setmetatable(instance, self)
    self.__index = self
    return instance
end

__all_components[0] = {}

__all_components[0]['ApplicationSystem'] = ApplicationSystem:new()
__all_components[0]['ApplicationSystem'].__type_name = 'ApplicationSystem'
__all_components[0]['ApplicationSystem'].__type_filepath = 'Scripts/ApplicationSystem'
__all_components[0]['ApplicationSystem'].gameObject = __all_game_objects[0]
__all_components[0]['ApplicationSystem'].transform = __all_game_objects[0].transform

__all_components[0]['ApplicationSystem'].enabled = true

__all_components[1] = {}

__all_components[1]['Camera'] = Camera:new()
__all_components[1]['Camera'].__type_name = 'Camera'
__all_components[1]['Camera'].__type_filepath = 'Component/Camera'
__all_components[1]['Camera'].gameObject = __all_game_objects[1]
__all_components[1]['Camera'].transform = __all_game_objects[1].transform

__all_components[1]['Camera'].backgroundColor = Color:new(0.000000, 0.250000, 1.000000, 1.000000)
__all_components[1]['Camera'].enabled = true
__all_components[1]['Camera'].farClipPlane = 1000.000000
__all_components[1]['Camera'].nearClipPlane = 0.100000

__all_components[2] = {}

__all_components[2]['Light'] = Light:new()
__all_components[2]['Light'].__type_name = 'Light'
__all_components[2]['Light'].__type_filepath = 'Component/Light'
__all_components[2]['Light'].gameObject = __all_game_objects[2]
__all_components[2]['Light'].transform = __all_game_objects[2].transform

__all_components[2]['Light'].color = Color:new(1.000000, 0.920000, 0.016000, 1.000000)
__all_components[2]['Light'].enabled = true
__all_components[2]['Light'].type = 'Spot'

__all_components[3] = {}

__all_components[3]['MeshRender'] = MeshRender:new()
__all_components[3]['MeshRender'].__type_name = 'MeshRender'
__all_components[3]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[3]['MeshRender'].gameObject = __all_game_objects[3]
__all_components[3]['MeshRender'].transform = __all_game_objects[3].transform

__all_components[3]['MeshRender'].enabled = true
__all_components[3]['MeshRender'].path = 'Barrel.mesh'

__all_components[3]['SpinnerController'] = SpinnerController:new()
__all_components[3]['SpinnerController'].__type_name = 'SpinnerController'
__all_components[3]['SpinnerController'].__type_filepath = 'Scripts/SpinnerController'
__all_components[3]['SpinnerController'].gameObject = __all_game_objects[3]
__all_components[3]['SpinnerController'].transform = __all_game_objects[3].transform

__all_components[3]['SpinnerController'].enabled = true
__all_components[3]['SpinnerController'].rotationSpeed = 10.000000

__all_components[4] = {}

__all_components[4]['MeshRender'] = MeshRender:new()
__all_components[4]['MeshRender'].__type_name = 'MeshRender'
__all_components[4]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[4]['MeshRender'].gameObject = __all_game_objects[4]
__all_components[4]['MeshRender'].transform = __all_game_objects[4].transform

__all_components[4]['MeshRender'].enabled = true
__all_components[4]['MeshRender'].path = 'Barrel.mesh'

__all_components[4]['SpinnerController'] = SpinnerController:new()
__all_components[4]['SpinnerController'].__type_name = 'SpinnerController'
__all_components[4]['SpinnerController'].__type_filepath = 'Scripts/SpinnerController'
__all_components[4]['SpinnerController'].gameObject = __all_game_objects[4]
__all_components[4]['SpinnerController'].transform = __all_game_objects[4].transform

__all_components[4]['SpinnerController'].enabled = true
__all_components[4]['SpinnerController'].rotationSpeed = 15.000000

__all_components[5] = {}

__all_components[5]['MeshRender'] = MeshRender:new()
__all_components[5]['MeshRender'].__type_name = 'MeshRender'
__all_components[5]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[5]['MeshRender'].gameObject = __all_game_objects[5]
__all_components[5]['MeshRender'].transform = __all_game_objects[5].transform

__all_components[5]['MeshRender'].enabled = true
__all_components[5]['MeshRender'].path = 'Barrel.mesh'

__all_components[5]['SpinnerController'] = SpinnerController:new()
__all_components[5]['SpinnerController'].__type_name = 'SpinnerController'
__all_components[5]['SpinnerController'].__type_filepath = 'Scripts/SpinnerController'
__all_components[5]['SpinnerController'].gameObject = __all_game_objects[5]
__all_components[5]['SpinnerController'].transform = __all_game_objects[5].transform

__all_components[5]['SpinnerController'].enabled = true
__all_components[5]['SpinnerController'].rotationSpeed = 10.000000

__all_components[6] = {}

__all_components[6]['MeshRender'] = MeshRender:new()
__all_components[6]['MeshRender'].__type_name = 'MeshRender'
__all_components[6]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[6]['MeshRender'].gameObject = __all_game_objects[6]
__all_components[6]['MeshRender'].transform = __all_game_objects[6].transform

__all_components[6]['MeshRender'].enabled = true
__all_components[6]['MeshRender'].path = 'Barrel.mesh'

__all_components[6]['SpinnerController'] = SpinnerController:new()
__all_components[6]['SpinnerController'].__type_name = 'SpinnerController'
__all_components[6]['SpinnerController'].__type_filepath = 'Scripts/SpinnerController'
__all_components[6]['SpinnerController'].gameObject = __all_game_objects[6]
__all_components[6]['SpinnerController'].transform = __all_game_objects[6].transform

__all_components[6]['SpinnerController'].enabled = true
__all_components[6]['SpinnerController'].rotationSpeed = 10.000000

__all_components[7] = {}

__all_components[7]['MeshRender'] = MeshRender:new()
__all_components[7]['MeshRender'].__type_name = 'MeshRender'
__all_components[7]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[7]['MeshRender'].gameObject = __all_game_objects[7]
__all_components[7]['MeshRender'].transform = __all_game_objects[7].transform

__all_components[7]['MeshRender'].enabled = true
__all_components[7]['MeshRender'].path = 'Barrel.mesh'

__all_components[7]['SpinnerController'] = SpinnerController:new()
__all_components[7]['SpinnerController'].__type_name = 'SpinnerController'
__all_components[7]['SpinnerController'].__type_filepath = 'Scripts/SpinnerController'
__all_components[7]['SpinnerController'].gameObject = __all_game_objects[7]
__all_components[7]['SpinnerController'].transform = __all_game_objects[7].transform

__all_components[7]['SpinnerController'].enabled = true
__all_components[7]['SpinnerController'].rotationSpeed = 10.000000

__all_components[8] = {}

__all_components[8]['MeshRender'] = MeshRender:new()
__all_components[8]['MeshRender'].__type_name = 'MeshRender'
__all_components[8]['MeshRender'].__type_filepath = 'Component/MeshRender'
__all_components[8]['MeshRender'].gameObject = __all_game_objects[8]
__all_components[8]['MeshRender'].transform = __all_game_objects[8].transform

__all_components[8]['MeshRender'].enabled = true
__all_components[8]['MeshRender'].path = 'ogrehead.mesh'

__all_components[8]['DroneController'] = DroneController:new()
__all_components[8]['DroneController'].__type_name = 'DroneController'
__all_components[8]['DroneController'].__type_filepath = 'Scripts/DroneController'
__all_components[8]['DroneController'].gameObject = __all_game_objects[8]
__all_components[8]['DroneController'].transform = __all_game_objects[8].transform

__all_components[8]['DroneController'].enabled = true
__all_components[8]['DroneController'].motionSpeed = 10.000000

for _, cmpTable in pairs(__all_components) do
    for _, cmpInstance in pairs(cmpTable) do
        if cmpInstance['Start'] ~= nil and type(cmpInstance['Start']) == 'function' then
            cmpInstance:Start()
        end
    end
end
__before_scene_components_awake();
__before_scene_components_start();

print("done")
