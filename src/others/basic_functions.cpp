#include "basic_functions.h"

std::string format_time_from_seconds(unsigned int seconds) {
    if (seconds < 60)
        return std::format("{:02d} secs", seconds);
    else if (seconds < 3600)
        return std::format("{:02d}:{:02d}", seconds / 60, seconds % 60);
    else if (seconds < 3600 * 24)
        return std::format("{:02d}:{:02d}:{:02d}", seconds / 3600, (seconds / 60) % 60, seconds % 60);

    return std::format("{:04d}:{:02d}:{:02d}", seconds / 3600, (seconds / 60) % 60, seconds % 60);
}
