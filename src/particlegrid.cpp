#include "particlegrid.h"
#include "renderer.h"
#include "settings.h"
#include <algorithm>
#include <iostream>
#include <numeric>

std::vector<std::pair<int, int>> get_dispersion_indices(vec2 direction) {
    std::vector<std::pair<int, int>> indices;
    while (direction.y >= 1.0f) {
        int start_pos = direction.x - direction.distance();
        int end_pos = direction.x + direction.distance();
        for (int i = start_pos; i <= end_pos; i++) {
            indices.push_back({i, (int)direction.y});
        }
        direction.x /= 2.0;
        direction.y -= 1.0f;
    }
    return indices;
}

ParticleGrid::ParticleGrid(int width_, int height_) : width(width_), height(height_), cells(width * height) {}

void ParticleGrid::update(std::unique_ptr<ParticleGrid> &next) {
    auto direction = Settings::get_direction();
    next->cells = std::vector<particle_cell_t>(width * height);
#pragma omp parallel for
    for (int y = 0; y < (int)std::ceil(direction.y); y++) {
        for (int i = 0; i < next->width; i++) {
            color_hsl color = Settings::get_color();
            color.jitter_hue(Settings::get_hue_jitter());
            get_cell(i, next->height - y).rgb = color;
        }
    }
#pragma omp parallel for
    for (int y = 0; y < next->height; y++) {
        for (int x = 0; x < next->width; x++) {
            auto &cell = get_cell(x, y);
            direction = Settings::get_direction();
            direction.x *= y / next->height;
            color_hsl hsl = cell.rgb;
            hsl.lumi = std::clamp(hsl.lumi - Settings::get_delumination(), 0.0f, 1.0f);
            hsl.saturation = std::clamp(hsl.saturation - Settings::get_desaturation(), 0.0f, 1.0f);
            cell.rgb = hsl;

            auto dispersion_indices = get_dispersion_indices(direction);
            cell.rgb /= dispersion_indices.size();
            for (auto &disp : dispersion_indices) {
                next->get_cell(x + disp.first, y - disp.second).rgb += cell.rgb;
            }
        }
    }

    

    Renderer::update(next.get());
}