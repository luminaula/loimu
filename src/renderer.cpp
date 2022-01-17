#include "renderer.h"
#include <chrono>
#include <memory>
#include <thread>

namespace Renderer {
static uint32_t frame_idx = 0;
static int width,height;
std::unique_ptr<std::jthread> worker_thread;
std::vector<ParticleGrid> grids;

using namespace std::literals::chrono_literals;

void worker(std::stop_token stop_token) {

    while (!stop_token.stop_requested()) {
        std::chrono::time_point start_time = std::chrono::steady_clock::now();
        auto &cur = grids[frame_idx % 2];
        auto &next = grids[(frame_idx + 1) % 2];
        next.update_particles(cur);
        frame_idx++;
        std::this_thread::sleep_until(start_time + 16ms);
    }
}

void init(int width_, int height_) {
    width = width_;
    height = height_;
    grids.push_back(ParticleGrid(width,height));
    grids.push_back(ParticleGrid(width,height));
    worker_thread = std::make_unique<std::jthread>(worker);
}

void deinit(){
    worker_thread->request_stop();
    worker_thread->join();
}

void to_qimage(QImage &image) {
    auto &grid = grids[frame_idx % 2];
    image = QImage((uint8_t *)grid.get_framebuffer().data(), width, height, QImage::Format_ARGB32);

}

// void update(ParticleGrid<color_rgb> *grid) {
//     auto &buf = buffer[(frame_idx+1) % 2];
//     for (size_t y = 0; y < height; y++) {
//         for (int x = 0; x < width; x++) {
//             buf[y * width + x] = grid->get_cell(x, y).value.to_pixel();
//         }
//     }
//     frame_idx++;
// }
} // namespace Renderer