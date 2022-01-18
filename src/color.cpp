#include "color.h"
#include <stddef.h>

namespace Lumi {

ColorHsl::ColorHsl(const ColorRgb &rgb) {
    float value_max, value_min;
    float r = rgb.red / 255.0f;
    float g = rgb.green / 255.0f;
    float b = rgb.blue / 255.0f;
    value_max = std::max(std::max(r, g), b);
    value_min = std::min(std::min(r, g), b);

    lumi = (value_max + value_min) / 2.0;
    if (value_max == value_min) {
        hue = 0.0f;
        saturation = 0.0f;
    } else {
        float range = value_max - value_min;
        if (lumi > 0.5) {
            saturation = range / (2 - value_max - value_min);
        } else {
            saturation = range / (value_max + value_min);
        }
        if (r > g && r > b)
            hue = (g - b) / range + (g < b ? 6.0f : 0.0f);
        else if (g > b)
            hue = (b - r) / range + 2.0f;
        else
            hue = (r - g) / range + 4.0f;

        hue /= 6;
    }
}

ColorRgb::ColorRgb(const ColorHsl &hsl) {
    if (hsl.saturation == 0.0f) {
        red = hsl.lumi * 255.0;
        green = hsl.lumi * 255.0;
        blue = hsl.lumi * 255.0;
    } else {
        float chroma = (1 - std::abs(2 * hsl.lumi - 1)) * hsl.saturation;
        float hue = hsl.hue * 6.0f;
        float x = chroma * (1 - std::abs(std::fmod(hue, 2.0f) - 1));
        float magnitude = hsl.lumi - chroma / 2;

        auto hue_to_rgb = [](float chroma, float x, float hue) -> auto {
            if (hue < 0.0f)
                return std::make_tuple(0.0f, 0.0f, 0.0f);
            int hue_floor = static_cast<int>(std::floor(hue));
            switch (hue_floor) {
            case 0:
                return std::make_tuple(chroma, x, 0.0f);
            case 1:
                return std::make_tuple(x, chroma, 0.0f);
            case 2:
                return std::make_tuple(0.0f, chroma, x);
            case 3:
                return std::make_tuple(0.0f, x, chroma);
            case 4:
                return std::make_tuple(x, 0.0f, chroma);
            case 5:
                return std::make_tuple(chroma, 0.0f, x);
            default:
                return std::make_tuple(0.0f, 0.0f, 0.0f);
            }
        };
        std::tie(red, green, blue) = hue_to_rgb(chroma, x, hue);
        red = (red + magnitude) * 255.0f;
        green = (green + magnitude) * 255.0f;
        blue = (blue + magnitude) * 255.0f;
    }
}

uint32_t ColorRgb::to_pixel() {
    uint32_t pix = 0xFF000000;
    uint8_t r = static_cast<uint8_t>(std::clamp(red, 0.0f, 255.0f));
    uint8_t b = static_cast<uint8_t>(std::clamp(blue, 0.0f, 255.0f));
    uint8_t g = static_cast<uint8_t>(std::clamp(green, 0.0f, 255.0f));
    pix |= r << 16;
    pix |= g << 8;
    pix |= b;
    return pix;
}
} // namespace Lumi