#include "stdafx.h"
#include "physics.h"

float Physics::G()
{
	static float const kG = 9.8f;
	return kG;
}