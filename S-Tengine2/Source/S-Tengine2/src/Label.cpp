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

Label::Label(Font * _font, Shader * _textShader, Shader * _backgroundShader, WrapMode _wrapMode, float _width) :
	width(_width),
	wrapMode(_wrapMode),
	measuredWidth(0.0f),
	measuredHeight(0.0f),
	textDirty(false),
	background(new MeshEntity(MeshFactory::getPlaneMesh(0.5f)))
{
	font = _font;
	++font->referenceCount;
	textShader = _textShader;
	backgroundShader = _backgroundShader;
	++textShader->referenceCount;
	++backgroundShader->referenceCount;
	
	Transform * trans = new Transform();
	trans->addChild(background, true);
	background->setShader(backgroundShader, true);
}

void Label::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}
	background->parents.at(0)->render(_matrixStack, _renderOptions);
	Entity::render(_matrixStack, _renderOptions);
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void Label::update(Step * _step){
	background->parents.at(0)->update(_step);
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
	textShader->decrementAndDelete();
	backgroundShader->decrementAndDelete();
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
		float lastX = 0.0;
		for(unsigned long int i = 0; i < oldText.size() - text.size(); i++) {
			if(lastX < offsetCache.back().x) {
				//lineWidths.pop_back();
			}
			lastX = offsetCache.back().x;
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
					{
						Glyph * glyph = font->getMeshInterfaceForChar(c);
						if(offset.x + glyph->advance.x/64 > width){
							newLine(&offset);
						}
						updateChar(&offset, c, ch);
						break;
					}
				case CHARACTER_WRAP_HYPHEN:
					{
						Glyph * nextGlyph = font->getMeshInterfaceForChar(ch);
						if(width > 0 && offset.x + nextGlyph->advance.x/64> width){
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
					if(width > 0 && offset.x > width){
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

	for(unsigned long int i = 0; i < childTransform->children.size(); i++) {
		Transform * t = dynamic_cast<Transform *>(childTransform->children.at(i));
		MeshEntity * me = dynamic_cast<MeshEntity *>(t->children.at(0));
		me->setVisible(true);
	}

	if(static_cast<int>(measuredHeight) == 0) {
		measuredHeight = font->getLineHeight();
	}

	background->parents.at(0)->reset();
	glm::vec3 transVec = childTransform->getTranslationVector();

	if(width == INFINITE_WIDTH) {
		background->parents.at(0)->translate(transVec.x + measuredWidth * 0.5f, (transVec.y - measuredHeight * 0.5f) + font->getLineHeight(), transVec.z);
		background->parents.at(0)->scale(measuredWidth, measuredHeight, 1);
	}else {
		background->parents.at(0)->translate(transVec.x + width * 0.5f, (transVec.y - measuredHeight * 0.5f) + font->getLineHeight(), transVec.z);
		background->parents.at(0)->scale(width, measuredHeight, 1);
	}

	background->parents.at(0)->rotate(childTransform->getOrientationQuat(), kOBJECT);

	float currentLineY = dynamic_cast<Transform*>(childTransform->children.at(0))->getTranslationVector().y;
	int beginIdx = 0;
	for(unsigned long int i = 0; i < childTransform->children.size(); ++i) {
		float letterY = dynamic_cast<Transform*>(childTransform->children.at(i))->getTranslationVector().y;
 		if(abs(currentLineY - letterY) > 0.005 || i == childTransform->children.size() - 1) {
			for(unsigned long int j = beginIdx; j <= i; j++) {
				 Transform * letterTrans = dynamic_cast<Transform*>(childTransform->children.at(j));
				 float xTrans = (-1 * (offsetCache.at(i).x - width)/2) - offsetCache.at(j).x;
				 letterTrans->translate(xTrans + offsetCache.at(i).x, 
										letterTrans->getTranslationVector().y, 
										letterTrans->getTranslationVector().z, false);
			}
			beginIdx = i;
			currentLineY = letterY;
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
		me->setShader(textShader, true);
		me->setVisible(true);
		childTransform->addChild(me)->translate(_offset->x, _offset->y, 0.f);
	}

	float adv = glyph->advance.x/64 + _offset->x;
	_offset->x = adv;
	if(adv > measuredWidth) {
		measuredWidth = adv;
	}
	offsetCache.push_back(glm::vec2(adv, _offset->y));
}

void Label::newLine(glm::vec2 * _offset){
	lineWidths.push_back(offsetCache.back().x);
	_offset->x = 0;
	float lineHeight = font->getLineHeight(); 
	_offset->y -= lineHeight;
	measuredHeight += lineHeight;
	offsetCache.back() = glm::vec2(_offset->x, _offset->y);
}

void Label::setText(std::wstring _text){
	oldText = text;
	text = _text;
	textDirty = true;
	updateText();
}
