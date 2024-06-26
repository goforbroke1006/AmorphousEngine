---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/10/24 10:49 PM
---

require "Core/__lib"
require "Core/LuaBehaviour"
require "Core/GameObject"

EnemyController = {}
function EnemyController:new()
    local instance = LuaBehaviour:new()
    setmetatable(instance, self)
    self.__index = self
    return instance
end

ProjectileController = {}
function ProjectileController:new()
    local instance = LuaBehaviour:new()
    setmetatable(instance, self)
    self.__index = self
    return instance
end

__global_prefab_game_objects = {}

__global_prefab_game_objects['./Prefabs/Bot.json'] = GameObject:new(-1, 'Bot')
__global_prefab_game_objects['./Prefabs/Bot.json'].transform.position:Set(0.000000, 0.000000, 0.000000)
__global_prefab_game_objects['./Prefabs/Bot.json'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__global_prefab_game_objects['./Prefabs/Bot.json'].transform.localScale:Set(1.000000, 1.000000, 1.000000)

__global_prefab_game_objects['./Prefabs/Bot.json'].__components['EnemyController'] = EnemyController:new()
__global_prefab_game_objects['./Prefabs/Bot.json'].__components['EnemyController'].__type_name = 'EnemyController'
__global_prefab_game_objects['./Prefabs/Bot.json'].__components['EnemyController'].gameObject = nil
__global_prefab_game_objects['./Prefabs/Bot.json'].__components['EnemyController'].transform = nil
__global_prefab_game_objects['./Prefabs/Bot.json'].__components['EnemyController'].enabled = true

__global_prefab_game_objects['./Prefabs/Projectile.json'] = GameObject:new(-1, 'Projectile')
__global_prefab_game_objects['./Prefabs/Projectile.json'].transform.position:Set(0.000000, 0.000000, 0.000000)
__global_prefab_game_objects['./Prefabs/Projectile.json'].transform.rotation:Set(0.000000, 0.000000, 0.000000, 1.000000)
__global_prefab_game_objects['./Prefabs/Projectile.json'].transform.localScale:Set(1.000000, 1.000000, 1.000000)

__global_prefab_game_objects['./Prefabs/Projectile.json'].__components['ProjectileController'] = ProjectileController:new()
__global_prefab_game_objects['./Prefabs/Projectile.json'].__components['ProjectileController'].__type_name = 'ProjectileController'
__global_prefab_game_objects['./Prefabs/Projectile.json'].__components['ProjectileController'].gameObject = nil
__global_prefab_game_objects['./Prefabs/Projectile.json'].__components['ProjectileController'].transform = nil
__global_prefab_game_objects['./Prefabs/Projectile.json'].__components['ProjectileController'].enabled = true

print(table_length(__global_prefab_game_objects['./Prefabs/Bot.json'].__components))