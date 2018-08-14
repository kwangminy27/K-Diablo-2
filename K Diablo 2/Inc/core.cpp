#include "stdafx.h"
#include "core.h"

#include "timer.h"
#include "input_manager.h"
#include "camera_manager.h"
#include "path_manager.h"
#include "texture_manager.h"
#include "animation_manager.h"
#include "audio_manager.h"
#include "scene_manager.h"
#include "object_manager.h"
#include "collision_manager.h"

using namespace std;

bool Core::Initialize(wstring const& _class_name, wstring const& _window_name, HINSTANCE _instance, HICON _icon)
{
	instance_ = _instance;

	_RegisterClass(_class_name, _icon);
	_CreateWindow(_class_name, _window_name);

	device_context_ = GetDC(window_);

	if (!_CreateBrush())
		return false;

	if (!_CreatePen())
		return false;

	if (!InputManager::GetSingleton()->Initialize())
		return false;

	if (!CameraManager::GetSingleton()->Initialize())
		return false;

	if (!PathManager::GetSingleton()->Initialize())
		return false;

	if (!TextureManager::GetSingleton()->Initialize())
		return false;

	if (!AnimationManager::GetSingleton()->Initialize())
		return false;

	if (!AudioManager::GetSingleton()->Initialize())
		return false;

	if (!CollisionManager::GetSingleton()->Initialize())
		return false;

	if (!SceneManager::GetSingleton()->Initialize())
		return false;

	if (!_CreateTimer())
		return false;

	return true;
}

int Core::Run()
{
	MSG message{};
	while (state() == MESSAGE_LOOP::RUN)
	{
		if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			timer_->Update();
			InputManager::GetSingleton()->Update();

			SceneManager::GetSingleton()->TrySceneChange();

			_Logic();

			ObjectManager::GetSingleton()->EraseExpiredSceneObject();
		}
	}

	return static_cast<int>(message.wParam);
}

HINSTANCE Core::instance() const
{
	return instance_;
}

HWND Core::window() const
{
	return window_;
}

HDC Core::device_context() const
{
	return device_context_;
}

HBRUSH Core::brush(COLOR _color)
{
	return brush_.at(static_cast<size_t>(_color));
}

HPEN Core::pen(COLOR _color)
{
	return pen_.at(static_cast<size_t>(_color));
}

MESSAGE_LOOP Core::state() const
{
	return state_;
}

void Core::set_state(MESSAGE_LOOP _state)
{
	state_ = _state;
}

void Core::_Release()
{
	ReleaseDC(window_, device_context_);

	_ReleaseBrush();
	_ReleasePen();
}

void Core::_Logic()
{
	float delta_time = timer_->delta_time() * time_scale_;

	_Input(delta_time);
	_Update(delta_time);
	_Collision(delta_time);
	_Render(delta_time);
}

void Core::_Input(float _time)
{
	if (InputManager::GetSingleton()->KeyPush("Pause"))
		time_scale_ = time_scale_ == 1.f ? 0.f : 1.f;

	SceneManager::GetSingleton()->Input(_time);
}

void Core::_Update(float _time)
{
	auto const& scene_manager = SceneManager::GetSingleton();

	scene_manager->Update(_time);
	scene_manager->LateUpdate(_time);

	CameraManager::GetSingleton()->Update(_time);
	AudioManager::GetSingleton()->Update();
}

void Core::_Collision(float _time)
{
	SceneManager::GetSingleton()->Collision(_time);
}

void Core::_Render(float _time)
{
	SceneManager::GetSingleton()->Render(device_context_, _time);

	wstring fps = to_wstring(timer_->frame_per_second());
	fps += L" FPS";
	TextOut(device_context_, 0, 0, fps.c_str(), static_cast<int>(fps.size()));
}

LRESULT Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	switch (_message)
	{
	case WM_KEYDOWN:
		switch (_w_param)
		{
		case VK_ESCAPE:
			DestroyWindow(_window);
			break;
		}
		return 0;
	case WM_DESTROY:
		Core::GetSingleton()->set_state(MESSAGE_LOOP::EXIT);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_window, _message, _w_param, _l_param);
}

void Core::_RegisterClass(wstring const& _class_name, HICON _icon)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = Core::_WindowProc;
	wc.hInstance = instance_;
	wc.hIcon = _icon;
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(LTGRAY_BRUSH));
	wc.lpszClassName = _class_name.c_str();
	RegisterClass(&wc);
}

void Core::_CreateWindow(wstring const& _class_name, wstring const& _window_name)
{
	window_ = CreateWindow(
		_class_name.c_str(), _window_name.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, instance_,
		nullptr
	);

	if (!window_)
		return;

	RECT rc{ 0, 0, static_cast<long>(RESOLUTION::WIDTH), static_cast<long>(RESOLUTION::HEIGHT) };
	AdjustWindowRect(&rc, WS_CAPTION | WS_SYSMENU, false);
	SetWindowPos(window_, HWND_TOP, 100, 50, rc.right - rc.left, rc.bottom - rc.top, NULL);

	ShowWindow(window_, SW_SHOW);
}

bool Core::_CreateBrush()
{
	if (!(brush_.at(static_cast<size_t>(COLOR::RED)) = CreateSolidBrush(RGB(255, 0, 0))))
		return false;

	if (!(brush_.at(static_cast<size_t>(COLOR::GREEN)) = CreateSolidBrush(RGB(0, 255, 0))))
		return false;

	if (!(brush_.at(static_cast<size_t>(COLOR::BLUE)) = CreateSolidBrush(RGB(0, 0, 255))))
		return false;

	return true;
}

bool Core::_CreatePen()
{
	if (!(pen_.at(static_cast<size_t>(COLOR::RED)) = CreatePen(PS_SOLID, 2, RGB(255, 0, 0))))
		return false;

	if (!(pen_.at(static_cast<size_t>(COLOR::GREEN)) = CreatePen(PS_SOLID, 2, RGB(0, 255, 0))))
		return false;

	if (!(pen_.at(static_cast<size_t>(COLOR::BLUE)) = CreatePen(PS_SOLID, 2, RGB(0, 0, 255))))
		return false;

	return true;
}

bool Core::_CreateTimer()
{
	timer_ = unique_ptr<Timer, function<void(Timer*)>>(new Timer, [](Timer* p) {
		delete p;
	});
	time_scale_ = 1.f;

	return true;
}

void Core::_ReleaseBrush()
{
	for(auto i = 0; i < brush_.size(); ++i)
		DeleteObject(brush_.at(i));
}

void Core::_ReleasePen()
{
	for (auto i = 0; i < pen_.size(); ++i)
		DeleteObject(pen_.at(i));
}