#pragma once

#include <TextArea.h>
#include <Font.h>

TextArea::TextArea(BulletWorld * _bulletWorld, Font * _font, Shader * _textShader) :
	VerticalLinearLayout(_bulletWorld),
	font(_font),
	textShader(_textShader)
{
	// set the default width and height to be auto and the three times font's height, respectively
	setPixelHeight(_font->getLineHeight()*3);
	setAutoresizeWidth();
	setWrapMode(kCHARACTER);
}

TextArea::~TextArea(){
	while(unusedLines.size() > 0){
		delete unusedLines.back();
		unusedLines.pop_back();
	}
}

void TextArea::update(Step * _step){
	if(updateRequired){
		updateText();
	}
	VerticalLinearLayout::update(_step);
}

void TextArea::setMeasuredWidths(){
	float w = width.getSize();
	VerticalLinearLayout::setMeasuredWidths();
	if(glm::abs(w - width.getSize()) > FLT_EPSILON){
		updateRequired = true;
	}
}

void TextArea::invalidateAllLines(){
	while(usedLines.size() > 0){
		usedLines.back()->invalidate();
		removeChild(usedLines.back(), false);
		unusedLines.push_back(usedLines.back());
		usedLines.pop_back();
	}
	textDisplayed = L"";
	// Do this to make sure all labels have the right wrap mode
	setWrapMode(wrapMode);
}

void TextArea::setWrapMode(WrapMode _wrapMode){
	for(auto label : usedLines) {
		label->wrapMode = _wrapMode;
	}
	for(auto label : unusedLines) {
		label->wrapMode = _wrapMode;
	}
	wrapMode = _wrapMode;
}

void TextArea::setText(std::wstring _text){
	text = _text;
	updateRequired = true;
	updateText();
}

void TextArea::setText(std::string _text){
	setText(std::wstring(_text.begin(), _text.end()));
}

std::wstring TextArea::getText(){
	return text;
}

void TextArea::updateText(){
	invalidateAllLines();
	std::wstring textToAdd = text;

	TextLabel * curLine;
	if(getWidth(false, false) > FLT_EPSILON){
		do{
			curLine = getNewLine();
			curLine->setText(textToAdd);
			textToAdd = curLine->textOverflow;
			textDisplayed += curLine->textDisplayed;
		}while(textToAdd.size() > 0 && !curLine->textDisplayed.empty());
		// if all the text is added, then the loop is successfull
		// if no text was actually added to a line, stop to avoid infinite loop (probably a zero-width text-area)

		updateRequired = false;
	}
	invalidateLayout();
}

TextLabel * TextArea::getNewLine() {
	TextLabel * line;
	if(unusedLines.size() > 0) {
		line = unusedLines.back();
		unusedLines.pop_back();
	}else{
		line = new TextLabel(world, font, textShader);
		line->setRationalWidth(1.f, this);
		line->horizontalAlignment = horizontalAlignment;
	}
	usedLines.push_back(line);
	addChild(line, false);
	line->wrapMode = wrapMode;
	return line;
}

TextLabel * TextArea::getCurrentLine(){
	if(usedLines.size() > 0){
		return usedLines.back();
	}else{
		return getNewLine();
	}
}

void TextArea::setShader(Shader * _shader, bool _configureDefaultAttributes){
	for(auto line : usedLines){
		line->setShader(_shader, _configureDefaultAttributes);
	}
	for(auto line : unusedLines){
		line->setShader(_shader, _configureDefaultAttributes);
	}
	textShader = _shader;
}

void TextArea::setFont(Font * _font, bool _updateText){
	for(auto line : usedLines){
		line->setFont(_font, false);
	}
	for(auto line : unusedLines){
		line->setFont(_font, false);
	}

	if(_updateText){
		setText(text);
		invalidateLayout();
	}
}

void TextArea::load(){
	if(!loaded){
		for(unsigned long int i = 0; i < unusedLines.size(); ++i){
			unusedLines.at(i)->load();
		}
	}
	VerticalLinearLayout::load();
}

void TextArea::unload(){
	if(loaded){
		for(unsigned long int i = 0; i < unusedLines.size(); ++i){
			unusedLines.at(i)->unload();
		}
	}
	VerticalLinearLayout::unload();
}