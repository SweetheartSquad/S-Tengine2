#pragma once

#include "ToolBar.h"
#include "ToolSet.h"

#include <cinder\gl\gl.h>

ToolBar::ToolBar(ci::Vec2i _pos) :
	pos(_pos)
{
}

void ToolBar::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::gl::pushMatrices();
		ci::gl::translate(pos);
		ci::Vec2i setPos(0,0);
		for(unsigned long int i = 0; i < toolsets.size(); ++i){
			ci::gl::translate(setPos);

			toolsets.at(i)->render(_matrixStack, _renderStack);
			setPos.y = toolsets.at(i)->buttons.size() * (toolsets.at(i)->iconSize.getHeight()+2) + 5;
		}
	ci::gl::popMatrices();
}

void ToolBar::update(Step * _step){
	for(unsigned long int i = 0; i < toolsets.size(); ++i){
		toolsets.at(i)->update(_step);
	}
}