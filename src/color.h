#pragma once

#include <cmath>
#include <functional>
#include <random>
#include <stdint.h>

namespace Lumi {

struct ColorHsl;
struct ColorRgb;

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

struct ColorHsl {
    float hue;
    float saturation;
    float lumi;
    ColorHsl() = default;
    ColorHsl(const ColorRgb &rgb);
    ColorHsl(float hue_, float saturation_, float lumi_) : hue(hue_), saturation(saturation_), lumi(lumi_) {}

    void randomize(float ColorHsl::*variable, float mean, float deviation) { this->*variable = rand_value_normal(mean, deviation); }

    void jitter(float ColorHsl::*variable, float deviation) {
        randomize(variable, this->*variable, deviation);
        this->*variable = std::clamp(this->*variable, 0.0f, 1.0f);
    }
};

struct ColorRgb {
    float red;
    float green;
    float blue;
    ColorRgb() = default;
    ColorRgb(float r, float g, float b) : red(r), green(g), blue(b) {}
    ColorRgb(const ColorHsl &hsl);

    ColorRgb &operator+=(const ColorRgb &right) {
        red += right.red;
        green += right.green;
        blue += right.blue;
        return *this;
    }

    friend ColorRgb operator+(ColorRgb left, const ColorRgb &right) {
        left.red += right.red;
        left.green += right.green;
        left.blue += right.blue;
        return left;
    }

    ColorRgb &operator/=(float val) {
        red /= val;
        green /= val;
        blue /= val;
        return *this;
    }

    friend ColorRgb operator/(ColorRgb left, const float &val) {
        left /= val;
        return left;
    }

    uint32_t to_pixel();

    void randomize(float ColorRgb::*variable, float mean, float deviation) { this->*variable = rand_value_normal(mean, deviation); }

    void jitter(float ColorRgb::*variable, float deviation) {
        randomize(variable, this->*variable, deviation);
        this->*variable = std::clamp(this->*variable, 0.0f, 255.0f);
    }

    void jitter_all(float deviation) {
        red = rand_value_normal(red, red * deviation);
        red = std::clamp(red, 0.0f, 255.0f);
        green = rand_value_normal(green, green * deviation);
        green = std::clamp(green, 0.0f, 255.0f);
        blue = rand_value_normal(blue, blue * deviation);
        blue = std::clamp(blue, 0.0f, 255.0f);
    }
};
} // namespace Lumi