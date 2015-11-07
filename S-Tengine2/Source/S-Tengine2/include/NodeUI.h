#pragma once

#include <node\NodeMeasurable.h>
#include <NodeBulletBody.h>
#include <MeshEntity.h>
#include <UIUnit.h>

class Scene;
class Mouse;
class Texture;
class StandardFrameBuffer;

enum BoxSizing{
	kCONTENT_BOX, // padding and margin are exterior to width and height
	kBORDER_BOX // padding and margin are interior to width and height
};

enum RenderMode {
	kENTITIES,
	kTEXTURE
};

class NodeUI : public NodeBulletBody{
protected:
	bool mouseEnabled;
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
	
    // The function to be called when you press the button down
	virtual void down();
    // The function to be called when you release the button
	virtual void up();
    // The function to be called when you release the button while still hovered over it
	virtual void click();
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

	Transform * uiElements;
	
	UIUnit width;
	UIUnit height;

	static ComponentShaderBase * bgShader;
	glm::vec4 bgColour;

public:
	// returns mouseEnabled
	bool isMouseEnabled();
	// sets mouseEnabled and creates/deletes the node's collider
	void setMouseEnabled(bool _mouseEnabled);

	Scene * scene;
	MeshEntity * background;

	StandardFrameBuffer * frameBuffer;
	Texture * renderedTexture;
	MeshEntity * texturedPlane;

	RenderMode renderMode;

	// how padding and margin affect width and height
	BoxSizing boxSizing;

	std::function<void(NodeUI * _this)> onClickFunction;
	//void (*onUpFunction)();

	NodeUI(BulletWorld * _world, Scene * _scene, RenderMode _renderMode = kENTITIES, bool _mouseEnabled = false);
	~NodeUI();
	
	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption) override;
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

	// saves the arguments into a member variable
	// these arguments are in the range 0-1, where 1,1,1,1 is fully opaque and white
	// but the stored colour is converted into the correct format for the tint shader
	void setBackgroundColour(float _r, float _g, float _b, float _a = 1.f);

	float getMarginLeft();
	float getMarginRight();
	float getMarginTop();
	float getMarginBottom();

	float getPaddingLeft();
	float getPaddingRight();
	float getPaddingTop();
	float getPaddingBottom();

	virtual Texture * renderToTexture();
	virtual MeshEntity * getTexturedPlane();
	
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
	void makeLayoutDirty();
	
	virtual Transform * addChild(NodeUI * _uiElement);
	// removes the element from the contents transform and returns the index it was found at
	// if it wasn't found, returns -1
	// note that the way this works is that _uiElements first parent is removed from the child list,
	// then _uiElement is removed from its first parent and the orphaned transform is deleted
	virtual signed long int removeChild(NodeUI * _uiElement);

	virtual void setTranslationPhysical(float _x, float _y, float _z, bool _relative = false) override;

	virtual void doRecursivleyOnUIChildren(std::function<void(NodeUI * _childOrThis)> _todo, bool _includeSelf = true);
	
	bool isFirstParentNodeUI();
	
	bool updateState;
	// sets updateState to _newState
	// typically _newState will be the result of a raycast which
	// indicates whether the mouse is currently over this node
	void setUpdateState(bool _newState);

	// Used for renderMode = Texture to carry whether the layout is dirty between update and render
	bool renderFrame;
private:

	Transform * margin;
	Transform * padding;

	virtual float getContentsWidth();
	virtual float getContentsHeight();
	virtual void repositionChildren();

	// check for rational-width children and resize them based on _root
	void resizeChildrenWidth(NodeUI * _root);
	// check for rational-height children and resize them based on _root
	void resizeChildrenHeight(NodeUI * _root);

	bool hasRenderModeParent(RenderMode _renderMode);

	void __renderForEntities(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	void __renderForTexture(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	void __updateForEntities(Step * _step);
	void __updateForTexture(Step * _step);
};