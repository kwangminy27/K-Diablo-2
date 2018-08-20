#include "stdafx.h"
#include "math.h"

using namespace TYPE;

float Math::PI()
{
	static float const kPI = 3.141592654f;
	return kPI;
}

float Math::GetDistance(Point const& _src, Point const& _dest)
{
	float width = _dest.x - _src.x;
	float height = _dest.y - _src.y;
	return sqrt((width * width) + (height * height));
}

float Math::GetAngle(Point const& _src, Point const& _dest)
{
	float width = _dest.x - _src.x;
	float diagonal = Math::GetDistance(_src, _dest);
	float cos_theta = width / diagonal;
	float theta = Math::ConvertToDegrees(acos(cos_theta));

	return _dest.y - _src.y < 0.f ? 360.f - theta : theta;
}

float Math::ConvertToDegrees(float _radians)
{
	return _radians * (180.f / Math::PI());
}

float Math::ConvertToRadians(float _degrees)
{
	return _degrees * (Math::PI() / 180.f);
}