#pragma once

#include "ConsoleGUI.h"

#include <cinder\gl\gl.h>

ConsoleEntry::ConsoleEntry(std::string _message, Type _type) :
	message(_message),
	type(_type)
{
}

ConsoleGUI::ConsoleGUI(ci::Rectf _rect, std::vector<ConsoleEntry> * _log) :
	rect(_rect),
	log(_log)
{
}

ConsoleGUI::~ConsoleGUI(){

}

void ConsoleGUI::resize(ci::Rectf _rect){
	rect = _rect;
}

void ConsoleGUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	if(log->size() > 0){
		switch (log->back().type){
		case ConsoleEntry::Type::kLOG:
		default:
			ci::gl::color(0.f, 0.f, 0.f, 0.5f);
			break;
		case ConsoleEntry::Type::kWARNING:
			ci::gl::color(1.f, 1.f, 0.f, 0.5f);
			break;
		case ConsoleEntry::Type::kERROR:
			ci::gl::color(1.f, 0.f, 0.f, 0.5f);
			break;
		}
		ci::gl::drawSolidRect(rect);
		ci::gl::color(1.f, 1.f, 1.f);
		ci::gl::drawStrokedRect(rect);
		ci::gl::drawString(log->back().message, ci::Vec2i(rect.x1, rect.y1), ci::ColorA(1.f, 1.f, 1.f, 1.f), ci::Font("Segoe UI", 15));
	}
}