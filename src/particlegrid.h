#pragma once

#include "color.h"
#include "renderer.h"
#include "settings.h"
#include "snow.h"
#include "vec.h"
#include <list>
#include <memory>
#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace Lumi {

std::vector<std::pair<int, int>> get_dispersion_indices(vec2 direction);

struct particle_cell_t {
    color_rgb value;
};

class ParticleGrid {
  private:
    int width, height;
    std::vector<particle_cell_t> cells;
    std::vector<uint32_t> buffer;

  public:
    ParticleGrid(int width_, int height_) : width(width_), height(height_), cells(width * height), buffer(width * height) {}
    particle_cell_t &get_cell(int x, int y) { return cells[std::clamp(y, 0, height - 1) * width + std::clamp(x, 0, width - 1)]; }
    void clear() { cells = std::vector<particle_cell_t>(width * height); }

    void spawn_fire();
    void update_particle_rows(ParticleGrid &next, int lower, int upper);
    void update_particles(ParticleGrid &next);
    void update_snow(std::list<Lumi::Snow> &snow);
    void update_framebuffer();
    const std::vector<uint32_t> &get_framebuffer() { return buffer; }
};
} // namespace Lumi