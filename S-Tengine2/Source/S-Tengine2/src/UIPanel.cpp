#pragma once

#include <UIPanel.h>
#include <MeshFactory.h>
#include <MeshInterface.h>

UIPanel::UIPanel(float _halfwidth, float _halfheight, BulletWorld * _world, Scene * _scene) :
	NodeUI(_world, _scene),
	NodeBulletBody(_world),
	MeshEntity(MeshFactory::getPlaneMesh(1.f)),
	width(_halfwidth*2.f),
	height(_halfheight*2.f)
{
	setColliderAsBox(_halfwidth, _halfheight, 0.1f);
	createRigidBody(0);

	for(unsigned long int i = 0; i < mesh->vertices.size(); ++i){
		mesh->vertices[i].x *= _halfwidth;
		mesh->vertices[i].y *= _halfheight;
		mesh->vertices[i].x += _halfwidth;
		mesh->vertices[i].y += _halfheight;
	}
	mesh->dirty = true;
}

float UIPanel::getMeasuredWidth(){
	return width;
}

float UIPanel::getMeasuredHeight(){
	return height;
}