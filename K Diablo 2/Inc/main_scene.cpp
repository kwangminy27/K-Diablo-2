#include "stdafx.h"
#include "main_scene.h"

#include "core.h"
#include "math.h"
#include "input_manager.h"
#include "camera_manager.h"
#include "scene.h"
#include "object_manager.h"
#include "ui.h"
#include "stage.h"
#include "player.h"
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
	Core::GetSingleton()->ResizeWindow({ 0.f, 0.f, static_cast<float>(RESOLUTION::GAME_WIDTH), static_cast<float>(RESOLUTION::GAME_HEIGHT) });

	auto const& input_manager = InputManager::GetSingleton();
	auto const& camera_manager = CameraManager::GetSingleton();
	auto const& object_manager = ObjectManager::GetSingleton();
	auto const& audio_manager = AudioManager::GetSingleton();

	auto sound_effect_instance = audio_manager->FindSoundEffect("town1")->CreateInstance();
	sound_effect_instance->Play(true);
	audio_manager->AddSoundEffectInstance("town1", move(sound_effect_instance));

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

	auto rogue_encampment = object_manager->CreateObject<Stage>("rogue_encampment", background_layer);
	rogue_encampment->set_size({ 7840.f, 3920.f });
	rogue_encampment->set_texture("rogue_encampment");
	camera_manager->set_world_size({ static_cast<float>(rogue_encampment->texture()->width()), static_cast<float>(rogue_encampment->texture()->height()) });

	auto player = object_manager->CreateObject<Player>("player", default_layer);
	player->set_position({ 3420.f, 1960.f });
	player->set_pivot({ 0.5f, 1.f });
	player->AddAnimationClip("town_neutral_0");
	player->AddAnimationClip("town_neutral_1");
	player->AddAnimationClip("town_neutral_2");
	player->AddAnimationClip("town_neutral_3");
	player->AddAnimationClip("town_neutral_4");
	player->AddAnimationClip("town_neutral_5");
	player->AddAnimationClip("town_neutral_6");
	player->AddAnimationClip("town_neutral_7");
	player->AddAnimationClip("town_neutral_8");
	player->AddAnimationClip("town_neutral_9");
	player->AddAnimationClip("town_neutral_10");
	player->AddAnimationClip("town_neutral_11");
	player->AddAnimationClip("town_neutral_12");
	player->AddAnimationClip("town_neutral_13");
	player->AddAnimationClip("town_neutral_14");
	player->AddAnimationClip("town_neutral_15");
	player->set_color_key(RGB(170, 170, 170));
	camera_manager->set_target(player);
	auto player_collider = dynamic_pointer_cast<RectCollider>(player->AddCollider<RectCollider>("PlayerCollider"));
	player_collider->set_model_info({ 0.f, 0.f, 42.f, 73.f });

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

	return true;
}

void MainScene::_Input(float _time)
{
	auto const& input_manager = InputManager::GetSingleton();
	auto const& camera_manager = CameraManager::GetSingleton();

	if (input_manager->KeyPressed("MoveLeft"))
		camera_manager->Move({ -1.f, 0.f }, _time);

	if (input_manager->KeyPressed("MoveUp"))
		camera_manager->Move({ 0.f, -1.f }, _time);

	if (input_manager->KeyPressed("MoveRight"))
		camera_manager->Move({ 1.f, 0.f }, _time);

	if (input_manager->KeyPressed("MoveDown"))
		camera_manager->Move({ 0.f, 1.f}, _time);

	if (input_manager->KeyPressed("MouseLeft"))
	{
		auto player = dynamic_pointer_cast<Player>(scene()->FindLayer("Default")->FindObject("player"));

		auto player_position = player->position();
		auto mouse_position = InputManager::GetSingleton()->mouse_world_position();

		auto direction = (mouse_position - player_position) / Math::GetDistance(player_position, mouse_position);
		
		float angle = Math::GetAngle(player_position, mouse_position);

		if (angle < 22.5f)
			player->ChangeAnimationClip("town_neutral_12");
		else if(angle < 45.f)
			player->ChangeAnimationClip("town_neutral_13");
		else if(angle < 67.5f)
			player->ChangeAnimationClip("town_neutral_14");
		else if(angle < 90.f)
			player->ChangeAnimationClip("town_neutral_15");
		else if (angle < 112.5f)
			player->ChangeAnimationClip("town_neutral_0");
		else if (angle < 135.f)
			player->ChangeAnimationClip("town_neutral_1");
		else if (angle < 157.5f)
			player->ChangeAnimationClip("town_neutral_2");
		else if (angle < 180.f)
			player->ChangeAnimationClip("town_neutral_3");
		else if (angle < 202.5f)
			player->ChangeAnimationClip("town_neutral_4");
		else if (angle < 225.f)
			player->ChangeAnimationClip("town_neutral_5");
		else if (angle < 247.5f)
			player->ChangeAnimationClip("town_neutral_6");
		else if (angle < 270.f)
			player->ChangeAnimationClip("town_neutral_7");
		else if (angle < 292.5f)
			player->ChangeAnimationClip("town_neutral_8");
		else if (angle < 315.f)
			player->ChangeAnimationClip("town_neutral_9");
		else if (angle < 337.5f)
			player->ChangeAnimationClip("town_neutral_10");
		else if (angle < 360.f)
			player->ChangeAnimationClip("town_neutral_11");

		player->Move(direction, _time);
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