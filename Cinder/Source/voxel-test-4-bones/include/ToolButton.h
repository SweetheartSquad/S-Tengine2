#pragma once

#include "NodeSelectable.h"
#include "NodeUpdatable.h"
#include "NodeRenderable.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

#include <functional>

class ToolSet;
class Step;

class ToolButton : public NodeSelectable, public NodeRenderable, public NodeUpdatable{
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
	
	std::function<void(void)> downCallback;
	std::function<void(void)> upCallback;

	void down();
	void up();
	void in();
	void out();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
	void update(Step * _step) override;			
};