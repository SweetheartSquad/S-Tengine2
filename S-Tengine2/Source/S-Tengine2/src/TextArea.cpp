#pragma once
 
#include <TextArea.h>
#include <Font.h>

TextArea::TextArea(BulletWorld * _bulletWorld, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	VerticalLinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld),
	font(_font),
	textShader(_textShader)
{
	setHeight(_height);
	setWidth(_width);
}
void TextArea::update(Step * _step){
	if(text != textDisplayed){
		updateRequired = true;
	}
	if(updateRequired){
		updateText();
	}
	VerticalLinearLayout::update(_step);
}

void TextArea::invalidateAllLines(){
	while(usedLines.size() > 0){
		usedLines.back()->invalidate();
		removeChild(usedLines.back());
		unusedLines.push_back(usedLines.back());
		usedLines.pop_back();
	}
	textDisplayed = L"";
}

void TextArea::setText(std::wstring _text){
	text = _text;
	updateRequired = true;
	updateText();
}

void TextArea::updateText(){
	invalidateAllLines();
	TextLabel * curLine = getCurrentLine();
	std::wstring textToAdd = text;
	curLine->setText(textToAdd);
	textToAdd = curLine->textOverflow;

	while(textToAdd.size() > 0){
		curLine = getNewLine();
		curLine->setText(textToAdd);
		if(curLine->textDisplayed.empty()){
			break; // if no text was actually added, break to avoid infinite loop (probably a zero-width text-area)
		}

		textDisplayed += curLine->textDisplayed;
		textToAdd = curLine->textOverflow;
	}

	updateRequired = false;
}

TextLabel * TextArea::getNewLine() {
	TextLabel * line;
	if(unusedLines.size() > 0) {
		line = unusedLines.back();
		unusedLines.pop_back();
	}else{
		line = new TextLabel(world, scene, font, textShader);
		line->setRationalWidth(1.f, this);
	}
	usedLines.push_back(line);
	addChild(line);
	return line;
}

TextLabel * TextArea::getCurrentLine(){
	if(usedLines.size() > 0){
		return usedLines.back();
	}else{
		return getNewLine();
	}
}