---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/5/24 11:20 AM
---

Color = {
    r = 0.0,
    g = 0.0,
    b = 0.0,
    a = 0.0,
}

function Color:new(red --[[number]], green --[[number]], blue --[[number]], alpha --[[number]])
    red = red or 1.0
    green = green or 1.0
    blue = blue or 1.0
    alpha = alpha or 1.0

    clr = {
        r = red,
        g = green,
        b = blue,
        a = alpha,
    }

    self.__index = self
    setmetatable(clr, self)

    return clr
end

Color.clear = Color:new(0.0, 0.0, 0.0, 0.0);

Color.black = Color:new(0.0, 0.0, 0.0, 1.0);
Color.white = Color:new(1.0, 1.0, 1.0, 1.0);
Color.grey = Color:new(0.5, 0.5, 0.5, 1.0);

Color.red = Color:new(1.0, 0.0, 0.0, 1.0);
Color.green = Color:new(0.0, 1.0, 0.0, 1.0);
Color.blue = Color:new(0.0, 0.0, 1.0, 1.0);

Color.magenta = Color:new(1.0, 0.0, 1.0, 1.0);
Color.cyan = Color:new(0.0, 1.0, 1.0, 1.0);

Color.yellow = Color:new(1.0, 0.92, 0.016, 1.0);
