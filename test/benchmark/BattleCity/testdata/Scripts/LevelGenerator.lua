---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/21/24 12:29 AM
---

require "Core/LuaBehaviour"

LevelGenerator = LuaBehaviour:new()

LevelGenerator.levels = {
    [0] = {
        { position = Vector3:new(-50.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, 20.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, 10.0), type = "brick" },

        { position = Vector3:new(-30.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, 20.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, 10.0), type = "brick" },

        { position = Vector3:new(-10.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(-10.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(-10.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(-10.0, 0.0, 20.0), type = "brick" },


        { position = Vector3:new(10.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(10.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(10.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(10.0, 0.0, 20.0), type = "brick" },

        { position = Vector3:new(30.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, 20.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, 10.0), type = "brick" },

        { position = Vector3:new(50.0, 0.0, 50.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, 40.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, 30.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, 20.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, 10.0), type = "brick" },

        --

        { position = Vector3:new(-50.0, 0.0, -50.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, -40.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(-50.0, 0.0, -20.0), type = "brick" },

        { position = Vector3:new(-30.0, 0.0, -50.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, -40.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(-30.0, 0.0, -20.0), type = "brick" },

        { position = Vector3:new(-10.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(-10.0, 0.0, -20.0), type = "brick" },
        { position = Vector3:new(-10.0, 0.0, -10.0), type = "brick" },
        --
        { position = Vector3:new(0.0, 0.0, -20.0), type = "brick" },
        --
        { position = Vector3:new(10.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(10.0, 0.0, -20.0), type = "brick" },
        { position = Vector3:new(10.0, 0.0, -10.0), type = "brick" },

        { position = Vector3:new(30.0, 0.0, -50.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, -40.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(30.0, 0.0, -20.0), type = "brick" },

        { position = Vector3:new(50.0, 0.0, -50.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, -40.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, -30.0), type = "brick" },
        { position = Vector3:new(50.0, 0.0, -20.0), type = "brick" },
    }
}

function LevelGenerator:Awake()
    --
end

function LevelGenerator:Start()
    if self.brickWallPrefab == nil then
        Debug.LogWarning("brickWallPrefab should not be empty")
        return
    end

    self:Generate(0)
end

function LevelGenerator:Update()
    -- TODO: move to base
    -- TODO: shot forward
    -- TODO: avoid obstacles
end

function LevelGenerator:Generate(level --[[number]])
    Debug.Log("LevelGenerator:Generate level " .. level)

    for _, request in pairs(self.levels[level]) do
        if request.type == "brick" then
            Object.Instantiate(
                    self.brickWallPrefab,
                    request.position,
                    Quaternion.Euler(0.0, 0.0, 0.0)
            );
        end

        -- TODO:
    end
end