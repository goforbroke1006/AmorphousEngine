---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by goforbroke.
--- DateTime: 1/1/24 3:42 PM
---

require 'math'
require 'Core/Debug'

Quaternion = {
    w = 0.0,
    x = 0.0,
    y = 0.0,
    z = 0.0,

    eulerAngles = Vector3,
}

function Quaternion:new(newX --[[number]], newY --[[number]], newZ --[[number]], newW --[[number]])
    if type(newX) ~= "number"
            or type(newY) ~= "number"
            or type(newZ) ~= "number"
            or type(newW) ~= "number"
    then
        error("Can not create Quaternion")
    end

    qrt = {
        x = newX,
        y = newY,
        z = newZ,
        w = newW,
    }

    self.__index = self
    setmetatable(qrt, self)

    return qrt
end

Quaternion.identity = Quaternion:new(0.0, 0.0, 0.0, 0.0)

function Quaternion.Euler(roll --[[number]], pitch --[[number]], yaw --[[number]])
    -- https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2

    roll = math.fmod(roll, 180)
    pitch = math.fmod(pitch, 180)
    yaw = math.fmod(yaw, 180)

    local rad_roll = math.rad(roll)
    local rad_pitch = math.rad(pitch)
    local rad_yaw = math.rad(yaw)

    local cr = math.cos(rad_roll * 0.5);
    local sr = math.sin(rad_roll * 0.5);
    local cp = math.cos(rad_pitch * 0.5);
    local sp = math.sin(rad_pitch * 0.5);
    local cy = math.cos(rad_yaw * 0.5);
    local sy = math.sin(rad_yaw * 0.5);

    q = Quaternion:new(0.0, 0.0, 0.0, 0.0);
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    q.eulerAngles = Vector3:new(roll, pitch, yaw)

    return q;
end

function Quaternion:Set(newX --[[number]], newY --[[number]], newZ --[[number]], newW --[[number]])
    -- https://docs.unity3d.com/ScriptReference/Quaternion.Set.html

    if type(newX) ~= "number"
            or type(newY) ~= "number"
            or type(newZ) ~= "number"
            or type(newW) ~= "number"
    then
        error("Can not set Quaternion")
    end

    self.x = newX
    self.y = newY
    self.z = newZ
    self.w = newW

    -- https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles#Source_code_2

    local sinr_cosp = 2 * (self.w * self.x + self.y * self.z);
    local cosr_cosp = 1 - 2 * (self.x * self.x + self.y * self.y);
    local roll = math.atan2(sinr_cosp, cosr_cosp);

    local sinp = math.sqrt(1 + 2 * (self.w * self.y - self.x * self.z));
    local cosp = math.sqrt(1 - 2 * (self.w * self.y - self.x * self.z));
    local pitch = 2 * math.atan2(sinp, cosp) - math.pi / 2;

    local siny_cosp = 2 * (self.w * self.z + self.x * self.y);
    local cosy_cosp = 1 - 2 * (self.y * self.y + self.z * self.z);
    local yaw = math.atan2(siny_cosp, cosy_cosp);

    roll = math.fmod(roll, 180)
    pitch = math.fmod(pitch, 180)
    yaw = math.fmod(yaw, 180)

    self.eulerAngles = Vector3:new(roll, pitch, yaw)
end

function Quaternion:Normalize()
    local magnitude = math.sqrt(
            math.pow(self.x, 2) + math.pow(self.y, 2) + math.pow(self.z, 2) + math.pow(self.w, 2)
    )
    if (magnitude ~= 0.0) then
        self.x = self.x / magnitude
        self.y = self.y / magnitude
        self.z = self.z / magnitude
    end
end

Quaternion.__mul = function(qtr --[[Quaternion]], arg --[[Quaternion|Vector3]])
    -- quaternion * quaternion
    if arg.x ~= nil and arg.y ~= nil and arg.z ~= nil and arg.w ~= nil then
        return Quaternion:new(
                qtr.w * arg.x + qtr.x * arg.w + qtr.y * arg.z - qtr.z * arg.y, -- x
                qtr.w * arg.y - qtr.x * arg.z + qtr.y * arg.w + qtr.z * arg.x, -- y
                qtr.w * arg.z + qtr.x * arg.y - qtr.y * arg.x + qtr.z * arg.w, -- z
                qtr.w * arg.w - qtr.x * arg.x - qtr.y * arg.y - qtr.z * arg.z -- w
        )
    end

    -- quaternion * vector
    if arg.x ~= nil and arg.y ~= nil and arg.z ~= nil then
        -- https://gamedev.stackexchange.com/a/28418

        local resultVec = Vector3:new(0.0, 0.0, 0.0)

        local num12 = qtr.x + qtr.x;
        local num2 = qtr.y + qtr.y;
        local num = qtr.z + qtr.z;
        local num11 = qtr.w * num12;
        local num10 = qtr.w * num2;
        local num9 = qtr.w * num;
        local num8 = qtr.x * num12;
        local num7 = qtr.x * num2;
        local num6 = qtr.x * num;
        local num5 = qtr.y * num2;
        local num4 = qtr.y * num;
        local num3 = qtr.z * num;
        local num15 = ((arg.x * ((1.0 - num5) - num3)) + (arg.y * (num7 - num9))) + (arg.z * (num6 + num10));
        local num14 = ((arg.x * (num7 + num9)) + (arg.y * ((1.0 - num8) - num3))) + (arg.z * (num4 - num11));
        local num13 = ((arg.x * (num6 - num10)) + (arg.y * (num4 + num11))) + (arg.z * ((1.0 - num8) - num5));

        resultVec.x = num15;
        resultVec.y = num14;
        resultVec.z = num13;

        return resultVec;
    end

    error("second arg has unexpected type")
end
