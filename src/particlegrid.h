#pragma once

#include "color.h"
#include "vec.h"
#include <memory>
#include <stddef.h>
#include <vector>

struct particle_cell_t {
    color_rgb rgb;
    vec2 force;
    vec2 accum;

    particle_cell_t &operator+=(const particle_cell_t &other) {
        rgb += other.rgb;
        return *this;
    }

    friend particle_cell_t operator+(particle_cell_t left, const particle_cell_t &right) {
        left.rgb += right.rgb;
        return left;
    }
    particle_cell_t &operator/=(float val) {
        rgb /= val;
        return *this;
    }
    friend particle_cell_t operator/(particle_cell_t left, const float &val) {
        left.rgb /= val;
        return left;
    }
};

class ParticleGrid {
  private:
    int width, height;
    std::vector<particle_cell_t> cells;
    float decay = 1.01f;

  public:
    ParticleGrid(int width_, int height_);
    particle_cell_t &get_cell(int x, int y) { return cells[std::clamp(y, 0, height - 1) * width + std::clamp(x, 0, width - 1)]; }
    void set_cell(int x, int y, particle_cell_t cell) { cells[std::clamp(y, 0, height - 1) * width + std::clamp(x, 0, width - 1)] = cell; }
    void update(std::unique_ptr<ParticleGrid> &next);
};