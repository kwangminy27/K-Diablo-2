#include "stdafx.h"
#include "logo_scene.h"

#include "core.h"
#include "input_manager.h"
#include "scene_manager.h"
#include "scene.h"
#include "main_scene.h"
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

	// TITLE
	auto title = object_manager->CreateObject<UI>("title", background_layer);
	title->set_texture("title");
	title->set_size({ 800.f, 600.f });
	title->set_enablement(false);

	auto d2_logo = object_manager->CreateObject<UI>("d2_logo", background_layer);
	d2_logo->set_position({ 209.f, 0.f });
	d2_logo->AddAnimationClip("d2_logo");
	d2_logo->set_color_key(RGB(11, 11, 11));

	auto single_player_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("single_player_button", ui_layer));
	single_player_button->set_position({ 250.f, 178.f });
	single_player_button->set_texture("single_player_button");
	single_player_button->set_size({ 270.f, 36.f });
	single_player_button->set_offset_flag(true);
	single_player_button->set_enablement(false);
	single_player_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_ChangeToCharacterSelect();
	});
	auto single_player_button_collider = dynamic_pointer_cast<RectCollider>(single_player_button->GetCollider("Button"));
	single_player_button_collider->set_model_info({ 0.f, 0.f, 270.f, 36.f });
	single_player_button_collider->set_collision_group_tag("UI");

	auto credit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("credit_button", ui_layer));
	credit_button->set_position({ 250.f, 499.f });
	credit_button->set_texture("credit_button");
	credit_button->set_size({ 133.f, 25.f });
	credit_button->set_offset_flag(true);
	credit_button->set_enablement(false);
	credit_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_ChangeToCredit();
	});
	auto credit_button_collider = dynamic_pointer_cast<RectCollider>(credit_button->GetCollider("Button"));
	credit_button_collider->set_model_info({ 0.f, 0.f, 133.f, 25.f });
	credit_button_collider->set_collision_group_tag("UI");

	auto video_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("video_button", ui_layer));
	video_button->set_position({ 387.f, 499.f });
	video_button->set_texture("video_button");
	video_button->set_size({ 133.f, 25.f });
	video_button->set_offset_flag(true);
	video_button->set_enablement(false);
	video_button->set_callback([](float _time) {
		AudioManager::GetSingleton()->Suspend();
		VideoManager::GetSingleton()->RenderVideo(L"d2intro640x292.avi");
		AudioManager::GetSingleton()->Resume();
	});
	auto video_button_collider = dynamic_pointer_cast<RectCollider>(video_button->GetCollider("Button"));
	video_button_collider->set_model_info({ 0.f, 0.f, 133.f, 25.f });
	video_button_collider->set_collision_group_tag("UI");

	auto d2_exit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("d2_exit_button", ui_layer));
	d2_exit_button->set_position({ 250.f, 532.f });
	d2_exit_button->set_texture("d2_exit_button");
	d2_exit_button->set_size({ 270.f, 35.f });
	d2_exit_button->set_offset_flag(true);
	d2_exit_button->set_enablement(false);
	d2_exit_button->set_callback([](float _time) {
		Core::GetSingleton()->set_state(MESSAGE_LOOP::EXIT);
		AudioManager::GetSingleton()->Suspend();
	});
	auto d2_exit_button_collider = dynamic_pointer_cast<RectCollider>(d2_exit_button->GetCollider("Button"));
	d2_exit_button_collider->set_model_info({ 0.f, 0.f, 270.f, 35.f });
	d2_exit_button_collider->set_collision_group_tag("UI");

	// CHARACTER_SELECT
	auto character_select_background = object_manager->CreateObject<UI>("character_select", background_layer);
	character_select_background->set_texture("character_select");
	character_select_background->set_size({ 800.f, 600.f });
	character_select_background->set_enablement(false);

	auto character_select_box = object_manager->CreateObject<UI>("character_select_box", background_layer);
	character_select_box->set_position({ 515.f, 60.f });
	character_select_box->set_texture("character_select_box");
	character_select_box->set_color_key(RGB(11, 11, 11));
	character_select_box->set_size({ 272.f, 93.f });
	character_select_box->set_enablement(false);

	auto create_new_character = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("create_new_character", ui_layer));
	create_new_character->set_position({ 518.f, 65.f });
	create_new_character->set_texture("create_new_character");
	create_new_character->set_size({ 266.f, 86.f });
	create_new_character->set_enablement(false);
	create_new_character->set_callback([this](float _time) {
		_ChangeToCharacterCreate();
	});
	auto create_new_character_collider = dynamic_pointer_cast<RectCollider>(create_new_character->GetCollider("Button"));
	create_new_character_collider->set_model_info({ 0.f, 0.f, 266.f, 86.f });
	create_new_character_collider->set_collision_group_tag("UI");

	auto exit_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("exit_button", ui_layer));
	exit_button->set_position({ 32.f, 540.f });
	exit_button->set_texture("exit_button");
	exit_button->set_size({ 126.f, 35.f });
	exit_button->set_offset_flag(true);
	exit_button->set_enablement(false);
	exit_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();

		switch (state_)
		{
		case LOGO_SCENE::CHARACTER_SELECT:
		case LOGO_SCENE::CREDIT:
			_ChangeToTitle();
			break;
		case LOGO_SCENE::CHARACTER_CREATE:
			_ChangeToCharacterSelect();
			break;
		}
	});
	auto exit_button_collider = dynamic_pointer_cast<RectCollider>(exit_button->GetCollider("Button"));
	exit_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	exit_button_collider->set_collision_group_tag("UI");

	auto delete_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("delete_button", ui_layer));
	delete_button->set_position({ 190.f, 540.f });
	delete_button->set_texture("delete_button");
	delete_button->set_size({ 126.f, 35.f });
	delete_button->set_offset_flag(true);
	delete_button->set_enablement(false);
	delete_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto delete_button_collider = dynamic_pointer_cast<RectCollider>(delete_button->GetCollider("Button"));
	delete_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	delete_button_collider->set_collision_group_tag("UI");

	auto ok_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("ok_button", ui_layer));
	ok_button->set_position({ 348.f, 540.f });
	ok_button->set_texture("ok_button");
	ok_button->set_size({ 126.f, 35.f });
	ok_button->set_offset_flag(true);
	ok_button->set_enablement(false);
	ok_button->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
	});
	auto ok_button_collider = dynamic_pointer_cast<RectCollider>(ok_button->GetCollider("Button"));
	ok_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	ok_button_collider->set_collision_group_tag("UI");

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

	auto character_select_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("character_select_button", ui_layer));
	character_select_button->set_position({ 630.f, 540.f });
	character_select_button->set_texture("ok_button");
	character_select_button->set_size({ 126.f, 35.f });
	character_select_button->set_offset_flag(true);
	character_select_button->set_enablement(false);
	character_select_button->set_callback([this](float _time) {
		auto const& audio_manager = AudioManager::GetSingleton();

		audio_manager->RemoveSoundEffectInstance("sorceress select");
		audio_manager->RemoveSoundEffectInstance("options");

		SceneManager::GetSingleton()->CreateNextScene<MainScene>("MainScene");
	});
	auto character_select_button_collider = dynamic_pointer_cast<RectCollider>(character_select_button->GetCollider("Button"));
	character_select_button_collider->set_model_info({ 0.f, 0.f, 126.f, 35.f });
	character_select_button_collider->set_collision_group_tag("UI");

	// CREDIT
	auto credit_background = object_manager->CreateObject<UI>("credit_background", background_layer);
	credit_background->set_texture("credit_background");
	credit_background->set_size({ 800.f, 600.f });
	credit_background->set_enablement(false);

	return true;
}

void LogoScene::_Input(float _time)
{
	if (InputManager::GetSingleton()->KeyPush("MouseLeft"))
	{
		switch (state_)
		{
		case LOGO_SCENE::TRADEMARK:
			state_ = LOGO_SCENE::TITLE;

			auto const& background_layer = scene()->FindLayer("Background");
			auto const& ui_layer = scene()->FindLayer("UI");

			background_layer->FindObject("trademark")->set_activation(false);
			background_layer->FindObject("title")->set_enablement(true);
			ui_layer->FindObject("single_player_button")->set_enablement(true);
			ui_layer->FindObject("credit_button")->set_enablement(true);
			ui_layer->FindObject("video_button")->set_enablement(true);
			ui_layer->FindObject("d2_exit_button")->set_enablement(true);

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

void LogoScene::_CreateCharacter()
{
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& ui_layer = scene()->FindLayer("UI");

	auto sorceress = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("sorceress", ui_layer));
	sorceress->set_position({ 450.f, 200.f });
	sorceress->AddAnimationClip("SONU1");
	sorceress->AddAnimationClip("SONU2");
	sorceress->AddAnimationClip("SONU3");
	sorceress->AddAnimationClip("SOFW");
	sorceress->AddAnimationClip("SOBW");
	sorceress->set_color_key(RGB(1, 1, 1));

	auto sorceress_skill = object_manager->CreateObject<UI>("sorceress_skill", ui_layer);
	sorceress_skill->set_position({ 451.f, 157.f });
	sorceress_skill->AddAnimationClip("SONU3s");
	sorceress_skill->AddAnimationClip("SOFWs");
	sorceress_skill->AddAnimationClip("SOBWs");
	sorceress_skill->set_color_key(RGB(1, 1, 1));
	sorceress_skill->set_enablement(false);

	sorceress->set_callback([p = sorceress.get(), p2 = sorceress_skill.get(), p3 = ui_layer.get()](float _time) {
		auto const& audio_manager = AudioManager::GetSingleton();

		if (p->GetCurrentAnimationClipTag() == "SONU2")
		{
			p->SetDefaultClip("SONU3");
			p->ChangeAnimationClip("SOFW");

			p2->set_enablement(true);
			p2->ChangeAnimationClip("SOFWs");

			p3->FindObject("character_select_button")->set_enablement(true);

			audio_manager->RemoveSoundEffectInstance("sorceress select");
			auto sorceress_select = audio_manager->FindSoundEffect("sorceress select")->CreateInstance();
			sorceress_select->Play();
			audio_manager->AddSoundEffectInstance("sorceress select", move(sorceress_select));
		}
		else if (p->GetCurrentAnimationClipTag() == "SONU3")
		{
			p->SetDefaultClip("SONU1");
			p->ChangeAnimationClip("SOBW");

			p2->ChangeAnimationClip("SOBWs");

			p3->FindObject("character_select_button")->set_enablement(false);

			audio_manager->RemoveSoundEffectInstance("sorceress deselect");
			auto sorceress_deselect = audio_manager->FindSoundEffect("sorceress deselect")->CreateInstance();
			sorceress_deselect->Play();
			audio_manager->AddSoundEffectInstance("sorceress deselect", move(sorceress_deselect));
		}
	});
	auto sorceress_button_collider = dynamic_pointer_cast<RectCollider>(sorceress->GetCollider("Button"));
	sorceress_button_collider->set_model_info({ 0.f, 0.f, 134.f, 222.f });
	auto sorceress_collider = dynamic_pointer_cast<RectCollider>(sorceress->AddCollider<RectCollider>("SorceressCollider"));
	sorceress_collider->set_model_info({ 0.f, 0.f, 134.f, 222.f });
	sorceress_collider->set_collision_group_tag("UI");
	sorceress_collider->SetCallBack([p = sorceress.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		if (p->GetCurrentAnimationClipTag() == "SONU1")
			p->ChangeAnimationClip("SONU2");
	}, COLLISION_CALLBACK::ENTER);
	sorceress_collider->SetCallBack([p = sorceress.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		if (p->GetCurrentAnimationClipTag() == "SONU1")
			p->ChangeAnimationClip("SONU2");
	}, COLLISION_CALLBACK::STAY);
	sorceress_collider->SetCallBack([p = sorceress.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		if (p->GetCurrentAnimationClipTag() == "SONU2")
			p->ChangeAnimationClip("SONU1");
	}, COLLISION_CALLBACK::LEAVE);
}

void LogoScene::_ChangeToTitle()
{
	state_ = LOGO_SCENE::TITLE;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("character_select")->set_enablement(false);
	background_layer->FindObject("character_select_box")->set_enablement(false);
	background_layer->FindObject("credit_background")->set_enablement(false);
	ui_layer->FindObject("create_new_character")->set_enablement(false);
	ui_layer->FindObject("exit_button")->set_enablement(false);
	ui_layer->FindObject("delete_button")->set_enablement(false);
	ui_layer->FindObject("ok_button")->set_enablement(false);

	background_layer->FindObject("title")->set_enablement(true);
	background_layer->FindObject("d2_logo")->set_enablement(true);
	ui_layer->FindObject("single_player_button")->set_enablement(true);
	ui_layer->FindObject("credit_button")->set_enablement(true);
	ui_layer->FindObject("video_button")->set_enablement(true);
	ui_layer->FindObject("d2_exit_button")->set_enablement(true);
}

void LogoScene::_ChangeToCharacterSelect()
{
	state_ = LOGO_SCENE::CHARACTER_SELECT;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	_ClearCharacter();

	background_layer->FindObject("title")->set_enablement(false);
	background_layer->FindObject("d2_logo")->set_enablement(false);
	background_layer->FindObject("character_create")->set_enablement(false);
	background_layer->FindObject("bonfire")->set_enablement(false);
	ui_layer->FindObject("single_player_button")->set_enablement(false);
	ui_layer->FindObject("credit_button")->set_enablement(false);
	ui_layer->FindObject("video_button")->set_enablement(false);
	ui_layer->FindObject("d2_exit_button")->set_enablement(false);

	background_layer->FindObject("character_select")->set_enablement(true);
	background_layer->FindObject("character_select_box")->set_enablement(true);
	ui_layer->FindObject("create_new_character")->set_enablement(true);
	ui_layer->FindObject("exit_button")->set_enablement(true);
	ui_layer->FindObject("delete_button")->set_enablement(true);
	ui_layer->FindObject("ok_button")->set_enablement(true);
}

void LogoScene::_ChangeToCharacterCreate()
{
	state_ = LOGO_SCENE::CHARACTER_CREATE;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("character_select")->set_enablement(false);
	background_layer->FindObject("character_select_box")->set_enablement(false);
	ui_layer->FindObject("create_new_character")->set_enablement(false);
	ui_layer->FindObject("delete_button")->set_enablement(false);
	ui_layer->FindObject("ok_button")->set_enablement(false);

	background_layer->FindObject("character_create")->set_enablement(true);
	background_layer->FindObject("bonfire")->set_enablement(true);

	_CreateCharacter();
}

void LogoScene::_ChangeToCredit()
{
	state_ = LOGO_SCENE::CREDIT;

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& ui_layer = scene()->FindLayer("UI");

	background_layer->FindObject("title")->set_enablement(false);
	background_layer->FindObject("d2_logo")->set_enablement(false);
	ui_layer->FindObject("single_player_button")->set_enablement(false);
	ui_layer->FindObject("credit_button")->set_enablement(false);
	ui_layer->FindObject("video_button")->set_enablement(false);
	ui_layer->FindObject("d2_exit_button")->set_enablement(false);

	background_layer->FindObject("credit_background")->set_enablement(true);
	ui_layer->FindObject("exit_button")->set_enablement(true);
}

void LogoScene::_ClearCharacter()
{
	auto const& ui_layer = scene()->FindLayer("UI");

	if (auto sorceress = ui_layer->FindObject("sorceress"))
	{
		sorceress->set_activation(false);
		ui_layer->FindObject("sorceress_skill")->set_activation(false);

		auto const& audio_manager = AudioManager::GetSingleton();
		audio_manager->RemoveSoundEffectInstance("sorceress select");
		audio_manager->RemoveSoundEffectInstance("sorceress deselect");
	}

	ui_layer->FindObject("character_select_button")->set_enablement(false);
}