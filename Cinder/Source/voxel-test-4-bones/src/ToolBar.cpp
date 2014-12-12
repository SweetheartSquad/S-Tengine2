#pragma once

#include "ToolBar.h"
#include "ToolSet.h"
#include "ToolButton.h"

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
			ToolSet * ts = dynamic_cast<ToolSet *>(children.at(i));
			if(ts != nullptr){
				ci::gl::translate(setPos);
				ts->render(_matrixStack, _renderStack);
				setPos.y = ts->getHeight();
			}
		}
	ci::gl::popMatrices();
}

void ToolBar::addSet(ToolSet * _set){
	children.push_back(_set);
	_set->parent = this;
}

void ToolBar::addButton(unsigned long int _setIndex, ToolButton * _button){
	if(_setIndex < children.size()){
		ToolSet * ts = dynamic_cast<ToolSet *>(children.at(_setIndex));
		ts->addButton(_button);
		_button->parent = ts;
	}else{
		// Error: index out of array bounds
	}
}

ToolBar::~ToolBar(){
	while(children.size() > 0){
		delete children.back();
		children.pop_back();
	}
}