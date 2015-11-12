#pragma once

#include <VerticalLinearLayout.h>
#include <TextLabel.h>

class TextArea : public VerticalLinearLayout{
public:
	Font * font;
	Shader * textShader;
	
	std::vector<TextLabel *> usedLines;
	std::vector<TextLabel *> unusedLines;
//	WrapMode wrapMode;


	TextArea(BulletWorld * _bulletWorld, Scene * _scene, Font * _font, Shader * _textShader, float width, float height = -1);
	virtual void update(Step * _step) override;
	
	void invalidateAllLines();
	void setText(std::wstring _text);
	std::wstring getText();
	void updateText();

	// returns the last TextLabel inserted into unusedLines
	// if unusedLines is empty, creates a new TextLabel and returns that instead
	// the returned line will be added to the contents list automatically
	virtual TextLabel * getNewLine();
private:
	std::wstring text;
	std::wstring textDisplayed;
	bool updateRequired;

	// returns the last line in usedLines
	// if usedLines is empty, calls getNewLine and returns the result
	TextLabel * getCurrentLine();
};