#pragma once

#include "ToolSet.h"
#include "ToolButton.h"

#include <cinder\gl\gl.h>

ToolSet::ToolSet(ci::Rectf _iconSize) :
	iconSize(_iconSize)
{
}

void ToolSet::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::gl::pushMatrices();
		ci::Vec2i iconPos(0,0);
		for(unsigned long int i = 0; i < buttons.size(); ++i){
			ci::gl::translate(iconPos);
			ci::gl::pushMatrices();
				ci::gl::scale(iconSize.getWidth(), iconSize.getHeight());
				buttons.at(i)->render(_matrixStack, _renderStack);
			ci::gl::popMatrices();
			iconPos.y = iconSize.getHeight()+2;
		}
	ci::gl::popMatrices();
}

void ToolSet::update(Step * _step){
	for(unsigned long int i = 0; i < buttons.size(); ++i){
		buttons.at(i)->update(_step);
	}
}

void ToolSet::addButton(ToolButton * _button){
	buttons.push_back(_button);
	_button->group = this;
}