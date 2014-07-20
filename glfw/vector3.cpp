#include "vector3.h"

Vector3::Vector3(void){
	x = 0;
	y = 0;
	z = 0;
}

Vector3::Vector3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}


Vector3::~Vector3(void)
{
}

float Vector3::magnitude(void){
	return (float)sqrt(x*x + y*y + z*z);
}

void Vector3::normalize(void){
	float magnitude = this->magnitude();
	
	if(magnitude <= TOLERANCE){
		magnitude = 1.0f;
	}

	x/=magnitude;
	y/=magnitude;
	z/=magnitude;

	if(fabs(x) < TOLERANCE){
		x = 0.0f;
	}
	if(fabs(y) < TOLERANCE){
		y = 0.0f;
	}
	if(fabs(z) < TOLERANCE){
		z = 0.0f;
	}
}

void Vector3::reverse(void){
	x = -x;
	y = -y;
	z = -z;
}

Vector3& Vector3::operator+=(Vector3 u){
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

Vector3& Vector3::operator-=(Vector3 u){
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

Vector3& Vector3::operator*=(float s){
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s){
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector3 Vector3::operator-(void)
{
	return Vector3(-x, -y, -z);
}

Vector3 operator + (Vector3 u, Vector3 v){
	return Vector3(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector3 operator - (Vector3 u, Vector3 v){
	return Vector3(u.x - v.x, u.y - v.y, u.z - v.z);
}

//Cross Product
Vector3 operator ^ (Vector3 u, Vector3 v){
	return Vector3( u.y*v.z - u.z*v.y,
				    -u.x*v.z + u.z*v.x, 
					u.x*v.y - u.y*v.x);
}

//Dot Product
float operator*(Vector3 u, Vector3 v) {     
	return (u.x*v.x + u.y*v.y + u.z*v.z); 
}

Vector3 operator*(float s, Vector3 u) {     
	return Vector3(u.x*s, u.y*s, u.z*s); 
}

Vector3 operator*(Vector3 u, float s) {     
	return Vector3(u.x*s, u.y*s, u.z*s); 
}

Vector3 operator/(Vector3 u, float s) {     
	return Vector3(u.x/s, u.y/s, u.z/s); 
}

float TripleScalarProduct(Vector3 u, Vector3 v, Vector3 w) {     
	return float((u.x * (v.y*w.z - v.z*w.y)) + 
		(u.y * (-v.x*w.z + v.z*w.x)) +                         
		(u.z * (v.x*w.y - v.y*w.x))); 
}
