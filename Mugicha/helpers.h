#pragma once

#include <chrono>

#define SCNOW std::chrono::system_clock::now()
using time_point = std::chrono::system_clock::time_point;

inline constexpr long long time_diff(time_point start, time_point end)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

inline constexpr long long time_diff(time_point start)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(SCNOW - start).count();
}