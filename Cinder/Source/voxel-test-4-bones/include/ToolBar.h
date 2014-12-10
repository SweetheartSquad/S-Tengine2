#pragma once

#include <vector>
#include <cinder\Rect.h>
#include <cinder\Vector.h>

#include "NodeRenderable.h"
#include "NodeParent.h"

class ToolSet;
class ToolButton;
class Step;

class ToolBar : public NodeRenderable, public NodeParent{
public:
	ci::Vec2i pos;
	bool vertical;
	
	// Adds _set to the list of children
	void addSet(ToolSet * _set);

	// Adds _button to the ToolSet at _setIndex
	void addButton(unsigned long int _setIndex, ToolButton * _button);

	ToolBar(ci::Vec2i _pos, bool _vertical = true);
	~ToolBar();
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;	
};