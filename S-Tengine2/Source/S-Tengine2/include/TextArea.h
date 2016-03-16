#pragma once

#include <VerticalLinearLayout.h>
#include <TextLabel.h>

class TextArea : public VerticalLinearLayout{
public:
	Font * font;
	Shader * textShader;

	std::vector<TextLabel *> usedLines;
	std::vector<TextLabel *> unusedLines;

	TextArea(BulletWorld * _bulletWorld, Font * _font, Shader * _textShader);
	~TextArea();

	virtual void update(Step * _step) override;

	void invalidateAllLines();
	void setText(std::wstring _text);
	std::wstring getText();
	// converts _text to an std::wstring and calls setText(std::wstring)
	void setText(std::string _text);
	void updateText();
	void setWrapMode(WrapMode _wrapMode);

	// returns the last TextLabel inserted into unusedLines
	// if unusedLines is empty, creates a new TextLabel and returns that instead
	// the returned line will be added to the contents list automatically
	virtual TextLabel * getNewLine();

	virtual void load() override;
	virtual void unload() override;
private:
	std::wstring text;
	std::wstring textDisplayed;
	bool updateRequired;
	WrapMode wrapMode;

	// returns the last line in usedLines
	// if usedLines is empty, calls getNewLine and returns the result
	TextLabel * getCurrentLine();

	virtual void setMeasuredWidths() override;
};