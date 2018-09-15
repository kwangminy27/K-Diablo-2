#include "stdafx.h"
#include "core.h"

#include "timer.h"
#include "input_manager.h"
#include "camera_manager.h"
#include "path_manager.h"
#include "texture_manager.h"
#include "texture.h"
#include "animation_manager.h"
#include "audio_manager.h"
#include "video_manager.h"
#include "scene_manager.h"
#include "object_manager.h"
#include "collision_manager.h"
#include "ai_manager.h"

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

	if (!VideoManager::GetSingleton()->Initialize())
		return false;

	if (!CollisionManager::GetSingleton()->Initialize())
		return false;

	if (!SceneManager::GetSingleton()->Initialize())
		return false;

	if (!AIManager::GetSingleton()->Initialize())
		return false;

	if (!_CreateTimer())
		return false;

	backbuffer_ = TextureManager::GetSingleton()->FindTexture("backbuffer");

	return true;
}

int Core::Run()
{
	MSG message{};
	while (state() == MESSAGE_LOOP::RUN)
	{
		cout << "this: " << this << ", singleton: " << Core::GetSingleton().get() << endl;

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

void Core::ResizeWindow(TYPE::Rectangle const& _rect, bool _full_screen_flag)
{
	RECT rect{};
	SetRect(&rect, static_cast<int>(_rect.left), static_cast<int>(_rect.top), static_cast<int>(_rect.right), static_cast<int>(_rect.bottom));
	AdjustWindowRect(&rect, WS_POPUP, false);
	MoveWindow(window_, 0, 0, rect.right - rect.left, rect.bottom - rect.top, true);

	if (_full_screen_flag)
	{
		DEVMODE device_mode{};
		device_mode.dmSize = sizeof(device_mode);
		device_mode.dmPelsWidth = rect.right - rect.left;
		device_mode.dmPelsHeight = rect.bottom - rect.top;
		device_mode.dmBitsPerPel = 32;
		device_mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN);
	}
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

int Core::GetFPS() const
{
	return timer_->frame_per_second_;
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

	CameraManager::GetSingleton()->Update(_time);

	scene_manager->LateUpdate(_time);

	AudioManager::GetSingleton()->Update();
}

void Core::_Collision(float _time)
{
	SceneManager::GetSingleton()->Collision(_time);
	CollisionManager::GetSingleton()->Collision(_time);
}

void Core::_Render(float _time)
{
	SceneManager::GetSingleton()->Render(backbuffer_->memory_device_context(), _time);
	InputManager::GetSingleton()->RenderMouseCursor(backbuffer_->memory_device_context(), _time);

	BitBlt(device_context_, 0, 0, static_cast<int>(RESOLUTION::WIDTH), static_cast<int>(RESOLUTION::HEIGHT), backbuffer_->memory_device_context(), 0, 0, SRCCOPY);
}

LRESULT Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	switch (_message)
	{
	case WM_DESTROY:
		Core::GetSingleton()->set_state(MESSAGE_LOOP::EXIT);
		AudioManager::GetSingleton()->Suspend();
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
		WS_POPUP,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, instance_,
		nullptr
	);

	if (!window_)
		return;

	ResizeWindow({ 0.f, 0.f, static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT) }, static_cast<bool>(DISPLAY::MODE));

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
	timer_ = unique_ptr<Timer, function<void(Timer*)>>(new Timer, [](Timer* _p) {
		delete _p;
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