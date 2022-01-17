#pragma once

#include "color.h"
#include "vec.h"

namespace Lumi {

namespace Settings {

float get_desaturation();
float get_delumination();
float get_color_jitter();
float get_scatter();
vec2 get_direction();
color_hsl get_color();

bool get_snow();

void set_desaturation(float val);
void set_delumination(float val);
void set_color_jitter(float val);
void set_scatter(float val);
void set_direction(vec2 vec);
void set_force_x(float x);
void set_force_y(float y);
void set_color(color_hsl color);

void set_snow(bool val);
} // namespace Settings
} // namespace Lumi