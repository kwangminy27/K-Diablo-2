#include "stdafx.h"
#include "text.h"

#include "texture.h"

using namespace std;
using namespace TYPE;

void Text::set_string(string const& _string)
{
	string_ = _string;
}

void Text::set_font_size(FONT_SIZE _font_size)
{
	font_size_ = _font_size;

	switch (font_size_)
	{
	/*case FONT_SIZE::_6:
		set_size({ 9.f, 11.f });
		set_texture("font6");
		break;
	case FONT_SIZE::_8:
		set_size({ 15.f, 14.f });
		set_texture("font8");*/
		break;
	case FONT_SIZE::_16:
		set_size({ 14.f, 16.f });
		set_texture("font16");
		break;
	}
}

Text::Text(Text const& _other) : Object(_other)
{
	string_ = _other.string_;
}

Text::Text(Text&& _other) noexcept : Object(move(_other))
{
	string_ = move(_other.string_);
}

void Text::_Release()
{
}

bool Text::_Initialize()
{
	Object::type_ = OBJECT::TEXT;

	set_color_key(RGB(1, 1, 1));

	return true;
}

void Text::_Input(float _time)
{
	Object::_Input(_time);
}

void Text::_Update(float _time)
{
	Object::_Update(_time);
}

void Text::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Text::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Text::_Render(HDC _device_context, float _time)
{
	Point rendering_position = position_;
	int offset_x{};
	int offset_y{};

	for (auto i = 0; i < string_.size(); ++i)
	{
		offset_x += static_cast<int>(size_.x);

		int ascii = string_.at(i);

		if (ascii == '\n')
		{
			offset_x = 0;
			offset_y += static_cast<int>(size_.y);

			continue;
		}

		int idx_x = ascii % static_cast<int>(FONT_FRAME::WIDTH_IDX);
		int idx_y = ascii / static_cast<int>(FONT_FRAME::WIDTH_IDX);

		Point frame_position{ idx_x * size_.x, idx_y * size_.y };

		TransparentBlt(_device_context, static_cast<int>(rendering_position.x + offset_x), static_cast<int>(rendering_position.y + offset_y), static_cast<int>(size_.x), static_cast<int>(size_.y), texture()->memory_device_context(), static_cast<int>(frame_position.x), static_cast<int>(frame_position.y), static_cast<int>(size_.x), static_cast<int>(size_.y), color_key_);

		switch (font_size_)
		{
		case FONT_SIZE::_6:
			break;
		case FONT_SIZE::_8:
			break;
		case FONT_SIZE::_16:
			if (ascii == 'W') {}
			else if (ascii == '0' || ascii == '@' || ascii == 'A' || ascii == 'M' || ascii == 'N' || ascii == 'O' || ascii == 'Q'
				|| ascii == 'R' || ascii == 'T' || ascii == 'U' || ascii == 'V' || ascii == 'X' || ascii == 'Y' || ascii == 'Z'
				|| ascii == 'a' || ascii == 'm' || ascii == 'o' || ascii == 'q' || ascii == 'u' || ascii == 'v' || ascii == 'w'
				|| ascii == 'x' || ascii == 'y')
				offset_x -= 4;
			else
				offset_x -= 7;

			break;
		}
	}
}

unique_ptr<Object, function<void(Object*)>> Text::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{ new Text(*this), [](Object* _p) {
		dynamic_cast<Text*>(_p)->_Release();
		delete dynamic_cast<Text*>(_p);
	} };
}