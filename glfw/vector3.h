#pragma once

#include <math.h>

#define TOLERANCE 0.0001f

class Vector3
{
public:
	Vector3(void);
	Vector3(float x, float y, float z);

	~Vector3(void);

	float x;
	float y;
	float z;

	float magnitude(void);
	void normalize(void);
	void reverse(void);

	Vector3& operator += (Vector3 u);
	Vector3& operator -= (Vector3 u);
	Vector3& operator *= (float s);
	Vector3& operator /= (float s);
	Vector3 operator - (void);
};

