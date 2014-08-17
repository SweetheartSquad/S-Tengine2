#include "Joint.h"

unsigned long int Joint::nextId = 0;
uint32_t Joint::nextColor = 0xFFFFFF;
std::map<uint32_t, Joint*> Joint::jointMap;

void Joint::init(){
	depth = 0;
	pos = ci::Vec3d(0,0,0);
	rotation = ci::Quatd(0,0,0,1);
	building = true;
	parent = NULL;

	jointMap.insert(JointPair(nextColor, this));
	color = Color::hex(nextColor);
	nextColor -= 0x000001;
	id = nextId;
	nextId += 1;
}

Joint::Joint(){
	init();
}

Joint::Joint(Joint * _parent){
	init();
	parent = _parent;
	while(_parent != nullptr){
		_parent = _parent->parent;
		depth += 1;
	}
}

Joint::~Joint(){}

void Joint::draw(){
	//gl::enableWireframe();

	//colour
	if(depth%3 == 0){
		gl::color(Color(0,1,1));
	}else if(depth%3 == 1){
		gl::color(Color(1,0,1));
	}else{
		gl::color(Color(1,1,0));
	}

	//draw joint
	gl::pushModelView();
		gl::translate(pos);
		gl::rotate(rotation);
		gl::drawSphere(Vec3f(0,0,0), 0.05);
	gl::popModelView();

	//draw bones
	for(Joint * child : children){
		Quatd orientation(Vec3d(0,1,0), child->pos-pos);
		gl::pushModelView();
			gl::translate(pos);
			gl::rotate(orientation);
			gl::drawSolidTriangle(Vec2f(0.05,0),Vec2f(-0.05,0),Vec2f(0,(pos-child->pos).length()));
			gl::rotate(Vec3f(0,90,0));
			gl::drawSolidTriangle(Vec2f(0.05,0),Vec2f(-0.05,0),Vec2f(0,(pos-child->pos).length()));
		gl::popModelView();
	}

	//gl::disableWireframe();
}