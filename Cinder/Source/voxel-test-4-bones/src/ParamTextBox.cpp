#pragma once

#include "ParamTextBox.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>

#include <cinder\Text.h>

ParamTextBox::ParamTextBox(Type _type, ci::Vec2i _pos, ci::Vec2i _size) :
    UiInteractable(_pos, ci::Area(0, 0, _size.x, _size.y)),
	NodeChild(nullptr),
    type(_type)
{
}

void ParamTextBox::downHandler(CinderApp * _app){
	// do nothing
}

void ParamTextBox::upHandler(CinderApp * _app){
	if(isHovered){
		isActive = !isActive;
	}
}

void ParamTextBox::setText(std::string _txt){
	switch (type)
	{
	case ParamTextBox::NORMAL:
		txt = _txt;
		break;
	case ParamTextBox::NUMBER:
		{
		char * n;
		char _tx = * _txt.c_str();
		long converted = strtol(&_tx, &n, 10);
		if(* n){
			// nothing
		}else{
			txt = _txt;
		}
		break;
		}
	default:
		break;
	}
}

void ParamTextBox::setText(ci::app::KeyEvent _k){
	switch (type)
	{
	case ParamTextBox::NORMAL:
		switch (_k.getCode()) {
		case ci::app::KeyEvent::KEY_BACKSPACE:
		case ci::app::KeyEvent::KEY_DELETE:
			txt.pop_back();
			break;
		default:
			txt.push_back(_k.getChar());
			break;
		}
	case ParamTextBox::NUMBER:
		switch (_k.getCode()) {
		case ci::app::KeyEvent::KEY_0:
		case ci::app::KeyEvent::KEY_1:
		case ci::app::KeyEvent::KEY_2:
		case ci::app::KeyEvent::KEY_3:
		case ci::app::KeyEvent::KEY_4:
		case ci::app::KeyEvent::KEY_5:
		case ci::app::KeyEvent::KEY_6:
		case ci::app::KeyEvent::KEY_7:
		case ci::app::KeyEvent::KEY_8:
		case ci::app::KeyEvent::KEY_9:
		default:
			break;
		}
		txt.push_back(_k.getChar());
		break;
	default:
		break;
	}
}

std::string ParamTextBox::getText(){
	return txt;
}

void ParamTextBox::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));
	if(isActive){
		ci::gl::color(0.25,0.25,0.25);

	}else if(isHovered){
		// over
		ci::gl::color(1,1,1);
	}else{
		// up
		ci::gl::color(displayColor);
	}

	ci::gl::pushMatrices();
	ci::gl::translate(pos);

	ci::gl::drawSolidRect(ci::Rectf(0, 0, size.getWidth(), size.getHeight()));

	ci::gl::enableAlphaBlending();
	ci::gl::drawStringCentered(txt, ci::Vec2f(0,0), ci::ColorA(1,1,1,1));
	ci::gl::disableAlphaBlending();

	ci::gl::popMatrices();
}

ParamTextBox::~ParamTextBox()
{
}