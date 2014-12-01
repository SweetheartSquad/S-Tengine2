#pragma once

#include <vector>
#include <cinder\Rect.h>

class ToolButton;

class ToolSet{
public:
	ToolSet(ci::Rectf _iconSize);

	ci::Rectf iconSize;

	std::vector<ToolButton *> buttons;

	void render(ci::Rectf _setRect);
};