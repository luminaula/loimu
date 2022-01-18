#include "settings.h"
#include "color.h"
#include "vec.h"

namespace Lumi {

namespace Settings {
static float desaturation;
static float delumination;
static float color_jitter;
static float scatter;
static float fire_width;
static bool snow;
static vec2 direction;
static ColorHsl hsl;
static ColorRgb rgb;

float get_desaturation() { return desaturation; }

float get_delumination() { return delumination; }

float get_color_jitter() { return color_jitter; }

float get_scatter() { return scatter; }

float get_fire_width() { return fire_width; }

vec2 get_direction() { return direction; }

ColorHsl get_color() { return hsl; }

bool get_snow() { return snow; }

void set_desaturation(float val) { desaturation = val; }

void set_delumination(float val) { delumination = val; }

void set_color_jitter(float val) { color_jitter = val; }

void set_scatter(float val) { scatter = val; }

void set_fire_width(float val) { fire_width = val; }

void set_direction(vec2 vec) { direction = vec; }

void set_force_x(float x) { direction.x = x; }

void set_force_y(float y) { direction.y = y; }

void set_color(ColorHsl col) { hsl = col; }

void set_snow(bool val) { snow = val; }

} // namespace Settings
} // namespace Lumi