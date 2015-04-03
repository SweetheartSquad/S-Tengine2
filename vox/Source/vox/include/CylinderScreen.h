#include "MeshEntity.h"
#include "MeshInterface.h"
#include "Resource.h"

class Step;

class CylinderScreen : public MeshEntity{
public:
	
	Transform * control;
	float prevControlValue;
	float speed;
	int numLevels;

	void update(Step * _step);

	CylinderScreen(float _speed, Transform * _control, int _numLevels = 4, Texture * _texture = nullptr, MeshInterface * _mesh = Resource::loadMeshFromObj("../assets/layer.vox"), Transform * _transform = new Transform(), Shader * _shader = nullptr);
};