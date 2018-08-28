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
#include "text.h"
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

	// character
	_CreateCharacterWindow();

	// inventory
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
		_ToggleCharacterWindow();

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

void MainScene::_CreateCharacterWindow()
{
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& ui_layer = scene()->FindLayer("UI");

	auto character = object_manager->CreateObject<UI>("character", ui_layer);
	character->set_position({ 0.f, 0.f });
	character->set_size({ 320.f, 432.f });
	character->set_texture("character");
	character->set_color_key(RGB(1, 1, 1));
	character->set_enablement(false);

	auto sorceress_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("sorceress_text", ui_layer));
	sorceress_text->set_position({ 50.f, 10.f });
	sorceress_text->set_font_size(FONT_SIZE::_16);
	sorceress_text->set_string("sorceress");
	sorceress_text->set_enablement(false);

	auto Sorceress_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Sorceress_text", ui_layer));
	Sorceress_text->set_position({ 200.f, 10.f });
	Sorceress_text->set_font_size(FONT_SIZE::_16);
	Sorceress_text->set_string("Sorceress");
	Sorceress_text->set_enablement(false);

	auto strength_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("strength_number", ui_layer));
	strength_number->set_position({ 73.f, 83.f });
	strength_number->set_font_size(FONT_SIZE::_16);
	strength_number->set_string("10");
	strength_number->set_enablement(false);

	auto dexterity_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("dexterity_number", ui_layer));
	dexterity_number->set_position({ 73.f, 145.f });
	dexterity_number->set_font_size(FONT_SIZE::_16);
	dexterity_number->set_string("25");
	dexterity_number->set_enablement(false);

	auto vitality_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("vitality_number", ui_layer));
	vitality_number->set_position({ 73.f, 231.f });
	vitality_number->set_font_size(FONT_SIZE::_16);
	vitality_number->set_string("10");
	vitality_number->set_enablement(false);

	auto energy_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("energy_number", ui_layer));
	energy_number->set_position({ 73.f, 293.f });
	energy_number->set_font_size(FONT_SIZE::_16);
	energy_number->set_string("35");
	energy_number->set_enablement(false);

	auto defence_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("defence_number", ui_layer));
	defence_number->set_position({ 270.f, 193.f });
	defence_number->set_font_size(FONT_SIZE::_16);
	defence_number->set_string("0");
	defence_number->set_enablement(false);

	auto stamina_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("stamina_number", ui_layer));
	stamina_number->set_position({ 270.f, 231.f });
	stamina_number->set_font_size(FONT_SIZE::_16);
	stamina_number->set_string("100");
	stamina_number->set_enablement(false);

	auto life_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("life_number", ui_layer));
	life_number->set_position({ 270.f, 255.f });
	life_number->set_font_size(FONT_SIZE::_16);
	life_number->set_string("40");
	life_number->set_enablement(false);

	auto mana_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("mana_number", ui_layer));
	mana_number->set_position({ 270.f, 293.f });
	mana_number->set_font_size(FONT_SIZE::_16);
	mana_number->set_string("60");
	mana_number->set_enablement(false);

	auto fire_resistance_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("fire_resistance_number", ui_layer));
	fire_resistance_number->set_position({ 270.f, 331.f });
	fire_resistance_number->set_font_size(FONT_SIZE::_16);
	fire_resistance_number->set_string("0");
	fire_resistance_number->set_enablement(false);

	auto cold_resistance_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("cold_resistance_number", ui_layer));
	cold_resistance_number->set_position({ 270.f, 355.f });
	cold_resistance_number->set_font_size(FONT_SIZE::_16);
	cold_resistance_number->set_string("0");
	cold_resistance_number->set_enablement(false);

	auto lightening_resistance_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("lightening_resistance_number", ui_layer));
	lightening_resistance_number->set_position({ 270.f, 379.f });
	lightening_resistance_number->set_font_size(FONT_SIZE::_16);
	lightening_resistance_number->set_string("0");
	lightening_resistance_number->set_enablement(false);

	auto poison_resistance_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("poison_resistance_number", ui_layer));
	poison_resistance_number->set_position({ 270.f, 403.f });
	poison_resistance_number->set_font_size(FONT_SIZE::_16);
	poison_resistance_number->set_string("0");
	poison_resistance_number->set_enablement(false);
}

void MainScene::_ToggleCharacterWindow()
{
	auto const& ui_layer = scene()->FindLayer("UI");

	auto enablement = ui_layer->FindObject("character")->enablement();
	ui_layer->FindObject("character")->set_enablement(enablement ^ true);
	ui_layer->FindObject("sorceress_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Sorceress_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("strength_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("dexterity_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("vitality_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("energy_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("defence_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("stamina_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("life_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("mana_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("fire_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("cold_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("lightening_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("poison_resistance_number")->set_enablement(enablement ^ true);
}