#include "Bone.h"

/** 
Generates quaternion which, when applied to direction vector v1, will result in another direction vector v2
Doesn't take into account any constraints
*/
Quatd orient(Vec3d v1, Vec3d v2){
	//check for zero-length source and target vectors
	if(v1.lengthSquared() == 0 || v2.lengthSquared() == 0){
		return Quatd(1,0,0,0);
	}

	Quatd q;
	Vec3d c = v1.cross(v2);
	q.v = c;
	if ( v1.lengthSquared() == 1 && v2.lengthSquared() == 1 ) {
		q.w = 1 + v1.dot(v2);
	} else {
		q.w = sqrt(v1.lengthSquared() * v2.lengthSquared()) + v1.dot(v2);
	}
	return q.normalized();
}

Bone::Bone(){
	start = ci::Vec3d(0,0,0);
	end = ci::Vec3d(0,0,0);
	rotation = 0;
	building = true;
	parent = NULL;
}

Bone::~Bone(){}

void Bone::draw(){
	//full bone shape
	Quatd orientation(Vec3d(0,1,0), end-start);
	gl::enableWireframe();
	//gl::drawSphere(start, 25, 4);

	gl::pushModelView();
		gl::translate(start);
		gl::rotate(orientation);
		gl::drawSolidTriangle(Vec2f(0.05,0),Vec2f(-0.05,0),Vec2f(0,(start-end).length()));
		gl::rotate(Vec3f(0,90,0));
		gl::drawSolidTriangle(Vec2f(0.05,0),Vec2f(-0.05,0),Vec2f(0,(start-end).length()));
	gl::popModelView();

	//gl::drawSphere(end, 25, 4);
	gl::disableWireframe();

	//individual line
	/*gl::lineWidth(1);
	gl::begin(GL_LINES);
	gl::enableWireframe();
	//gl::drawLine(start, end);
	gl::color(0,1,0.5);
	gl::vertex(start);
	gl::color(1,0,0.5);
	gl::vertex(end);
	gl::disableWireframe();
	gl::end();*/
}