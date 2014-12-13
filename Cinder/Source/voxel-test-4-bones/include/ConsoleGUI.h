#pragma once

#include "NodeRenderable.h"
#include "ConsoleEntry.h"


#include <vector>

#include <cinder\Font.h>
#include <cinder\Text.h>

class CinderApp;
class ConsoleGUI : public NodeRenderable {
public:
	ConsoleGUI(float _size, std::vector<ConsoleEntry *> * _log, unsigned long int _displayLength);
	~ConsoleGUI();

	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;

	float size;
	unsigned long int displayLength;
	std::vector<ConsoleEntry *> * log;

	void resize(CinderApp * _app);

	std::vector<ci::TextBox> textboxes;
};