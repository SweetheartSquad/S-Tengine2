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
		setText(textAll);
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
	if(textAll == _text){
		// same text, return early
		return;
	}
	invalidate();
	textAll = _text;

	// find out where the first overflow in the text would occur
	unsigned long int i;
	for(i = 0; i < textAll.size(); ++i){
		if(textAll.at(i) == '\n'){
			++i;
			// newline character
			break;
		}else if(!canFit(font->getGlyphWidthHeight(textAll.at(i)).x)){
			// width overflow
			break;
		}else{
			insertChar(textAll.at(i));
		}
	}
	
	// the start of the text to i fit
	textDisplayed = textAll.substr(0, i);
	// i+1 to the end of the text did not fit
	if(i < textAll.size()){
		textOverflow = textAll.substr(i);
	}else{
		textOverflow = L"";
	}
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
	setShader(getShader(), true);
	//setPixelWidth(glyph->metrics.width/64);
	//setPixelHeight(glyph->metrics.height/64);

	//paddingLeft.setPixelSize(glyph->metrics.horiBearingX/64);
	//paddingRight.setPixelSize((glyph->metrics.horiAdvance - glyph->metrics.width - glyph->metrics.horiBearingX)/64);
	//paddingBottom.setPixelSize((glyph->metrics.vertAdvance - glyph->metrics.height - glyph->metrics.vertBearingY)/64);
	//paddingTop.setPixelSize(glyph->metrics.vertBearingY/64);
	
	/*paddingLeft.setPixelSize(0);
	paddingRight.setPixelSize(0);
	paddingBottom.setPixelSize(0);
	paddingTop.setPixelSize(0);*/

	/*marginLeft.setPixelSize(2);
	marginRight.setPixelSize(2);
	marginBottom.setPixelSize(2);
	marginTop.setPixelSize(2);*/

	//setMargin(2);
}

void UIGlyph::load(){
	MeshEntity::load();
}
void UIGlyph::unload(){
	MeshEntity::unload();
}