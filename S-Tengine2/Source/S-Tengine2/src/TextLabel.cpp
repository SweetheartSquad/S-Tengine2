#pragma once

#include <TextLabel.h>
#include <TextArea.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>
#include <RenderOptions.h>

TextLabel::TextLabel(BulletWorld* _world, Scene* _scene, Font* _font, Shader* _textShader, float _width):
	HorizontalLinearLayout(_world, _scene),
	font(_font),
	textShader(_textShader),
	updateRequired(false),
	lineWidth(0.f)
{
	setHeight(font->getLineHeight());
	setWidth(_width);
}

void TextLabel::render(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
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
	font->load();
	textShader->load();
	HorizontalLinearLayout::load();
}

void TextLabel::setText(std::wstring _text){
	/*if(textAll == _text){
		// same text, return early
		return;
	}*/
	invalidate();
	
	invalidateLayout();

	textAll = _text;

	updateRequired = true;

	layoutDirty = true;

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
		unusedGlyphs.push_back(usedGlyphs.back());
		usedGlyphs.pop_back();
	}

	lineWidth = 0.f;
	textDisplayed = L"";
}

void TextLabel::insertChar(wchar_t _char){
	Glyph * glyphMesh = font->getMeshInterfaceForChar(_char);
	UIGlyph * glyph = getGlyph(_char, glyphMesh);
	usedGlyphs.push_back(glyph);
	addChild(glyph);
	lineWidth += glyphMesh->advance.x/64.f;
	textDisplayed += _char;
}

UIGlyph * TextLabel::getGlyph(wchar_t _char, Glyph * _glyphMesh){
	UIGlyph * res;
	if(unusedGlyphs.size() > 0){
		res = unusedGlyphs.back();
		res->setGlyphMesh(_glyphMesh);
		unusedGlyphs.pop_back();
	}else{
		res = new UIGlyph(world, scene, _glyphMesh, textShader, _char);
	}
	return res;
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
	NodeShadable(_shader),
	character(_character),
	glyph(nullptr)
{
	setGlyphMesh(_mesh);
	boxSizing = kCONTENT_BOX;
	mouseEnabled = false;
}

void UIGlyph::setGlyphMesh(Glyph * _newGlyph){
	if(glyph != nullptr){
		uiElements->removeChild(glyph);
	}
	glyph = _newGlyph;

	uiElements->addChild(glyph, false);
	setPixelWidth(glyph->advance.x/64);
	setPixelHeight(glyph->advance.y/64);
	
	glyph->configureDefaultVertexAttributes(shader);
}

void UIGlyph::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * prev = _renderOptions->shader;
	NodeShadable::applyShader(_renderOptions);
	NodeUI::render(_matrixStack, _renderOptions);
	_renderOptions->shader = prev;
}