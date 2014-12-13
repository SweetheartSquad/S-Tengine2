#pragma once

#include "ToolButton.h"
#include "ToolSet.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>

ToolButton::ToolButton(Type _type, std::string _label, void (*_downCallback)(CinderApp * _app), void (*_upCallback)(CinderApp * _app)):
	UiInteractable(ci::Vec2i(0,0), ci::Area(0,0,0,0)),
	NodeChild(nullptr),
	label(_label),
	type(_type),
	downCallback(_downCallback),
	upCallback(_upCallback)
{

}

void ToolButton::downHandler(CinderApp * _app){
	if(downCallback != nullptr){
		downCallback(_app);
	}
}
void ToolButton::upHandler(CinderApp * _app){
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
			for(unsigned long int i = 0; i < dynamic_cast<ToolSet *>(parent)->children.size(); ++i){
				dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(parent)->children.at(i))->isActive = false;
			}
			isActive = true;
		}
		break;
	default:
		break;
	}

	if(isHovered){
		if(upCallback != nullptr){
			upCallback(_app);
		}
	}
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