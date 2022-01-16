#include "particlegrid.h"
#include "renderer.h"
#include "settings.h"
#include <algorithm>
#include <iostream>
#include <numeric>

std::vector<std::pair<int, int>> get_dispersion_indices(vec2 direction) {
    std::vector<std::pair<int, int>> indices;
    while (direction.y >= 1.0f) {
        if (direction.x > 0.0f) {
            int start_pos = direction.x - direction.distance();
            int end_pos = direction.x + direction.distance();
            for (int i = start_pos; i <= end_pos; i++) {
                indices.push_back({i, (int)direction.y});
            }
            direction.x -= 1.0f;
        }
        else{
            int end_pos = direction.x - direction.distance();
            int start_pos = direction.x + direction.distance();
            for(int i=start_pos; i>= end_pos;i--){
                indices.push_back({i, (int)direction.y});
            }
            direction.x += 1.0;
        }
        direction.y -= 1.0f;
    }
    return indices;
}

ParticleGrid::ParticleGrid(int width_, int height_) : width(width_), height(height_), cells(width * height) {}

void ParticleGrid::update(std::unique_ptr<ParticleGrid> &next) {
    auto direction = Settings::get_direction();
    next->cells = std::vector<particle_cell_t>(width * height);
    for (int y = 0; y < (int)std::ceil(direction.y); y++) {
        for (int i = next->width / 5; i < next->width - next->width / 5; i++) {
            color_hsl color = Settings::get_color();
            color.jitter(&color_hsl::hue, Settings::get_hue_jitter());
            color.jitter(&color_hsl::lumi, 0.1f);
            auto &cell = get_cell(i, next->height - y);
            cell.rgb = color;
        }
    }

#pragma omp parallel for
    for (int y = 0; y < next->height; y++) {
        float current_height = next->height - y;
        // uint32_t rand_seed = rand();
        for (int x = 0; x < next->width; x++) {
            auto &cell = get_cell(x, y);
            direction = Settings::get_direction();
            if(x % 2){
                direction.x += Settings::get_scatter();
            }
            else{
                direction.x -= Settings::get_scatter();
            }
            direction.x *= current_height / next->height * 2;
            color_hsl hsl = cell.rgb;
            hsl.lumi = std::clamp(hsl.lumi / (1.0f + Settings::get_delumination()), 0.0f, 1.0f);
            hsl.saturation = std::clamp(hsl.saturation - Settings::get_desaturation(), 0.1f, 1.0f);
            cell.rgb = hsl;

            auto dispersion_indices = get_dispersion_indices(direction);
            if(dispersion_indices.empty()){
                continue;
            }
            cell.rgb /= dispersion_indices.size();
            for (auto &disp : dispersion_indices) {
                auto &cell_next = next->get_cell(x + disp.first, y - disp.second);
                cell_next.rgb += cell.rgb;

            }
        }
    }

    Renderer::update(next.get());
}