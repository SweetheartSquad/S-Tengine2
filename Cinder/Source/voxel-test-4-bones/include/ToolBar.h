#pragma once

#include <vector>
#include <cinder\Rect.h>
#include <cinder\Vector.h>

class ToolSet;

class ToolBar{
public:
	std::vector<ToolSet *> toolsets;

	ci::Vec2i pos;

	ToolBar(ci::Vec2i _pos);

	void render();
};