#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long hours = (seconds / 3600);
    long minutes = (seconds / 60) % 60;
    seconds = seconds % 60;

    return {std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
            std::to_string(seconds)};
}