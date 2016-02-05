#pragma once

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>


class Keyboard;
class Mouse;
class MousePerspectiveCamera;

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
    

    /// Initialize the vehicle. Create rendering and physics components. Called by the application.
    void Init(BulletWorld * _world, MeshInterface * _chassisMesh, MeshInterface * _wheelMesh, Shader * _chassisShader, Shader * _wheelShader);

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
protected:
	Mouse * mouse;
	Keyboard * keyboard;
    
    // Current left/right steering angle (-1 to 1)
    float currentSteeringAngle;

    // raycast vehicle
    btRaycastVehicle::btVehicleTuning m_tuning;
    btVehicleRaycaster * m_vehicleRayCaster;
    btRaycastVehicle * m_vehicle;

    std::vector<MeshEntity *> wheels;

    float m_fEngineForce;
    float m_fBreakingForce;

    float m_fmaxEngineForce;
    float m_fmaxBreakingForce;

    float m_fVehicleSteering;
    float m_fsteeringIncrement;
    float m_fsteeringClamp;
    float m_fwheelRadius;
    float m_fwheelWidth;
    float m_frollInfluence;
    float m_fsuspensionRestLength;
};