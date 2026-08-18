#pragma once

struct Color
{
    union
    {
        struct
        {
            float r, g, b, a;
        };

        float data[4];
    };

    constexpr Color()
        : r(0), g(0), b(0), a(1)
    {
    }

    constexpr Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
    {
    }

    const float& operator [](int index) const
    {
        return data[index];
    }

    float& operator [](int index)
    {
        return data[index];
    }

    static constexpr Color Black()       { return {0, 0, 0, 1}; }
    static constexpr Color White()       { return {1, 1, 1, 1}; }
    static constexpr Color Red()         { return {1, 0, 0, 1}; }
    static constexpr Color Green()       { return {0, 1, 0, 1}; }
    static constexpr Color Blue()        { return {0, 0, 1, 1}; }
    static constexpr Color Yellow()      { return {1, 1, 0, 1}; }
    static constexpr Color Cyan()        { return {0, 1, 1, 1}; }
    static constexpr Color Magenta()     { return {1, 0, 1, 1}; }
    static constexpr Color Transparent() { return {0, 0, 0, 0}; }
};





