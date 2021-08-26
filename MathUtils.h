#pragma once
#ifndef MATHUTILS_H
#define MATHUTILS_H

float lerp(float start, float end, float delta)
{
	return start * (1.0f - delta) + end * (delta);
}

#endif // !MATHUTILS_H
