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

struct Rectangle{
  int x0,y0,x1,y1;
  Rectangle() = default;
  Rectangle(int x0_,int y0_,int x1_, int y1_) : x0(x0_),y0(y0_),x1(x1_),y1(y1_){

  }
};

struct ParticleCell {
    color_rgb value;
    vec2 force;
};

class ParticleGrid {
  private:
    int width, height;
    std::vector<ParticleCell> cells;
    std::vector<uint32_t> buffer;
    std::vector<Rectangle> grid;

  public:
    ParticleGrid(int width_, int height_) : width(width_), height(height_), cells(width * height), buffer(width * height) {
      for(int y=0;y<height; y+= 100){
        for(int x=0;x<width; x+= 100){
          Rectangle rect(x,y,x+100,y+100);
          grid.push_back(rect);
        }
      }
    }
    std::vector<Rectangle> get_grid(){return grid;}
    ParticleCell &get_cell(int x, int y) { return cells[std::clamp(y, 0, height - 1) * width + std::clamp(x, 0, width - 1)]; }
    void clear() { cells = std::vector<ParticleCell>(width * height); }
    void spawn_fire();
    void update_particle_block(ParticleGrid &next, Rectangle rect);
    void update_particle_rows(ParticleGrid &next, int lower, int upper);
    void update_particles(ParticleGrid &next);
    void update_snow(std::list<Lumi::Snow> &snow);
    void update_framebuffer();
    const std::vector<uint32_t> &get_framebuffer() { return buffer; }
};
} // namespace Lumi