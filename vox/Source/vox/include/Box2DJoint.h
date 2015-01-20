#pragma once

#include <Box2D/Box2D.h>

/******************************
*	Joint Types
*
*   Revolute - a hinge or pin, where the bodies rotate about a common point
*	Distance - a point on each body will be kept at a fixed distance apart
*	Prismatic - the relative rotation of the two bodies is fixed, and they can slide along an axis
*	Line - a combination of revolute and prismatic joints, useful for modelling vehicle suspension
*	Weld - holds the bodies at the same orientation
*	Pulley - a point on each body will be kept within a certain distance from a point in the world, 
*	where the sum of these two distances is fixed, kinda... (sheesh... there is no succinct way to describe this)
*	Friction - reduces the relative motion between the two bodies
*	Gear - controls two other joints (revolute or prismatic) so that the movement of one affects the other
*	Mouse - pulls a point on one body to a location in the world
*
*	See http://www.iforce2d.net/b2dtut/joints-overview
*
*****************************/
class Box2DJoint {
public:
	Box2DJoint();
	~Box2DJoint();
	
};