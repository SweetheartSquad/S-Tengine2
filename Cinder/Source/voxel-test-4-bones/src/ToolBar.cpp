#pragma once

#include "ToolBar.h"
#include "ToolSet.h"
#include "ToolButton.h"

#include <cinder\gl\gl.h>

ToolBar::ToolBar(ci::Vec2i _pos, bool _vertical) :
	pos(_pos),
	vertical(_vertical)
{
}

void ToolBar::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::gl::pushMatrices();
		ci::gl::translate(pos);
		ci::Vec2i setPos(0,0);
		for(unsigned long int i = 0; i < children.size(); ++i){
			ci::gl::translate(setPos);

			dynamic_cast<ToolSet *>(children.at(i))->render(_matrixStack, _renderStack);
			if(vertical){
				setPos.y = dynamic_cast<ToolSet *>(children.at(i))->children.size() * (dynamic_cast<ToolSet *>(children.at(i))->iconSize.getHeight()+2) + 5;
			}else{
				setPos.x = dynamic_cast<ToolSet *>(children.at(i))->children.size() * (dynamic_cast<ToolSet *>(children.at(i))->iconSize.getWidth()+2) + 5;
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