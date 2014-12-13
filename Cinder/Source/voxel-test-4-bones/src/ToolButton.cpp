#pragma once

#include "ToolButton.h"
#include "ToolSet.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>
#include <cinder\gl\Texture.h>

ToolButton::ToolButton(std::string _label, Type _type, ci::Vec2i _iconSize, void (*_downCallback)(CinderApp * _app), void (*_upCallback)(CinderApp * _app)):
	NodeChild(nullptr),
	NodeSelectable(),
	isHovered(false),
	isDown(false),
	isActive(false),
	displayColor((float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, (float)(std::rand()%255)/255.f, 1.f),
	type(_type),
	downCallback(_downCallback),
	upCallback(_upCallback)
{
	textbox.setText(_label);
	textbox.setSize(_iconSize);
	textbox.setAlignment(ci::TextBox::Alignment::LEFT);
	textbox.setFont(ci::Font("Segoe UI", 15));
	textbox.setColor(ci::ColorA(0.f, 0.f, 0.f, 1.f));
}

void ToolButton::down(CinderApp * _app){
	isHovered = true;
	isDown = true;
	if(downCallback != nullptr){
		downCallback(_app);
	}
}
void ToolButton::up(CinderApp * _app){
	isDown = false;
	switch (type){
	case ToolButton::kNORMAL:
		isActive = false;
		break;
	case ToolButton::kTOGGLE:
		if(isHovered){
			isActive = !isActive;
		}
		break;
	case ToolButton::kRADIO:
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
void ToolButton::in(){
	isHovered = true;
}
void ToolButton::out(){
	isHovered = false;
}

void ToolButton::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	// button rendering logic (colour-based atm, replace with textures later)
	if(isDown){
		if(isActive){
			if(isHovered){
				// down + active
				textbox.setBackgroundColor(ci::ColorA(0.25f, 0.25f, 0.25f, 1.f));
			}else{
				// down + active, but moused out (show active)
				textbox.setBackgroundColor(ci::ColorA(0.5f, 0.5f, 0.5f, 1.f));
			}
		}else if(isHovered){
			// down
			textbox.setBackgroundColor(ci::ColorA(0.25f, 0.25f, 0.25f, 1.f));
		}else{
			// down, but moused out (show up)
			textbox.setBackgroundColor(displayColor);
		}
	}else if(isHovered){
		// over
		textbox.setBackgroundColor(ci::ColorA(1.f, 1.f, 1.f, 1.f));
	}else if(isActive){
		// active
		textbox.setBackgroundColor(ci::ColorA(0.5f, 0.5f, 0.5f, 1.f));
	}else{
		// up
		textbox.setBackgroundColor(displayColor);
	}

	
	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));
	ci::gl::color(1.f, 1.f, 1.f, 1.f);
	ci::gl::draw(ci::gl::Texture(textbox.render()));
}

void ToolButton::pressProgrammatically(CinderApp * _app){
	in();
	down(_app);
	up(_app);
	out();
}