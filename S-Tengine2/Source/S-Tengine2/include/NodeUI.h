#pragma once

#include <NodeBulletBody.h>
#include <Entity.h>

class Scene;
class Mouse;

class NodeUI : public virtual NodeBulletBody, public virtual Entity{
protected:
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

	bool layoutDirty;
	
	void updateColider();
public:
	Scene * scene;

	void (*onDownFunction)(NodeUI * _this);
	//void (*onUpFunction)();

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
	// Should return the total width of the UI elemnent including attibutes such as padding, overflow, etc
	virtual float getMeasuredWidth() = 0;
	// Should return the total width of the UI elemnent including attibutes such as padding, overflow, etc
	virtual float getMeasuredHeight() = 0;

	void setMarginLeft(float _margin);
	void setMarginRight(float _margin);
	void setMarginTop(float _margin);
	void setMarginBottom(float _margin);

	void setPaddingLeft(float _padding);
	void setPaddingRight(float _padding);
	void setPaddingTop(float _padding);
	void setPaddingBottom(float _padding);

	float getMarginLeft();
	float getMarginRight();
	float getMarginTop();
	float getMarginBottom();

	float getPaddingLeft();
	float getPaddingRight();
	float getPaddingTop();
	float getPaddingBottom();

	bool isLayoutDirty();

private:
	float marginLeft;
	float marginRight;
	float marginTop;
	float marginBottom;

	float paddingLeft;
	float paddingRight;
	float paddingTop;
	float paddingBottom;
};