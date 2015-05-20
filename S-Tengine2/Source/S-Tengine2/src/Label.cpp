#pragma once

#include <Label.h>
#include <Font.h>
#include <MeshInterface.h>
#include <MeshEntity.h>
#include <node\NodeChild.h>
#include <shader/BaseComponentShader.h>
#include <shader/ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>
#include <ostream>
#include <string>
#include <MeshFactory.h>
#include <CharacterUtils.h>

Label::Label(Font * _font, Shader * _shader, WrapMode _wrapMode, float _width) :
	width(_width),
	wrapMode(_wrapMode),
	textDirty(false)
{
	font = _font;
	++font->referenceCount;
	shader = _shader;
	++shader->referenceCount;
}

void Label::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}
	Entity::render(_matrixStack, _renderOptions);
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void Label::update(Step * _step){
	Entity::update(_step);
}

void Label::unload(){
	Entity::unload();
	font->unload();
}

void Label::load(){
	font->load();
	Entity::load();
}

Label::~Label(){
	font->decrementAndDelete();
	shader->decrementAndDelete();
}

void Label::appendText(std::wstring _text){
	oldText = text;
	text += _text;
	textDirty = true;
	updateText();
}

std::wstring Label::getText(){
	return text;
}

void Label::updateText(){
	glm::vec2 offset(0.f, 0.f);
	textDirty = false;

	for(unsigned long int i = text.size(); i < childTransform->children.size(); ++i) {
		Transform * t = dynamic_cast<Transform *>(childTransform->children.at(i));
		MeshEntity * me = dynamic_cast<MeshEntity *>(t->children.at(0));
		me->setVisible(false);
	}
	
	int strOffset = 0;
	for(unsigned long int i = 0; i < text.size() && i < oldText.size(); i++) {
		if(text.at(i) != oldText.at(i)) {
			break;
		}	
		strOffset++;
	}

	if(text.size() < oldText.size()) {
		for(unsigned long int i = 0; i < oldText.size() - text.size(); i++) {
			offsetCache.pop_back();
		}
	}

	if(offsetCache.size() > 0) {
		offset = offsetCache.at(offsetCache.size() - 1);
	}

	for(unsigned long int c = strOffset; c < text.size(); ++c) {
		wchar_t ch = text.at(c);
		if(ch == '\n'){
			newLine(&offset);
		}else{
			switch(wrapMode){
				case CHARACTER_WRAP:
					updateChar(&offset, c, ch);
					if(offset.x > width){
						newLine(&offset);
					}
					break;
				case CHARACTER_WRAP_HYPHEN:
					{
						Glyph * nextGlyph = font->getMeshInterfaceForChar(ch);
						if(offset.x + nextGlyph->advance.x/64> width){
							if(!CharacterUtils::isSpace(ch)){
								updateChar(&offset, c, '-');
							}	
							newLine(&offset);
							updateChar(&offset, c, ch);
						}else {
							updateChar(&offset, c, ch);
						}
						break;
					}
				case WORD_WRAP:
					if(offset.x > width){
						for(unsigned long int i = 0; i < text.size() && c < text.size(); ++i) {
							updateChar(&offset, c, text.at(c));
							if(CharacterUtils::isSpace(text.at(c))){
								newLine(&offset);
								break;
							}
							c++;
						}
					}else {
						updateChar(&offset, c, ch);
					}
					break;
				default: break;
			}
		}
	}
}

void Label::updateChar(glm::vec2 * _offset, int _index, wchar_t _c){
	Glyph * glyph = font->getMeshInterfaceForChar(_c);
	if(childTransform->children.size() > _index){
		Transform * t = dynamic_cast<Transform *>(childTransform->children.at(_index));
		t->translate(_offset->x, _offset->y, 0.f, false);
		MeshEntity * me = dynamic_cast<MeshEntity *>(t->children.at(0));
		me->setVisible(true);
		me->childTransform->children.clear();
		me->childTransform->children.push_back(glyph);
	}else {
		MeshEntity * me = new MeshEntity(glyph);
		me->setShader(shader, true);
		childTransform->addChild(me)->translate(_offset->x, _offset->y, 0.f);
	}

	float adv = glyph->advance.x/64 + _offset->x;
	_offset->x = adv;
	offsetCache.push_back(glm::vec2(adv, _offset->y));
}

void Label::newLine(glm::vec2 * _offset){
	lineWidths.push_back(offsetCache.back().x);
	_offset->x = 0;
	_offset->y -= font->lineGapRatio * ((font->face->size->metrics.ascender + font->face->size->metrics.descender)/64);
	offsetCache.push_back(glm::vec2(_offset->x, _offset->y));
}

void Label::setText(std::wstring _text){
	oldText = text;
	text = _text;
	textDirty = true;
	updateText();
}
