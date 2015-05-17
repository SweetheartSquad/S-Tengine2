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

	//while(childTransform->children.size() > 0){
	//	delete childTransform->children.back();
	//	childTransform->children.pop_back();
	//}

	for(unsigned long int i = text.size(); i < childTransform->children.size(); ++i) {
		Transform * t = dynamic_cast<Transform *>(childTransform->children.at(i));
		MeshEntity * me = dynamic_cast<MeshEntity *>(t->children.at(0));
		me->setVisible(false);
	}

	for(unsigned long int c = 0; c < text.size(); ++c) {
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
		me->mesh = glyph;
		std::cout<<me->mesh<<"\n";
	}else {
		MeshEntity * me = new MeshEntity(glyph);
		me->setShader(shader, true);
		childTransform->addChild(me)->translate(_offset->x, _offset->y, 0.f);
	}
	_offset->x += glyph->advance.x/64;
}

void Label::newLine(glm::vec2 * _offset){
	_offset->x = 0;
	_offset->y -= font->lineGapRatio * ((font->face->size->metrics.ascender + font->face->size->metrics.descender)/64);
}

void Label::setText(std::wstring _text){
	text = _text;
	textDirty = true;
	updateText();
}
