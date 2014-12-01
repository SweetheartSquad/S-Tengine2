#pragma once

#include "ToolButton.h"
#include "ToolSet.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>

ToolButton::ToolButton(Type _type):
	NodeSelectable(),
	hovered(false),
	active(false),
	justPressed(false),
	displayColor((float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f),
	type(_type)
{

}

void ToolButton::down(CinderApp * _app){
	hovered = true;
	justPressed = true;
	switch (type){
	case ToolButton::NORMAL:
		active = true;
		break;
	case ToolButton::TOGGLE:
		active = !active;
		break;
	case ToolButton::RADIO:
		active = true;
		break;
	default:
		break;
	}
	if(!downCallback._Empty()){
		downCallback(_app);
	}
}
void ToolButton::up(CinderApp * _app){
	if(type == NORMAL){
		active = false;
	}else if(type == RADIO){
		if(hovered){
			// need to unclick sibling
			for(unsigned long int i = 0; i < group->buttons.size(); ++i){
				group->buttons.at(i)->active = false;
			}
			active = true;
		}
	}
	if(hovered){
		if(!upCallback._Empty()){
			upCallback(_app);
		}
	}
}
void ToolButton::in(){
	hovered = true;
}
void ToolButton::out(){
	hovered = false;
}

void ToolButton::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));
	ci::gl::color(active && hovered ? displayColor-ci::Color(0.2f,0.2f,0.2f) : displayColor);
	ci::gl::drawSolidRect(ci::Rectf(0,0,1,1));
}