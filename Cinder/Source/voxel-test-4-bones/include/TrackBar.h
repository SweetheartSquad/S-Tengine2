#pragma once

#include "UiInteractable.h"
#include "NodeUpdatable.h"

class Step;

class TrackBar : public UiInteractable, public virtual NodeUpdatable{
public:
	float min, max, step;
	float * target;
	ci::Vec2i handleSize;

    // The function to be called when you are starting a click
	void downHandler(CinderApp * _app);
    // The fucntion to be called when you complete a click
	void upHandler(CinderApp * _app);

	void update(Step * _step) override;
	void resize(CinderApp * _app);

	TrackBar(float * _target, ci::Vec2i _pos, ci::Vec2i _barSize, ci::Vec2i _handleSize, float _min, float _max, float _step, ci::Vec2i * _mousePos);
	void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderStack) override;
private:
	ci::Vec2i * mousePos;
	ci::Vec2i oldPos;
};