#pragma once

#include "NodeRenderable.h"
#include "ConsoleEntry.h"


#include <vector>

#include <cinder\Rect.h>

class ConsoleGUI : public NodeRenderable {
public:
	ConsoleGUI(float _size, std::vector<ConsoleEntry> * _log, unsigned long int _displayLength);
	~ConsoleGUI();

	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	float size;
	unsigned long int displayLength;
	std::vector<ConsoleEntry> * log;
};