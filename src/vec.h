#pragma once

#include <cmath>

namespace Lumi {

struct vec2 {
    float x;
    float y;

    vec2() : x(0.0f), y(0.0f) {}

    float distance() { return std::sqrt(x * x + y * y); }

    vec2 &operator+=(const vec2 &right) {
        x += right.x;
        y += right.y;
        return *this;
    }

    vec2 &operator/=(float val) {
        x /= val;
        y /= val;
        return *this;
    }
};
} // namespace Lumi