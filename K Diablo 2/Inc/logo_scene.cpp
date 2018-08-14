#include "stdafx.h"
#include "logo_scene.h"

#include "input_manager.h"
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
	VideoManager::GetSingleton()->RenderVideo(L"BlizNorth640x480.avi");

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