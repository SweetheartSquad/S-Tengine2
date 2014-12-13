#pragma once

#include "ParamTextBox.h"

#include "CinderRenderOptions.h"

#include <cinder\gl\gl.h>
#include <cinder\gl\Texture.h>

ParamTextBox::ParamTextBox(Type _type, ci::Vec2i _pos, ci::Vec2i _size) :
    UiInteractable(_pos, ci::Area(0, 0, _size.x, _size.y)),
	NodeChild(nullptr),
    type(_type)
{
    textbox.setText("");
	textbox.setSize(ci::Vec2i(size.getWidth(), size.getHeight()));
	textbox.setAlignment(ci::TextBox::Alignment::LEFT);
	textbox.setFont(ci::Font("Segoe UI", 15));
	textbox.setColor(ci::ColorA(0.f, 0.f, 0.f, 1.f));
}

void ParamTextBox::downHandler(CinderApp * _app){
	// do nothing
}

void ParamTextBox::upHandler(CinderApp * _app){
	isActive = isHovered;
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
    textbox.setText(txt);
}

void ParamTextBox::setText(ci::app::KeyEvent _k){
	if(_k.getCode() == ci::app::KeyEvent::KEY_BACKSPACE || _k.getCode() == ci::app::KeyEvent::KEY_DELETE){
        if(txt.size() > 0){
		    txt.pop_back();
		}
	}else{
	    switch (type)
	    {
	    case ParamTextBox::NORMAL:
			txt.push_back(_k.getChar());
			break;
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
    textbox.setText(txt);
}

std::string ParamTextBox::getText(){
	return txt;
}

void ParamTextBox::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
    ci::gl::pushMatrices();
	ci::gl::translate(pos);

	((CinderRenderOptions *)_renderStack)->ciShader->uniform("pickingColor", ci::Color::hex(pickingColor));
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

    ci::gl::popMatrices();
}

ParamTextBox::~ParamTextBox()
{
}