#pragma once

#include <node\NodeMeasurable.h>
#include <NodeBulletBody.h>
#include <MeshEntity.h>
#include <UIUnit.h>

class Scene;
class Mouse;
enum BoxSizing{
	kCONTENT_BOX, // padding and margin are exterior to width and height
	kBORDER_BOX // padding and margin are interior to width and height
};

class NodeUI : public virtual NodeBulletBody, public virtual Entity{
protected:
	UIUnit marginLeft;
	UIUnit marginRight;
	UIUnit marginBottom;
	UIUnit marginTop;
	
	UIUnit paddingLeft;
	UIUnit paddingRight;
	UIUnit paddingBottom;
	UIUnit paddingTop;

	Mouse * mouse;
	// Whether the button is under the mouse
	bool isHovered;
	// Whether the mouse clicked the button (and hasn't been released)
	bool isDown;
	// Whether the button is active (only affects RADIO and TOGGLE buttons)
	bool isActive;
	
    // The function to be called when you are starting a click
	virtual void down();
    // The function to be called when you complete a click
	virtual void up();
    // The function to be called when you mouseover
	virtual void in();
    // The function to be called when you mouseout
	virtual void out();

	bool layoutDirty;
	
	static TriMesh * colliderMesh;
	// destroys the current rigid body and creates a new one which is sized to match the bounding box of the element
	void updateCollider();

	// if the width or height are kAUTO, sets the measured size to the measurement of the children
	// also adjusts the background and calls repositionChildren() to match size
	void autoResize();

	Transform * contents;
	
	UIUnit width;
	UIUnit height;

	static ComponentShaderBase * bgShader;
	glm::vec4 bgColour;

public:
	bool mouseEnabled;
	Scene * scene;
	MeshEntity * background;

	// how padding and margin affect width and height
	BoxSizing boxSizing;

	std::function<void(NodeUI * _this)> onClickFunction;
	//void (*onUpFunction)();

	NodeUI(BulletWorld * _world, Scene * _scene);
	
	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
	virtual void load() override;
	virtual void unload() override;

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

	// sets the width
	// value < 0: the dimension will be auto-resized
	// 0 <= value <= 1: the dimension will be sized as a ratio of its parent's size
	// value > 1: the dimension will be sized in units (typically screen pixels)
	void setWidth(float _width);
	// sets the height
	// value < 0: the dimension will be auto-resized
	// 0 <= value <= 1: the dimension will be sized as a ratio of its parent's size
	// value > 1: the dimension will be sized in units (typically screen pixels)
	void setHeight(float _height);

	// enables rational width
	// the dimension will be sized as a ratio of its parent's size
	// if _parent is nullptr, only sets the member variable and doesn't change the current size
	void setRationalWidth(float _rationalWidth, NodeUI * _parent = nullptr);
	// enables rational height
	// the dimension will be sized as a ratio of its parent's size
	// if _parent is nullptr, only sets the member variable and doesn't change the current size
	void setRationalHeight(float _rationalHeight, NodeUI * _parent = nullptr);
	
	// enables pixel width
	// the dimension will be sized in pixels
	void setPixelWidth(float _pixelWidth);
	// enables pixel height
	// the dimension will be sized in pixels
	void setPixelHeight(float _pixelHeight);
	
	// enables autoresizing width
	// the dimension will be sized as a ratio of its parent's size
	void setAutoresizeWidth();
	// enables autoresizing height
	// the dimension will be sized as a ratio of its parent's size
	void setAutoresizeHeight();

	// recalculates measuredWidth using _root as the basis for rational sizes
	void setMeasuredWidths(NodeUI * _root);
	// recalculates measuredHeight using _root as the basis for rational sizes
	void setMeasuredHeights(NodeUI * _root);

	void setBackgroundColour(float _r, float _g, float _b, float _a = 1.f);

	float getMarginLeft();
	float getMarginRight();
	float getMarginTop();
	float getMarginBottom();

	float getPaddingLeft();
	float getPaddingRight();
	float getPaddingTop();
	float getPaddingBottom();
	
	// If kPIXEL, returns the pixelWidth
	// otherwise, returns measuredWidth
	virtual float getWidth();
	// If kPIXEL, returns the pixelHeight
	// otherwise, returns measuredHeight
	virtual float getHeight();

	// returns the width of the UI element, optionally including the padding and margin
	virtual float getWidth(bool _includePadding, bool _includeMargin);
	// returns the height of the UI element, optionally including the padding and margin
	virtual float getHeight(bool _includePadding, bool _includeMargin);

	bool isLayoutDirty();
	
	virtual Transform * addChild(NodeUI * _uiElement);
	// removes the element from the contents transform and returns the index it was found at
	// if it wasn't found, returns -1
	// note that the way this works is that _uiElements first parent is removed from the child list,
	// then _uiElement is removed from its first parent and the orphaned transform is deleted
	virtual signed long int removeChild(NodeUI * _uiElement);

private:
	virtual float getContentsWidth();
	virtual float getContentsHeight();
	virtual void repositionChildren();

	// check for rational-width children and resize them based on _root
	void resizeChildrenWidth(NodeUI * _root);
	// check for rational-height children and resize them based on _root
	void resizeChildrenHeight(NodeUI * _root);
};