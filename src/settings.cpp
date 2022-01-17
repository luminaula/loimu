#include "settings.h"
#include "vec.h"
#include "color.h"

namespace Settings{
	static float desaturation;
	static float delumination;
	static float color_jitter;
	static float scatter;
	static vec2 direction;
	static color_hsl hsl;
	static color_rgb rgb;

	float get_desaturation(){
		return desaturation;
	}

	float get_delumination(){
		return delumination;
	}

	float get_color_jitter(){
		return color_jitter;
	}

	float get_scatter(){
		return scatter;
	}
	

	vec2 get_direction(){
		return direction;
	}

	color_hsl get_color(){
		return hsl;
	}

	void set_desaturation(float val){
		desaturation = val;
	}

	void set_delumination(float val){
		delumination = val;
	}

	void set_color_jitter(float val){
		color_jitter = val;
	}

	void set_scatter(float val){
		scatter = val;
	}

	void set_direction(vec2 vec){
		direction = vec;
	}

	void set_force_x(float x){
		direction.x = x;
	}

	void set_force_y(float y){
		direction.y = y;
	}

	void set_color(color_hsl col){
		hsl = col;
	}

}