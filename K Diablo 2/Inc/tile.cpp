#include "stdafx.h"
#include "tile.h"

using namespace std;

pair<int, int> const& Tile::idx() const
{
	return idx_;
}

TILE_OPTION Tile::option() const
{
	return option_;
}

void Tile::set_idx(std::pair<int, int> const& _idx)
{
	idx_ = _idx;
}

void Tile::set_option(TILE_OPTION _option)
{
	option_ = _option;
}

Tile::Tile(Tile const& _other) : Object(_other)
{
	option_ = _other.option_;
	idx_ = _other.idx_;
}

Tile::Tile(Tile&& _other) noexcept : Object(move(_other))
{
	option_ = move(_other.option_);
	idx_ = move(_other.idx_);
}

void Tile::_Release()
{
}

bool Tile::_Initialize()
{
	Object::type_ = OBJECT::TILE;

	return true;
}

void Tile::_Input(float _time)
{
	Object::_Input(_time);
}

void Tile::_Update(float _time)
{
	Object::_Update(_time);
}

void Tile::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Tile::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Tile::_Render(HDC _device_context, float _time)
{
	Object::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Tile::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Tile{ *this }, [](Object* _p) {
		dynamic_cast<Tile*>(_p)->_Release();
		delete dynamic_cast<Tile*>(_p);
	}};
}