#pragma once

#include "BitmapFont.h"
#include "MeshFactory.h"
#include "Rectangle.h"
#include <vector>

BitmapFont::BitmapFont(Texture * _fontTextue, int _asciiStart, int _rows, int _columns):
	QuadMesh(GL_QUADS, GL_STATIC_DRAW),
	asciiStart(_asciiStart),
	rows(_rows),
	columns(_columns),
	modSize(1)
{
	pushTexture2D(_fontTextue);	
}

BitmapFont::~BitmapFont(){
}

void BitmapFont::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderStack){
	QuadMesh::render(_matrixStack, _renderStack);
}

void BitmapFont::setText(std::string _text){
	text = _text;
	createQuads();
}

void BitmapFont::createQuads(){
	std::vector<int> chars;
	for(unsigned long int i = 0; i < text.length(); ++i){
		char c = text.at(i);	
		chars.push_back(static_cast<int>(c) - 32);
	}
	int textWidth = textures.at(0)->width/columns;
	int textHeight = textures.at(0)->width/rows;
	pushMultipleFrames(chars, textWidth, textHeight, textures.at(0)->width);
	int col = 0;
	int row = 0;
    
	for(vox::Rectangle frame : frames){
		pushVert(Vertex((0 + col) * modSize, (1.f + row) * modSize, 0.f));
		pushVert(Vertex((1.f + col) * modSize, (1.f + row) * modSize, 0.f));
		pushVert(Vertex((1.f + col) * modSize, (0 + row) * modSize, 0.f));
		pushVert(Vertex((0 + col) * modSize, (0 + row) * modSize, 0.f));

		int a = vertices.size()-4;
		int b = vertices.size()-3;
		int c = vertices.size()-2;
		int d = vertices.size()-1;
		pushQuad(a, b, c, d);
		setNormal(a, 0.0, 0.0, 1.0);
		setNormal(b, 0.0, 0.0, 1.0);
		setNormal(c, 0.0, 0.0, 1.0);
		setNormal(d, 0.0, 0.0, 1.0);
		vertices.at(a).u  = frame.getTopLeft().x;
	    vertices.at(a).v  = frame.getTopLeft().y;
	    vertices.at(b).u  = frame.getTopRight().x;
	    vertices.at(b).v  = frame.getTopRight().y;
	    vertices.at(c).u  = frame.getBottomRight().x;
	    vertices.at(c).v  = frame.getBottomRight().y;
	    vertices.at(d).u  = frame.getBottomLeft().x;
	    vertices.at(d).v  = frame.getBottomLeft().y;
		col++;
	}
}

void BitmapFont::pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth){
	for(unsigned long int i = 0; i < _frames.size(); i++){
		pushFrame(_frames.at(i)%columns, _frames.at(i)/rows, _width, _height);
	}
}

void BitmapFont::pushFrame(int _column, int _row, float _width, float _height){
	float u, v, rW, rH;
	rW = _width / textures.at(0)->width;
	rH = _height / textures.at(0)->height;
	u = rW * _column;
	v = rH * _row;
	frames.push_back(vox::Rectangle(u, v, rW, rH));
}

void BitmapFont::setSizeMod(float _mod){
	modSize = _mod;
}