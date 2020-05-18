#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long total) { 

int minutes, seconds, hours;
minutes = total / 60;
seconds = total % 60;
hours = minutes / 60;
minutes = minutes % 60;
string ans = to_string(hours) +":"+ to_string(minutes) +":"+ to_string(seconds);

    return ans; }