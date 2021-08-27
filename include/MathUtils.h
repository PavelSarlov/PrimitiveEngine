#pragma once
#ifndef MATHUTILS_H
#define MATHUTILS_H

static float lerp(float start, float end, float delta)
{
	return start + (end - start) * (delta);
}

#endif // !MATHUTILS_H
