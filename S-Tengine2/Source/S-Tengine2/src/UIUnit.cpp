#pragma once

#include <UIUnit.h>

UIUnit::UIUnit() :
	sizeMode(kPIXEL),
	pixelSize(0.f),
	rationalSize(1.f),
	measuredSize(0.f),
	rationalTarget(nullptr)
{
}

void UIUnit::setSize(float _size, UIUnit * _target){
	if(_size < 0){
		setAutoSize();
	}else if(_size <= FLT_EPSILON){
		setPixelSize(0);
	}else if(_size <= 1.00001f){
		setRationalSize(_size, _target);
	}else{
		setPixelSize(_size);
	}
}

void UIUnit::setRationalSize(float _rationalSize, const UIUnit * const _target){
	sizeMode = kRATIO;
	rationalSize = _rationalSize;
	rationalTarget = _target;
}
void UIUnit::setAutoSize(){
	sizeMode = kAUTO;
}

void UIUnit::setPixelSize(float _pixelSize){
	sizeMode = kPIXEL;
	pixelSize = _pixelSize;
}
float UIUnit::getSize() const{
	switch (sizeMode){
	case kPIXEL:
		return pixelSize;
	case kRATIO:
	//	return rationalSize * rationalTarget->getSize();
	case kAUTO:
	default:
		return measuredSize;
	}
}