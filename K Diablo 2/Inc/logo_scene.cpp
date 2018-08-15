#include "stdafx.h"
#include "logo_scene.h"

#include "core.h"
#include "input_manager.h"
#include "scene.h"
#include "object_manager.h"
#include "ui.h"
#include "button.h"
#include "texture.h"
#include "video_manager.h"
#include "audio_manager.h"
#include "point_collider.h"
#include "rect_collider.h"

using namespace std;

LogoScene::LogoScene(LogoScene const& _other) : SceneComponent(_other)
{
	state_ = _other.state_;
}

LogoScene::LogoScene(LogoScene&& _other) noexcept : SceneComponent(move(_other))
{
	state_ = move(_other.state_);
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

	auto mouse = object_manager->CreateObject<UI>("mouse", ui_layer);
	mouse->set_texture("mouse");
	mouse->set_color_key(RGB(0, 0, 0));
	mouse->set_size({ 32.f, 26.f });
	auto mouse_collider = dynamic_pointer_cast<PointCollider>(mouse->AddCollider<PointCollider>("MouseCursor"));
	mouse_collider->set_collision_group_tag("UI");
	input_manager->set_mouse(mouse);

	// TRADEMARK
	auto trademark = object_manager->CreateObject<UI>("trademark", background_layer);
	trademark->set_texture("trademark");
	trademark->set_size({ 800.f, 600.f });

	// TITLE_SCREEN
	auto title_screen = object_manager->CreateObject<UI>("title_screen", background_layer);
	title_screen->set_texture("title_screen");
	title_screen->set_size({ 800.f, 600.f });
	title_screen->set_enablement(false);

	auto D2Logo = object_manager->CreateObject<UI>("D2Logo", background_layer);
	D2Logo->set_position({ 209.f, 0.f });
	D2Logo->AddAnimationClip("D2Logo");
	D2Logo->set_color_key(RGB(11, 11, 11));

	auto start_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("StartButton", ui_layer));
	start_button->set_position({ 264.f, 178.f });
	start_button->set_texture("wide_button_blank");
	start_button->set_size({ 272.f, 35.f });
	start_button->set_enablement(false);
	start_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_ChangeToCharacterSelect();
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

	auto edit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("EditButton", ui_layer));
	edit_button->set_position({ 264.f, 220.f });
	edit_button->set_texture("wide_button_blank");
	edit_button->set_size({ 272.f, 35.f });
	edit_button->set_enablement(false);
	edit_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto edit_button_collider = dynamic_pointer_cast<RectCollider>(edit_button->AddCollider<RectCollider>("Button"));
	edit_button_collider->set_model_info({ 0.f, 0.f, 272.f, 35.f });
	edit_button_collider->set_collision_group_tag("UI");
	edit_button_collider->SetCallBack([p = edit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	edit_button_collider->SetCallBack([p = edit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	edit_button_collider->SetCallBack([p = edit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	auto credits_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("CreditsButton", ui_layer));
	credits_button->set_position({ 264.f, 499.f });
	credits_button->set_texture("short_button_blank");
	credits_button->set_size({ 135.f, 25.f });
	credits_button->set_enablement(false);
	credits_button->set_callback([](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto credits_button_collider = dynamic_pointer_cast<RectCollider>(credits_button->AddCollider<RectCollider>("Button"));
	credits_button_collider->set_model_info({ 0.f, 0.f, 135.f, 25.f });
	credits_button_collider->set_collision_group_tag("UI");
	credits_button_collider->SetCallBack([p = credits_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	credits_button_collider->SetCallBack([p = credits_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	credits_button_collider->SetCallBack([p = credits_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	auto video_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("VideoButton", ui_layer));
	video_button->set_position({ 402.f, 499.f });
	video_button->set_texture("short_button_blank");
	video_button->set_size({ 135.f, 25.f });
	video_button->set_enablement(false);
	video_button->set_callback([](float _time) {
		AudioManager::GetSingleton()->Suspend();
		VideoManager::GetSingleton()->RenderVideo(L"d2intro640x292.avi");
		AudioManager::GetSingleton()->Resume();
	});
	auto video_button_collider = dynamic_pointer_cast<RectCollider>(video_button->AddCollider<RectCollider>("Button"));
	video_button_collider->set_model_info({ 0.f, 0.f, 135.f, 25.f });
	video_button_collider->set_collision_group_tag("UI");
	video_button_collider->SetCallBack([p = video_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	video_button_collider->SetCallBack([p = video_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	video_button_collider->SetCallBack([p = video_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	auto d2_exit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("D2ExitButton", ui_layer));
	d2_exit_button->set_position({ 264.f, 532.f });
	d2_exit_button->set_texture("wide_button_blank");
	d2_exit_button->set_size({ 272.f, 35.f });
	d2_exit_button->set_enablement(false);
	d2_exit_button->set_callback([](float _time) {
		Core::GetSingleton()->set_state(MESSAGE_LOOP::EXIT);
	});
	auto d2_exit_button_collider = dynamic_pointer_cast<RectCollider>(d2_exit_button->AddCollider<RectCollider>("Button"));
	d2_exit_button_collider->set_model_info({ 0.f, 0.f, 272.f, 35.f });
	d2_exit_button_collider->set_collision_group_tag("UI");
	d2_exit_button_collider->SetCallBack([p = d2_exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	d2_exit_button_collider->SetCallBack([p = d2_exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	d2_exit_button_collider->SetCallBack([p = d2_exit_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	// CHARACTER_SELECT
	auto character_select_background = object_manager->CreateObject<UI>("character_select", background_layer);
	character_select_background->set_texture("character_select");
	character_select_background->set_size({ 800.f, 600.f });
	character_select_background->set_enablement(false);

	auto exit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("ExitButton", ui_layer));
	exit_button->set_position({ 32.f, 540.f });
	exit_button->set_texture("exit_button");
	exit_button->set_size({ 126.f, 35.f });
	exit_button->set_enablement(false);
	exit_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_ChangeToTitleScreen();
	});
	auto exit_button_collider = dynamic_pointer_cast<RectCollider>(exit_button->AddCollider<RectCollider>("Button"));
	exit_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
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

	auto delete_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("DeleteButton", ui_layer));
	delete_button->set_position({ 190.f, 540.f });
	delete_button->set_texture("delete_button");
	delete_button->set_size({ 126.f, 35.f });
	delete_button->set_enablement(false);
	delete_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto delete_button_collider = dynamic_pointer_cast<RectCollider>(delete_button->AddCollider<RectCollider>("Button"));
	delete_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	delete_button_collider->set_collision_group_tag("UI");
	delete_button_collider->SetCallBack([p = delete_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	delete_button_collider->SetCallBack([p = delete_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	delete_button_collider->SetCallBack([p = delete_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	auto ok_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("OkButton", ui_layer));
	ok_button->set_position({ 348.f, 540.f });
	ok_button->set_texture("ok_button");
	ok_button->set_size({ 126.f, 35.f });
	ok_button->set_enablement(false);
	ok_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto ok_button_collider = dynamic_pointer_cast<RectCollider>(ok_button->AddCollider<RectCollider>("Button"));
	ok_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	ok_button_collider->set_collision_group_tag("UI");
	ok_button_collider->SetCallBack([p = ok_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionEnter(_src, _dest, _time);
	}, COLLISION_CALLBACK::ENTER);
	ok_button_collider->SetCallBack([p = ok_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollision(_src, _dest, _time);
	}, COLLISION_CALLBACK::STAY);
	ok_button_collider->SetCallBack([p = ok_button.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		p->OnCollisionLeave(_src, _dest, _time);
	}, COLLISION_CALLBACK::LEAVE);

	// CHARACTER_CREATE
	auto character_create = object_manager->CreateObject<UI>("character_create", background_layer);
	character_create->set_texture("character_create");
	character_create->set_size({ 800.f, 600.f });
	character_create->set_enablement(false);

	auto bornfire = object_manager->CreateObject<UI>("bonfire", background_layer);
	bornfire->set_position({ 350.f, 280.f });
	bornfire->AddAnimationClip("bonfire");
	bornfire->set_color_key(RGB(11, 11, 11));
	bornfire->set_enablement(false);

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
			ui_layer->FindObject("EditButton")->set_enablement(true);
			ui_layer->FindObject("CreditsButton")->set_enablement(true);
			ui_layer->FindObject("VideoButton")->set_enablement(true);
			ui_layer->FindObject("D2ExitButton")->set_enablement(true);

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

void LogoScene::_ChangeToTitleScreen()
{
	state_ = LOGO_SCENE::TITLE_SCREEN;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("character_select")->set_enablement(false);
	ui_layer->FindObject("ExitButton")->set_enablement(false);
	ui_layer->FindObject("DeleteButton")->set_enablement(false);
	ui_layer->FindObject("OkButton")->set_enablement(false);

	background_layer->FindObject("title_screen")->set_enablement(true);
	background_layer->FindObject("D2Logo")->set_enablement(true);
	ui_layer->FindObject("StartButton")->set_enablement(true);
	ui_layer->FindObject("EditButton")->set_enablement(true);
	ui_layer->FindObject("CreditsButton")->set_enablement(true);
	ui_layer->FindObject("VideoButton")->set_enablement(true);
	ui_layer->FindObject("D2ExitButton")->set_enablement(true);
}

void LogoScene::_ChangeToCharacterSelect()
{
	state_ = LOGO_SCENE::CHARACTER_SELECT;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("title_screen")->set_enablement(false);
	background_layer->FindObject("D2Logo")->set_enablement(false);
	ui_layer->FindObject("StartButton")->set_enablement(false);
	ui_layer->FindObject("EditButton")->set_enablement(false);
	ui_layer->FindObject("CreditsButton")->set_enablement(false);
	ui_layer->FindObject("VideoButton")->set_enablement(false);
	ui_layer->FindObject("D2ExitButton")->set_enablement(false);

	background_layer->FindObject("character_select")->set_enablement(true);
	ui_layer->FindObject("ExitButton")->set_enablement(true);
	ui_layer->FindObject("DeleteButton")->set_enablement(true);
	ui_layer->FindObject("OkButton")->set_enablement(true);
}

void LogoScene::_ChangeToCharacterCreate()
{
	state_ = LOGO_SCENE::CHARACTER_CREATE;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("character_select")->set_enablement(false);
	background_layer->FindObject("character_create")->set_enablement(true);
	background_layer->FindObject("bornfire")->set_enablement(true);
}