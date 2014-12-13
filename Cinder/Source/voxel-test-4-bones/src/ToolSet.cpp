#pragma once

#include "ToolSet.h"
#include "ToolButton.h"
#include "ToolBar.h"

#include <cinder\gl\gl.h>

ToolSet::ToolSet() :
	NodeChild(nullptr)
{
}

void ToolSet::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::gl::pushMatrices();
		ci::Vec2i iconPos(0,0);
		for(unsigned long int i = 0; i < children.size(); ++i){
			ToolButton * tb = dynamic_cast<ToolButton *>(children.at(i));
			if(tb != nullptr){
				ci::gl::translate(iconPos);
				tb->render(_matrixStack, _renderStack);
				if(dynamic_cast<ToolBar *>(parent)->vertical){
					iconPos.y = tb->size.y;
				}else{
					iconPos.x = tb->size.x;
				}
			}
		}
	ci::gl::popMatrices();
}

void ToolSet::addButton(ToolButton * _button){
	children.push_back(_button);
	_button->parent = this;
}

ToolSet::~ToolSet(){
	while(children.size() > 0){
		delete children.back();
		children.pop_back();
	}
}

unsigned long int ToolSet::getWidth(){
	int res = 0;
	for(unsigned long int i = 0; i < children.size(); ++i){
		ToolButton * tb = dynamic_cast<ToolButton *>(children.at(i));
		if(tb != nullptr){
			if(dynamic_cast<ToolBar *>(parent)->vertical){
				res = std::max(res, tb->size.x);
			}else{
				res += tb->size.x;
			}
		}
	}
	return res;
}
unsigned long int ToolSet::getHeight(){
	int res = 0;
	for(unsigned long int i = 0; i < children.size(); ++i){
		ToolButton * tb = dynamic_cast<ToolButton *>(children.at(i));
		if(tb != nullptr){
			if(dynamic_cast<ToolBar *>(parent)->vertical){
				res += tb->size.y;
			}else{
				res = std::max(res, tb->size.y);
			}
		}
	}
	return res;
}