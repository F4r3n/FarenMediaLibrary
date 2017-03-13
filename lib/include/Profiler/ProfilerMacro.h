#pragma once
#include <iostream>
#define PROFILER_START(name) Profile profiler_##name = Profile(#name);
#define PROFILER_DISPLAY(name) std::cout << #name << " " << (float)Profiler::get().getTime(#name)/1000.0f << " ms" << std::endl;
#define PROFILER_STOP(name) p##name.stop();
