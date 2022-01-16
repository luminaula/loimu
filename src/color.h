#pragma once

#include <cmath>
#include <functional>
#include <random>
#include <stdint.h>

struct color_hsl;
struct color_rgb;

template <std::floating_point T> T rand_value_normal(T mean, T deviation) {
    static std::random_device rd{};
    static std::mt19937 gen{rd()};
    std::normal_distribution<> distribution{mean, deviation};
    return distribution(gen);
}

template <std::integral T> T rand_value(T low, T high) {
    if (low > high) {
        std::swap(low, high);
    }
    return static_cast<T>(rand()) % (high + 1 - low) + low;
}

template <std::floating_point T> T rand_value(T low, T high) {
    if (low > high) {
        std::swap(low, high);
    }
    return static_cast<T>(rand()) / RAND_MAX * (high - low) + low;
}

struct color_hsl {
    float hue;
    float saturation;
    float lumi;
    color_hsl() = default;
    color_hsl(const color_rgb &rgb);
    color_hsl(float hue_, float saturation_, float lumi_) : hue(hue_), saturation(saturation_), lumi(lumi_) {}

    void randomize(float color_hsl::*variable, float mean, float deviation) { this->*variable = rand_value_normal(mean, deviation); }

    void jitter(float color_hsl::*variable, float deviation){
        randomize(variable,this->*variable,deviation);
        this->*variable = std::clamp(this->*variable,0.0f,1.0f);
    }

};

struct color_rgb {
    float red;
    float green;
    float blue;
    color_rgb() = default;
    color_rgb(float r, float g, float b) : red(r), green(g), blue(b) {}
    color_rgb(const color_hsl &hsl);


    color_rgb &operator+=(const color_rgb &right) {
        red += right.red;
        green += right.green;
        blue += right.blue;
        return *this;
    }

    friend color_rgb operator+(color_rgb left, const color_rgb &right) {
        left.red += right.red;
        left.green += right.green;
        left.blue += right.blue;
        return left;
    }

    color_rgb &operator/=(float val) {
        red /= val;
        green /= val;
        blue /= val;
        return *this;
    }

	friend color_rgb operator/(color_rgb left, const float &val){
		left /= val;
		return left;
	}

    uint32_t to_pixel();
};
