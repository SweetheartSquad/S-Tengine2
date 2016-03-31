#pragma once

#include <NodeUI.h>
#include <Texture.h>


class Texture_NineSliced;

// An individual patch in a nine-sliced texture
class Patch : public Texture{
public:
	// root texture for the patch
	Texture_NineSliced * root;
	// patch start and end
	unsigned long int x1, x2, y1, y2;

	Patch(Texture_NineSliced * _root);
	
	virtual void loadImageData() override;

	void resize(unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2);
};

/*
The format for a nine-sliced texture is based on Android's NinePatch system
The first row and column of the texture is reserved for markup (the last channel, typically alpha, is treated as the input)
The stretchable region is defined by a value of 255; anything else is the border region

NOTE: Unlike Android's NinePatch, there's only one stretchable region (first one found is used), and there's no "padding" defined by the right/bottom edges
*/
class Texture_NineSliced : public Texture{
private:
	// slice coordinates (first opaque pixel and first transparent pixel after opaque pixels in first column/row)
	unsigned long int x1, x2, y1, y2;
public:
	// nine-sliced patches (in the order top-left, top, top-right, left, center, right, bottom-left, bottom, bottom-right)
	Patch * patches[9];
	explicit Texture_NineSliced(std::string _src, bool _autoRelease, bool _useMipmaps = true);
	virtual ~Texture_NineSliced();

	virtual void load() override;
	virtual void unload() override;
};


// this is really messy and hacked together b/c of how our UI system is setup, but visually it does exactly what it's supposed to
class NodeUI_NineSliced : public NodeUI{
protected:
	// patches
	NodeUI * tl, * t, * tr, * l, * c, * r, * bl, * b, * br;
	// content
	NodeUI * container;
public:
	NodeUI_NineSliced(BulletWorld * _world, Texture_NineSliced * _texture);
	virtual ~NodeUI_NineSliced();

	// set the size of the 8 border patches
	void setBorder(float _all);
	// set the size of the 8 border patches
	void setBorder(float _leftAndRight, float _bottomAndTop);
	// set the size of the 8 border patches
	void setBorder(float _left, float _right, float _bottom, float _top);

	virtual Transform * addChild(NodeUI * _uiElement, bool _invalidateLayout = true);

	// since the background of a NineSliced node is hidden and replaced by the 
	// patches, this changes the patches' background colours instead
	virtual void setBackgroundColour(float _r, float _g, float _b, float _a = 1.f) override;

	virtual float getContentsWidth() override;
	virtual float getContentsHeight() override;

	// sets the scale mode on this node, along with all of the patches
	// only valid args are GL_NEAREST and GL_LINEAR
	void setScaleMode(GLenum _scalemode);
};