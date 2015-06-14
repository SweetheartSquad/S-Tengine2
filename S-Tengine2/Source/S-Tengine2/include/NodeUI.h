#pragma once

#include <NodeBulletBody.h>
#include <MeshEntity.h>

class Scene;
class Mouse;


enum VerticalAlignment{
	kBOTTOM,
	kCENTER,
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

	MeshEntity * background;
	Transform * contents;
public:
	Scene * scene;

	std::function<void(NodeUI * _this)> onDownFunction;
	//void (*onUpFunction)();

	NodeUI(BulletWorld * _world, Scene * _scene);

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	void setMarginLeft(float _margin);
	void setMarginRight(float _margin);
	void setMarginTop(float _margin);
	void setMarginBottom(float _margin);

	void setPaddingLeft(float _padding);
	void setPaddingRight(float _padding);
	void setPaddingTop(float _padding);
	void setPaddingBottom(float _padding);

	void setWidth(float _width);
	void setHeight(float _height);

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
	virtual float getWidth(bool _includePadding, bool _includeMargin) = 0;
	// If not autoresizing, returns the fixed height
	// If autoresizing, returns the total height of the UI element including attibutes such as padding, overflow, etc.
	virtual float getHeight(bool _includePadding, bool _includeMargin) = 0;

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

	float width;
	float height;

	void autoResize();
};