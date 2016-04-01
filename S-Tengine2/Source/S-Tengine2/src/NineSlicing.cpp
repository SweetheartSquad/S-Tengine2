#pragma once

#include <NineSlicing.h>
#include <TextureUtils.h>
#include <MeshInterface.h>

Patch::Patch(Texture_NineSliced * _root) :
	Texture("", false, false, _root->useMipmaps),
	NodeResource(false),
	root(_root),
	x1(0),
	x2(0),
	y1(0),
	y2(0)
{

}

void Patch::loadImageData(){
	// calculate the texture properties based on the provided coordinates
	// (the number of channels is taken from the root image though)
	width = x2-x1;
	height = y2-y1;
	channels = root->channels;
	numPixels = width * height;
	numBytes = numPixels * channels;

	// allocate memory for the texture
	data = (unsigned char *)malloc(sizeof(unsigned char) * numBytes);
	// copy the texture data from the source
	for(unsigned long int y = y1; y < y2; ++y){
		for(unsigned long int x = x1; x < x2; ++x){
			memcpy(data + (x - x1)*channels + (y - y1) * width * channels, sweet::TextureUtils::getPixelPointer(root, x, y), sizeof(unsigned char) * channels);
		}
	}
}

void Patch::resize(unsigned long int _x1, unsigned long int _y1, unsigned long int _x2, unsigned long int _y2){
	x1 = _x1;
	x2 = _x2;
	y1 = _y1;
	y2 = _y2;
}

Texture_NineSliced::Texture_NineSliced(std::string _src, bool _autoRelease, bool _useMipmaps) :
	Texture(_src, true, _autoRelease, _useMipmaps),
	NodeResource(_autoRelease)
{
	for(unsigned long int i = 0; i < 9; ++i){
		patches[i] = new Patch(this);
	}
}

Texture_NineSliced::~Texture_NineSliced(){
	for(unsigned long int i = 0; i < 9; ++i){
		delete patches[i];
	}
}

void Texture_NineSliced::load(){
	// usually we load components first to avoid this, but we need to know
	// this texture's properties in order to find the patches
	bool loaded2 = loaded;
	Texture::load();
	if(!loaded2){
		// first row/column is reserved for the markup, and the next row is assumed to be the start of the border patches, so we start with the third pixel from the edge
		x1 = x2 = y1 = y2 = 2;
		// find slice coordinates (it's assumed that the border patches are at least 1 pixel
		for(; x2 < width-1; ++x2){
			unsigned char p = sweet::TextureUtils::getPixel(this, x2, 0, channels-1);
			if(x1 == 2 && p == 255){
				x1 = x2;
			}else if(x1 != 2 && p != 255){
				break;
			}
		}
		for(; y2 < height-1; ++y2){
			unsigned char p = sweet::TextureUtils::getPixel(this, 0, y2, channels-1);
			if(y1 == 2 && p == 255){
				y1 = y2;
			}else if(y1 != 2 && p != 255){
				break;
			}
		}

		// inform patches of their size
		patches[0]->resize(1, 1, x1, y1);
		patches[1]->resize(x1, 1, x2, y1);
		patches[2]->resize(x2, 1, width, y1);

		patches[3]->resize(1, y1, x1, y2);
		patches[4]->resize(x1, y1, x2, y2);
		patches[5]->resize(x2, y1, width, y2);

		patches[6]->resize(1, y2, x1, height);
		patches[7]->resize(x1, y2, x2, height);
		patches[8]->resize(x2, y2, width, height);

		// load patches
		for(unsigned long int i = 0; i < 9; ++i){
			patches[i]->load();
		}
	}
}

void Texture_NineSliced::unload(){
	if(loaded){
		for(unsigned long int i = 0; i < 9; ++i){
			patches[i]->unload();
		}
	}

	Texture::unload();
}


NodeUI_NineSliced::NodeUI_NineSliced(BulletWorld * _world, Texture_NineSliced * _texture) :
	NodeUI(_world),
	tl(new NodeUI(world)),
	t(new NodeUI(world)),
	tr(new NodeUI(world)),
	l(new NodeUI(world)),
	c(new NodeUI(world)),
	r(new NodeUI(world)),
	bl(new NodeUI(world)),
	b(new NodeUI(world)),
	br(new NodeUI(world)),
	container(new NodeUI(world))
{
	background->setVisible(false);
	
	// set up all the constants for the patches
	NodeUI * patches[9] = {tl, t, tr, l, c, r, bl, b, br};
	for(unsigned long int i = 0; i < 9; ++i){
		NodeUI::addChild(patches[i]);
		patches[i]->boxSizing = kCONTENT_BOX;
		patches[i]->setRationalHeight(1.f, this);
		patches[i]->setRationalWidth(1.f, this);
		patches[i]->background->mesh->uvEdgeMode = GL_CLAMP_TO_EDGE;
		patches[i]->background->mesh->pushTexture2D(_texture->patches[i]);
	}

	b->boxSizing = kBORDER_BOX;
	b->setWidth(1.f);

	br->setWidth(0);
	br->setMarginLeft(1);
	br->background->childTransform->scale(-1,1,1,false);
	br->background->meshTransform->scale(-1,1,1,false);
	br->background->meshTransform->translate(1,0,0,false);

	tl->setHeight(0);
	tl->setMarginBottom(1);
	tl->background->childTransform->scale(1,-1,1,false);
	tl->background->meshTransform->scale(1,-1,1,false);
	tl->background->meshTransform->translate(0,1,0,false);

	t->setHeight(0);
	t->setWidth(1);
	t->setMarginBottom(1);
	t->background->childTransform->scale(1,-1,1,false);
	t->background->meshTransform->scale(1,-1,1,false);
	t->background->meshTransform->translate(0,1,0,false);

	tr->setHeight(0);
	tr->setWidth(0);
	tr->setMarginLeft(1);
	tr->setMarginBottom(1);
	tr->background->childTransform->scale(-1,-1,1,false);
	tr->background->meshTransform->scale(-1,-1,1,false);
	tr->background->meshTransform->translate(1,1,0,false);

	l->boxSizing = kBORDER_BOX;
	l->setHeight(1.f);

	c->setRationalHeight(1.f, this);
	c->setRationalWidth(1.f, this);
	c->boxSizing = kBORDER_BOX;

	r->setHeight(1.f);
	r->setWidth(0);
	r->setMarginLeft(1.f);
	r->background->childTransform->scale(-1,1,1,false);
	r->background->meshTransform->scale(-1,1,1,false);
	r->background->meshTransform->translate(1,0,0,false);
	
	NodeUI::addChild(container);
	container->setRationalWidth(1.f, this);
	container->setRationalHeight(1.f, this);
	container->background->setVisible(false);

	// set an initial border
	setBorder(0.33f);
}

NodeUI_NineSliced::~NodeUI_NineSliced(){

}

void NodeUI_NineSliced::setBorder(float _all){
	setBorder(_all, _all, _all, _all);
}

void NodeUI_NineSliced::setBorder(float _leftAndRight, float _bottomAndTop){
	setBorder(_leftAndRight, _leftAndRight, _bottomAndTop, _bottomAndTop);
}

Transform * NodeUI_NineSliced::addChild(NodeUI * _uiElement, bool _invalidateLayout){
	return container->addChild(_uiElement, _invalidateLayout);
}

void NodeUI_NineSliced::setBorder(float _left, float _right, float _bottom, float _top){
	// bottom
	bl->setHeight(_bottom);
	bl->setWidth(_left);

	b->setHeight(_bottom);
	b->setMargin(_left, _right, 0, 0);
	
	br->setHeight(_bottom);
	br->setPaddingLeft(_right);

	// top
	tl->setWidth(_left);
	tl->setPaddingBottom(_top);

	t->setMarginLeft(_left);
	t->setMarginRight(_right);
	t->setPaddingBottom(_top);
	t->setPaddingRight(_right+_left);
	t->paddingRight.pixelSize *= -1.f;
	t->paddingRight.rationalSize *= -1.f;
	
	tr->setPaddingLeft(_right);
	tr->setPaddingBottom(_top);


	// middle
	l->setWidth(_left);
	l->setMargin(0, 0, _bottom, _top);

	c->setMargin(_left, _right, _bottom, _top);
	
	r->setMarginTop(_top);
	r->setMarginBottom(_bottom);
	r->setPaddingLeft(_right);
	r->setPaddingTop(_top+_bottom);
	r->paddingTop.pixelSize *= -1;
	r->paddingTop.rationalSize *= -1;
}

void NodeUI_NineSliced::setBackgroundColour(float _r, float _g, float _b, float _a){
	tl->setBackgroundColour(_r, _g, _b, _a);
	t->setBackgroundColour(_r, _g, _b, _a);
	tr->setBackgroundColour(_r, _g, _b, _a);
	l->setBackgroundColour(_r, _g, _b, _a);
	c->setBackgroundColour(_r, _g, _b, _a);
	r->setBackgroundColour(_r, _g, _b, _a);
	bl->setBackgroundColour(_r, _g, _b, _a);
	b->setBackgroundColour(_r, _g, _b, _a);
	br->setBackgroundColour(_r, _g, _b, _a);
}

float NodeUI_NineSliced::getContentsWidth(){
	float w = 0.0f;
	// take the maximum of the width of the contents
	for(unsigned long int i = 0; i < container->uiElements->children.size(); ++i) {
		Transform * trans = container->uiElements->children.at(i)->asTransform();
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				w = std::max(w, trans->children.at(0)->asNodeUI()->getWidth(true, true));
			}
		}
	}
	return w;
}
float NodeUI_NineSliced::getContentsHeight(){
	float h = 0.0f;
	// take the maximum of the height of the contents66
	for(unsigned long int i = 0; i < container->uiElements->children.size(); ++i) {
		Transform * trans = container->uiElements->children.at(i)->asTransform();
		if(trans != nullptr) {
			if(trans->children.size() > 0) {
				h = std::max(h, trans->children.at(0)->asNodeUI()->getHeight(true, true));
			}
		}
	}
	return h;
}


void NodeUI_NineSliced::setScaleMode(GLenum _scalemode){
	background->mesh->setScaleMode(_scalemode);
	tl->background->mesh->setScaleMode(_scalemode);
	t->background->mesh->setScaleMode(_scalemode);
	tr->background->mesh->setScaleMode(_scalemode);
	l->background->mesh->setScaleMode(_scalemode);
	c->background->mesh->setScaleMode(_scalemode);
	r->background->mesh->setScaleMode(_scalemode);
	bl->background->mesh->setScaleMode(_scalemode);
	b->background->mesh->setScaleMode(_scalemode);
	br->background->mesh->setScaleMode(_scalemode);
}