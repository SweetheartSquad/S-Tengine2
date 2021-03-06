#pragma once

#include <TextLabelControlled.h>
#include <StringUtils.h>
#include <iomanip>

TextLabelControlled::TextLabelControlled(float * const _target, float _valueMin, float _valueMax, BulletWorld* _world, Font * _font, Shader * _textShader) :
	TextLabel(_world, _font, _textShader),
	target(_target),
	valueMin(_valueMin),
	valueMax(_valueMax),
	prevValue(*_target + 1),
	value(*_target + 1),
	prefix(L""),
	suffix(L""),
	decimals(0)
{
}

void TextLabelControlled::update(Step * _step){
	value = *target;
	if(value < valueMin){
		value = valueMin;
	}else if(value > valueMax){
		value = valueMax;
	}
	if(glm::abs(value - prevValue) > FLT_EPSILON){
		std::wstringstream ss;
		ss.precision(decimals);
		ss << prefix << std::setprecision(decimals) << std::fixed << value << suffix;
		// because of the decimals, we might not actually need to update the text
		if(getText(false) != ss.str()){
			setText(ss.str());
		}
		prevValue = value;
	}
	TextLabel::update(_step);
}