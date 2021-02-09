#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

namespace kmeans::par::tp {

    class thread_pool {

        public:
        
            thread_pool(const unsigned int T = std::thread::hardware_concurrency()) : 
                running_tasks(),
                stop() {
                    for (unsigned int i = 0; i < T; ++i)
                        workers.emplace_back(std::bind(&thread_pool::run_task, this));
                }
            
            ~thread_pool() {
                std::unique_lock<std::mutex> lock(mtx);
                stop = true;
                task_done.notify_all();
                lock.unlock();
                for (auto& w : workers)
                    w.join();
            }
            
            template<class F> 
            void enqueue(F&& f) {
                std::unique_lock<std::mutex> lock(mtx);
                tasks.emplace_back(std::forward<F>(f));
                task_done.notify_one();
                return;
            }
            
            void wait_all_threads() {
                std::unique_lock<std::mutex> lock(mtx);
                all_done.wait(lock, [this](){ return tasks.empty() && (running_tasks == 0); });
                return;
            }
            
        private:

            std::vector<std::thread> workers;
            std::deque<std::function<void()>> tasks;
            std::mutex mtx;
            std::condition_variable task_done;
            std::condition_variable all_done;
            
            unsigned int running_tasks;
            bool stop;

            void run_task() {
                while (true) {
                    std::unique_lock<std::mutex> lock(mtx);
                    task_done.wait(lock, [this](){ return stop || !tasks.empty(); });
                    if (!tasks.empty()) {
                        ++running_tasks;
                        auto func = tasks.front();
                        tasks.pop_front();
                        lock.unlock();
                        func();
                        lock.lock();
                        --running_tasks;
                        all_done.notify_one();
                    }
                    else if (stop)
                        break;
                }
                return;
            }
    };

} // kmeans::par::tp

#endif