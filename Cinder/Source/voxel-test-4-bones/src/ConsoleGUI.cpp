#pragma once

#include "ConsoleGUI.h"

#include <cinder\gl\gl.h>
#include <cinder\app\App.h>

ConsoleGUI::ConsoleGUI(float _size, std::vector<ConsoleEntry> * _log, unsigned long int _displayLength) :
	size(_size),
	log(_log),
	displayLength(_displayLength)
{
}

ConsoleGUI::~ConsoleGUI(){

}

void ConsoleGUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	for(unsigned long int i = 1; i <= log->size() && i <= displayLength; ++i){
		ConsoleEntry * l = &log->at(log->size() - i);

		switch (l->type){
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

		ci::gl::drawSolidRect(ci::Rectf(0, ci::app::getWindowHeight()-size*i, ci::app::getWindowWidth(), ci::app::getWindowHeight()-size*(i-1)));
		ci::gl::drawString(l->message, ci::Vec2i(0, ci::app::getWindowHeight()-size*i), ci::ColorA(1.f, 1.f, 1.f, 1.f), ci::Font("Segoe UI", size));
		
	}

	ci::gl::color(1,1,1,1);
	ci::gl::drawStrokedRect(ci::Rectf(0, ci::app::getWindowHeight()-size*displayLength, ci::app::getWindowWidth(), ci::app::getWindowHeight()));
}