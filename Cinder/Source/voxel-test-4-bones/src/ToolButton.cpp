#pragma once

#include "ToolButton.h"
#include "ToolSet.h"

#include <cinder\gl\gl.h>

ToolButton::ToolButton():
	NodeSelectable(),
	hovered(false),
	active(false),
	displayColor((float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f)
{

}

void ToolButton::down(){
	switch (type){
	case ToolButton::NORMAL:
		active = true;
		break;
	case ToolButton::TOGGLE:
		active = !active;
		break;
	case ToolButton::RADIO:
		// need to unclick siblings
		for(unsigned long int i = 0; i < group->buttons.size(); ++i){
			group->buttons.at(i)->active = false;
		}
		active = true;
		break;
	default:
		break;
	}

	downCallback();
}
void ToolButton::up(){
	active = false;
	if(hovered){
		upCallback();
	}
}
void ToolButton::in(){
	hovered = true;
}
void ToolButton::out(){
	hovered = false;
}

void ToolButton::render(ci::Rectf _iconRect){
	ci::gl::color(ci::Color::hex(pickingColor));
	ci::gl::drawSolidRect(_iconRect);
	ci::gl::color(displayColor);
	ci::gl::drawStrokedRect(_iconRect);
}