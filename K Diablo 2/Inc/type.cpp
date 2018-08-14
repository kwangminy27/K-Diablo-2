#include "stdafx.h"

using namespace std;
using namespace TYPE;

Point Point::operator+(float _value) const
{
	return Point{ x + _value, y + _value };
}

Point Point::operator+(Point const& _value) const
{
	return Point{ x + _value.x, y + _value.y };
}

void Point::operator+=(float _value)
{
	x += _value;
	y += _value;
}

void Point::operator+=(Point const& _value)
{
	x += _value.x;
	y += _value.y;
}

Point Point::operator-(float _value) const
{
	return Point{ x - _value, y - _value };
}

Point Point::operator-(Point const& _value) const
{
	return Point{ x - _value.x, y - _value.y };
}

void Point::operator-=(float _value)
{
	x -= _value;
	y -= _value;
}

void Point::operator-=(Point const& _value)
{
	x -= _value.x;
	y -= _value.y;
}

Point Point::operator*(float _value) const
{
	return Point{ x * _value, y * _value };
}

Point Point::operator*(Point const& _value) const
{
	return Point{ x * _value.x, y * _value.y };
}

void Point::operator*=(float _value)
{
	x *= _value;
	y *= _value;
}

void Point::operator*=(Point const& _value)
{
	x *= _value.x;
	y *= _value.y;
}

Point Point::operator/(float _value) const
{
	return Point{ x / _value, y / _value };
}

Point Point::operator/(Point const& _value) const
{
	return Point{ x / _value.x, y / _value.y };
}

void Point::operator/=(float _value)
{
	x /= _value;
	y /= _value;
}

void Point::operator/=(Point const& _value)
{
	x /= _value.x;
	y /= _value.y;
}

bool Pixel_8::operator==(Pixel_8 const& _pixel) const
{
	return r == _pixel.r;
}

bool Pixel_24::operator==(Pixel_24 const& _pixel) const
{
	return (r == _pixel.r) && (g == _pixel.g) && (b == _pixel.b);
}

bool Pixel_32::operator==(Pixel_32 const& _pixel) const
{
	return (r == _pixel.r) && (g == _pixel.g) && (b == _pixel.b) && (a == _pixel.a);
}