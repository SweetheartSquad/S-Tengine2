#pragma once

#include "NodeSelectable.h"
#include "NodeRenderable.h"
#include "NodeChild.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

#include <string>

class CinderApp;
class ToolSet;
class Step;

class ToolButton : public NodeSelectable, public NodeRenderable, public NodeChild{
public:
	ci::ColorA displayColor;

	enum Type{
		kNORMAL,
		kTOGGLE,
		kRADIO
	} type;
	
	ToolButton(std::string _label, Type _type, ci::Vec2i _iconSize, void (*_downCallback)(CinderApp * _app) = nullptr, void (*_upCallback)(CinderApp * _app) = nullptr);

	// Whether the button is under the mouse
	bool isHovered;
	// Whether the mouse clicked the button (and hasn't been released)
	bool isDown;
	// Whether the button is active (only affects RADIO and TOGGLE buttons)
	bool isActive;

	// Size of icon to render
	ci::Vec2i iconSize;

	// Label describing button's function
	std::string label;
	
	void (*downCallback)(CinderApp * _app);
	void (*upCallback)(CinderApp * _app);

	void down(CinderApp * _app);
	void up(CinderApp * _app);
	void in();
	void out();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void pressProgrammatically(CinderApp * _app);
};