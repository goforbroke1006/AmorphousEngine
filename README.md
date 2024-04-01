# Amorphous Engine

Open source 3D graphics + Lua scripting similar to Unity3D components.

### Usage

Scenes and Prefabs is JSON files. No editor app but it is easy to create it manually.

Components are LUA files with LUA tables:

```lua
require "Core/KeyCode"
require "Core/Application"

ApplicationSystem = LuaBehaviour:new()

function ApplicationSystem:Start()
    -- TODO:
end

function ApplicationSystem:Update()
    if Input.GetKeyUp(KeyCode.Escape) then
        Debug.Log("Quit...");
        Application.Quit();
    end

    local playerCmp = Object.FindObjectOfType(PlayerController)
    if playerCmp == nil then
        Debug.Log("<<< GAME OVER >>>");
        Application.Quit();
    end
end
```

### Status

| Feature        | Part               | Status                                  |
|----------------|--------------------|-----------------------------------------|
| Object         |                    |                                         |
|                | Scene manipulation | Instantiate(...), Destroy(...) ok       |
|                | Components access  | GetComponent(table), GetComponents() ok |
| Input          |                    |                                         |
|                | Keyboard           | W,A,S,D and arrows pressing/releasing   |
|                | Mouse              | Click event for LMB                     |
| Transformation |                    |                                         |
|                | Vector             | Most of functions                       |
|                | Quaternion         | Most of functions                       |
|                | Transform          | Translate() and Rotate() works now      |
| Physics        |                    |                                         |
|                | BoxCollider        | OnCollisionEnter() works                |
|                | CapsuleCollider    | -                                       |
