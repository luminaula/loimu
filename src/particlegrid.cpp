#include "particlegrid.h"
#include "renderer.h"
#include "settings.h"
#include <algorithm>
#include <iostream>
#include <numeric>

namespace Lumi {

std::vector<std::pair<int, int>> get_dispersion_indices(vec2 direction) {
    std::vector<std::pair<int, int>> indices;
    float width = 4.0f;
    while (direction.y >= 0.0f) {
        if (direction.x >= 0.5f) {
            int start_pos = direction.x - width / 2;
            int end_pos = direction.x + width / 2;
            for (int i = start_pos; i <= end_pos; i++) {
                indices.push_back({i, std::ceil(direction.y)});
            }
            direction.x -= 1.0f;
        } else if (direction.x <= -0.5f) {
            int start_pos = direction.x + width / 2;
            int end_pos = direction.x - width / 2;
            for (int i = start_pos; i >= end_pos; i--) {
                indices.push_back({i, std::ceil(direction.y)});
            }
            direction.x += 1.0;
        } else {
            for (int i = -1; i <= 1; i++) {
                indices.push_back({i, std::ceil(direction.y)});
            }
        }

        width -= 1.0f;
        if (width < 1.0f) {
            width = 1.0f;
        }
        direction.y -= 1.0f;
    }
    return indices;
}

void ParticleGrid::spawn_fire() {
    auto direction = Settings::get_direction();
    float fire_width = Settings::get_fire_width();
    float fire_start = (1.0f - fire_width) / 2.0f;
    float fire_end = fire_start + fire_width;
    for (int y = 0; y < (int)std::ceil(direction.y); y++) {
        for (int i = fire_start * width; i < width * fire_end; i++) {
            color_hsl color = Settings::get_color();
            color.jitter(&color_hsl::hue, Settings::get_color_jitter());
            color.jitter(&color_hsl::lumi, 0.2f);
            auto &cell = get_cell(i, height - y);
            cell.value = color;
        }
    }
}

void ParticleGrid::update_particle_rows(ParticleGrid &next, int lower, int upper) {
    auto direction = Settings::get_direction();
    for (int y = lower; y < std::min(upper, next.height); y++) {
        float current_height = height - y;
        for (int x = 0; x < next.width; x++) {
            auto &cell = get_cell(x, y);
            float value_average = (cell.value.red + cell.value.blue + cell.value.green) / 3.0f;
            if (value_average < 1.0f) {
                continue;
            }
            cell.value.jitter_all(Settings::get_color_jitter());
            direction = Settings::get_direction();
            direction.x *= current_height * 2.0 / (float)next.height;
            if (x % 2) {
                direction.x += Settings::get_scatter();
            } else {
                direction.x -= Settings::get_scatter();
            }

            cell.value /= (1.0f + Settings::get_delumination());

            float desaturation = Settings::get_desaturation();
            auto desaturate = [](float &val, float target, float amount) {
                if (val > target) {
                    val -= amount;
                } else {
                    val += amount;
                }
            };
            desaturate(cell.value.red, value_average, desaturation);
            desaturate(cell.value.blue, value_average, desaturation);
            desaturate(cell.value.green, value_average, desaturation);

            auto dispersion_indices = get_dispersion_indices(direction);
            if (dispersion_indices.empty()) {
                continue;
            }
            cell.value /= dispersion_indices.size();
            for (auto &disp : dispersion_indices) {
                auto &cell_next = next.get_cell(x + disp.first, y - disp.second);
                cell_next.value += cell.value;
            }
        }
    }
}

void ParticleGrid::update_particles(ParticleGrid &next) { update_particle_rows(next, 0, next.height); }

void ParticleGrid::update_framebuffer() {
    for (int i = 0; i < width * height; i++) {
        buffer[i] = cells[i].value.to_pixel();
    }
}

void ParticleGrid::update_snow(std::list<Lumi::Snow> &snow) {
    for (auto flake = snow.begin(); flake != snow.end(); flake++) {
        auto &cell = get_cell((*flake).x, (*flake).y);
        color_hsl hsl = cell.value;
        if ((*flake).y >= height - 10 || (*flake).x >= width - 12  || (*flake).x <= 12 || hsl.lumi > 0.05 ) {
            auto iter_copy = flake;
            flake++;
            snow.erase(iter_copy);
            continue;
        }
        for (int y = 0; y < 12; y++) {
            for (int x = 0; x < 12; x++) {
                if (Lumi::snow_pattern[y][x]) {
                    buffer[((*flake).y + y) * width + ((*flake).x + x + std::cos((*flake).offset) * 20.0f)] = 0xFFFFFFFF;
                }
            }
        }

        (*flake).offset += 0.1f;
        (*flake).y += 1.0f;
        (*flake).x += Settings::get_direction().x;
    }

    Lumi::Snow flake;
    flake.x = rand_value(0, width - 13);
    if ((int)flake.x % 25 == 0 && Settings::get_snow()) {
        flake.y = 15.0f;
        flake.offset = rand_value(0.0, M_PI);
        snow.push_back(flake);
    }
}
} // namespace Lumi