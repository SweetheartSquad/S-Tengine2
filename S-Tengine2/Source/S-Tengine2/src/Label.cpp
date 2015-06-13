#pragma once

#include <Label.h>
#include <Font.h>
#include <MeshInterface.h>
#include <MeshEntity.h>
#include <node\NodeChild.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>
#include <ostream>
#include <string>
#include <MeshFactory.h>
#include <CharacterUtils.h>


Label::Label(BulletWorld * _bulletWorld, Scene * _scene, Font * _font, Shader * _textShader, Shader * _backgroundShader, WrapMode _wrapMode, float _width) :
	NodeUI(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld),
	MeshEntity(MeshFactory::getPlaneMesh()),
	width(_width),
	wrapMode(_wrapMode),
	measuredWidth(0.0f),
	measuredHeight(0.0f),
	textDirty(false),
	background(new MeshEntity(MeshFactory::getPlaneMesh(0.5f))),
	alignment(LEFT),
	wasAppended(false),
	lineCount(1),
	offset(glm::vec2(0.0f, 0.0f))
{
	font = _font;
	++font->referenceCount;
	textShader = _textShader;
	backgroundShader = _backgroundShader;
	++textShader->referenceCount;
	++backgroundShader->referenceCount;

	setShader(textShader, true);

	background->setShader(backgroundShader, true);
	backgroundTransform = childTransform->addChild(background);
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
	wasAppended = true;
	oldText = text;
	text += _text;
	textDirty = true;
	updateText();
	wasAppended = false;
}

std::wstring Label::getText(){
	return text;
}

void Label::updateAlignment(){
	if(childTransform->children.size() > 1 + BACKGROUND_OFFSET){
		// Set currentLineY to the value of the first childs y translation
		float currentLineY = dynamic_cast<Transform*>(childTransform->children.at(1 + BACKGROUND_OFFSET))->getTranslationVector().y;
		int beginIdx = 1 + BACKGROUND_OFFSET;
		switch (alignment){
			case CENTER:
				{
					// loop through each of the children of the child transfom. This will be all of the characters
					for(unsigned long int i = 1 + BACKGROUND_OFFSET; i < childTransform->children.size(); i++) {
						// letterY is the y transform of the current child
						float letterY = dynamic_cast<Transform *>(childTransform->children.at(i))->getTranslationVector().y;
						// We take the absolute of the difference of the two y values
						float f = abs(currentLineY - letterY);
						// If there is a difference we know we are on a new line
						// TODO - What happens for letters which move based on font positioning?
						// We also want to enter the statement if we are on the last letter
 						if(f > 0.005 || i == childTransform->children.size() - 1) {
							// Loop through each letter for the row so we can align it
							// Start from the being idx which starts as 0 and gets set to i
							// Include i in the loop since it is in the row
							for(unsigned long int j = beginIdx; j <= i; j++) {
								 int o = childTransform->children.size() > 2 ? 1 : 0;
								 // Get the offset for the letter at j
								 Transform * letterTrans = dynamic_cast<Transform*>(childTransform->children.at(j));
								 // Center the letters based on the current letter's transform and the last letter in the row's transform.
								 // Again we are using the cache offset here to get the last letter in the previos row if we aren't on the current row
								 float xTrans = (-1 * (offsetCache.at((i - 1) - o).x - width)/2) - offsetCache.at(j - 1).x;
								 letterTrans->translate(xTrans + offsetCache.at((i - 1) - o).x , 
														letterTrans->getTranslationVector().y, 
														letterTrans->getTranslationVector().z, false);
							}
							// Update the begin index to the first letter in the new row
							beginIdx = i;
							// Update the currentLineY for the next iteration of the loop
							currentLineY = letterY;
						}
					}
				}
			case LEFT : break;
		}
	}
}

float Label::getMeasuredWidth(){
	return measuredWidth;
}

float Label::getMeasuredHeight(){
	return measuredHeight;
}

void Label::setAlignment(Alignment _alignment){
	alignment = _alignment;
	updateAlignment();
}

void Label::updateText(){
	
	wasAppended = false;

	glm::vec2 cacheOffset = glm::vec2(offset);

	textDirty = false;
	
	int strOffset = 0;
	
	if(wasAppended) {
		strOffset = text.size() - (text.size() - oldText.size());
		if(offsetCache.size() > 0) {
			//offset = offsetCache.at(strOffset - 1);	
		}
	}else {
		textInternal.clear();
		lineCount = 1;
		offset = glm::vec2(0.f, 0.f);
	}

	int a = 0;

	for(unsigned long int c = strOffset; c < text.size(); ++c) {
		wchar_t ch = text.at(c);
		if(ch == '\n'){
			offset.x = 0.f;
			textInternal += L'\n';
			a++;
		}else{
			switch(wrapMode){
				case CHARACTER_WRAP:
					{
						Glyph * glyph = font->getMeshInterfaceForChar(ch);
						float glyphWidth = glyph->advance.x/64;
						if(offset.x + glyphWidth > width){
							textInternal += L'\n';
							a++;
							offset.x = 0.f;
						}
						offset.x += glyphWidth;
						textInternal += ch;
						break;
					}
				case CHARACTER_WRAP_HYPHEN:
					{
						Glyph * nextGlyph = font->getMeshInterfaceForChar(ch);
						float glyphWidth = nextGlyph->advance.x/64;
						if(width > 0 && offset.x + glyphWidth > width){
							if(!CharacterUtils::isSpace(ch)){
								textInternal += L'\n';
							}	
							textInternal += L'\n';
						}else {
							offset.x += glyphWidth;
							textInternal += ch;
						}
						break;
					}
				case WORD_WRAP:
					{
						Glyph * nextGlyph = font->getMeshInterfaceForChar(ch);
						float glyphWidth = nextGlyph->advance.x/64;
						if(width > 0 && glyphWidth + offset.x > width){
							for(unsigned long int i = 1; i < text.size() && c < text.size(); ++i) {
								updateChar(&offset, c, text.at(c));
								if(CharacterUtils::isSpace(text.at(c))){
									textInternal += L'\n';
									break;
								}
								c++;
							}
						}else {
							offset.x += glyphWidth;
							textInternal += ch;
						}
						break;
					}
				default: break;
			}
		}
	}

	int updateDiff = 0;

	offset = cacheOffset;

	if(wasAppended){
		updateDiff = strOffset - a;
		if(offsetCache.size() > 0 && strOffset > 0){
			offset = offsetCache.at(strOffset - 1);
		}
	}else {
		offset = glm::vec2(0.f, 0.f);
	}
	
	for(unsigned long int c = updateDiff; c < textInternal.size(); ++c) {
		wchar_t ch = textInternal.at(c);
		if(ch == '\n'){
			newLine(&offset, c);
			lineCount++;
		}else{
			updateChar(&offset, c, ch);
		}
	}

	if(childTransform->children.size() > 1) {
		measuredHeight = font->getLineHeight() * lineCount;
	}else {
		measuredHeight = 0.f;
	}

	glm::vec3 transVec = childTransform->getTranslationVector();

	if(width == INFINITE_WIDTH) {
		backgroundTransform->translate(transVec.x + measuredWidth * 0.5f, (transVec.y - measuredHeight * 0.5f) + font->getLineHeight(), transVec.z, false);
		backgroundTransform->scale(measuredWidth, measuredHeight, 1, false);
	}else {
		backgroundTransform->translate(transVec.x + width * 0.5f, (transVec.y - measuredHeight * 0.5f) + font->getLineHeight(), transVec.z, false);
		backgroundTransform->scale(width, measuredHeight, 1, false);
	}

	updateAlignment();

	//if(!wasAppended){
		for(unsigned long int i = 1 + BACKGROUND_OFFSET; i < childTransform->children.size(); i++) {
			Transform * t = dynamic_cast<Transform *>(childTransform->children.at(i));
			MeshEntity * me = dynamic_cast<MeshEntity *>(t->children.at(0));
			me->setVisible(true);
		}
	//}
}

void Label::updateChar(glm::vec2 * _offset, int _index, wchar_t _c){
	_index += 2;
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

	if(measuredWidth < width) {
		measuredWidth = width;
	}

	if(offsetCache.size() < childTransform->children.size()){
		offsetCache.push_back(glm::vec2(adv, _offset->y));
	}else {
		offsetCache.at(_index) = glm::vec2(adv, _offset->y);
	}
}

void Label::newLine(glm::vec2 * _offset, int _idx){
	float lineHeight = font->getLineHeight(); 
	_offset->y -= lineHeight;
	_offset->x = 0.f;
}

void Label::setText(std::wstring _text){
	oldText = text;
	text = _text;
	textDirty = true;
	updateText();
}
