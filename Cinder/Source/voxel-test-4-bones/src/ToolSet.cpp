#pragma once

#include "ToolSet.h"
#include "ToolButton.h"

#include <cinder\gl\gl.h>

ToolSet::ToolSet(ci::Area _iconSize) :
	iconSize(_iconSize),
	NodeChild(nullptr)
{
}

void ToolSet::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::gl::pushMatrices();
		ci::Vec2i iconPos(0,0);
		for(unsigned long int i = 0; i < children.size(); ++i){
			ci::gl::translate(iconPos);
			ci::gl::pushMatrices();
				ci::gl::scale(iconSize.getWidth(), iconSize.getHeight());
				dynamic_cast<ToolButton *>(children.at(i))->render(_matrixStack, _renderStack);
			ci::gl::popMatrices();
			iconPos.y = iconSize.getHeight()+2;
		}
	ci::gl::popMatrices();
}

void ToolSet::addButton(ToolButton * _button){
	children.push_back(_button);
	_button->parent = this;
}

ToolSet::~ToolSet(){
	while(children.size() > 0){
		delete children.at(children.size()-1);
		children.pop_back();
	}
}