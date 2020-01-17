/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_TIMER_HPP
#  define OCTINC_TIMER_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  if __cplusplus < 201103L && !defined(nullptr)
#    define nullptr NULL
#  endif
#  include "../Junior/heap.hpp"

namespace Octinc {
uint64_t now() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::system_clock::now() -
               std::chrono::time_point<std::chrono::system_clock>())
        .count();
}
typedef std::function<void(uint32_t, void*)> timer_callback;
struct timer_msg {
#define MAX_MACRO_COMPILE_SUPPORT
    uint32_t timer_id;
    uint32_t elapsed;
    uint64_t expired;
    bool repeat;
    void* userdata;
    timer_callback action;
    timer_msg()
        : timer_id(0),
          elapsed(0),
          expired(
              std::numeric_limits<uint64_t>::max MAX_MACRO_COMPILE_SUPPORT()),
          repeat(false),
          userdata(nullptr) {}

    bool operator<(const timer_msg& other) const {
        return this->elapsed < other.elapsed;
    }
};
class timer {
   public:
    class TimerQueue {
       public:
        TimerQueue() {
            _stop.store(false);
            _flag.store(false);
        }
        void run() {
            while (true) {
                while (true) {
                    if (_stop) {
                        return;
                    }
                    timer_msg msg;
                    uint64_t t = now();
                    {
                        std::lock_guard<std::mutex> guard(_mutex);
                        if (!_timers.empty()) {
                            msg = _timers.top();
                        }
                    }
                    if (msg.expired >= t) {
                        break;
                    }
                    if (msg.action) {
                        msg.action(msg.timer_id, msg.userdata);
                    }
                    {
                        std::lock_guard<std::mutex> guard(_mutex);
                        if (!_timers.empty()) {
                            _timers.pop();
                            if (msg.repeat) {
                                msg.expired = msg.elapsed + now();
                                _timers.push(msg);
                            }
                        }
                    }
                }
                int32_t sleep_time = std::numeric_limits<int32_t>::max
                MAX_MACRO_COMPILE_SUPPORT();
                {
                    std::lock_guard<std::mutex> guard(_mutex);
                    if (!_timers.empty()) {
                        sleep_time =
                            static_cast<int32_t>(_timers.top().expired - now());
                        if (sleep_time < 0) {
                            continue;
                        }
                    }
                }
                if (_stop) {
                    return;
                }
                std::unique_lock<std::mutex> lck(_mutex);
                if (_flag.load() == false) {
                    if (_cv.wait_for(
                            lck, std::chrono::milliseconds(sleep_time),
                            [this]() -> bool { return this->_flag.load(); })) {
                        fprintf(stderr, "TIMER:cv is time out:%d\n",
                                sleep_time);
                    } else {
                        fprintf(stderr, "TIMER:cv flag is true\n");
                    }
                }

                _flag.store(false);
            }
        }

        void push(timer_msg& msg) {
            std::unique_lock<std::mutex> lck(_mutex);
            _timers.push(msg);
            _flag.store(true);
            _cv.notify_all();
        }
        void clear() {
            std::lock_guard<std::mutex> guard(_mutex);
            while (!_timers.empty()) {
                _timers.pop();
            }
        }
        void remove(uint32_t time_id) {
            std::lock_guard<std::mutex> guard(_mutex);
            _timers.remove_if(
                [&](const timer_msg& msg) { return msg.timer_id == time_id; });
        }

       private:
        std::atomic_bool _stop;
        heap<timer_msg> _timers;
        std::mutex _mutex;
        std::condition_variable _cv;
        std::atomic_bool _flag;
    };

    timer() {
        _queue = std::make_shared<TimerQueue>();
        std::thread([](std::shared_ptr<TimerQueue> queue) { queue->run(); },
                    _queue)
            .detach();
    }

    ~timer() { _queue->clear(); }

    uint32_t start(int elapsed_ms, bool repeat, timer_callback action,
                   void* userdata)
    /*
    Start a function.
    */
    {
        timer_msg msg;
        msg.elapsed = elapsed_ms;
        msg.expired = elapsed_ms + now();
        msg.repeat = repeat;
        msg.userdata = userdata;
        msg.timer_id = s_timer_id++;
        msg.action = action;
        _queue->push(msg);
        return msg.timer_id;
    }

    void remove(uint32_t time_id)
    /*
    Remove a function.
    */
    {
        _queue->remove(time_id);
    }

   private:
    std::shared_ptr<TimerQueue> _queue;
    static uint32_t s_timer_id;
};

uint32_t timer::s_timer_id = 1;

#undef MAX_MACRO_COMPILE_SUPPORT

}  // namespace Octinc

#endif
