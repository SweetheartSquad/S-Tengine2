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
	wrapMode(kNONE)
{
	// set the default width and height to be auto and the font's height, respectively
	setPixelHeight(font->getLineHeight());
	setAutoresizeWidth();

	textShader->incrementReferenceCount();
}

TextLabel::~TextLabel(){
	while(unusedGlyphs.size() > 0){
		delete unusedGlyphs.back();
		unusedGlyphs.pop_back();
	}

	textShader->decrementAndDelete();
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

std::wstring TextLabel::getText(bool _clipped){
	return _clipped ? textDisplayed : textAll;
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
				width += font->getGlyphAdvance(c).x;
			}
			if(canFit(width + font->getGlyphAdvance(' ').x)) {
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
			}else if(!canFit(font->getGlyphAdvance(textAll.at(idx)).x)){
				// width overflow
				break;
			}else{
				insertChar(textAll.at(idx));
			}
		}
	}else if(wrapMode == kTRUNCATE){
		float periodLength = font->getGlyphAdvance('.').x * 3.0f;
		for(idx = 0; idx < textAll.size(); ++idx){
			if(textAll.at(idx) == '\n'){
				++idx;
				textDisplayed += '\n';
				// newline character
				break;
			}else if(!canFit(font->getGlyphAdvance(textAll.at(idx)).x + periodLength)){
				// width overflow
				insertChar('.');
				insertChar('.');
				insertChar('.');
				break;
			}else{
				insertChar(textAll.at(idx));
			}
		}
	}else if(wrapMode == kNONE){
		for(idx = 0; idx < textAll.size(); ++idx){
			if(textAll.at(idx) == '\n'){
				++idx;
				textDisplayed += '\n';
				// newline character
			}else{
				insertChar(textAll.at(idx));
			}
		}
	}
	return idx;
}

void TextLabel::setShader(Shader * _shader){
	if(textShader != _shader){
		textShader->decrementAndDelete();
		textShader = _shader;
		textShader->incrementReferenceCount();
		for(auto glyph : usedGlyphs){
			glyph->setShader(textShader);
		}for(auto glyph : unusedGlyphs){
			glyph->setShader(textShader);
		}
	}
}

void TextLabel::setFont(Font * _font, bool _updateText){
	font = _font;
	if(loaded){
		font->load();
	}
	setPixelHeight(font->getLineHeight());

	if(_updateText){
		setText(textAll);
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

UIGlyph::~UIGlyph(){
}

void UIGlyph::setGlyph(Glyph * _newGlyph){
	if(glyphMesh != nullptr){
		uiElements->removeChild(glyphMesh);
		glyphMesh->decrementAndDelete();
	}
	glyphMesh = _newGlyph->mesh;
	glyphMesh->incrementReferenceCount();

	uiElements->addChild(glyphMesh, false);
	setPixelWidth(_newGlyph->advance.x/64);
	setPixelHeight(_newGlyph->advance.y/64);

	glyphMesh->configureDefaultVertexAttributes(shader);
}

void UIGlyph::setShader(Shader * _shader){
	if(shader != _shader){
		shader->decrementAndDelete();
		shader = _shader;
		shader->incrementReferenceCount();
	}
}

void UIGlyph::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	Shader * prev = _renderOptions->shader;
	NodeShadable::applyShader(_renderOptions);
	NodeUI::render(_matrixStack, _renderOptions);
	_renderOptions->shader = prev;
}