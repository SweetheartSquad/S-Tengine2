#pragma once

#include <UIUnit.h>

UIUnit::UIUnit() :
	sizeMode(kPIXEL),
	pixelSize(0.f),
	rationalSize(1.f),
	measuredSize(0.f)
{
}

void UIUnit::setSize(float _size){
	if(_size < 0){
		setAutoSize();
	}else if(_size <= 1.00001f){
		setRationalSize(_size);
	}else{
		setPixelSize(_size);
	}
}

void UIUnit::setRationalSize(float _rationalSize){
	sizeMode = kRATIO;
	rationalSize = _rationalSize;
}
void UIUnit::setAutoSize(){
	sizeMode = kAUTO;
}

void UIUnit::setPixelSize(float _pixelSize){
	sizeMode = kPIXEL;
	pixelSize = _pixelSize;
}

float UIUnit::getSize(){
	switch (sizeMode){
	case kPIXEL:
		return pixelSize;
	case kRATIO:
	case kAUTO:
	default:
		return measuredSize;
	}
}