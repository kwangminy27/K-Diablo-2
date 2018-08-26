#include "stdafx.h"
#include "main_scene.h"

#include "core.h"
#include "math.h"
#include "input_manager.h"
#include "camera_manager.h"
#include "scene_manager.h"
#include "scene.h"
#include "logo_scene.h"
#include "object_manager.h"
#include "ui.h"
#include "stage.h"
#include "player.h"
#include "missile.h"
#include "effect.h"
#include "button.h"
#include "texture.h"
#include "audio_manager.h"
#include "point_collider.h"
#include "rect_collider.h"

using namespace std;

MainScene::MainScene(MainScene const& _other)
{
}

MainScene::MainScene(MainScene&& _other) noexcept
{
}

void MainScene::_Release()
{
}

bool MainScene::_Initialize()
{
	Core::GetSingleton()->ResizeWindow({ 0.f, 0.f, static_cast<float>(RESOLUTION::GAME_WIDTH), static_cast<float>(RESOLUTION::GAME_HEIGHT) }, static_cast<bool>(DISPLAY::MODE));

	auto const& input_manager = InputManager::GetSingleton();
	auto const& camera_manager = CameraManager::GetSingleton();
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& audio_manager = AudioManager::GetSingleton();

	if (!object_manager->CreatePrototype<Missile>("ice_bolt", scene()))
		return false;

	auto town1 = audio_manager->FindSoundEffect("town1")->CreateInstance();
	town1->Play(true);
	audio_manager->AddSoundEffectInstance("town1", move(town1));

	auto rain2 = audio_manager->FindSoundEffect("rain2")->CreateInstance();
	rain2->Play(true);
	audio_manager->AddSoundEffectInstance("rain2", move(rain2));

	auto const& background_layer = scene()->FindLayer("Background");
	auto const& default_layer = scene()->FindLayer("Default");
	auto const& ui_layer = scene()->FindLayer("UI");

	auto mouse = object_manager->CreateObject<UI>("mouse", ui_layer);
	mouse->set_texture("mouse");
	mouse->set_color_key(RGB(0, 0, 0));
	mouse->set_size({ 32.f, 26.f });
	auto mouse_collider = dynamic_pointer_cast<PointCollider>(mouse->AddCollider<PointCollider>("MouseCursor"));
	mouse_collider->set_collision_group_tag("UI");
	input_manager->set_mouse(mouse);

	camera_manager->set_size({ static_cast<float>(RESOLUTION::GAME_WIDTH), static_cast<float>(RESOLUTION::GAME_HEIGHT) });

	auto rogue_encampment = dynamic_pointer_cast<Stage>(object_manager->CreateObject<Stage>("rogue_encampment", background_layer));
	rogue_encampment->CreateTile(STAGE::ISOMETRIC, 100, 100, { 16000.f, 8000.f }, { 160.f, 80.f }, "base");
	camera_manager->set_world_size({ rogue_encampment->stage_size().x, rogue_encampment->stage_size().y });

	auto player = dynamic_pointer_cast<Player>(object_manager->CreateObject<Player>("player", default_layer));
	player->set_position({ 8000, 4000.f });
	player->set_size({ 42.f, 73.f });
	player->set_pivot({ 0.5f, 0.5f });
	for (int i = 0; i < 16; ++i)
	{
		string town_neutral_tag = "town_neutral_" + to_string(i);
		string town_walk_tag = "town_walk_" + to_string(i);
		string run_tag = "run_" + to_string(i);
		string skill_casting_tag = "skill_casting_" + to_string(i);
		string skill_casting_special_tag = "skill_casting_special_" + to_string(i);

		player->AddAnimationClip(town_neutral_tag.c_str());
		player->AddAnimationClip(town_walk_tag.c_str());
		player->AddAnimationClip(run_tag.c_str());
		player->AddAnimationClip(skill_casting_tag.c_str());
		player->AddAnimationClip(skill_casting_special_tag.c_str());
	}
	player->set_color_key(RGB(170, 170, 170));
	player->set_stage(rogue_encampment);
	player->set_astar_interval(0.01f);
	camera_manager->set_target(player);
	auto player_collider = dynamic_pointer_cast<RectCollider>(player->AddCollider<RectCollider>("PlayerCollider"));
	player_collider->set_model_info({ 0.f, 0.f, 42.f, 73.f });
	player_collider->set_pivot({ 0.5f, 0.5f });

	auto ice_cast_new_1 = object_manager->CreateObject<Effect>("ice_cast_new_1", default_layer);
	ice_cast_new_1->AddAnimationClip("ice_cast_new_1");
	ice_cast_new_1->SetAnimationCallback("ice_cast_new_1", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_1->set_color_key(RGB(1, 1, 1));
	ice_cast_new_1->set_enablement(false);

	auto ice_cast_new_2 = object_manager->CreateObject<Effect>("ice_cast_new_2", default_layer);
	ice_cast_new_2->AddAnimationClip("ice_cast_new_2");
	ice_cast_new_2->SetAnimationCallback("ice_cast_new_2", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_2->set_color_key(RGB(1, 1, 1));
	ice_cast_new_2->set_enablement(false);

	auto ice_cast_new_3 = object_manager->CreateObject<Effect>("ice_cast_new_3", default_layer);
	ice_cast_new_3->AddAnimationClip("ice_cast_new_3");
	ice_cast_new_3->SetAnimationCallback("ice_cast_new_3", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_3->set_color_key(RGB(1, 1, 1));
	ice_cast_new_3->set_enablement(false);

	auto character = object_manager->CreateObject<UI>("character", ui_layer);
	character->set_position({ 0.f, 0.f });
	character->set_size({ 320.f, 432.f });
	character->set_texture("character");
	character->set_color_key(RGB(1, 1, 1));
	character->set_enablement(false);

	auto inventory = object_manager->CreateObject<UI>("inventory", ui_layer);
	inventory->set_position({ 320.f, 0.f });
	inventory->set_size({ 320.f, 432.f });
	inventory->set_texture("inventory");
	inventory->set_color_key(RGB(1, 1, 1));
	inventory->set_enablement(false);

	auto cold_skill = object_manager->CreateObject<UI>("cold_skill", ui_layer);
	cold_skill->set_position({ 320.f, 0.f });
	cold_skill->set_size({ 320.f, 432.f });
	cold_skill->set_texture("cold_skill");
	cold_skill->set_color_key(RGB(1, 1, 1));
	cold_skill->set_enablement(false);

	auto light_skill = object_manager->CreateObject<UI>("light_skill", ui_layer);
	light_skill->set_position({ 320.f, 0.f });
	light_skill->set_size({ 320.f, 432.f });
	light_skill->set_texture("light_skill");
	light_skill->set_color_key(RGB(1, 1, 1));
	light_skill->set_enablement(false);

	auto fire_skill = object_manager->CreateObject<UI>("fire_skill", ui_layer);
	fire_skill->set_position({ 320.f, 0.f });
	fire_skill->set_size({ 320.f, 432.f });
	fire_skill->set_texture("fire_skill");
	fire_skill->set_color_key(RGB(1, 1, 1));
	fire_skill->set_enablement(false);

	auto control_panel = object_manager->CreateObject<UI>("control_panel", ui_layer);
	control_panel->set_position({ 0.f, 376.f });
	control_panel->set_size({ 640.f, 104.f });
	control_panel->set_texture("control_panel");
	control_panel->set_color_key(RGB(1, 1, 1));

	auto health = object_manager->CreateObject<UI>("health", ui_layer);
	health->set_position({ 30.f, 387.f });
	health->set_size({ 80.f, 80.f });
	health->set_texture("health");
	health->set_color_key(RGB(0, 0, 0));

	auto mana = object_manager->CreateObject<UI>("mana", ui_layer);
	mana->set_position({ 529.f, 387.f });
	mana->set_size({ 80.f, 80.f });
	mana->set_texture("mana");
	mana->set_color_key(RGB(0, 0, 0));

	auto loading_screen = object_manager->CreateObject<UI>("loading_screen", ui_layer);
	loading_screen->set_position({ 194.f, 62.f });
	loading_screen->AddAnimationClip("loading_screen");
	loading_screen->set_color_key(RGB(1, 1, 1));

	auto pentspin_left = object_manager->CreateObject<UI>("pentspin_left", ui_layer);
	pentspin_left->set_position({ 20.f, 165.f });
	pentspin_left->AddAnimationClip("pentspin_left");
	pentspin_left->set_color_key(RGB(1, 1, 1));
	pentspin_left->set_enablement(false);

	auto pentspin_right = object_manager->CreateObject<UI>("pentspin_right", ui_layer);
	pentspin_right->set_position({ 568.f, 165.f });
	pentspin_right->AddAnimationClip("pentspin_right");
	pentspin_right->set_color_key(RGB(1, 1, 1));
	pentspin_right->set_enablement(false);

	auto exit = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("exit", ui_layer));
	exit->set_position({ 223.f, 169.f });
	exit->set_size({ 194.f, 38.f });
	exit->set_texture("exit");
	exit->set_color_key(RGB(0, 0, 0));
	exit->set_enablement(false);
	exit->set_callback([this](float _time) {
		auto const& audio_manager = AudioManager::GetSingleton();
		audio_manager->RemoveSoundEffectInstance("town1");
		audio_manager->RemoveSoundEffectInstance("rain2");
		audio_manager->RemoveSoundEffectInstance("LightDirt1");
		audio_manager->RemoveSoundEffectInstance("LightDirtRun1");

		Core::GetSingleton()->ResizeWindow({ 0.f, 0.f, static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT) }, static_cast<bool>(DISPLAY::MODE));

		SceneManager::GetSingleton()->CreateNextScene<LogoScene>("LogoScene");
	});
	auto exit_collider = dynamic_pointer_cast<RectCollider>(exit->GetCollider("Button"));
	exit_collider->set_model_info({ 0.f, 0.f, 194.f, 38.f });
	exit_collider->set_collision_group_tag("UI");
	exit_collider->SetCallBack([this](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		auto const& ui_layer = scene()->FindLayer("UI");

		ui_layer->FindObject("pentspin_left")->set_position({ 20.f, 165.f });
		ui_layer->FindObject("pentspin_right")->set_position({ 568.f, 165.f });
	}, COLLISION_CALLBACK::ENTER);

	auto return_to_game = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("return_to_game", ui_layer));
	return_to_game->set_position({ 199.f, 217.f });
	return_to_game->set_size({ 242.f, 38.f });
	return_to_game->set_texture("return_to_game");
	return_to_game->set_color_key(RGB(0, 0, 0));
	return_to_game->set_enablement(false);
	return_to_game->set_callback([this](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("select")->Play();

		auto const& ui_layer = scene()->FindLayer("UI");

		ui_layer->FindObject("pentspin_left")->set_enablement(false);
		ui_layer->FindObject("pentspin_right")->set_enablement(false);
		ui_layer->FindObject("exit")->set_enablement(false);
		ui_layer->FindObject("return_to_game")->set_enablement(false);
	});
	auto return_to_game_collider = dynamic_pointer_cast<RectCollider>(return_to_game->GetCollider("Button"));
	return_to_game_collider->set_model_info({ 0.f, 0.f, 242.f, 38.f });
	return_to_game_collider->set_collision_group_tag("UI");
	return_to_game_collider->SetCallBack([this](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		auto const& ui_layer = scene()->FindLayer("UI");

		ui_layer->FindObject("pentspin_left")->set_position({ 20.f, 217.f });
		ui_layer->FindObject("pentspin_right")->set_position({ 568.f, 217.f });
	}, COLLISION_CALLBACK::ENTER);

	return true;
}

void MainScene::_Input(float _time)
{
	auto const& core = Core::GetSingleton();
	auto const& input_manager = InputManager::GetSingleton();
	
	auto const& ui_layer = scene()->FindLayer("UI");

	if (input_manager->KeyPush("Character"))
	{
		auto enablement = ui_layer->FindObject("character")->enablement();
		ui_layer->FindObject("character")->set_enablement(enablement ^ true);
	}

	if (input_manager->KeyPush("Inventory"))
	{
		auto enablement = ui_layer->FindObject("inventory")->enablement();
		ui_layer->FindObject("inventory")->set_enablement(enablement ^ true);
	}

	if (input_manager->KeyPush("SkillTree"))
	{
		auto enablement = ui_layer->FindObject("cold_skill")->enablement();
		ui_layer->FindObject("cold_skill")->set_enablement(enablement ^ true);
	}

	if (input_manager->KeyPush("ESC"))
	{
		ui_layer->FindObject("pentspin_left")->set_enablement(true);
		ui_layer->FindObject("pentspin_right")->set_enablement(true);
		ui_layer->FindObject("exit")->set_enablement(true);
		ui_layer->FindObject("return_to_game")->set_enablement(true);
	}
}

void MainScene::_Update(float _time)
{
}

void MainScene::_LateUpdate(float _time)
{
}

void MainScene::_Collision(float _time)
{
}

void MainScene::_Render(HDC _device_context, float _time)
{
}