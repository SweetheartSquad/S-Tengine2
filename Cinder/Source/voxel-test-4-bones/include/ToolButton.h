#pragma once

#include "NodeSelectable.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

#include <functional>

class ToolSet;

class ToolButton : public NodeSelectable{
public:
	ci::Color displayColor;

	enum Type{
		NORMAL,
		TOGGLE,
		RADIO
	} type;

	ToolSet * group;

	ToolButton();

	bool hovered;
	bool active;
	
	std::function<void(void)> downCallback;
	std::function<void(void)> upCallback;

	void down();
	void up();
	void in();
	void out();

	void render(ci::Rectf _iconRect);
};