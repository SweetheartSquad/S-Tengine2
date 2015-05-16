#pragma once

#include <NodeBulletBody.h>
#include <Entity.h>

class Scene;
class Mouse;

class NodeUI : public virtual NodeBulletBody, public virtual Entity{
private:
	Mouse * mouse;
	// Whether the button is under the mouse
	bool isHovered;
	// Whether the mouse clicked the button (and hasn't been released)
	bool isDown;
	// Whether the button is active (only affects RADIO and TOGGLE buttons)
	bool isActive;
	
    // The function to be called when you are starting a click
	void down();
    // The function to be called when you complete a click
	void up();
    // The function to be called when you mouseover
	void in();
    // The function to be called when you mouseout
	void out();
public:
	Scene * scene;

	NodeUI(BulletWorld * _world, Scene * _scene);

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	// renders the UI element in it's "over" state
	virtual void renderOver(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	// renders the UI element in it's "down" state
	virtual void renderDown(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	// renders the UI element in it's "mousedown" state
	virtual void renderActive(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	// called by the functions to render each individual state by default (so if you leave a state unimplemented, this will render instead)
	virtual void renderDefault(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
};