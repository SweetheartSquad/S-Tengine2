#pragma once

#include <node\NodeMeasurable.h>
#include <NodeBulletBody.h>
#include <MeshEntity.h>
#include <UIUnit.h>
#include <Plane.h>
#include <EventManager.h>

class Scene;
class Mouse;
class Texture;
class FBOTexture;
class StandardFrameBuffer;
class OrthographicCamera;

enum BoxSizing{
	kCONTENT_BOX, // padding and margin are exterior to width and height
	kBORDER_BOX // padding and margin are interior to width and height
};

enum RenderMode{
	kENTITIES,
	kTEXTURE
};

class NodeUI : public NodeBulletBody{
public:
	// manages mouse events (click, mouseout, mousein, mousedown, mouseup, mousewheel)
	sweet::EventManager eventManager;
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

	
	static TriMesh * colliderMesh;
	// destroys the current rigid body and creates a new one which is sized to match the bounding box of the element
	void updateCollider();

	// if the width or height are kAUTO, sets the measured size to the measurement of the children
	// also adjusts the background and calls repositionChildren() to match size
	virtual void autoResize();

	Transform * uiElements;
	
	UIUnit width;
	UIUnit height;

	static ComponentShaderBase * bgShader;
	glm::vec4 bgColour;

	// changes the visiblity and also sets renderFrame to true
	virtual void setVisible(bool _visible) override;

public:
	// if this NodeUI is contained within the standard layout system (i.e. it was added to the hierarchy using NodeUI::addChild)
	// this is a reference to the NodeUI which contains it
	// this is useful because there are multiple transform nodes inbetween children and parents in the NodeUI system
	NodeUI * nodeUIParent;

	// returns mouseEnabled
	bool isMouseEnabled();
	// sets mouseEnabled and creates/deletes the node's collider
	void setMouseEnabled(bool _mouseEnabled);

	Plane * background;

	OrthographicCamera * textureCam;
	StandardFrameBuffer * frameBuffer;
	FBOTexture * renderedTexture;
	MeshEntity * texturedPlane;


	// how padding and margin affect width and height
	BoxSizing boxSizing;

	NodeUI(BulletWorld * _world, RenderMode _renderMode = kENTITIES, bool _mouseEnabled = false);
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
	void setRationalWidth(float _rationalWidth, NodeUI * _parent);
	// enables rational height
	// the dimension will be sized as a ratio of its parent's size
	void setRationalHeight(float _rationalHeight, NodeUI * _parent);
	
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
	
	// the dimension will be sized as a ratio of its opposing unit
	// if _parent is nullptr, only sets the member variable and doesn't change the current size
	void setSquareWidth(float _rationalWidth);
	// the dimension will be sized as a ratio of its opposing unit
	// if _parent is nullptr, only sets the member variable and doesn't change the current size
	void setSquareHeight(float _rationalHeight);

	// recalculates measuredWidth
	virtual void setMeasuredWidths();
	// recalculates measuredHeight
	virtual void setMeasuredHeights();

	// saves the arguments into a member variable
	// these arguments are in the range 0-1, where 1,1,1,1 is fully opaque and white
	// but the stored colour is converted into the correct format for the tint shader
	virtual void setBackgroundColour(float _r, float _g, float _b, float _a = 1.f);

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

	// returns whether the node needs to resize itself or layout its children
	bool isLayoutDirty();
	// indicates to the node that it needs to resize or layout its children because of changes
	// also calls invalidateRenderFrame()
	void invalidateLayout();
	
	// Adds _uiElement in the UI hierarchy for this element
	// i.e. it is placed under a new transform, and then the new transform is placed under this->uiElements, and the new transform is returned
	//
	// if _invalidateLayout is true, the layout is invalidated immediately after the child is added
	// if it is false, you must call invalidateLayout manually in order for the new child to be integrated properly
	virtual Transform * addChild(NodeUI * _uiElement, bool _invalidateLayout = true);

	// removes the element from the contents transform and returns the index it was found at
	// if it wasn't found, returns -1
	// note that the way this works is that _uiElements first parent is removed from the child list,
	// then _uiElement is removed from its first parent and the orphaned transform is deleted
	//
	// if _invalidateLayout is true, the layout is invalidated immediately after the child is added
	// if it is false, you must call invalidateLayout manually in order for the new child to be integrated properly
	virtual signed long int removeChild(NodeUI * _uiElement, bool _invalidateLayout = true);

	virtual void translatePhysical(glm::vec3 _translation, bool _relative = true) override;

	virtual void doRecursivelyOnUIChildren(std::function<void(NodeUI * _childOrThis)> _todo, bool _includeSelf = true);
	
	bool isFirstParentNodeUI();
	
	bool updateState;
	// sets updateState to _newState
	// typically _newState will be the result of a raycast which
	// indicates whether the mouse is currently over this node
	void setUpdateState(bool _newState);


	void setRenderMode(RenderMode _newRenderMode);
private:

	Transform * margin;
	Transform * padding;

	virtual float getContentsWidth();
	virtual float getContentsHeight();
	virtual void repositionChildren();

	void resizeChildrenWidth();
	void resizeChildrenHeight();

	void __renderForEntities(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	void __renderForTexture(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions);
	void __updateForEntities(Step * _step);
	void __updateForTexture(Step * _step);
	

	// Used for renderMode = kTEXTURE
	// indicates whether the texture representation needs to be updated
	// NOTE: do not change or query directly, use invalidateRenderFrame() or isRenderFrameDirty() instead
	bool __renderFrameDirty;
	// indicates whether the node needs to resize itself or layout its children
	// NOTE: do not change or query directly, use invalidateLayout() or isLayoutDirty() instead
	bool __layoutDirty;

	// invalidates the render frame if any NodeUI children in the hierarchy are invalid
	bool __evaluateChildRenderFrames();
protected:
	RenderMode renderMode;
	// returns whether the texture representation needs to be updated
	// if the renderMode is not kTEXTURE, this is always false
	bool isRenderFrameDirty();
	// indicates to the node that it needs to update the texture representation because of changes
	void invalidateRenderFrame();
};