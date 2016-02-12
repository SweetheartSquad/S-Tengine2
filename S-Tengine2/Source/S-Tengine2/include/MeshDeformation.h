#pragma once

#include <MeshInterface.h>
#include <Easing.h>

class MeshDeformation abstract{
public:
	// Flares along the Y-axis
	// NOTE: value and bounds arguments don't necessarily work
	static void flare(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing);
	// Twists around the Y-axis
	// NOTE: value and bounds arguments don't necessarily work
	static void twist(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing);
	// Bends in some way? Probably doesn't work
	// NOTE: value and bounds arguments don't necessarily work
	static void bend(MeshInterface * _mesh, float _lowerVal, float _upperVal, float _lowerBound, Easing::Type _easing);
};