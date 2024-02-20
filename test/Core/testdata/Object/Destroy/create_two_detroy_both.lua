---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/20/24 12:34 AM
---

require 'Core/__lib'
require 'Core/__external'
require 'Core/__runtime'
require 'Core/Debug'
require 'Core/LuaBehaviour'
require 'Core/Object'
require 'Core/GameObject'

Debug.level = DebugLevelDebug

-- ensure that scene empty
print(table_length(__all_game_objects))
print(table_length(__all_components))

BotController = LuaBehaviour:new()
BotController.motionSpeed = 1.0
function BotController:Start()  end
function BotController:Update()
  self.transform:Translate(Vector3.forward * self.motionSpeed);
end

EnemyController = LuaBehaviour:new()
function EnemyController:Start()  end
function EnemyController:Update()  end

local orig = GameObject:new(-1, 'Some entity')
orig.transform.position:Set(10.0, 20.0, 30.0)
orig.__components['BotController'] = BotController
orig.__components['BotController'].__type_name = 'BotController'
orig.__components['EnemyController'] = EnemyController
orig.__components['EnemyController'].__type_name = 'EnemyController'

local clone1 = Object.Instantiate(orig)
print(clone1.name)
print(table_length(__all_game_objects))
print(table_length(__all_components))

local clone2 = Object.Instantiate(orig)
print(clone2.name)
print(table_length(__all_game_objects))
print(table_length(__all_components))

-- check Update works fine
__before_update_frame(); __on_update_frame(); __after_update_frame();

Object.Destroy(clone1)
print(table_length(__all_game_objects))
print(table_length(__all_components))

Object.Destroy(clone2)
print(table_length(__all_game_objects))
print(table_length(__all_components))
