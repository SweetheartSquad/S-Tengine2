#pragma once

#include "Cube.h"
#include "MeshInterface.h"
#include "MeshFactory.h"

Cube::Cube(glm::vec3 _center, float _scale) :
	MeshEntity(MeshFactory::getCubeMesh())
{
	parents.at(0)->scale(_scale, _scale, _scale);
	parents.at(0)->translate(_center);
}

Cube::~Cube(void){
}

void Cube::setColour(float red, float green, float blue, float alpha){
	for(int i=0; i<24; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setLeftColour(float _red, float _green, float _blue, float _alpha){
	for(int i=16; i<20; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setRightColour(float _red, float _green, float _blue, float _alpha){
	for(int i=20; i<24; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setTopColour(float _red, float _green, float _blue, float _alpha){
	for(int i=0; i<4; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setBottomColour(float _red, float _green, float _blue, float _alpha){
	for(int i=0; i<8; i++){
		mesh->vertices.at(i).red = _red;
		mesh->vertices.at(i).green = _green;
		mesh->vertices.at(i).blue = _blue;
		mesh->vertices.at(i).alpha = _alpha;
	}
}

void Cube::setFrontColour(float red, float green, float blue, float alpha){
	for(int i=8; i<12; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setBackColour(float red, float green, float blue, float alpha){
	for(int i=12; i<16; i++){
		mesh->vertices.at(i).red = red;
		mesh->vertices.at(i).green = green;
		mesh->vertices.at(i).blue = blue;
		mesh->vertices.at(i).alpha = alpha;
	}
}

void Cube::setVertexColour(int index, float red, float green, float blue, float alpha){
	mesh->vertices.at(index).red = red;
	mesh->vertices.at(index).green = green;
	mesh->vertices.at(index).blue = blue;
	mesh->vertices.at(index).alpha = alpha;
}