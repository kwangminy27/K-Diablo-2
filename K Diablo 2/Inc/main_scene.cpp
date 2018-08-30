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
#include "melee_monster.h"
#include "missile.h"
#include "spin_ice_bolt.h"
#include "frozen_orb.h"
#include "nova.h"
#include "effect.h"
#include "frozen_armor.h"
#include "button.h"
#include "text.h"
#include "texture.h"
#include "audio_manager.h"
#include "point_collider.h"
#include "rect_collider.h"
#include "circle_collider.h"
#include "ai_manager.h"
#include "bar.h"

using namespace std;
using namespace TYPE;

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

	// prototype
	if (!object_manager->CreatePrototype<Missile>("ice_bolt", scene()))
		return false;

	if (!object_manager->CreatePrototype<FrozenOrb>("ice_orb", scene()))
		return false;

	if (!object_manager->CreatePrototype<SpinIceBolt>("spin_ice_bolt", scene()))
		return false;

	if (!object_manager->CreatePrototype<Nova>("frost_nova", scene()))
		return false;

	if (!object_manager->CreatePrototype<Missile>("frost_nova_piece", scene()))
		return false;

	if (!object_manager->CreatePrototype<Missile>("ice_blast", scene()))
		return false;

	// sound
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
	rogue_encampment->set_texture("rogue_encampment");
	rogue_encampment->set_size({ static_cast<float>(rogue_encampment->texture()->width()), static_cast<float>(rogue_encampment->texture()->height()) });
	rogue_encampment->CreateTile(STAGE::ISOMETRIC, 50, 50, { 8000.f, 4000.f }, { 160.f, 80.f }, "base");
	camera_manager->set_world_size({ rogue_encampment->stage_size().x, rogue_encampment->stage_size().y });

	// player
	auto player = dynamic_pointer_cast<Player>(object_manager->CreateObject<Player>("player", default_layer));
	player->set_position({ 4000, 2000.f });
	player->set_size({ 42.f, 73.f });
	player->set_pivot({ 0.5f, 1.f });
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
	player->set_astar_interval(0.02f);
	player->set_max_hp(100.f);
	player->set_hp(100.f);
	player->set_max_mp(200.f);
	player->set_mp(200.f);
	camera_manager->set_target(player);
	auto player_collider = dynamic_pointer_cast<RectCollider>(player->AddCollider<RectCollider>("PlayerCollider"));
	player_collider->set_model_info({ 0.f, 0.f, 42.f, 73.f });
	player_collider->set_pivot({ 0.5f, 1.f });

	auto ice_cast_new_1 = object_manager->CreateObject<Effect>("ice_cast_new_1", ui_layer);
	ice_cast_new_1->AddAnimationClip("ice_cast_new_1");
	ice_cast_new_1->SetAnimationCallback("ice_cast_new_1", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_1->set_color_key(RGB(1, 1, 1));
	ice_cast_new_1->set_enablement(false);

	auto ice_cast_new_2 = object_manager->CreateObject<Effect>("ice_cast_new_2", ui_layer);
	ice_cast_new_2->AddAnimationClip("ice_cast_new_2");
	ice_cast_new_2->SetAnimationCallback("ice_cast_new_2", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_2->set_color_key(RGB(1, 1, 1));
	ice_cast_new_2->set_enablement(false);

	auto ice_cast_new_3 = object_manager->CreateObject<Effect>("ice_cast_new_3", ui_layer);
	ice_cast_new_3->AddAnimationClip("ice_cast_new_3");
	ice_cast_new_3->SetAnimationCallback("ice_cast_new_3", [_player = player.get()]() {
		_player->set_state(PLAYER::IDLE);
	});
	ice_cast_new_3->set_color_key(RGB(1, 1, 1));
	ice_cast_new_3->set_enablement(false);

	auto frozen_armor = dynamic_pointer_cast<FrozenArmor>(ObjectManager::GetSingleton()->CreateObject<FrozenArmor>("frozen_armor", default_layer));
	frozen_armor->AddAnimationClip("frozen_armor");
	frozen_armor->set_color_key(RGB(1, 1, 1));
	frozen_armor->set_enablement(false);

	auto teleport = dynamic_pointer_cast<Effect>(ObjectManager::GetSingleton()->CreateObject<Effect>("teleport", default_layer));
	teleport->AddAnimationClip("teleport");
	teleport->set_color_key(RGB(1, 1, 1));
	teleport->set_enablement(false);

	// character
	_CreateCharacterWindow();

	// inventory
	_CreateInventoryWindow();

	// skill_tree
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

	// ui
	auto control_panel = object_manager->CreateObject<UI>("control_panel", ui_layer);
	control_panel->set_position({ 0.f, 376.f });
	control_panel->set_size({ 640.f, 104.f });
	control_panel->set_texture("control_panel");
	control_panel->set_color_key(RGB(1, 1, 1));

	auto const& health = dynamic_pointer_cast<Bar>(object_manager->CreateObject<Bar>("health", ui_layer));
	health->set_position({ 30.f, 387.f });
	health->set_size({ 80.f, 80.f });
	health->set_texture("health");
	health->set_color_key(RGB(0, 0, 0));
	health->set_cutting_direction(BAR_CUTTING_DIRECTION::DOWN);
	health->set_ui_flag(true);

	auto health_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("health_text", ui_layer));
	health_text->set_position({ 10.f, 367.f });
	health_text->set_font_size(FONT_SIZE::_16);

	auto const& mana = dynamic_pointer_cast<Bar>(object_manager->CreateObject<Bar>("mana", ui_layer));
	mana->set_position({ 530.f, 387.f });
	mana->set_size({ 80.f, 80.f });
	mana->set_texture("mana");
	mana->set_color_key(RGB(0, 0, 0));
	mana->set_cutting_direction(BAR_CUTTING_DIRECTION::DOWN);
	mana->set_ui_flag(true);

	auto mana_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("mana_text", ui_layer));
	mana_text->set_position({ 499.f, 367.f });
	mana_text->set_font_size(FONT_SIZE::_16);

	auto left_skill_tap = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("left_skill_tap", ui_layer));
	left_skill_tap->set_position({ 118.f, 432.f });
	left_skill_tap->set_size({ 48.f, 48.f });
	left_skill_tap->set_texture("attack_icon");
	left_skill_tap->set_color_key(RGB(0, 0, 0));
	left_skill_tap->set_offset_flag(false);
	left_skill_tap->set_callback([this](float _time) {
	});

	auto left_skill_tap_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("left_skill_tap_text", ui_layer));
	left_skill_tap_text->set_position({ 98.f, 412.f });
	left_skill_tap_text->set_font_size(FONT_SIZE::_16);
	left_skill_tap_text->set_string("NORMAL ATTACK");
	left_skill_tap_text->set_enablement(false);

	auto left_skill_tap_collider = dynamic_pointer_cast<RectCollider>(left_skill_tap->GetCollider("Button"));
	left_skill_tap_collider->set_model_info({ 0.f, 0.f, 48.f, 48.f });
	left_skill_tap_collider->set_collision_group_tag("UI");
	left_skill_tap_collider->SetCallBack([_left_skill_tap_text = left_skill_tap_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_left_skill_tap_text->set_enablement(true);
	}, COLLISION_CALLBACK::ENTER);
	left_skill_tap_collider->SetCallBack([_left_skill_tap_text = left_skill_tap_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_left_skill_tap_text->set_enablement(false);
	}, COLLISION_CALLBACK::LEAVE);

	auto right_skill_tap = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("right_skill_tap", ui_layer));
	right_skill_tap->set_position({ 474.f, 432.f });
	right_skill_tap->set_size({ 48.f, 48.f });
	right_skill_tap->set_texture("ice_bolt_icon");
	right_skill_tap->set_color_key(RGB(0, 0, 0));
	right_skill_tap->set_offset_flag(false);
	right_skill_tap->set_callback([this](float _time) {
	});

	auto right_skill_tap_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("right_skill_tap_text", ui_layer));
	right_skill_tap_text->set_position({ 456.f, 412.f });
	right_skill_tap_text->set_font_size(FONT_SIZE::_16);
	right_skill_tap_text->set_string("ICE BOLT");
	right_skill_tap_text->set_enablement(false);

	auto right_skill_tap_collider = dynamic_pointer_cast<RectCollider>(right_skill_tap->GetCollider("Button"));
	right_skill_tap_collider->set_model_info({ 0.f, 0.f, 48.f, 48.f });
	right_skill_tap_collider->set_collision_group_tag("UI");
	right_skill_tap_collider->SetCallBack([_right_skill_tap_text = right_skill_tap_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_right_skill_tap_text->set_enablement(true);
	}, COLLISION_CALLBACK::ENTER);
	right_skill_tap_collider->SetCallBack([_right_skill_tap_text = right_skill_tap_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_right_skill_tap_text->set_enablement(false);
	}, COLLISION_CALLBACK::LEAVE);

	auto loading_screen = object_manager->CreateObject<UI>("loading_screen", ui_layer);
	loading_screen->AddAnimationClip("loading_screen");
	loading_screen->set_color_key(RGB(1, 1, 1));

	// option
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

	// monster
	_CreateHellBovine({4100.f, 2000.f });
	_CreateHellBovine({4200.f, 2000.f });
	_CreateHellBovine({4300.f, 2000.f });
	_CreateHellBovine({4400.f, 2000.f });
	_CreateHellBovine({4000.f, 2100.f });
	_CreateHellBovine({4000.f, 2200.f });
	_CreateHellBovine({4000.f, 2300.f });
	_CreateHellBovine({4000.f, 2400.f });

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
		_ToggleInventoryWindow();

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

	auto Level_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Level_number", ui_layer));
	Level_number->set_position({ 20.f, 45.f });
	Level_number->set_font_size(FONT_SIZE::_16);
	Level_number->set_string("1");
	Level_number->set_enablement(false);

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
	defence_number->set_position({ 265.f, 193.f });
	defence_number->set_font_size(FONT_SIZE::_16);
	defence_number->set_string("0");
	defence_number->set_enablement(false);

	auto stamina_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("stamina_number", ui_layer));
	stamina_number->set_position({ 265.f, 231.f });
	stamina_number->set_font_size(FONT_SIZE::_16);
	stamina_number->set_string("100");
	stamina_number->set_enablement(false);

	auto life_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("life_number", ui_layer));
	life_number->set_position({ 225.f, 255.f });
	life_number->set_font_size(FONT_SIZE::_16);
	life_number->set_string("40");
	life_number->set_enablement(false);

	auto mana_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("mana_number", ui_layer));
	mana_number->set_position({ 225.f, 293.f });
	mana_number->set_font_size(FONT_SIZE::_16);
	mana_number->set_string("60");
	mana_number->set_enablement(false);

	auto max_life_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("max_life_number", ui_layer));
	max_life_number->set_position({ 265.f, 255.f });
	max_life_number->set_font_size(FONT_SIZE::_16);
	max_life_number->set_string("40");
	max_life_number->set_enablement(false);

	auto max_mana_number = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("max_mana_number", ui_layer));
	max_mana_number->set_position({ 265.f, 293.f });
	max_mana_number->set_font_size(FONT_SIZE::_16);
	max_mana_number->set_string("60");
	max_mana_number->set_enablement(false);

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

	auto Level_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Level_text", ui_layer));
	Level_text->set_position({ 0.f, 32.f });
	Level_text->set_font_size(FONT_SIZE::_8);
	Level_text->set_string("Level");
	Level_text->set_enablement(false);

	auto Expression_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Expression_text", ui_layer));
	Expression_text->set_position({ 70.f, 32.f });
	Expression_text->set_font_size(FONT_SIZE::_8);
	Expression_text->set_string("Expression");
	Expression_text->set_enablement(false);

	auto Next_Level_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Next_Level_text", ui_layer));
	Next_Level_text->set_position({ 200.f, 32.f });
	Next_Level_text->set_font_size(FONT_SIZE::_8);
	Next_Level_text->set_string("Next_Level");
	Next_Level_text->set_enablement(false);

	auto Strength_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Strength_text", ui_layer));
	Strength_text->set_position({ 0.f, 83.f });
	Strength_text->set_font_size(FONT_SIZE::_8);
	Strength_text->set_string("Strength");
	Strength_text->set_enablement(false);

	auto Dexterity_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Dexterity_text", ui_layer));
	Dexterity_text->set_position({ 0.f, 145.f });
	Dexterity_text->set_font_size(FONT_SIZE::_8);
	Dexterity_text->set_string("Dexterity");
	Dexterity_text->set_enablement(false);

	auto Vitality_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Vitality_text", ui_layer));
	Vitality_text->set_position({ 0.f, 231.f });
	Vitality_text->set_font_size(FONT_SIZE::_8);
	Vitality_text->set_string("Vitality");
	Vitality_text->set_enablement(false);

	auto Energy_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Energy_text", ui_layer));
	Energy_text->set_position({ 0.f, 293.f });
	Energy_text->set_font_size(FONT_SIZE::_8);
	Energy_text->set_string("Energy");
	Energy_text->set_enablement(false);

	auto Defence_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Defence_text", ui_layer));
	Defence_text->set_position({ 150.f, 193.f });
	Defence_text->set_font_size(FONT_SIZE::_8);
	Defence_text->set_string("Defence");
	Defence_text->set_enablement(false);

	auto Stamina_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Stamina_text", ui_layer));
	Stamina_text->set_position({ 150.f, 231.f });
	Stamina_text->set_font_size(FONT_SIZE::_8);
	Stamina_text->set_string("Stamina");
	Stamina_text->set_enablement(false);

	auto Life_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Life_text", ui_layer));
	Life_text->set_position({ 150.f, 255.f });
	Life_text->set_font_size(FONT_SIZE::_8);
	Life_text->set_string("Life");
	Life_text->set_enablement(false);

	auto Mana_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Mana_text", ui_layer));
	Mana_text->set_position({ 150.f, 293.f });
	Mana_text->set_font_size(FONT_SIZE::_8);
	Mana_text->set_string("Mana");
	Mana_text->set_enablement(false);

	auto Fire_Resistance_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Fire_Resistance_text", ui_layer));
	Fire_Resistance_text->set_position({ 170.f, 331.f });
	Fire_Resistance_text->set_font_size(FONT_SIZE::_8);
	Fire_Resistance_text->set_string("Fire\nResistance");
	Fire_Resistance_text->set_enablement(false);

	auto Cold_Resistance_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Cold_Resistance_text", ui_layer));
	Cold_Resistance_text->set_position({ 170.f, 355.f });
	Cold_Resistance_text->set_font_size(FONT_SIZE::_8);
	Cold_Resistance_text->set_string("Cold\nResistance");
	Cold_Resistance_text->set_enablement(false);

	auto Lightening_Resistance_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Lightening_Resistance_text", ui_layer));
	Lightening_Resistance_text->set_position({ 170.f, 379.f });
	Lightening_Resistance_text->set_font_size(FONT_SIZE::_8);
	Lightening_Resistance_text->set_string("Lightening\nResistance");
	Lightening_Resistance_text->set_enablement(false);

	auto Poison_Resistance_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("Poison_Resistance_text", ui_layer));
	Poison_Resistance_text->set_position({ 170.f, 403.f });
	Poison_Resistance_text->set_font_size(FONT_SIZE::_8);
	Poison_Resistance_text->set_string("Poison\nResistance");
	Poison_Resistance_text->set_enablement(false);

	auto character_window_CLOSE_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("character_window_CLOSE_text", ui_layer));
	character_window_CLOSE_text->set_position({ 114.f, 368.f });
	character_window_CLOSE_text->set_font_size(FONT_SIZE::_16);
	character_window_CLOSE_text->set_string("CLOSE");
	character_window_CLOSE_text->set_enablement(false);

	auto character_window_cancel_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("character_window_cancel_button", ui_layer));
	character_window_cancel_button->set_position({ 128.f, 388.f });
	character_window_cancel_button->set_size({ 32.f, 32.f });
	character_window_cancel_button->set_texture("cancel_button");
	character_window_cancel_button->set_offset_flag(true);
	character_window_cancel_button->set_enablement(false);
	character_window_cancel_button->set_callback([this, _character_window_CLOSE_text = character_window_CLOSE_text.get()](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_character_window_CLOSE_text->set_enablement(false);
		_ToggleCharacterWindow();
	});

	auto character_window_cancel_button_collider = dynamic_pointer_cast<RectCollider>(character_window_cancel_button->GetCollider("Button"));
	character_window_cancel_button_collider->set_model_info({ 0.f, 0.f, 32.f, 32.f });
	character_window_cancel_button_collider->set_collision_group_tag("UI");
	character_window_cancel_button_collider->SetCallBack([_character_window_CLOSE_text = character_window_CLOSE_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_character_window_CLOSE_text->set_enablement(true);
	}, COLLISION_CALLBACK::ENTER);
	character_window_cancel_button_collider->SetCallBack([_character_window_CLOSE_text = character_window_CLOSE_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_character_window_CLOSE_text->set_enablement(false);
	}, COLLISION_CALLBACK::LEAVE);
}

void MainScene::_CreateInventoryWindow()
{
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& ui_layer = scene()->FindLayer("UI");

	auto inventory = object_manager->CreateObject<UI>("inventory", ui_layer);
	inventory->set_position({ 320.f, 0.f });
	inventory->set_size({ 320.f, 432.f });
	inventory->set_texture("inventory");
	inventory->set_color_key(RGB(1, 1, 1));
	inventory->set_enablement(false);

	auto inventory_window_CLOSE_text = dynamic_pointer_cast<Text>(object_manager->CreateObject<Text>("inventory_window_CLOSE_text", ui_layer));
	inventory_window_CLOSE_text->set_position({ 324.f, 368.f });
	inventory_window_CLOSE_text->set_font_size(FONT_SIZE::_16);
	inventory_window_CLOSE_text->set_string("CLOSE");
	inventory_window_CLOSE_text->set_enablement(false);

	auto inventory_window_cancel_button = dynamic_pointer_cast<Button>(object_manager->CreateObject<Button>("inventory_window_cancel_button", ui_layer));
	inventory_window_cancel_button->set_position({ 338.f, 384.f });
	inventory_window_cancel_button->set_size({ 32.f, 32.f });
	inventory_window_cancel_button->set_texture("cancel_button");
	inventory_window_cancel_button->set_offset_flag(true);
	inventory_window_cancel_button->set_enablement(false);
	inventory_window_cancel_button->set_callback([this, _inventory_window_CLOSE_text = inventory_window_CLOSE_text.get()](float _time) {
		AudioManager::GetSingleton()->FindSoundEffect("button")->Play();
		_inventory_window_CLOSE_text->set_enablement(false);
		_ToggleInventoryWindow();
	});

	auto inventory_window_cancel_button_collider = dynamic_pointer_cast<RectCollider>(inventory_window_cancel_button->GetCollider("Button"));
	inventory_window_cancel_button_collider->set_model_info({ 0.f, 0.f, 32.f, 32.f });
	inventory_window_cancel_button_collider->set_collision_group_tag("UI");
	inventory_window_cancel_button_collider->SetCallBack([_inventory_window_CLOSE_text = inventory_window_CLOSE_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_inventory_window_CLOSE_text->set_enablement(true);
	}, COLLISION_CALLBACK::ENTER);
	inventory_window_cancel_button_collider->SetCallBack([_inventory_window_CLOSE_text = inventory_window_CLOSE_text.get()](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
		_inventory_window_CLOSE_text->set_enablement(false);
	}, COLLISION_CALLBACK::LEAVE);
}

void MainScene::_ToggleCharacterWindow()
{
	auto const& ui_layer = scene()->FindLayer("UI");

	auto enablement = ui_layer->FindObject("character")->enablement();

	ui_layer->FindObject("character")->set_enablement(enablement ^ true);
	ui_layer->FindObject("sorceress_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Sorceress_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("Level_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("strength_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("dexterity_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("vitality_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("energy_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("defence_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("stamina_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("life_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("mana_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("max_life_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("max_mana_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("fire_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("cold_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("lightening_resistance_number")->set_enablement(enablement ^ true);
	ui_layer->FindObject("poison_resistance_number")->set_enablement(enablement ^ true);

	ui_layer->FindObject("Level_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Expression_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Next_Level_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("Strength_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Dexterity_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Vitality_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Energy_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("Defence_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Stamina_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Life_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Mana_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("Fire_Resistance_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Cold_Resistance_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Lightening_Resistance_text")->set_enablement(enablement ^ true);
	ui_layer->FindObject("Poison_Resistance_text")->set_enablement(enablement ^ true);

	ui_layer->FindObject("character_window_cancel_button")->set_enablement(enablement ^ true);
}

void MainScene::_ToggleInventoryWindow()
{
	auto const& ui_layer = scene()->FindLayer("UI");

	auto enablement = ui_layer->FindObject("inventory")->enablement();

	ui_layer->FindObject("inventory")->set_enablement(enablement ^ true);

	ui_layer->FindObject("inventory_window_cancel_button")->set_enablement(enablement ^ true);
}

void MainScene::_CreateHellBovine(Point const& _position)
{
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& default_layer = scene()->FindLayer("Default");

	auto hell_bovine = dynamic_pointer_cast<MeleeMonster>(object_manager->CreateObject<MeleeMonster>("hell_bovine", default_layer));
	hell_bovine->set_position(_position);
	hell_bovine->set_size({ 141.f, 144.f });
	hell_bovine->set_pivot({ 0.5f, 1.f });
	hell_bovine->set_walk_speed(100.f);
	hell_bovine->set_max_hp(100.f);
	hell_bovine->set_hp(100.f);
	for (int i = 0; i < 8; ++i)
	{
		string hell_bovine_neutral = "hell_bovine_neutral_"s + to_string(i);
		string hell_bovine_walk = "hell_bovine_walk_"s + to_string(i);
		string hell_bovine_attack1 = "hell_bovine_attack1_"s + to_string(i);
		string hell_bovine_attack2 = "hell_bovine_attack2_"s + to_string(i);
		string hell_bovine_get_hit = "hell_bovine_get_hit_"s + to_string(i);
		string hell_bovine_death = "hell_bovine_death_"s + to_string(i);
		string hell_bovine_dead = "hell_bovine_dead_"s + to_string(i);

		hell_bovine->AddAnimationClip(hell_bovine_neutral.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_walk.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_attack1.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_attack2.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_get_hit.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_death.c_str());
		hell_bovine->AddAnimationClip(hell_bovine_dead.c_str());
	}
	hell_bovine->set_color_key(RGB(170, 170, 170));
	hell_bovine->set_territory_radius(150.f);

	auto hell_bovine_body_collider = dynamic_pointer_cast<RectCollider>(hell_bovine->AddCollider<RectCollider>("MonsterBody"));
	hell_bovine_body_collider->set_model_info({ 0.f, 0.f, 70.f, 144.f });
	hell_bovine_body_collider->set_pivot({ 0.5f, 1.f });

	auto hell_bovine_territory_collider = dynamic_pointer_cast<CircleCollider>(hell_bovine->AddCollider<CircleCollider>("hell_bovine_territory_collider"));
	hell_bovine_territory_collider->set_model_info({ 0.f, 0.f, hell_bovine->territory_radius() });
	hell_bovine_territory_collider->SetCallBack([_hell_bovine = hell_bovine.get()](shared_ptr<Collider> const _src, shared_ptr<Collider> const _dest, float _time) {
		if (_src->tag() == "PlayerCollider")
		{
			auto const& player = dynamic_pointer_cast<Player>(_src->object());
			auto const& stage = dynamic_pointer_cast<Stage>(player->stage());

			_hell_bovine->set_target(player);
			_hell_bovine->set_stage(stage);
			if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
				_hell_bovine->set_state(MONSTER_STATE::WALK);

			auto& travel_path_stack = _hell_bovine->travel_path_stack();

			travel_path_stack = AIManager::GetSingleton()->ProcessAStar(_hell_bovine->position(), player->position(), stage);
			if (!travel_path_stack.empty())
			{
				_hell_bovine->set_astar_complete_flag(false);
				_hell_bovine->set_arrival_flag(false);
				_hell_bovine->set_next_target_point(stage->GetTileCenterPosition(travel_path_stack.top()));
				_hell_bovine->set_final_target_point(player->position());
			}
		}
		else if (_dest->tag() == "PlayerCollider")
		{
			auto const& player = dynamic_pointer_cast<Player>(_dest->object());
			auto const& stage = dynamic_pointer_cast<Stage>(player->stage());

			_hell_bovine->set_target(player);
			_hell_bovine->set_stage(stage);
			if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
				_hell_bovine->set_state(MONSTER_STATE::WALK);

			auto& travel_path_stack = _hell_bovine->travel_path_stack();

			travel_path_stack = AIManager::GetSingleton()->ProcessAStar(_hell_bovine->position(), player->position(), stage);
			if (!travel_path_stack.empty())
			{
				_hell_bovine->set_astar_complete_flag(false);
				_hell_bovine->set_arrival_flag(false);
				_hell_bovine->set_next_target_point(stage->GetTileCenterPosition(travel_path_stack.top()));
				_hell_bovine->set_final_target_point(player->position());
			}
		}
	}, COLLISION_CALLBACK::ENTER);
	hell_bovine_territory_collider->SetCallBack([_hell_bovine = hell_bovine.get()](shared_ptr<Collider> const _src, shared_ptr<Collider> const _dest, float _time) {
		if (_src->tag() == "PlayerCollider")
		{
			auto const& player = dynamic_pointer_cast<Player>(_src->object());
			auto const& stage = dynamic_pointer_cast<Stage>(player->stage());

			_hell_bovine->set_target(player);
			_hell_bovine->set_stage(stage);

			auto& travel_path_stack = _hell_bovine->travel_path_stack();

			_hell_bovine->set_astar_elapsed_time(_hell_bovine->astar_elapsed_time() + _time);
			if (_hell_bovine->astar_elapsed_time() >= _hell_bovine->astar_interval())
			{
				_hell_bovine->set_astar_elapsed_time(_hell_bovine->astar_elapsed_time() - _hell_bovine->astar_interval());

				travel_path_stack = AIManager::GetSingleton()->ProcessAStar(_hell_bovine->position(), player->position(), stage);
				if (!travel_path_stack.empty())
				{
					if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
						_hell_bovine->set_state(MONSTER_STATE::WALK);

					_hell_bovine->set_astar_complete_flag(false);
					_hell_bovine->set_arrival_flag(false);
					_hell_bovine->set_next_target_point(stage->GetTileCenterPosition(travel_path_stack.top()));
					_hell_bovine->set_final_target_point(player->position());
				}
			}
		}
		else if (_dest->tag() == "PlayerCollider")
		{
			auto const& player = dynamic_pointer_cast<Player>(_dest->object());
			auto const& stage = dynamic_pointer_cast<Stage>(player->stage());

			_hell_bovine->set_target(player);
			_hell_bovine->set_stage(stage);

			auto& travel_path_stack = _hell_bovine->travel_path_stack();

			_hell_bovine->set_astar_elapsed_time(_hell_bovine->astar_elapsed_time() + _time);
			if (_hell_bovine->astar_elapsed_time() >= _hell_bovine->astar_interval())
			{
				_hell_bovine->set_astar_elapsed_time(_hell_bovine->astar_elapsed_time() - _hell_bovine->astar_interval());

				travel_path_stack = AIManager::GetSingleton()->ProcessAStar(_hell_bovine->position(), player->position(), stage);
				if (!travel_path_stack.empty())
				{
					if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
						_hell_bovine->set_state(MONSTER_STATE::WALK);

					_hell_bovine->set_astar_complete_flag(false);
					_hell_bovine->set_arrival_flag(false);
					_hell_bovine->set_next_target_point(stage->GetTileCenterPosition(travel_path_stack.top()));
					_hell_bovine->set_final_target_point(player->position());
				}
			}
		}
	}, COLLISION_CALLBACK::STAY);
	hell_bovine_territory_collider->SetCallBack([_hell_bovine = hell_bovine.get()](shared_ptr<Collider> const _src, shared_ptr<Collider> const _dest, float _time) {
		if (_src->tag() == "PlayerCollider")
		{
			_hell_bovine->clear_target();

			if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
				_hell_bovine->set_state(MONSTER_STATE::NEUTRAL);
		}
		else if (_dest->tag() == "PlayerCollider")
		{
			_hell_bovine->clear_target();

			if ((_hell_bovine->state() != MONSTER_STATE::DEATH) && (_hell_bovine->state() != MONSTER_STATE::DEAD))
				_hell_bovine->set_state(MONSTER_STATE::NEUTRAL);
		}
	}, COLLISION_CALLBACK::LEAVE);
}