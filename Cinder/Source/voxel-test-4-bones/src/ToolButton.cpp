#pragma once

#include "ToolButton.h"
#include "ToolSet.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>

ToolButton::ToolButton(Type _type):
	NodeSelectable(),
	isHovered(false),
	isDown(false),
	isActive(false),
	displayColor((float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f),
	type(_type)
{

}

void ToolButton::down(CinderApp * _app){
	isHovered = true;
	isDown = true;
	if(!downCallback._Empty()){
		downCallback(_app);
	}
}
void ToolButton::up(CinderApp * _app){
	isDown = false;
	switch (type){
	case ToolButton::NORMAL:
		isActive = false;
		break;
	case ToolButton::TOGGLE:
		if(isHovered){
			isActive = !isActive;
		}
		break;
	case ToolButton::RADIO:
		if(isHovered){
			// need to unclick sibling
			for(unsigned long int i = 0; i < group->buttons.size(); ++i){
				group->buttons.at(i)->isActive = false;
			}
			isActive = true;
		}
		break;
	default:
		break;
	}

	if(isHovered){
		if(!upCallback._Empty()){
			upCallback(_app);
		}
	}
}
void ToolButton::in(){
	isHovered = true;
}
void ToolButton::out(){
	isHovered = false;
}

void ToolButton::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));

	// button rendering logic (colour-based atm, replace with textures later)
	if(isDown){
		if(isActive){
			if(isHovered){
				// down + active
				ci::gl::color(0.25,0.25,0.25);
			}else{
				// down + active, but moused out (show active)
				ci::gl::color(0.5,0.5,0.5);
			}
		}else if(isHovered){
			// down
			ci::gl::color(0.25,0.25,0.25);
		}else{
			// down, but moused out (show up)
			ci::gl::color(displayColor);
		}
	}else if(isHovered){
		// over
		ci::gl::color(1,1,1);
	}else if(isActive){
		// active
		ci::gl::color(0.5,0.5,0.5);
	}else{
		// up
		ci::gl::color(displayColor);
	}

	ci::gl::drawSolidRect(ci::Rectf(0,0,1,1));
}