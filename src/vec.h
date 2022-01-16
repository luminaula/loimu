#pragma once

#include <cmath>

struct vec2 {
    float x;
    float y;

    float distance() { return std::sqrt(x * x + y * y); }

	vec2 &operator+=(const vec2 &right){
		x += right.x;
		y += right.y;
		return *this;
	}
};