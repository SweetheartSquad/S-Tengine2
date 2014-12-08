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
		for(unsigned long int i = 0; i < children.size(); ++i){
			ci::gl::translate(setPos);

			dynamic_cast<ToolSet *>(children.at(i))->render(_matrixStack, _renderStack);
			setPos.y = dynamic_cast<ToolSet *>(children.at(i))->children.size() * (dynamic_cast<ToolSet *>(children.at(i))->iconSize.getHeight()+2) + 5;
		}
	ci::gl::popMatrices();
}

void ToolBar::addSet(ToolSet * _set){
	children.push_back(_set);
	_set->parent = this;
}

ToolBar::~ToolBar(){
	while(children.size() > 0){
		delete children.at(children.size()-1);
		children.pop_back();
	}
}