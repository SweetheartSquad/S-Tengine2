#pragma once

#include "NodeSelectable.h"
#include "NodeRenderable.h"

#include <cinder\Rect.h>
#include <cinder\Color.h>

class CinderApp;

class UiInteractable : public NodeSelectable, public NodeRenderable {
public:
    ci::Color displayColor;

	UiInteractable(ci::Vec2i _pos, ci::Area _size);
	~UiInteractable();

    ci::Vec2i pos;
	ci::Area size;

	bool isHovered;
	bool isDown;
	bool isActive;

    // The function to be called when you are starting a click
	void down(CinderApp * _app);
    // The fucntion to be called when you complete a click
	void up(CinderApp * _app);
	void in();
	void out();
	//virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override = 0;

	void pressProgrammatically(CinderApp * _app);

    virtual void downHandler(CinderApp * _app) = 0;
    virtual void upHandler(CinderApp * _app) = 0;
};