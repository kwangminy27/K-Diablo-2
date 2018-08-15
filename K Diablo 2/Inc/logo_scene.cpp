#include "stdafx.h"
#include "logo_scene.h"

#include "input_manager.h"
#include "scene.h"
#include "object_manager.h"
#include "ui.h"
#include "button.h"
#include "texture.h"
#include "video_manager.h"
#include "audio_manager.h"
#include "rect_collider.h"

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
	auto const& input_manager = InputManager::GetSingleton();
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& audio_manager = AudioManager::GetSingleton();

	auto sound_effect_instance = audio_manager->FindSoundEffect("options")->CreateInstance();
	sound_effect_instance->Play(true);
	audio_manager->AddSoundEffectInstance("options", move(sound_effect_instance));

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	auto trademark = object_manager->CreateObject<UI>("trademark", background_layer);
	trademark->set_texture("trademark");
	trademark->set_size({ 800.f, 600.f });

	auto title_screen = object_manager->CreateObject<UI>("title_screen", background_layer);
	title_screen->set_texture("title_screen");
	title_screen->set_size({ 800.f, 600.f });
	title_screen->set_enablement(false);

	auto D2Logo = object_manager->CreateObject<UI>("D2Logo", background_layer);
	D2Logo->set_position({ 209.f, 0.f });
	D2Logo->AddAnimationClip("D2Logo");
	D2Logo->set_color_key(RGB(11, 11, 11));

	auto mouse = object_manager->CreateObject<UI>("mouse", ui_layer);
	mouse->set_texture("mouse");
	mouse->set_color_key(RGB(0, 0, 0));
	mouse->set_size({ 32.f, 26.f });
	auto mouse_collider = dynamic_pointer_cast<RectCollider>(mouse->AddCollider<RectCollider>("MouseCursor"));
	mouse_collider->set_model_info({ 0.f, 0.f, 32.f, 26.f });
	mouse_collider->set_collision_group_tag("UI");
	input_manager->set_mouse(mouse);

	auto start_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("StartButton", ui_layer));
	start_button->set_position({ 264.f, 178.f });
	start_button->set_texture("wide_button_blank");
	start_button->set_size({ 272.f, 35.f });
	start_button->set_enablement(false);
	start_button->set_callback([](float _time) { 
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto start_button_collider = dynamic_pointer_cast<RectCollider>(start_button->AddCollider<RectCollider>("Button"));
	start_button_collider->set_model_info({ 0.f, 0.f, 272.f, 35.f });
	start_button_collider->set_collision_group_tag("UI");
	start_button_collider->SetCallBack([p = start_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	start_button_collider->SetCallBack([p = start_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	start_button_collider->SetCallBack([p = start_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	auto exit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("ExitButton", ui_layer));
	exit_button->set_position({ 264.f, 532.f });
	exit_button->set_texture("wide_button_blank");
	exit_button->set_size({ 272.f, 35.f });
	exit_button->set_enablement(false);
	exit_button->set_callback([](float _time) { 
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto exit_button_collider = dynamic_pointer_cast<RectCollider>(exit_button->AddCollider<RectCollider>("Button"));
	exit_button_collider->set_model_info({ 0.f, 0.f, 272.f, 35.f });
	exit_button_collider->set_collision_group_tag("UI");
	exit_button_collider->SetCallBack([p = exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	exit_button_collider->SetCallBack([p = exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	exit_button_collider->SetCallBack([p = exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	return true;
}

void LogoScene::_Input(float _time)
{
	if (InputManager::GetSingleton()->KeyPush("MouseLeft"))
	{
		switch (state_)
		{
		case LOGO_SCENE::TRADEMARK:
			state_ = LOGO_SCENE::TITLE_SCREEN;

			auto const& background_layer = scene()->FindLayer("Background");
			auto const& ui_layer = scene()->FindLayer("UI");

			background_layer->FindObject("trademark")->set_activation(false);
			background_layer->FindObject("title_screen")->set_enablement(true);
			ui_layer->FindObject("StartButton")->set_enablement(true);
			ui_layer->FindObject("ExitButton")->set_enablement(true);

			break;
		}
	}
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