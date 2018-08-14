#pragma once

class Timer
{
	friend class Core;
public:
	void Update();

	float delta_time() const;
	int frame_per_second() const;

private:
	Timer() = default;
	Timer(Timer const& _other);
	Timer(Timer&& _other) noexcept;
	Timer& operator=(Timer const&) = default;
	Timer& operator=(Timer&&) noexcept = default;

	std::chrono::time_point<std::chrono::high_resolution_clock> last_time_point_{ std::chrono::high_resolution_clock::now() };
	float delta_time_{};
	int frame_per_second_{};
};