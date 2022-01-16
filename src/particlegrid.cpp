#include "particlegrid.h"
#include "renderer.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include "settings.h"

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
    next->cells = std::vector<particle_cell_t>(width * height);
    #pragma omp parallel for
    for (int y = 0; y < next->height; y++) {
        for (int x = 0; x < next->width; x++) {
            auto &cell = get_cell(x, y);
            color_hsl hsl = cell.rgb;
            hsl.lumi = std::clamp(hsl.lumi - 0.002f, 0.0f, 1.0f);
            hsl.saturation = std::clamp(hsl.saturation - 0.0001f, 0.0f, 1.0f);
            cell.rgb = hsl;

            auto dispersion_indices = get_dispersion_indices({0.0f,2.0f});
			cell.rgb /= dispersion_indices.size();
            for (auto &disp : dispersion_indices) {
                next->get_cell(x + disp.first, y - disp.second).rgb += cell.rgb;
            }

        }
    }
    for (int i = 0; i <= next->width; i++) {
        color_hsl fire_color;
        fire_color.randomize(&color_hsl::hue, 0.1, 0.01);
        fire_color.randomize(&color_hsl::lumi, 0.8, 0.1);
        fire_color.randomize(&color_hsl::saturation, 0.8, 0.1);
        particle_cell_t &cell = next->get_cell(i,next->height);
        cell.rgb = fire_color;
        cell.force.x = 0.0f;
		cell.force.y = 2.0f;
    }

    Renderer::update(next.get());
}