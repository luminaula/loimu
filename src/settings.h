#pragma once

#include "vec.h"
#include "color.h"

namespace Settings {


	float get_desaturation();
	float get_delumination();
	vec2 get_direction();
	color_hsl get_color();


	void set_desaturation(float val);
	void set_delumination(float val);
	void set_direction(vec2 vec);
	void set_direction_x(float x);
	void set_direction_y(float y);
	void set_color(color_hsl color);
}