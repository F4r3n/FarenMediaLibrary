#pragma once
#include <iostream>
#define PROFILER_MEASURE(name) Profile p##name = Profile(#name);
#define PROFILER_DISPLAY(name) std::cout << #name << " " << Profiler::get().getTime(#name)/1000.0f << " ms" << std::endl;
