#pragma once

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>


class Keyboard;
class Mouse;
class MousePerspectiveCamera;

struct BulletVehicleWheelDefinition{
	// position of the wheel expressed as a percentage of the chassis
	glm::vec3 pos;
	// mesh representing the wheel (bounding box of mesh is used to determine wheel's radius
	MeshInterface * mesh;
	// whether the wheel is treated as a front wheel or a back wheel
	bool isFrontWheel;
	// wheel radius
	float radius;


	float steeringMultiplier;
	float engineMultiplier;
	float brakingMultiplier;
};

// Partially based on the forklift demo and on the one found here: http://urho3d.prophpbb.com/topic1354.html
class BulletVehicle : public Entity{
public:
	MousePerspectiveCamera * orbitalCamera;
	float orbitalCameraRadius;

	enum SteeringMode{
		kTWO_WHEEL, // front wheels power vehicle
		kFOUR_WHEEL, // front wheels and back wheels power vehicle (back turns opposite to front)
	} steeringMode;

    BulletVehicle();
    ~BulletVehicle();
    
	// adds a wheel definition
	// _pos is the wheel's position expressed as a percentage of the chassis's bounding box
	// NOTE: must be called prior to init or it won't do anthing
	void addWheelDefinition(glm::vec3 _pos, MeshInterface * _wheelMesh, bool _front);

    // actually create the vehicle
    void init(BulletWorld * _world, MeshInterface * _chassisMesh, Shader * _shader);

    virtual void update(Step * _step) override;
	
    // updates an individual wheel with steering angle and engine/breaking forces
	// if _reverse, steering angle is reversed
	void updateWheel(unsigned long int _index, bool _reverse);


	// calculates steering angle and engine/braking forces
	void handleInput();
	// update wheels with steering angle and engine/breaking forces
	// wheels are updated differently based on steering type
	void updateWheels();
	// repositions wheels based on rigid body and vehicle properties
	void realignWheels();
    
	BulletMeshEntity * chassis;

	void enable();
	void disable();
	bool isEnabled();

	std::vector<MeshEntity * > wheels;
	std::vector<BulletVehicleWheelDefinition> wheelDefinitions;
protected:
	bool enabled;
	Mouse * mouse;
	Keyboard * keyboard;
    
    // Current left/right steering angle (-1 to 1)
    float currentSteeringAngle;

    // raycast vehicle
    btRaycastVehicle::btVehicleTuning m_tuning;
    btVehicleRaycaster * m_vehicleRayCaster;
    btRaycastVehicle * m_vehicle;
	

    float m_fEngineForce;
    float m_fBrakingForce;

    float m_fmaxEngineForce;
    float m_fmaxBrakingForce;

    float m_fVehicleSteering;
    float m_fsteeringIncrement;
    float m_fsteeringClamp;
    float m_frollInfluence;
};