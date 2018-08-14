#include "stdafx.h"
#include "logo_scene.h"

#include "input_manager.h"
#include "scene.h"
#include "object_manager.h"
#include "ui.h"
#include "video_manager.h"
#include "audio_manager.h"

using namespace std;

LogoScene::LogoScene(LogoScene const& _other) : SceneComponent(_other)
{
}

LogoScene::LogoScene(LogoScene&& _other) noexcept : SceneComponent(move(_other))
{
}

void LogoScene::_Release()
{
}

bool LogoScene::_Initialize()
{
	//VideoManager::GetSingleton()->RenderVideo(L"BlizNorth640x480.avi");
	AudioManager::GetSingleton()->FindSoundEffect("options")->Play();

	auto const& input_manager = InputManager::GetSingleton();
	auto const& object_manager = ObjectManager::GetSingleton();

	auto const& ui_layer = scene()->FindLayer("UI");

	auto mouse = object_manager->CreateObject<UI>("mouse", ui_layer);
	mouse->set_texture("mouse");
	mouse->set_size({ 32.f, 26.f });

	input_manager->set_mouse(mouse);

	auto trademark = object_manager->CreateObject<UI>("trademark", ui_layer);
	trademark->set_texture("trademark");
	trademark->set_size({ 800.f, 600.f });

	auto D2Logo = object_manager->CreateObject<UI>("D2Logo", ui_layer);
	D2Logo->set_position({ 209.f, 50.f });
	D2Logo->AddAnimationClip("D2Logo");
	D2Logo->set_color_key(RGB(11, 11, 11));

	return true;
}

void LogoScene::_Input(float _time)
{
}

void LogoScene::_Update(float _time)
{
}

void LogoScene::_LateUpdate(float _time)
{
}

void LogoScene::_Collision(float _time)
{
}

void LogoScene::_Render(HDC _device_context, float _time)
{
}