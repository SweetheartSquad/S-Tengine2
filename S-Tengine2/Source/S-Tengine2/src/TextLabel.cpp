#pragma once

#include <TextLabel.h>
#include <TextArea.h>
#include <GL/glew.h>
#include <MeshFactory.h>
#include <Font.h>
#include <RenderOptions.h>
#include <StringUtils.h>

TextLabel::TextLabel(BulletWorld* _world, Font* _font, Shader* _textShader):
	HorizontalLinearLayout(_world),
	font(_font),
	textShader(_textShader),
	updateRequired(false),
	lineWidth(0.f),
	wrapMode(kCHARACTER)
{
	// set the default width and height to be auto and the font's height, respectively
	setPixelHeight(font->getLineHeight());
	setAutoresizeWidth();
}

void TextLabel::render(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOptions){
	HorizontalLinearLayout::render(_matrixStack, _renderOptions);
}

void TextLabel::update(Step * _step){
	if(updateRequired){
		updateText();
	}
	HorizontalLinearLayout::update(_step);
}

void TextLabel::unload(){
	if(loaded){
		textShader->unload();
		font->unload();
		for(unsigned long int i = 0; i < unusedGlyphs.size(); ++i){
			unusedGlyphs.at(i)->unload();
		}
	}
	HorizontalLinearLayout::unload();
}

void TextLabel::load(){
	if(!loaded){
		font->load();
		textShader->load();
		for(unsigned long int i = 0; i < unusedGlyphs.size(); ++i){
			unusedGlyphs.at(i)->load();
		}
	}
	HorizontalLinearLayout::load();
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

void TextLabel::setText(std::string _text){
	setText(std::wstring(_text.begin(), _text.end()));
}

void TextLabel::updateText(){
	// find out where the first overflow in the text would occur
	unsigned long int idx = wordWrap();

	// idx to the end of the text did not fit
	if(idx < textAll.size()){
		textOverflow = textAll.substr(idx);
	}else{
		textOverflow = L"";
	}

	invalidateLayout();
	updateRequired = false;
}

unsigned long int TextLabel::wordWrap(){
	unsigned long int idx = 0;
	if(wrapMode == kWORD) {
		std::vector<std::wstring> words = sweet::StringUtils::split(textAll, ' ');
		for(unsigned long int i = 0; i < words.size(); ++i) {
			std::wstring word = words.at(i);
			float width = 0.f;
			for(auto c : word) {
				width += font->getGlyphWidthHeight(c).x;
			}
			if(canFit(width) + font->getGlyphWidthHeight(' ').x) {
				for(auto c : word) {
					if(c == '\n') {
						++idx;
						textDisplayed += '\n';
						return idx; // return early
					}
					insertChar(c);
					idx++;
				}
				if(i != words.size()-1){
					insertChar(' ');
				}
				idx++;
			}else {
				textDisplayed += '\n';
				break;
			}
		}
	}else if(wrapMode == kCHARACTER) {
		for(idx = 0; idx < textAll.size(); ++idx){
			if(textAll.at(idx) == '\n'){
				++idx;
				textDisplayed += '\n';
				// newline character
				break;
			}else if(!canFit(font->getGlyphWidthHeight(textAll.at(idx)).x)){
				// width overflow
				break;
			}else{
				insertChar(textAll.at(idx));
			}
		}
	}
	return idx;
}

float TextLabel::getContentsHeight(){
	return font->getLineHeight();
}

float TextLabel::getContentsWidth(){
	return lineWidth;
}

void TextLabel::invalidate(){
	while(usedGlyphs.size() > 0){
		removeChild(usedGlyphs.back(), false);
		unusedGlyphs.push_back(usedGlyphs.back());
		usedGlyphs.pop_back();
	}

	lineWidth = 0.f;
	textDisplayed = L"";
}

void TextLabel::insertChar(wchar_t _char){
	Glyph * glyph = font->getGlyphForChar(_char);
	UIGlyph * uiGlyph = getGlyph(_char, glyph);
	usedGlyphs.push_back(uiGlyph);
	addChild(uiGlyph, false);
	lineWidth += glyph->advance.x/64.f;
	textDisplayed += _char;
}

UIGlyph * TextLabel::getGlyph(wchar_t _char, Glyph * _glyph){
	UIGlyph * res;
	if(unusedGlyphs.size() > 0){
		res = unusedGlyphs.back();
		res->setGlyph(_glyph);
		res->character = _char;
		unusedGlyphs.pop_back();
	}else{
		res = new UIGlyph(world, _glyph, textShader, _char);
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

UIGlyph::UIGlyph(BulletWorld * _world, Glyph * _mesh, Shader * _shader, wchar_t _character) :
	NodeUI(_world),
	NodeShadable(_shader),
	character(_character),
	glyphMesh(nullptr)
{
	setGlyph(_mesh);
	boxSizing = kCONTENT_BOX;
	mouseEnabled = false;
}

void UIGlyph::setGlyph(Glyph * _newGlyph){
	if(glyphMesh != nullptr){
		uiElements->removeChild(glyphMesh);
	}
	glyphMesh = _newGlyph->mesh;

	uiElements->addChild(glyphMesh, false);
	setPixelWidth(_newGlyph->advance.x/64);
	setPixelHeight(_newGlyph->advance.y/64);
	
	glyphMesh->configureDefaultVertexAttributes(shader);
}

void UIGlyph::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * prev = _renderOptions->shader;
	NodeShadable::applyShader(_renderOptions);
	NodeUI::render(_matrixStack, _renderOptions);
	_renderOptions->shader = prev;
}