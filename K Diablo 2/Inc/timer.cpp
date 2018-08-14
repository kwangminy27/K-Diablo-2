#include "stdafx.h"
#include "timer.h"

using namespace std;
using namespace chrono;

void Timer::Update()
{
	static int frame_per_second{};
	static float accumulation_time{};

	auto duration = high_resolution_clock::now() - last_time_point_;
	delta_time_ = duration.count() * 0.000'000'001f;
	last_time_point_ = high_resolution_clock::now();

	++frame_per_second;
	accumulation_time += delta_time_;
	if (accumulation_time >= 1.f)
	{
		frame_per_second_ = frame_per_second;
		frame_per_second = 0;
		accumulation_time -= 1.f;
	}
}

float Timer::delta_time() const
{
	return delta_time_;
}

int Timer::frame_per_second() const
{
	return frame_per_second_;
}

Timer::Timer(Timer const& _other)
{
	*this = _other;
}

Timer::Timer(Timer&& _other) noexcept
{
	*this = move(_other);
}