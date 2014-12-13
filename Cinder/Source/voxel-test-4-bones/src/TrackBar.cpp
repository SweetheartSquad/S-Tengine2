#pragma once

#include "TrackBar.h"

#include <cinder\gl\gl.h>

#include "CinderRenderOptions.h"
#include "Step.h"
#include "CinderApp.h"
#include "CMD_SetTime.h"
#include "UI.h"


TrackBar::TrackBar(float * _target, ci::Vec2i _pos, ci::Vec2i _barSize, ci::Vec2i _handleSize, float _min, float _max, float _step, ci::Vec2i * _mousePos) :
	UiInteractable(_pos, _barSize),
	handleSize(_handleSize),
	target(_target),
	min(_min),
	max(_max),
	mousePos(_mousePos)
{
	if(min > max){
		// >_>
		throw;
	}else{
		if(*target < min){
			*target = min;
		}else if(*target > max){
			*target = max;
		}
	}
}

void TrackBar::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	ci::Color color;
	// button rendering logic (colour-based atm, replace with textures later)
	if(isDown){
		if(isActive){
			if(isHovered){
				// down + active
				color = ci::Color(0.25f, 0.25f, 0.25f);
			}else{
				// down + active, but moused out (show active)
				color = ci::Color(0.5f, 0.5f, 0.5f);
			}
		}else if(isHovered){
			// down
			color = ci::ColorA(0.25f, 0.25f, 0.25f);
		}else{
			// down, but moused out (show up)
			color = displayColor;
		}
	}else if(isHovered){
		// over
		color = ci::Color(1.f, 1.f, 1.f);
	}else if(isActive){
		// active
		color = ci::Color(0.5f, 0.5f, 0.5f);
	}else{
		// up
		color = displayColor;
	}
	
	ci::gl::color(0.1f, 0.1f, 0.1f);
	ci::gl::drawSolidRect(ci::Rectf(pos.x, pos.y, pos.x+size.x, pos.y+size.y));
	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));
	ci::gl::color(color);
	ci::gl::drawSolidRect(ci::Rectf(pos.x+*target, pos.y, pos.x+*target+handleSize.x, pos.y+handleSize.y));
}


void TrackBar::downHandler(CinderApp * _app){
	oldPos = *mousePos;
}
void TrackBar::upHandler(CinderApp * _app){
	isActive = false;
}

void TrackBar::resize(CinderApp * _app){
	size.x = _app->getWindowWidth();
}

void TrackBar::update(Step * _step){
	if(isDown){
		*target += mousePos->x - oldPos.x;
		oldPos = *mousePos;
	}
	// need to make this a command somehow
}