#pragma once

#include "ToolBar.h"
#include "ToolSet.h"

#include <cinder\gl\gl.h>

ToolBar::ToolBar(ci::Vec2i _pos) :
	pos(_pos)
{
}

void ToolBar::render(){
	ci::gl::pushMatrices();
		ci::gl::translate(pos);
		ci::Rectf setRect(0,0,0,0);
		for(unsigned long int i = 0; i < toolsets.size(); ++i){
			setRect.y2 += toolsets.at(i)->buttons.size() * (toolsets.at(i)->iconSize.getHeight()+2) + 5;
			setRect.x2 = std::max(setRect.x2, toolsets.at(i)->iconSize.getWidth());
			toolsets.at(i)->render(setRect);
			setRect.y1 += toolsets.at(i)->buttons.size() * (toolsets.at(i)->iconSize.getHeight()+2) + 5;
		}
	ci::gl::popMatrices();
}