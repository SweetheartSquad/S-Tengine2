#pragma once

#include "NodeSelectable.h"
#include "NodeRenderable.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

#include <functional>

class CinderApp;
class ToolSet;
class Step;

class ToolButton : public NodeSelectable, public NodeRenderable{
public:
	ci::Color displayColor;

	enum Type{
		NORMAL,
		TOGGLE,
		RADIO
	} type;

	ToolSet * group;

	ToolButton(Type _type);

	bool hovered;
	bool active;
	bool justPressed;
	
	std::function<void(CinderApp *)> downCallback;
	std::function<void(CinderApp *)> upCallback;

	void down(CinderApp * _app);
	void up(CinderApp * _app);
	void in();
	void out();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;		
};