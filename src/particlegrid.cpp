#include "particlegrid.h"
#include "renderer.h"
#include <algorithm>
#include <iostream>
#include <numeric>

ParticleGrid::ParticleGrid(int width_, int height_) : width(width_), height(height_), cells(width * height) {}

void ParticleGrid::update(std::unique_ptr<ParticleGrid> &next) {
    std::vector<std::pair<int, int>> fire_pattern;
    fire_pattern.push_back(std::make_pair(0, 0));
    fire_pattern.push_back(std::make_pair(0, 1));
    fire_pattern.push_back(std::make_pair(0, 2));
    fire_pattern.push_back(std::make_pair(-1, 1));
    fire_pattern.push_back(std::make_pair(1, 1));
#pragma omp parallel for
    for (int y = 0; y < next->height; y++) {
        for (int x = 0; x < next->width; x++) {
            particle_cell_t cell;
            cell.rgb = {0.0, 0.0, 0.0};
            for (const auto &fire : fire_pattern) {
                cell += get_cell(x + fire.first, y + fire.second);
            }
            cell /= fire_pattern.size();
            color_hsl hsl = cell.rgb;
            hsl.lumi = std::clamp(hsl.lumi - 0.003f, 0.0f, 1.0f);
            // hsl.saturation = std::clamp(hsl.saturation - 0.007f,0.0f,1.0f);
            cell.rgb = hsl;
            next->set_cell(x, y, cell);
        }
    }
    static float hue = 0.0f;
    for (int i = 0; i < next->width; i++) {
        color_hsl fire_color;
        fire_color.randomize(&color_hsl::hue, 0.15, 0.1);
        fire_color.randomize(&color_hsl::lumi, 0.8, 0.1);
        fire_color.randomize(&color_hsl::saturation, 0.6, 0.3);
        particle_cell_t cell;
        cell.rgb = fire_color;
        next->set_cell(i, next->height - 1, cell);
    }
    hue = std::fmod(hue, 1.0f);
    hue += 0.001;
    Renderer::update(next.get());
}