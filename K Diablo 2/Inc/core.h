#pragma once

#include "singleton.h"

class Timer;

class Core final : public Singleton<Core>
{
	friend class Singleton<Core>;
public:
	bool Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance, HICON _icon);
	int Run();

	HINSTANCE instance() const;
	HWND window() const;
	HDC device_context() const;
	HBRUSH brush(COLOR _color);
	HPEN pen(COLOR _color);
	MESSAGE_LOOP state() const;

	void set_state(MESSAGE_LOOP _state);

private:
	Core() = default;
	Core(Core const&) = delete;
	Core& operator=(Core const&) = delete;

	virtual void _Release() override;

	void _Logic();
	void _Input(float _time);
	void _Update(float _time);
	void _Collision(float _time);
	void _Render(float _time);

	static LRESULT CALLBACK _WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param);
	void _RegisterClass(std::wstring const& _class_name, HICON _icon);
	void _CreateWindow(std::wstring const& _class_name, std::wstring const& _window_name);

	bool _CreateBrush();
	bool _CreatePen();
	bool _CreateTimer();

	void _ReleaseBrush();
	void _ReleasePen();

	HINSTANCE instance_{};
	HWND window_{};
	HDC device_context_{};
	std::array<HBRUSH, static_cast<size_t>(COLOR::END)> brush_{};
	std::array<HPEN, static_cast<size_t>(COLOR::END)> pen_{};
	MESSAGE_LOOP state_{ MESSAGE_LOOP::RUN };
	std::unique_ptr<Timer, std::function<void(Timer*)>> timer_{};
	float time_scale_{};
};