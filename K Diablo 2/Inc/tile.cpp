#include "stdafx.h"
#include "tile.h"

using namespace std;

TILE Tile::type() const
{
	return type_;
}

pair<int, int> const& Tile::idx() const
{
	return idx_;
}

void Tile::set_type(TILE _type)
{
	type_ = _type;
}

void Tile::set_idx(std::pair<int, int> const& _idx)
{
	idx_ = _idx;
}

Tile::Tile(Tile const& _other) : Object(_other)
{
}

Tile::Tile(Tile&& _other) noexcept : Object(move(_other))
{
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