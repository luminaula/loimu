#include "renderer.h"
#include "snow.h"
#include <barrier>
#include <chrono>
#include <list>
#include <memory>
#include <thread>
#include <vector>

namespace Lumi {

namespace Renderer {
static uint32_t frame_idx = 0;
static int width, height;
std::vector<ParticleGrid> grids;
std::list<Lumi::Snow> snow_flakes;

std::unique_ptr<std::jthread> worker_thread;
std::vector<std::jthread> subworkers;

std::barrier work_barrier(std::thread::hardware_concurrency() + 1);

using namespace std::literals::chrono_literals;

static void sub_worker(std::stop_token stop_token, int low, int high) {
    while (!stop_token.stop_requested()) {
        work_barrier.arrive_and_wait();
        auto &cur = grids[frame_idx % 2];
        auto &next = grids[(frame_idx + 1) % 2];
        cur.update_particle_rows(next, low, high);
        if (stop_token.stop_requested()) {
            break;
        }
        work_barrier.arrive_and_wait();
    }
}

static void worker(std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
        frame_idx++;
        std::chrono::time_point start_time = std::chrono::steady_clock::now();
        auto &cur = grids[frame_idx % 2];
        auto &next = grids[(frame_idx + 1) % 2];
        next.clear();
        cur.spawn_fire();
        work_barrier.arrive_and_wait();
        work_barrier.arrive_and_wait();
        next.update_framebuffer();
        next.update_snow(snow_flakes);
        std::this_thread::sleep_until(start_time + 16ms);
    }
    for (auto &thread : subworkers) {
        thread.request_stop();
    }
    work_barrier.arrive_and_drop();
}

void init(int width_, int height_) {
    width = width_;
    height = height_;
    grids.push_back(ParticleGrid(width, height));
    grids.push_back(ParticleGrid(width, height));
    int work_step = height / std::thread::hardware_concurrency() + 1;
    int low = 0;
    for (int i = 0; i < std::thread::hardware_concurrency(); i++) {
        subworkers.push_back(std::jthread(sub_worker, low, low + work_step));
        low += work_step;
    }
    worker_thread = std::make_unique<std::jthread>(worker);
}

void deinit() {
    worker_thread->request_stop();
    worker_thread->join();
}

void to_qimage(QImage &image) {
    auto &grid = grids[frame_idx % 2];
    image = QImage((uint8_t *)grid.get_framebuffer().data(), width, height, QImage::Format_ARGB32);
}

} // namespace Renderer
} // namespace Lumi