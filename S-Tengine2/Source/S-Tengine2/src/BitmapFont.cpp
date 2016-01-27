#pragma once

#include <vector>

#include <BitmapFont.h>
#include <Rectangle.h>
#include <Texture.h>

BitmapFont::BitmapFont(Texture * _fontTextue, int _asciiStart, int _rows, int _columns, bool _padFront, WrapMode _wrapMode) :
	MeshEntity(new QuadMesh(true)),
	asciiStart(_asciiStart),
	rows(_rows),
	kerning(0.f),
	columns(_columns),
	padFront(_padFront),
	wrapMode(_wrapMode),
	modSize(1)
{
	mesh->pushTexture2D(_fontTextue);	
	width = 1.f;
	modSize = 1.f;
}

BitmapFont::~BitmapFont(){
}

void BitmapFont::setText(std::string _text){
	text = _text;
	createQuads();
}

void BitmapFont::createQuads(){
	layoutHeight = 0;
	layoutWidth = 0;
	frames.clear();
	mesh->vertices.clear();
	mesh->indices.clear();
	mesh->dirty = true;
	std::vector<int> chars;

	for(unsigned long int i = 0; i < text.length(); ++i){
		char c = text.at(i);	
		chars.push_back(static_cast<int>(c) - asciiStart);
	}
	float w = 0.0f;
	modSize = 0.3f;
	float charW = 1.0f / (columns);
	charW *= modSize * width;
	if(chars.size() > 0){
		std::vector<int>::iterator it;
		it = chars.begin();
		if(padFront) {
			chars.insert(it, ' ' - asciiStart);	
		}
		for(unsigned long int i = 0; i < text.length(); ++i){
			w += charW;
			switch(wrapMode) {
				case WORD_WRAP : 
					if(w > width && chars.at(i) == static_cast<int>(' ') - asciiStart){
						w = 0.0f;
						it = chars.begin();
						chars.insert(it + i, '\n');
						w += charW;
						if(chars.size() > i + 1){
							if(padFront) {
								if(chars.at(i + 1) != ' ' - asciiStart){
									it = chars.begin();
									chars.insert(it + i + 1, ' ' - asciiStart);	
									w += charW;
								}
							}
						}
					}
					break;
				case CHARACTER_WRAP : 
					if(w > width){
						w = 0.0f;
						it = chars.begin();
						chars.insert(it + i, '\n');
						w += charW;
						if(chars.size() > i + 1){
							if(padFront) {
								if(chars.at(i + 1) != ' ' - asciiStart){
									it = chars.begin();
									chars.insert(it + i + 1, ' ' - asciiStart);	
									w += charW;
								}
							}
						}
					}
					break;
				case CHARACTER_WRAP_HYPHEN : 
					if(w > width){
						w = 0.0f;
						it = chars.begin();
						chars.insert(it + i, 45 - asciiStart);
						w += charW;
						if(chars.size() > i + 1){
							it = chars.begin();
							chars.insert(it + i + 1, '\n');
							w += charW;
							if(padFront) {
								if(chars.size() > i + 2){
									if(chars.at(i + 2) != ' ' - asciiStart){
										it = chars.begin();
										chars.insert(it + i + 2, ' ' - asciiStart);	
										w += charW;
									}
								}
							}
						}
					}
					break;
			}
		}
	}

	int textWidth = mesh->getTexture(0)->width/columns;
	int textHeight = mesh->getTexture(0)->width/rows;
	pushMultipleFrames(chars, textWidth, textHeight, mesh->getTexture(0)->width);
	int col = 0;
	int row = 0;
	int count = 0;
	for(sweet::Rectangle frame : frames){
		if(chars.at(count) != '\n'){
			mesh->pushVert(Vertex(((0 + col + kerning) * (modSize * kerning)), (1.f - row) * modSize, 0.f));
			mesh->pushVert(Vertex(((1.f + col + kerning) * (modSize * kerning)), (1.f - row) * modSize, 0.f));
			mesh->pushVert(Vertex(((1.f + col + kerning) * (modSize * kerning)), (0 - row) * modSize, 0.f));
			mesh->pushVert(Vertex(((0 + col + kerning) * (modSize * kerning)), (0 - row) * modSize, 0.f));
			int a = mesh->vertices.size()-4;
			int b = mesh->vertices.size()-3;
			int c = mesh->vertices.size()-2;
			int d = mesh->vertices.size()-1;
			mesh->setNormal(a, 0.0, 0.0, 1.0);
			mesh->setNormal(b, 0.0, 0.0, 1.0);
			mesh->setNormal(c, 0.0, 0.0, 1.0);
			mesh->setNormal(d, 0.0, 0.0, 1.0);
			mesh->vertices.at(a).u  = frame.getTopLeft().x;
			mesh->vertices.at(a).v  = frame.getTopLeft().y;
			mesh->vertices.at(b).u  = frame.getTopRight().x - 1/columns * kerning;
			mesh->vertices.at(b).v  = frame.getTopRight().y;
			mesh->vertices.at(c).u  = frame.getBottomRight().x - 1/columns * kerning;
			mesh->vertices.at(c).v  = frame.getBottomRight().y;
			mesh->vertices.at(d).u  = frame.getBottomLeft().x;
			mesh->vertices.at(d).v  = frame.getBottomLeft().y;
		}
		if(chars.at(count) == '\n'){
			row++;
			col = 0;
		}else{
			col++;
			layoutWidth += modSize * kerning;
		}
		count++;
		layoutHeight += modSize * kerning;
	}

	/*meshQ->pushVert(Vertex(0.f, layoutHeight, 0.f));
	meshQ->pushVert(Vertex(layoutWidth, layoutHeight, 0.f));
	meshQ->pushVert(Vertex(layoutWidth, 0, 0.f));
	meshQ->pushVert(Vertex(0.f, 0, 0.f));

	int a = meshQ->vertices.size()-4;
	int b = meshQ->vertices.size()-3;
	int c = meshQ->vertices.size()-2;
	int d = meshQ->vertices.size()-1;
	meshQ->pushQuad(a, b, c, d);
	meshQ->setNormal(a, 0.0, 0.0, 1.0);
	meshQ->setNormal(b, 0.0, 0.0, 1.0);
	meshQ->setNormal(c, 0.0, 0.0, 1.0);
	meshQ->setNormal(d, 0.0, 0.0, 1.0);
	meshQ->vertices.at(a).u  = 0.f;
	meshQ->vertices.at(a).v  = 0.f;
	meshQ->vertices.at(b).u  = 0.f;
	meshQ->vertices.at(b).v  = 0.f;
	meshQ->vertices.at(c).u  = 0.f;
	meshQ->vertices.at(c).v  = 0.f;
	meshQ->vertices.at(d).u  = 0.f;
	meshQ->vertices.at(d).v  = 0.f;*/
}

void BitmapFont::pushMultipleFrames(std::vector<int> _frames, float _width, float _height, float _textureWidth){
	for(unsigned long int i = 0; i < _frames.size(); i++){
		pushFrame(_frames.at(i)%columns, _frames.at(i)/rows, _width, _height);
	}
}

void BitmapFont::pushFrame(int _column, int _row, float _width, float _height){
	float u, v, rW, rH;
	rW = _width / mesh->getTexture(0)->width;
	rH = _height / mesh->getTexture(0)->height;
	u = rW * _column;
	v = rH * _row;
	frames.push_back(sweet::Rectangle(u, v, rW, rH));
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

void BitmapFont::renderText(){
	createQuads();
}

void BitmapFont::setSizeMod(float _mod){
	modSize = _mod;
	createQuads();
}