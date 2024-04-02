---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 2/19/24 11:46 PM
---

require "Core/__external"
require "Core/__physics"
require "Core/Collision"

function __before_scene_components_awake()
    for _, cmpTable in pairs(__all_components) do
        for _, cmpInstance in pairs(cmpTable) do
            if cmpInstance['Awake'] ~= nil and type(cmpInstance['Awake']) == "function"
                    and cmpInstance.enabled
            then
                local fn = cmpInstance['Awake']
                local status, err = pcall(fn, cmpInstance)
                if err ~= nil then
                    Debug.LogError(err)
                end
            end
        end
    end
end

function __before_scene_components_start()
    for _, cmpTable in pairs(__all_components) do
        for _, cmpInstance in pairs(cmpTable) do
            if cmpInstance['Start'] ~= nil and type(cmpInstance['Start']) == "function"
                    and cmpInstance.enabled
            then
                local fn = cmpInstance['Start']
                local status, err = pcall(fn, cmpInstance)
                if err ~= nil then
                    Debug.LogError(err)
                end
            end
        end
    end
end

function __before_update_frame()
    --if __application_quit then
    --    return
    --end

    Time.deltaTime = __time_delta

    for keyCode, keyState in pairs(__global_buttons_pressed) do
        if keyState == true then
            if __global_buttons_hold[keyCode] == true then
                __global_buttons_pressed[keyCode] = false
            else
                __global_buttons_hold[keyCode] = true
            end
        end
    end

    for keyCode, keyState in pairs(__global_buttons_released) do
        if keyState == true then
            __global_buttons_hold[keyCode] = false
        end
    end
end

function __on_update_frame()
    --if __application_quit then
    --    return
    --end

    for _, cmpTable in pairs(__all_components) do
        for _, cmpInstance in pairs(cmpTable) do
            if cmpInstance['enabled'] and cmpInstance['Update'] ~= nil and type(cmpInstance['Update']) == "function" then
                local fn = cmpInstance['Update']
                local status, err = pcall(fn, cmpInstance)
                if err ~= nil then
                    Debug.LogError(err)
                end
            end
        end
    end
end

function __check_all_collisions()
    -- TODO: benchmark and metrics show that this method takes most of time.

    --if __application_quit then
    --    return
    --end

    local colliders = {}
    for _, cmpTable in pairs(__all_components) do
        for _, cmpInstance in pairs(cmpTable) do
            if cmpInstance["IsA"] ~= nil
                    and type(cmpInstance["IsA"]) == "function"
                    and cmpInstance:IsA("Collider")
            then
                colliders[cmpInstance.gameObject.__instanceID] = cmpInstance

                --Debug.LogTrace("Found '" .. cmpInstance.gameObject.name .. "' colliders")
            end
        end
    end

    --Debug.LogTrace("Found " .. table_length(colliders) .. " colliders")

    for objID1, col1 in pairs(colliders) do
        for objID2, col2 in pairs(colliders) do
            if objID1 ~= objID2 then
                if PhysicsSystem.hasCollision(col1, col2) then
                    for _, cmp in pairs(__all_components[objID1]) do
                        if cmp["OnCollisionEnter"] ~= nil then
                            cmp:OnCollisionEnter(
                                    Collision:new(__all_game_objects[objID2])
                            )
                        end
                    end
                    for _, cmp in pairs(__all_components[objID2]) do
                        if cmp["OnCollisionEnter"] ~= nil then
                            cmp:OnCollisionEnter(
                                    Collision:new(__all_game_objects[objID1])
                            )
                        end
                    end
                end
            end
        end
    end
end

function __after_update_frame()
    --if __application_quit then
    --    return
    --end

    for keyCode, _ in pairs(__global_buttons_pressed) do
        __global_buttons_pressed[keyCode] = false
    end

    for keyCode, _ in pairs(__global_buttons_released) do
        __global_buttons_released[keyCode] = false
    end

end
