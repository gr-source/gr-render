#ifndef RECT_H
#define RECT_H

struct Rect
{
    union {
        struct {
            float x;
            float y;
            float w;
            float h;
        };
        float data[4];
    };

    constexpr Rect()
        : x(0), y(0), w(0), h(0)
    {
    }

    constexpr Rect(float x, float y, float w, float h)
        : x(x), y(y), w(w), h(h)
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
};


#endif // !RECT_H






