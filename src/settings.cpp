#include "settings.h"
#include "vec.h"
#include "color.h"

namespace Settings{
	static float desaturation;
	static float delumination;
	static float hue_jitter;
	static vec2 direction;
	static color_hsl color;

	float get_desaturation(){
		return desaturation;
	}

	float get_delumination(){
		return delumination;
	}

	float get_hue_jitter(){
		return hue_jitter;
	}
	

	vec2 get_direction(){
		return direction;
	}

	color_hsl get_color(){
		return color;
	}

	void set_desaturation(float val){
		desaturation = val;
	}

	void set_delumination(float val){
		delumination = val;
	}

	void set_hue_jitter(float val){
		hue_jitter = val;
	}

	void set_direction(vec2 vec){
		direction = vec;
	}

	void set_direction_x(float x){
		direction.x = x;
	}

	void set_direction_y(float y){
		direction.y = y;
	}

	void set_color(color_hsl col){
		color = col;
	}

}