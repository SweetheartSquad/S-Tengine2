#pragma once

#include <node/NodeMeasurable.h>


float NodeMeasurable::getMeasuredWidth(){
	return measuredWidth;
}
float NodeMeasurable::getMeasuredHeight(){
	return measuredHeight;
}
void NodeMeasurable::setMeasuredWidth(float _width){
	measuredWidth = _width;
}
void NodeMeasurable::setMeasuredHeight(float _height){
	measuredHeight = _height;
}