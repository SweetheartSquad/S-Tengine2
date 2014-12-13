#pragma once

#include "ConsoleGUI.h"

#include <cinder\gl\gl.h>
#include <cinder\gl\Texture.h>
#include "CinderApp.h"

ConsoleGUI::ConsoleGUI(float _size, std::vector<ConsoleEntry *> * _log, unsigned long int _displayLength) :
	size(_size),
	log(_log),
	displayLength(_displayLength)
{
	for(unsigned long int i = 0; i < displayLength; ++i){
		ci::TextBox test;
		test.setSize(ci::Vec2i(ci::app::getWindowWidth(), size));
		test.setAlignment(ci::TextBox::Alignment::LEFT);
		test.setFont(ci::Font("Segoe UI", _size));
		test.setBackgroundColor(ci::ColorA(0.25f, 0.25f, 0.25f, 0.75f));
		test.setColor(ci::ColorA(1.f, 1.f, 1.f, 1.f));
		textboxes.push_back(test);
	}
}

void ConsoleGUI::resize(CinderApp * _app){
	for(unsigned long int i = 0; i < displayLength; ++i){
		textboxes.at(i).setSize(ci::Vec2i(_app->getWindowWidth(), size));
	}
}

ConsoleGUI::~ConsoleGUI(){
	textboxes.clear();
}

void ConsoleGUI::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack){
	unsigned long int i;
	for(i = 1; i <= log->size() && i <= displayLength; ++i){
		ci::gl::pushMatrices();
		ConsoleEntry * l = log->at(log->size() - i);

		switch (l->type){
		case ConsoleEntry::Type::kLOG:
		default:
			textboxes.at(i-1).setColor(ci::ColorA(1.f, 1.f, 1.f, 1.f));
			break;
		case ConsoleEntry::Type::kWARNING:
			textboxes.at(i-1).setColor(ci::ColorA(1.f, 0.5f, 0.f, 1.f));
			break;
		case ConsoleEntry::Type::kERROR:
			textboxes.at(i-1).setColor(ci::ColorA(1.f, 0.f, 0.f, 1.f));
			break;
		}
		textboxes.at(i-1).setText(l->message);

		ci::gl::translate(0, ci::app::getWindowHeight()-size*i);
		ci::gl::draw(ci::gl::Texture(textboxes.at(i-1).render()));
		ci::gl::popMatrices();
	}

	ci::gl::color(1,1,1,1);
}