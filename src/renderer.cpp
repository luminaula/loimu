#include "renderer.h"
#include "snow.h"
#include <barrier>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Lumi {

namespace Renderer {
static uint32_t frame_idx = 0;
static int width, height;
std::vector<ParticleGrid> grids;
std::list<Lumi::Snow> snow_flakes;

class stop_token
{
public:
    stop_token() : stop(std::make_unique<std::atomic_bool>(false))
    {
    }
    bool stop_requested() const noexcept
    {
        return *stop;
    }

    void request_stop() noexcept
    {
        *stop = true;
    }
private:
    std::unique_ptr<std::atomic_bool> stop;
};

std::unique_ptr<std::thread> worker_thread;
std::unique_ptr<stop_token> worker_stop_token;
std::vector<std::thread> subworkers;
std::vector<std::unique_ptr<stop_token>> subworker_stop_tokens;

std::barrier work_barrier(std::thread::hardware_concurrency() + 1);
std::queue<Rectangle> block_queue;
std::mutex block_queue_mutex;

using namespace std::literals::chrono_literals;



static void sub_worker(stop_token* stop_token, int low, int high) {
    while (!stop_token->stop_requested()) {
        work_barrier.arrive_and_wait();
        auto &cur = grids[frame_idx % 2];
        auto &next = grids[(frame_idx + 1) % 2];
        while (!block_queue.empty()) {
            Rectangle rect;
            {
                std::lock_guard<std::mutex> lk(block_queue_mutex);
                rect = std::move(block_queue.front());
                if (block_queue.empty())
                    break;
                block_queue.pop();
            }
            cur.update_particle_block(next, rect);
        }
        if (stop_token->stop_requested()) {
            break;
        }
        work_barrier.arrive_and_wait();
    }
}

static void worker(stop_token* stop_token) {
    while (!stop_token->stop_requested()) {
        frame_idx++;
        std::chrono::time_point start_time = std::chrono::steady_clock::now();
        auto &cur = grids[frame_idx % 2];
        auto &next = grids[(frame_idx + 1) % 2];
        next.clear();
        cur.spawn_fire();
        auto blocks = next.get_block_indices();
        {
            std::lock_guard<std::mutex> lk(block_queue_mutex);
            int dim = std::sqrt(blocks.size());
            for (int i = 0; i < dim; i++) {
                int j,k;
                if(i%2){
                    j = 0;
                    k = 1;
                }
                else{
                    j = 1;
                    k = 0;
                }
                for (; j < dim; j += 2) {
                    block_queue.push(blocks[i * dim + j]);
                }
                for (; k < dim; k += 2) {
                    block_queue.push(blocks[i * dim + k]);
                }
            }
        }
        work_barrier.arrive_and_wait();
        work_barrier.arrive_and_wait();
        next.update_framebuffer();
        next.update_snow(snow_flakes);
        std::this_thread::sleep_until(start_time + 16ms);
    }
    for (auto &token : subworker_stop_tokens) {
        token->request_stop();
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
        subworker_stop_tokens.emplace_back(std::make_unique<stop_token>());
        subworkers.push_back(std::thread(sub_worker, subworker_stop_tokens.back().get(), low, low + work_step));
        low += work_step;
    }
    worker_stop_token = std::make_unique<stop_token>();
    worker_thread = std::make_unique<std::thread>(worker, worker_stop_token.get());
}

void deinit() {
    worker_stop_token->request_stop();
    worker_thread->join();
}

void to_qimage(QImage &image) {
    auto &grid = grids[frame_idx % 2];
    image = QImage((uint8_t *)grid.get_framebuffer().data(), width, height, QImage::Format_ARGB32);
}

} // namespace Renderer
} // namespace Lumi