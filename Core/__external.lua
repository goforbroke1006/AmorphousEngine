---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/13/24 4:54 AM
---

require "Core/KeyCode"
require "Core/Time"

__time_delta = 0.0

__scene_next_game_object_instance_id = nil

--- Table with all game objects, placed on scene.
--- Key is instance ID.
--- Value is table that inherited from GameObject.
__all_game_objects = {}
__all_components = {}

__global_buttons_pressed = {
    [KeyCode.W] = false,
    [KeyCode.A] = false,
    [KeyCode.S] = false,
    [KeyCode.D] = false,

    [KeyCode.UpArrow] = false,
    [KeyCode.DownArrow] = false,
    [KeyCode.RightArrow] = false,
    [KeyCode.LeftArrow] = false,

    [KeyCode.Escape] = false,
    [KeyCode.Ctrl] = false,
    [KeyCode.Space] = false,

    [KeyCode.Mouse0] = false,
    [KeyCode.Mouse1] = false,
    [KeyCode.Mouse2] = false,
}
__global_buttons_hold = {}
__global_buttons_released = {
    [KeyCode.W] = false,
    [KeyCode.A] = false,
    [KeyCode.S] = false,
    [KeyCode.D] = false,

    [KeyCode.UpArrow] = false,
    [KeyCode.DownArrow] = false,
    [KeyCode.RightArrow] = false,
    [KeyCode.LeftArrow] = false,

    [KeyCode.Escape] = false,
    [KeyCode.Ctrl] = false,
    [KeyCode.Space] = false,

    [KeyCode.Mouse0] = false,
    [KeyCode.Mouse1] = false,
    [KeyCode.Mouse2] = false,
}

-- TODO: load from ${PROJECT_DIR}/input-mapping.json
__input_mapping = {
    axes = {
        ["Vertical"] = {
            ["positive"] = { [0] = KeyCode.W, [1] = KeyCode.UpArrow },
            ["negative"] = { [0] = KeyCode.S, [1] = KeyCode.DownArrow },
        },
        ["Horizontal"] = {
            ["positive"] = { [0] = KeyCode.D, [1] = KeyCode.RightArrow },
            ["negative"] = { [0] = KeyCode.A, [1] = KeyCode.LeftArrow },
        },
    },
    buttons = {
        ["Fire1"] = {
            ["triggers"] = { [0] = KeyCode.Mouse0 }
        },
    }
}

__application_quit = false

__global_prefab_game_objects = {}

function __before_scene_components_awake()
    for _, cmpInstance in pairs(__all_components) do
        if cmpInstance['Awake'] ~= nil and type(cmpInstance['Awake']) == "function" then
            local fn = cmpInstance['Awake']
            local status, err = pcall(fn, cmpInstance)
            if err ~= nil then
                Debug.LogError(err)
            end
        end
    end
end

function __before_scene_components_start()
    for _, cmpInstance in pairs(__all_components) do
        if cmpInstance['Start'] ~= nil and type(cmpInstance['Start']) == "function" then
            local fn = cmpInstance['Start']
            local status, err = pcall(fn, cmpInstance)
            if err ~= nil then
                Debug.LogError(err)
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

    for _, cmpInstance in pairs(__all_components) do
        if cmpInstance['Update'] ~= nil and cmpInstance.enabled then
            local fn = cmpInstance['Update']
            local status, err = pcall(fn, cmpInstance)
            if err ~= nil then
                Debug.LogError(err)
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
