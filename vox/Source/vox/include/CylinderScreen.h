#include "MeshEntity.h"
#include "MeshInterface.h"
#include "Resource.h"

class Step;

class CylinderScreen : public MeshEntity{
public:
	
	float * control;
	float prevControlValue;
	float speed;

	void update(Step * _step);

	CylinderScreen(float _speed, float * _control, Texture * _texture = nullptr, MeshInterface * _mesh = Resource::loadMeshFromObj("../assets/layer.vox"), Transform * _transform = new Transform(), Shader * _shader = nullptr);
};