/* Octinc includes library.
 * 
 * Copyright (C) 2019-2019 Octinc Repositories, WeKw team.
 * 
 * The source uses the GNU AFFERO GENERAL PUBLIC LICENSE Version 3,
 * please follow the licence.
 */
#ifndef OCTINC_LOG_HPP
#  define OCTINC_LOG_HPP
#  ifndef STD_BITS_STDCPP_H
#    define STD_BITS_STDCPP_H
#    include <bits/stdc++.h>
#  endif
#  include "chrono_meter.hpp"

namespace Octinc {
enum LogLevel {
    LEVEL_CLOSE = 0,
    LEVEL_VERBOSE = 1,
    LEVEL_DEBUG = 2,
    LEVEL_INFO = 3,
    LEVEL_WARN = 4,
    LEVEL_ERROR = 5,
};

template <typename... Args>
void logcat(const char* format, Args const&... args) {
    if (sizeof...(args) == 0) {
        fprintf(stderr, "%s", FormatTime().c_str());
        fprintf(stderr, "OCTINC_LOG:%s", format);
    } else {
        fprintf(stderr, "%s", FormatTime().c_str());
        fprintf(stderr, "OCTINC_LOG:");
        fprintf(stderr, format, args...);
    }
}

class Logger {
   private:
    int _level = LEVEL_ERROR + 1;

   public:
    void setLevel(int level) {
        if (level == LEVEL_CLOSE) level = LEVEL_ERROR + 1;

        _level = level;
    }

    template <typename... Args>
    void verbose(const char* fmt, Args const&... args) {
        if (_level <= LEVEL_VERBOSE) logcat(fmt, args...);
    }

    template <typename... Args>
    void debug(const char* fmt, Args const&... args) {
        if (_level <= LEVEL_DEBUG) logcat(fmt, args...);
    }

    template <typename... Args>
    void info(const char* fmt, Args const&... args) {
        if (_level <= LEVEL_INFO) logcat(fmt, args...);
    }

    template <typename... Args>
    void warn(const char* fmt, Args const&... args) {
        if (_level <= LEVEL_WARN) logcat(fmt, args...);
    }

    template <typename... Args>
    void error(const char* fmt, Args const&... args) {
        if (_level <= LEVEL_ERROR) logcat(fmt, args...);
    }
};

}  // namespace Octinc

#endif
