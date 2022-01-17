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
                indices.push_back({i, (int)direction.y});
            }
            direction.x -= 1.0f;
        } else if (direction.x <= -0.5f) {
            int start_pos = direction.x + width / 2;
            int end_pos = direction.x - width / 2;
            for (int i = start_pos; i >= end_pos; i--) {
                indices.push_back({i, (int)direction.y});
            }
            direction.x += 1.0;
        } else {
            for (int i = -1; i <= 1; i++) {
                indices.push_back({i, (int)direction.y});
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

void ParticleGrid::update_particle_rows(ParticleGrid &next, int lower, int upper) {}

void ParticleGrid::update_particles(ParticleGrid &next) {
    auto direction = Settings::get_direction();
    next.cells = std::vector<particle_cell_t>(width * height);
    for (int y = 0; y < (int)std::ceil(direction.y); y++) {
        for (int i = 0; i < next.width; i++) {
            color_hsl color = Settings::get_color();
            color.jitter(&color_hsl::hue, Settings::get_color_jitter());
            color.jitter(&color_hsl::lumi, 0.1f);
            auto &cell = get_cell(i, next.height - y);
            cell.value = color;
        }
    }
    // race condition for effects is a dirty hack
    //  #pragma omp parallel for
    for (int y = 0; y < next.height; y++) {
        float current_height = next.height - y;
        for (int x = 0; x < next.width; x++) {
            auto &cell = get_cell(x, y);
            cell.value.jitter_all(Settings::get_color_jitter() * 10);
            float value_average = (cell.value.red + cell.value.blue + cell.value.green) / 3.0f;
            if (value_average < 1.0f) {
                continue;
            }
            direction = Settings::get_direction();
            direction.x *= current_height / next.height;
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
    for (int i = 0; i < width * height; i++) {
        next.buffer[i] = next.cells[i].value.to_pixel();
    }
}
