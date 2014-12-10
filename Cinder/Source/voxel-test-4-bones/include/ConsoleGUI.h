#pragma once

#include "NodeRenderable.h"

#include <vector>

#include <cinder\Rect.h>

class ConsoleEntry{
public:
	std::string message;
	enum Type{
		kLOG,
		kWARNING,
		kERROR
	} type;

	ConsoleEntry(std::string _message, Type _type);
};

class ConsoleGUI : public NodeRenderable {
public:
	ConsoleGUI(ci::Rectf _rect, std::vector<ConsoleEntry> * _log);
	~ConsoleGUI();

	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	void resize(ci::Rectf _rect);

	ci::Rectf rect;
	std::vector<ConsoleEntry> * log;
};