#pragma once

#include "NodeSelectable.h"
#include "NodeRenderable.h"
#include "NodeChild.h"

#include <string>

#include <cinder\Rect.h>
#include <cinder\Color.h>

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
	
	ToolButton(Type _type, std::string _label, void (*_downCallback)(CinderApp * _app) = nullptr, void (*_upCallback)(CinderApp * _app) = nullptr);

	std::string label;

	// Whether the button is under the mouse
	bool isHovered;
	// Whether the mouse clicked the button (and hasn't been released)
	bool isDown;
	// Whether the button is active (only affects RADIO and TOGGLE buttons)
	bool isActive;
	
    // The function you want to call when you start clicking the button
	void (*downCallback)(CinderApp * _app);
    // The function you want to call when you finish clicking the button (press + release)
	void (*upCallback)(CinderApp * _app);

    // The function to be called when you are starting a click
	void down(CinderApp * _app);
    // The fucntion to be called when you complete a click
	void up(CinderApp * _app);
	void in();
	void out();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void pressProgrammatically(CinderApp * _app);
};