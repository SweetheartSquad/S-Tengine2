#pragma once

#include "RandomGround.h"

#include "MeshInterface.h"
#include "NumberUtils.h"

#include <glew/glew.h>
#include <PuppetGame.h>


RandomGround::RandomGround(Box2DWorld * _world, int _numPoints, float _threshold, Texture * _texture):
	 Box2DMeshEntity(_world, new MeshInterface(GL_QUADS, GL_STATIC_DRAW), b2_staticBody, false),
	 NodeTransformable(new Transform()),
	 NodeRenderable(),
	 NodeChild(nullptr)
{
	float slope = 0;
	b2Vec2 * p = static_cast<b2Vec2 *>(calloc(_numPoints, sizeof (b2Vec2)));
	for(int i = 0; i < _numPoints; ++i) {
		p[i] = b2Vec2(0, 0);
		p[i].x = static_cast<float>(i);
		if (i > 1) {
			slope = p[i-1].y-p[i-2].y + vox::NumberUtils::randomFloat(-_threshold, _threshold);
			slope = std::max(-_threshold, std::min(_threshold, slope));
			p[i].y = p[i-1].y + slope;
			if(p[i].y < 0.1f){
				p[i].y = 0.1f;
			}
		}else if (i == 1) {
			p[i].y = p[0].y + vox::NumberUtils::randomFloat(-_threshold, _threshold);
		}else {
			p[i].y = -0.1f;
		}
    }

	p[_numPoints-2].y = -0.1f;
	p[_numPoints-1].y = -0.1f;
	p[_numPoints-1].x = p[0].x;
	
	if(_texture != nullptr){
		mesh->pushTexture2D(_texture);
	}

	float maxY = 0.0f;
	for(auto v : mesh->vertices){
		maxY = std::max(v.y, maxY);
	}

	for(auto i = 1; i < _numPoints - 1; ++i){
		if(i % 2 == 0){
			mesh->pushVert(Vertex(glm::vec3(p[i].x, -0.1, 1), glm::vec2(p[i].x / _numPoints, -0.1)));
			mesh->pushVert(Vertex(glm::vec3(p[i].x, p[i].y, 1), glm::vec2(p[i].x  / _numPoints, p[i].y / maxY)));
		}else{
			mesh->pushVert(Vertex(glm::vec3(p[i].x - 1, p[i - 1].y, 1), glm::vec2(p[i].x  / _numPoints,   p[i].y / maxY)));
			mesh->pushVert(Vertex(glm::vec3(p[i].x - 1, -0.1, 1), glm::vec2(p[i].x  / _numPoints, -0.1)));
		}
	}
	
	//Weird problem with chain's destructor being called twice
	//This works but it may be a memory leak
	b2ChainShape * chain = new b2ChainShape();
	chain->CreateChain(p, _numPoints);
	b2Fixture * f = getNewFixture(*chain, 1);

	b2Filter sf;
	sf.categoryBits = PuppetGame::kGROUND;
	f->SetFilterData(sf);
}

RandomGround::~RandomGround(){
}