#include "stdafx.h"
#include "stage.h"

#include "math.h"
#include "camera_manager.h"
#include "tile.h"
#include "texture.h"
#include "rect_collider.h"

using namespace std;
using namespace TYPE;

Point const& Stage::stage_size() const
{
	return stage_size_;
}

void Stage::CreateTile(TILE _type, int _idx_width, int _idx_height, Point const& _stage_size, Point const& _tile_size, string const& _tag)
{
	stage_size_ = _stage_size;
	tile_size_ = _tile_size;

	tile_collection_.resize(_idx_height);
	for (auto i = 0; i < _idx_height; ++i)
		tile_collection_.at(i).resize(_idx_width);

	switch (_type)
	{
	case TILE::NORMAL:
		break;
	case TILE::ISOMETRIC:
		auto start_position = Point{ stage_size_.x * 0.5f, 0.f };
		auto tile_size_div_2 = _tile_size * 0.5f;

		for (auto i = 0; i < tile_collection_.size(); ++i)
		{
			for (auto j = 0; j < tile_collection_.at(0).size(); ++j)
			{
				auto tile = shared_ptr<Tile>(new Tile, [](Tile* _p) {
					_p->_Release();
					delete _p;
				});

				tile->_Initialize();

				tile->set_position({ start_position.x + tile_size_div_2.x * (j - i - 1), start_position.y + tile_size_div_2.y * (j + i) });
				tile->set_size(_tile_size);
				tile->set_type(TILE::ISOMETRIC);
				tile->set_idx({ static_cast<int>(j), static_cast<int>(i) });

				tile->set_texture(_tag);
				tile->set_color_key(RGB(255, 0, 255));

				tile_collection_.at(i).at(j) = move(tile);
			}
		}

		break;
	}
}

pair<int, int> Stage::FindIsometricIndex(Point const& _position)
{
	pair<int, int> isometric_index{};

	auto y_intercept_for_iso_x_idx = static_cast<int>(clamp(_position.y + (_position.x - stage_size_.y) * 0.5f, 0.f, stage_size_.y * (1.f - numeric_limits<float>::epsilon())));
	auto y_intercept_for_iso_y_idx = static_cast<int>(clamp(_position.y - (_position.x - stage_size_.y) * 0.5f, 0.f, stage_size_.y * (1.f - numeric_limits<float>::epsilon())));

	isometric_index.first = static_cast<int>(y_intercept_for_iso_x_idx / tile_size_.y);
	isometric_index.second = static_cast<int>(y_intercept_for_iso_y_idx / tile_size_.y);

	return isometric_index;
}

Stage::Stage(Stage const& _other) : Object(_other)
{
}

Stage::Stage(Stage&& _other) noexcept : Object(move(_other))
{
}

void Stage::_Release()
{
}

bool Stage::_Initialize()
{
	type_ = OBJECT::STAGE;

	return true;
}

void Stage::_Input(float _time)
{
	Object::_Input(_time);

	auto const& camera_view_start = CameraManager::GetSingleton()->position();
	auto const& camera_view_end = Point{ camera_view_start.x + static_cast<float>(RESOLUTION::GAME_WIDTH), camera_view_start.y + static_cast<float>(RESOLUTION::GAME_HEIGHT) };

	auto LT = FindIsometricIndex(camera_view_start);
	auto RT = FindIsometricIndex(camera_view_start + Point{ static_cast<float>(RESOLUTION::GAME_WIDTH) , 0.f });
	auto LB = FindIsometricIndex(camera_view_start + Point{ 0.f, static_cast<float>(RESOLUTION::GAME_HEIGHT) });
	auto RB = FindIsometricIndex(camera_view_end);

	view_range_idx_x_ = { LT.first, RB.first };
	view_range_idx_y_ = { RT.second, LB.second };

	for (auto i = view_range_idx_y_.first; i <= view_range_idx_y_.second; ++i)
		for (auto j = view_range_idx_x_.first; j <= view_range_idx_x_.second; ++j)
			dynamic_pointer_cast<Tile>(tile_collection_.at(i).at(j))->_Input(_time);
}

void Stage::_Update(float _time)
{
	Object::_Update(_time);

	for (auto i = view_range_idx_y_.first; i <= view_range_idx_y_.second; ++i)
		for (auto j = view_range_idx_x_.first; j <= view_range_idx_x_.second; ++j)
			dynamic_pointer_cast<Tile>(tile_collection_.at(i).at(j))->_Update(_time);
}

void Stage::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);

	for (auto i = view_range_idx_y_.first; i <= view_range_idx_y_.second; ++i)
		for (auto j = view_range_idx_x_.first; j <= view_range_idx_x_.second; ++j)
			dynamic_pointer_cast<Tile>(tile_collection_.at(i).at(j))->_LateUpdate(_time);
}

void Stage::_Collision(float _time)
{
	Object::_Collision(_time);

	for (auto i = view_range_idx_y_.first; i <= view_range_idx_y_.second; ++i)
		for (auto j = view_range_idx_x_.first; j <= view_range_idx_x_.second; ++j)
			dynamic_pointer_cast<Tile>(tile_collection_.at(i).at(j))->_Collision(_time);
}

void Stage::_Render(HDC _device_context, float _time)
{
	for (auto i = view_range_idx_y_.first; i <= view_range_idx_y_.second; ++i)
		for (auto j = view_range_idx_x_.first; j <= view_range_idx_x_.second; ++j)
			dynamic_pointer_cast<Tile>(tile_collection_.at(i).at(j))->_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Stage::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Stage{ *this }, [](Object* _p) {
		dynamic_cast<Stage*>(_p)->_Release();
		delete dynamic_cast<Stage*>(_p);
	}};
}