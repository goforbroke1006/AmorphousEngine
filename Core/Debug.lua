---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 12/29/23 8:39 PM
---

DebugLevelTrace = 1
DebugLevelDebug = 2
DebugLevelWarn = 3
DebugLevelError = 4

Debug = {}

Debug.level = DebugLevelTrace

function Debug.LogTrace(message --[[string]])
    if Debug.level <= DebugLevelTrace then
        print('TRACE: ' .. message)
    end
end

function Debug.Log(message --[[string]])
    if Debug.level <= DebugLevelDebug then
        print('DEBUG: ' .. message)
    end
end

function Debug.LogWarning(message --[[string]])
    if Debug.level <= DebugLevelWarn then
        print('WARN:  ' .. message)
    end
end

function Debug.LogError(message --[[string]])
    if Debug.level <= DebugLevelError then
        print('ERROR: ' .. message)
    end
end
