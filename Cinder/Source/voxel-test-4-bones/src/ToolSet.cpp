#pragma once

#include "ToolSet.h"
#include "ToolButton.h"

#include <cinder\gl\gl.h>

ToolSet::ToolSet(ci::Rectf _iconSize) :
	iconSize(_iconSize)
{
}

void ToolSet::render(ci::Rectf _setRect){
	ci::gl::translate(_setRect.x1, _setRect.y1);
	ci::Rectf iconRect(0,0,iconSize.getWidth(),iconSize.getHeight());
	for(unsigned long int i = 0; i < buttons.size(); ++i){
		buttons.at(i)->render(iconRect);
		iconRect.y1 += iconSize.getHeight()+2;
		iconRect.y2 += iconSize.getHeight()+2;
	}
}