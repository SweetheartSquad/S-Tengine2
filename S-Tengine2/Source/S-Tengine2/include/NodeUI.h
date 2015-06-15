#pragma once

#include <NodeBulletBody.h>
#include <MeshEntity.h>

class Scene;
class Mouse;

enum VerticalAlignment{
	kBOTTOM,
	kMIDDLE,
	kTOP
};
enum HorizontalAlignment{
	kLEFT,
	kCENTER,
	kRIGHT
};

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
	
	// destroys the current rigid body and creates a new one which is sized to match the bounding box of the element
	void updateCollider();
	
	bool autoResizingWidth;
	bool autoResizingHeight;
	
	HorizontalAlignment horizontalAlignment;
	VerticalAlignment verticalAlignment;

	Transform * contents;
public:
	Scene * scene;
	MeshEntity * background;

	std::function<void(NodeUI * _this)> onDownFunction;
	//void (*onUpFunction)();

	NodeUI(BulletWorld * _world, Scene * _scene);

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	void setMarginLeft(float _margin);
	void setMarginRight(float _margin);
	void setMarginTop(float _margin);
	void setMarginBottom(float _margin);
	void setMargin(float _all);
	void setMargin(float _leftAndRight, float _bottomAndTop);
	void setMargin(float _left, float _right, float _bottom, float _top);

	void setPaddingLeft(float _padding);
	void setPaddingRight(float _padding);
	void setPaddingTop(float _padding);
	void setPaddingBottom(float _padding);
	void setPadding(float _all);
	void setPadding(float _leftAndRight, float _bottomAndTop);
	void setPadding(float _left, float _right, float _bottom, float _top);

	void setWidth(float _width);
	void setHeight(float _height);

	void setBackgroundColour(float _r, float _g, float _b, float _a = 1.f);

	float getMarginLeft();
	float getMarginRight();
	float getMarginTop();
	float getMarginBottom();

	float getPaddingLeft();
	float getPaddingRight();
	float getPaddingTop();
	float getPaddingBottom();

	// If not autoresizing, returns the fixed width
	// If autoresizing, returns the total width of the UI element including attibutes such as padding, overflow, etc.
	virtual float getWidth(bool _includePadding, bool _includeMargin);
	// If not autoresizing, returns the fixed height
	// If autoresizing, returns the total height of the UI element including attibutes such as padding, overflow, etc.
	virtual float getHeight(bool _includePadding, bool _includeMargin);

	bool isLayoutDirty();
	
	virtual Transform * addChild(NodeUI * _uiElement);
	// removes the element from the contents transform and returns the index it was found at
	// if it wasn't found, returns -1
	// note that the way this works is that _uiElements first parent is removed from the child list,
	// then _uiElement is removed from its first parent and orphaned transform is deleted
	virtual signed long int removeChild(NodeUI * _uiElement);

private:
	float marginLeft;
	float marginRight;
	float marginTop;
	float marginBottom;

	float paddingLeft;
	float paddingRight;
	float paddingTop;
	float paddingBottom;

	float width;
	float height;
	
	void autoResize();
	virtual void autoResizeWidth();
	virtual void autoResizeHeight();
	virtual void repositionChildren();
};