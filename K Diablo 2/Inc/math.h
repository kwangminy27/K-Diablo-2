#pragma once

class Math
{
public:
	static float PI();
	static float GetDistance(TYPE::Point const& _src, TYPE::Point const& _dest);
	static float GetAngle(TYPE::Point const& _src, TYPE::Point const& _dest);
	static float ConvertToDegrees(float _radians);
	static float ConvertToRadians(float _degrees);
};