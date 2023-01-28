#include <iostream>
#include <thread>
#include <future>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>

std::random_device rd;  // random number generator
std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
std::uniform_int_distribution<> dist(1, 100);

int generate_random() {
    int random_number = dist(gen);
    return random_number;
}

const int NUM_THREADS = 4; // number of threads in the pool
std::queue<std::packaged_task<int()>> tasks;
std::mutex mtx;
std::condition_variable cv;
std::mutex cout_mtx; // want to avoid racing conditions with std::out

bool stop = false;

std::future<int> addTask(std::function<int()> task) {
    std::packaged_task<int()> pt(task);

    // Get the future associated with the packaged task
    std::future<int> future = pt.get_future();

    std::unique_lock<std::mutex> lock(mtx);
    tasks.push(std::move(pt));
    cv.notify_one();

    return future;
}

void worker()
{
    std::unique_lock output_lock(cout_mtx);
    std::cout << "[THREAD " << std::this_thread::get_id() << "] Worker started on thread " << std::endl;
    output_lock.unlock();

    while (true) {
        std::packaged_task<int()> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return stop || !tasks.empty(); });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front()); // no copy support so move
            tasks.pop();
        }
        task();
    }
}

int main()
{
    std::cout << "[MAIN] Main thead id " << std::this_thread::get_id() << std::endl;
    std::vector<std::thread> pool;
    for (int i = 0; i < NUM_THREADS; ++i) {
        pool.emplace_back(worker);
    }

    auto task_wait_time = std::chrono::seconds(1);
    int task_count = 120; // 120 tasks
    float expected_runtime = (float) task_count * (float) task_wait_time.count() / (float) pool.size(); // would be good to round

    std::unique_lock output_lock(cout_mtx);
    std::cout << "[MAIN] Expected runtime " << expected_runtime << " seconds" << std::endl;
    output_lock.unlock();

    // add tasks to the queue
    std::vector<std::future<int>> results;
    for (int i = 0; i < task_count; ++i) {
        auto task = [i, task_wait_time]() {

            // Sync writing to std output
            std::unique_lock lock(cout_mtx);
            std::cout << "[THREAD " << std::this_thread::get_id() << "] Task " << i << " started." << std::endl;
            lock.unlock();

            std::this_thread::sleep_for(task_wait_time);

            int result = generate_random();

            lock.lock();
            std::cout << "[THREAD " << std::this_thread::get_id() << "] Task " << i << " got result " << result << std::endl;
            lock.unlock();

            return result;
        };
        results.push_back(addTask(task));
    }

    {
        std::unique_lock<std::mutex> lock(mtx);
        stop = true;
    }
    cv.notify_all();
    for (auto& thread : pool) {
        thread.join();
    }

    // Because promises are pushed in-order
    // the results are also in order (even though they are likely not computed this way)
    for (auto& result : results) {
        std::cout << "[MAIN] Got result " << result.get() << std::endl;
    }

    return 0;
}
