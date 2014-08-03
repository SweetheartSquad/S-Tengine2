#include "Bone.h"

Bone::Bone(){
	start = ci::Vec3d(0,0,0);
	end = ci::Vec3d(0,0,0);
	rotation = 0;
	building = true;
	parent = NULL;
}

Bone::~Bone(){}

void Bone::draw(){
	gl::lineWidth(1);
	gl::begin(GL_LINES);
	gl::enableWireframe();
	//gl::drawLine(start, end);
	gl::color(0,1,0.5);
	gl::vertex(start);
	gl::color(1,0,0.5);
	gl::vertex(end);
	
	gl::disableWireframe();
	gl::end();
}