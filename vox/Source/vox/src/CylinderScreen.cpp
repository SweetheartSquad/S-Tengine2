#include "CylinderScreen.h"

CylinderScreen::CylinderScreen(float _speed, float * _control, int _numLevels, Texture * _texture, MeshInterface * _mesh, Transform * _transform, Shader * _shader):
	MeshEntity(_mesh, _transform, _shader),
	NodeTransformable(_transform),
	NodeChild(nullptr),
	NodeRenderable(),
	control(_control),
	prevControlValue(*control),
	speed(_speed),
	numLevels(_numLevels)
{
	if(_texture != nullptr){
		mesh->pushTexture2D(_texture);
	}
}

void CylinderScreen::update(Step * _step){
	MeshEntity::update(_step);

	float dif = *control - prevControlValue;
	float layers = 1.f/numLevels;
	for(unsigned long int i = 0; i < mesh->getVertCount(); i += 3){
		float x1 = mesh->vertices.at(i).u+(dif)/speed;//_step->deltaTimeCorrection*-0.0005;//i/(float)3;//;
		float y1 = mesh->vertices.at(i).v;//1.f - me->mesh->vertices.at(i).y * 0.2f;
		float x2 = mesh->vertices.at(i+1).u+(dif)/speed;//_step->deltaTimeCorrection*-0.0005;//i/(float)3;//;
		float y2 = mesh->vertices.at(i+1).v;//1.f - me->mesh->vertices.at(i).y * 0.2f;
		float x3 = mesh->vertices.at(i+2).u+(dif)/speed;//_step->deltaTimeCorrection*-0.0005;//i/(float)3;//;
		float y3 = mesh->vertices.at(i+2).v;//1.f - me->mesh->vertices.at(i).y * 0.2f;

		if(x1 > 1.02 /*|| x2 > 1 || x3 > 1*/){
			x1 -= 1.0;
			x2 -= 1.0;
			x3 -= 1.0;

			y1 += layers;
			y2 += layers;
			y3 += layers;
		}if(x3 < 0.02 /* || x2 < 0 || x3 < 0*/){
			x1 += 1.0;
			x2 += 1.0;
			x3 += 1.0;

			y1 -= layers;
			y2 -= layers;
			y3 -= layers;
		}
			
		if(y1 > 1.0001){
			y1 -= 1;
			y2 -= 1;
			y3 -= 1;
		}else if(y1 < 0.0001){
			y1 += 1;
			y2 += 1;
			y3 += 1;
		}

		mesh->setUV(i, x1, y1);
		mesh->setUV(i+1, x2, y2);
		mesh->setUV(i+2, x3, y3);
	}
	mesh->dirty = true;
	prevControlValue = *control;
}