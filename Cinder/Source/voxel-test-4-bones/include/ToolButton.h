#pragma once

#include "NodeSelectable.h"
#include "NodeRenderable.h"
#include "NodeChild.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

#include <functional>

class CinderApp;
class ToolSet;
class Step;

class ToolButton : public NodeSelectable, public NodeRenderable, public NodeChild{
public:
	ci::Color displayColor;

	enum Type{
		NORMAL,
		TOGGLE,
		RADIO
	} type;
	
	ToolButton(Type _type);

	// Whether the button is under the mouse
	bool isHovered;
	// Whether the mouse clicked the button (and hasn't been released)
	bool isDown;
	// Whether the button is active (only affects RADIO and TOGGLE buttons)
	bool isActive;
	
	std::function<void(CinderApp *)> downCallback;
	std::function<void(CinderApp *)> upCallback;

	void down(CinderApp * _app);
	void up(CinderApp * _app);
	void in();
	void out();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;		
};