#include "particlegrid.h"
#include "renderer.h"
#include "settings.h"
#include <algorithm>
#include <iostream>
#include <numeric>

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

void ParticleGrid::spawn_fire(){
    auto direction = Settings::get_direction();
    for (int y = 0; y < (int)std::ceil(direction.y); y++) {
        for (int i = 0; i < width; i++) {
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
    for (int y = lower; y < std::min(upper,next.height); y++) {
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

void ParticleGrid::update_particles(ParticleGrid &next) {
    
    update_particle_rows(next,0,next.height);

    
}

void ParticleGrid::update_framebuffer(){
    for (int i = 0; i < width * height; i++) {
        buffer[i] = cells[i].value.to_pixel();
    }
}

