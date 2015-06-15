#pragma once

#include <VerticalLinearLayout.h>
#include <TextLabel.h>

class TextArea : public VerticalLinearLayout{
public:
	Font * font;
	Shader * textShader;

	std::vector<NodeChild *> unusedLines;
//	WrapMode wrapMode;


	TextArea(BulletWorld * _bulletWorld, Scene * _scene, Font * _font, Shader * _textShader, float width, float height = UI_INFINITE);
	
	void invalidateAllLines();
	void setText(std::wstring _text);
	void removeLine(TextLabel * _line);

	// returns the last TextLabel inserted into unusedLines
	// if unusedLines is empty, creates a new TextLabel and returns that instead
	// note that the TextLabel returned ALWAYS HAS A PARENT TRANSFORM ALREADY
	TextLabel * getLine();
	
	virtual void update(Step * _step) override;
private:
	std::wstring text;
	bool updateRequired;

	TextLabel * currentLine();
};