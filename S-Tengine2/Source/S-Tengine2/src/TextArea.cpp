#pragma once
 
#include <TextArea.h>
#include <Font.h>

TextArea::TextArea(BulletWorld * _bulletWorld, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height) :
	VerticalLinearLayout(_bulletWorld, _scene),
	NodeBulletBody(_bulletWorld),
	font(_font),
	textShader(_textShader)
{
	autoResizingWidth = autoResizingHeight = true;
}

void TextArea::invalidateAllLines(){
	for(unsigned long int i = 0; i < contents->children.size(); ++i) {
		TextLabel * line = dynamic_cast<TextLabel *>(dynamic_cast<Transform *>(contents->children.at(i))->children.at(0));
		line->invalidate();
	}
}

void TextArea::setText(std::wstring _text){
	text = _text;
	invalidateAllLines();	
	updateRequired = true;
}

void TextArea::removeLine(TextLabel * _line){
	contents->removeChild(_line->parents.at(0));
	unusedLines.push_back(_line);
}

TextLabel * TextArea::getLine() {
	TextLabel * line = nullptr;
	if(unusedLines.size() > 0) {
		line = dynamic_cast<TextLabel *>(unusedLines.back());
		//unusedLines.pop_back();
	}else{
		line = new TextLabel(world, scene, font, textShader);
		line->textArea = this;
		Transform * t = new Transform();
		t->addChild(line, false);
	}
	return line;
}

void TextArea::update(Step * _step){
	if(updateRequired) {
		TextLabel * curLine = currentLine();
		if(curLine == nullptr){
			curLine = getLine();
			contents->addChild(curLine->parents.at(0), false);
		}
		//contents->addChild(curLine->parents.at(0), false);
		float curY = 0.f;
		for(unsigned long int i = 0; i < text.size(); ++i){
			if(!curLine->canFit(font->getGlyphWidthHeight(text.at(i)).x)){
				curY -= font->getLineHeight();
				curLine = getLine();
				contents->addChild(curLine->parents.at(0), false);
			}
			curLine->insertChar(text.at(i));	
			curLine->parents.at(0)->translate(0.f, curY, 0.f, false);
		}
		updateRequired = false;
	}
	VerticalLinearLayout::update(_step);
}

TextLabel * TextArea::currentLine(){
	if(contents->children.size() > 0){
		return dynamic_cast<TextLabel *>(dynamic_cast<Transform *>(contents->children.back())->children.at(0));
	}else{
		return nullptr;
	}
}