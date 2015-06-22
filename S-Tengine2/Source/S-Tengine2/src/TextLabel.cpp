#pragma once

#include <TextLabel.h>
#include <TextArea.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>

TextLabel::TextLabel(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, float _width):
	HorizontalLinearLayout(_world, _scene),
	NodeBulletBody(_world),
	font(_font),
	textShader(_textShader),
	updateRequired(false),
	lineWidth(0.f)
{
	setHeight(font->getLineHeight());
	setWidth(_width);
	setBackgroundColour(1,1,1,0);
}

void TextLabel::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	GLboolean depth = glIsEnabled(GL_DEPTH_TEST);
	if(depth == GL_TRUE){
		glDisable(GL_DEPTH_TEST);
	}
	HorizontalLinearLayout::render(_matrixStack, _renderOptions);
	if(depth == GL_TRUE){
		glEnable(GL_DEPTH_TEST);
	}
}

void TextLabel::update(Step * _step){
	if(updateRequired){
		updateText();
	}
	HorizontalLinearLayout::update(_step);
}

void TextLabel::unload(){
	HorizontalLinearLayout::unload();
	textShader->unload();
	font->unload();
}

void TextLabel::load(){
	HorizontalLinearLayout::load();
	textShader->load();
	font->load();
}

void TextLabel::setText(std::wstring _text){
	/*if(textAll == _text){
		// same text, return early
		return;
	}*/
	invalidate();
	textAll = _text;

	updateRequired = true;
	updateText();
}

void TextLabel::updateText(){
	// find out where the first overflow in the text would occur
	unsigned long int i;
	for(i = 0; i < textAll.size(); ++i){
		if(textAll.at(i) == '\n'){
			++i;
			textDisplayed += '\n';
			// newline character
			break;
		}else if(!canFit(font->getGlyphWidthHeight(textAll.at(i)).x)){
			// width overflow
			break;
		}else{
			insertChar(textAll.at(i));
		}
	}
	
	// i to the end of the text did not fit
	if(i < textAll.size()){
		textOverflow = textAll.substr(i);
	}else{
		textOverflow = L"";
	}

	updateRequired = false;
}

float TextLabel::getContentsHeight(){
	return font->getLineHeight();
}

float TextLabel::getContentsWidth(){
	return lineWidth;
}

void TextLabel::invalidate(){
	while(usedGlyphs.size() > 0){
		removeChild(usedGlyphs.back());
		delete usedGlyphs.back();
		usedGlyphs.pop_back();
	}

	lineWidth = 0.f;
	textDisplayed = L"";
}

void TextLabel::insertChar(wchar_t _char){
	Glyph * glyphMesh = font->getMeshInterfaceForChar(_char);
	UIGlyph * glyph = new UIGlyph(world, scene, glyphMesh, textShader, _char);
	usedGlyphs.push_back(glyph);
	addChild(glyph);
	lineWidth += glyphMesh->advance.x/64.f;
	textDisplayed += _char;
}

bool TextLabel::canFit(float _width){
	if(width.sizeMode != kAUTO){
		if(_width + lineWidth > getWidth(false, false)){
			return false;
		}
	}
	return true;
}

// GlyphMeshEntity definitions //

UIGlyph::UIGlyph(BulletWorld * _world, Scene * _scene, Glyph * _mesh, Shader * _shader, wchar_t _character) :
	NodeUI(_world, _scene),
	NodeBulletBody(_world),
	MeshEntity(_mesh, _shader),
	character(_character),
	glyph(nullptr)
{
	setGlyphMesh(_mesh);
	boxSizing = kCONTENT_BOX;
}

void UIGlyph::setGlyphMesh(Glyph * _newGlyph){
	if(glyph != nullptr){
		contents->removeChild(glyph);
	}
	mesh = glyph = _newGlyph;
	contents->addChild(_newGlyph, false);
	setPixelWidth(glyph->advance.x/64);
	setPixelHeight(glyph->advance.y/64);
	setShader(shader, true);
}

void UIGlyph::load(){
	MeshEntity::load();
}
void UIGlyph::unload(){
	MeshEntity::unload();
}
void UIGlyph::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	MeshEntity::render(_matrixStack, _renderOptions);
}