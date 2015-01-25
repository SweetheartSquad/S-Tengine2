#pragma once

#include "BitmapFont.h"
#include "MeshFactory.h"
#include "Rectangle.h"
#include "node/NodeTransformable.h"
#include <vector>

BitmapFont::BitmapFont(Texture * _fontTextue, int _asciiStart, int _rows, int _columns):
	MeshEntity(),
	NodeTransformable(new Transform()),
	NodeChild(nullptr),
	NodeRenderable(),
	asciiStart(_asciiStart),
	rows(_rows),
	columns(_columns),
	meshQ(new QuadMesh(GL_QUADS, GL_STATIC_DRAW)),
	modSize(1),
	kerning(0.f)
{
	mesh = meshQ;
	meshQ->pushTexture2D(_fontTextue);	
	width = 1.f;
	modSize = 1.f;
}

BitmapFont::~BitmapFont(){
}

void BitmapFont::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	MeshEntity::render(_matrixStack, _renderStack);
}

void BitmapFont::setText(std::string _text){
	text = _text;
	createQuads();
}

void BitmapFont::createQuads(){
	layoutHeight = 0;
	layoutWidth = 0;
	kerning = 0.75f;
	mesh->vertices.clear();
	mesh->indices.clear();
	unload();
	load();
	std::vector<int> chars;

	for(unsigned long int i = 0; i < text.length(); ++i){
		char c = text.at(i);	
		chars.push_back(static_cast<int>(c) - 32);
	}

	float w = 0.0f;
	float charW = 1.0f/columns;
	std::vector<int>::iterator it;
	for(unsigned long int i = 0; i < text.length(); ++i){
		w+=charW;
		if(w > width){
			w = 0.0f;
			it = chars.begin();
			chars.insert(it + i, 45 - asciiStart);
		}
	}

	int textWidth = meshQ->textures.at(0)->width/columns;
	int textHeight = meshQ->textures.at(0)->width/rows;
	pushMultipleFrames(chars, textWidth, textHeight, meshQ->textures.at(0)->width);
	int col = 0;
	int row = 0;
	int count = 0;

	for(vox::Rectangle frame : frames){
		meshQ->pushVert(Vertex(((0 + col + kerning) * (modSize * kerning)), (1.f - row) * modSize, 0.f));
		meshQ->pushVert(Vertex(((1.f + col + kerning) * (modSize * kerning)), (1.f - row) * modSize, 0.f));
		meshQ->pushVert(Vertex(((1.f + col + kerning) * (modSize * kerning)), (0 - row) * modSize, 0.f));
		meshQ->pushVert(Vertex(((0 + col + kerning) * (modSize * kerning)), (0 - row) * modSize, 0.f));

		int a = meshQ->vertices.size()-4;
		int b = meshQ->vertices.size()-3;
		int c = meshQ->vertices.size()-2;
		int d = meshQ->vertices.size()-1;
		meshQ->pushQuad(a, b, c, d);
		meshQ->setNormal(a, 0.0, 0.0, 1.0);
		meshQ->setNormal(b, 0.0, 0.0, 1.0);
		meshQ->setNormal(c, 0.0, 0.0, 1.0);
		meshQ->setNormal(d, 0.0, 0.0, 1.0);
		meshQ->vertices.at(a).u  = frame.getTopLeft().x;
	    meshQ->vertices.at(a).v  = frame.getTopLeft().y;
	    meshQ->vertices.at(b).u  = frame.getTopRight().x - 1/columns * kerning;
	    meshQ->vertices.at(b).v  = frame.getTopRight().y;
	    meshQ->vertices.at(c).u  = frame.getBottomRight().x - 1/columns * kerning;
	    meshQ->vertices.at(c).v  = frame.getBottomRight().y;
	    meshQ->vertices.at(d).u  = frame.getBottomLeft().x;
	    meshQ->vertices.at(d).v  = frame.getBottomLeft().y;
		if(chars.at(col) == 45 - asciiStart){
			row++;
			col = 0;
		}else{
			col++;
			layoutWidth += modSize * kerning;
		}
		count++;
		layoutHeight += modSize * kerning;
	}
}

void BitmapFont::pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth){
	for(unsigned long int i = 0; i < _frames.size(); i++){
		pushFrame(_frames.at(i)%columns, _frames.at(i)/rows, _width, _height);
	}
}

void BitmapFont::pushFrame(int _column, int _row, float _width, float _height){
	float u, v, rW, rH;
	rW = _width / meshQ->textures.at(0)->width;
	rH = _height / meshQ->textures.at(0)->height;
	u = rW * _column;
	v = rH * _row;
	frames.push_back(vox::Rectangle(u, v, rW, rH));
}

float BitmapFont::getWidth(){
	return layoutWidth;
}

float BitmapFont::getHeight(){
	return layoutHeight;
}

void BitmapFont::setWidthMod(float _mod){
	width = _mod;
	createQuads();
}

void BitmapFont::setSizeMod(float _mod){
	modSize = _mod;
	createQuads();
}