#include "renderer.h"
#include <chrono>
#include <memory>
#include <thread>

namespace Renderer {
std::vector<uint32_t> buffer[2];
static uint32_t frame_idx = 0;
std::unique_ptr<std::jthread> worker_thread;

using namespace std::literals::chrono_literals;

void worker(std::stop_token stop_token) {
    size_t grid_id = 0;
    std::unique_ptr<ParticleGrid> grids[2];

    grids[0] = std::make_unique<ParticleGrid>(800, 800);
    grids[1] = std::make_unique<ParticleGrid>(800, 800);
    while (!stop_token.stop_requested()) {
        std::chrono::time_point start_time = std::chrono::steady_clock::now();
        auto &cur = grids[grid_id % 2];
        auto &next = grids[(grid_id + 1) % 2];
        next->update(cur);
        grid_id++;
        // std::this_thread::sleep_until(start_time + 16ms);
    }
}

void init() {
    buffer[0].resize(800 * 800);
    buffer[1].resize(800 * 800);
    worker_thread = std::make_unique<std::jthread>(worker);
}

void to_qimage(QImage &image) {
    auto &buf = buffer[frame_idx % 2];
    image = QImage((uint8_t *)buf.data(), 800, 800, QImage::Format_ARGB32);
}

void update(ParticleGrid *grid) {
    auto &buf = buffer[frame_idx++ % 2];
    for (size_t y = 0; y < 800; y++) {
        for (int x = 0; x < 800; x++) {
            buf[y * 800 + x] = grid->get_cell(x, y).rgb.to_pixel();
        }
    }
}
} // namespace Renderer