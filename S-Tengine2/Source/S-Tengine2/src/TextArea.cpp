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
	std::wstring textToAdd = text;

	TextLabel * curLine;
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

TextLabel * TextArea::getNewLine() {
	TextLabel * line;
	if(unusedLines.size() > 0) {
		line = unusedLines.back();
		unusedLines.pop_back();
	}else{
		line = new TextLabel(world, scene, font, textShader, 1.f);
		line->setRationalWidth(1.f, this);
		line->horizontalAlignment = horizontalAlignment;
		line->verticalAlignment = verticalAlignment;
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