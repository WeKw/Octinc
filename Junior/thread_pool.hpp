/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_THREAD_POOL__
#define OCTINC_THREAD_POOL__

#if __cplusplus < 201103L
#error "should use C++11 implementation"
#endif

#include <cassert>
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace Octinc {
class nocopyable {
   private:
    nocopyable(const nocopyable& x) = delete;
    nocopyable& operator=(const nocopyable& x) = delete;

   public:
    nocopyable() = default;
    ~nocopyable() = default;
};

class thread_pool : public nocopyable {
   public:
    typedef std::function<void()> Task;

    explicit thread_pool(const std::string& name = std::string());
    ~thread_pool();

    void start(int numThreads);
    /*
    Set the number of threads and create numThreads threads.
    */
    void stop();
    /*
    The thread pool ends.
    */
    void run(const Task& f);
    /*
    Let task f run in the thread pool.
    */

    void set_size(int maxSize)
    /*
    Set the task queue to store the maximum number of tasks.
    */
    {
        _maxQueueSize = maxSize;
    }

    bool full();
    /*
    Determine if the task queue is full.
    */
    Task take();
    /*
    Take a task from the task queue.
    */

   private:
    void __run();

    std::mutex _mutex;
    std::condition_variable _notEmpty;
    std::condition_variable _notFull;
    std::string _name;
    std::vector<std::thread> _threads;
    std::deque<Task> _queue;
    size_t _maxQueueSize;
    bool _running;
};

thread_pool::thread_pool(const std::string& name)
    : _name(name), _maxQueueSize(0), _running(false) {}

thread_pool::~thread_pool() {
    if (_running) stop();
}

void thread_pool::start(int numThreads) {
    assert(_threads.empty());
    _running = true;
    _threads.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i)
        _threads.push_back(std::thread(&thread_pool::__run, this));
}

void thread_pool::stop() {
    std::unique_lock<std::mutex> lock(_mutex);
    _running = false;
    _notEmpty.notify_all();

    for (size_t i = 0; i < _threads.size(); ++i) _threads[i].join();
}

void thread_pool::run(const Task& f) {
    if (_threads.empty())
        f();
    else {
        std::unique_lock<std::mutex> lock(_mutex);

        while (full()) _notFull.wait(lock);

        assert(!full());
        _queue.push_back(f);
        _notEmpty.notify_one();
    }
}

thread_pool::Task thread_pool::take() {
    std::unique_lock<std::mutex> lock(_mutex);

    while (_queue.empty() && _running) _notEmpty.wait(lock);

    Task task;

    if (!_queue.empty()) {
        task = _queue.front();
        _queue.pop_front();

        if (_maxQueueSize > 0) _notFull.notify_one();
    }

    return task;
}

bool thread_pool::full() {
    return _maxQueueSize > 0 && _queue.size() >= _maxQueueSize;
}

void thread_pool::__run() {
    try {
        while (_running) {
            Task task = take();
            if (task) task();
        }
    }

    catch (const std::exception& ex) {
        fprintf(stderr, "THREAD_POOL:exception caught in thread_pool %s\n",
                _name.c_str());
        fprintf(stderr, "THREAD_POOL:reason: %s\n", ex.what());
        abort();
    }

    catch (...) {
        fprintf(stderr, "THREAD_POOL:exception caught in thread_pool %s\n",
                _name.c_str());
    }
}
}  // namespace Octinc

#endif
