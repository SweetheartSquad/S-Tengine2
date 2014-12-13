#pragma once

#include "UiInteractable.h"
#include "NodeChild.h"

#include <string>

#include <cinder\Rect.h>
#include <cinder\Color.h>
#include <cinder\app\KeyEvent.h>

class CinderApp;

class ParamTextBox : public UiInteractable, public NodeChild
{
public:
    enum Type{
		NORMAL,
		NUMBER
	} type;

	ParamTextBox(Type _type, ci::Vec2i _pos, ci::Vec2i _size);
	~ParamTextBox();

	void setText(std::string _txt);
	void setText(ci::app::KeyEvent _k);
	std::string getText();

    // The function to be called when you are starting a click
	void downHandler(CinderApp * _app);
    // The fucntion to be called when you complete a click
	void upHandler(CinderApp * _app);

	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
private:
	std::string txt;

};