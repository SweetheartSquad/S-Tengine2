#pragma once

#include "BulletVehicle.h"

#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

#include <MeshFactory.h>
#include <MeshInterface.h>

#include <sweet/Input.h>
#include <MousePerspectiveCamera.h>


BulletVehicle::BulletVehicle() :
	currentSteeringAngle(0.0f),
	keyboard(&Keyboard::getInstance()),
	mouse(&Mouse::getInstance()),
	orbitalCameraRadius(3.f),
	steeringMode(kTWO_WHEEL)
{

    m_fEngineForce = 0.0f;                                                 
    m_fBreakingForce = 0.0f;                                               
                                                                        
    m_fmaxEngineForce = 2500.f;//this should be engine/velocity dependent  
    m_fmaxBreakingForce = 100.f;                                           
                                                                        
    m_fVehicleSteering = 0.0f;                                             
    m_fsteeringIncrement = 0.04f;                                          
    m_fsteeringClamp = 0.3f;                                               
    m_fwheelRadius = 0.5f;                                                 
    m_fwheelWidth = 0.4f;                         
	m_frollInfluence = 0.01f;//1.0f;         
	m_fsuspensionRestLength = 0.6f;//0.6         

	m_tuning.m_suspensionStiffness = 14.f;//20.f;                                 
    m_tuning.m_suspensionDamping = 2.0f;//2.3f;                                    
    m_tuning.m_suspensionCompression = 4.0f;//4.4f;       
	m_tuning.m_frictionSlip = 1000;// (wheel friction) BT_LARGE_FLOAT;    ;
	m_tuning.m_maxSuspensionForce;
	m_tuning.m_maxSuspensionTravelCm;

    m_vehicleRayCaster = nullptr;
    m_vehicle = nullptr;

    wheels.clear();
}


BulletVehicle::~BulletVehicle(){
    delete m_vehicleRayCaster;
    delete m_vehicle;
    
    wheels.clear();
}
void BulletVehicle::Init(BulletWorld * _world, MeshInterface * _chassisMesh, MeshInterface * _wheelMesh, Shader * _chassisShader, Shader * _wheelShader){
	float mass = 500;
	m_tuning.m_suspensionStiffness = mass*0.01f;//20.f;  
	float linearDamping = 0.2f;
	float angularDamping = 0.5f;

	chassis = new BulletMeshEntity(_world, _chassisMesh, _chassisShader); // TODO: chassis mesh, shader
	childTransform->addChild(chassis);
	
	sweet::Box chassisBbox = _chassisMesh->calcBoundingBox();
	sweet::Box wheelBbox = _wheelMesh->calcBoundingBox();
	m_fwheelRadius = wheelBbox.height * 0.5f;
	m_fsuspensionRestLength = chassisBbox.height*0.25f;
	m_tuning.m_suspensionCompression = wheelBbox.height;

	chassis->setColliderAsBoundingBox();

	chassis->createRigidBody(mass);
	chassis->body->setDamping(linearDamping, angularDamping);
    
	// setup bullet vehicle
    m_vehicleRayCaster = new btDefaultVehicleRaycaster( _world->world );
	m_vehicle = new btRaycastVehicle( m_tuning, chassis->body, m_vehicleRayCaster );
	_world->world->addVehicle( m_vehicle );
	
	// 0,1,2 = x,y,z
    int rightIndex = 0;
    int upIndex = 1;
    int forwardIndex = 2;
    m_vehicle->setCoordinateSystem( rightIndex, upIndex, forwardIndex );

    float connectionHeight = -0.4f;//1.2f;
    bool isFrontWheel=true;
    btVector3 wheelDirectionCS0(0,-1,0);
    btVector3 wheelAxleCS(-1,0,0);

	btVector3 connectionPointCS0(chassisBbox.width*0.4f, -chassisBbox.height*0.25f, chassisBbox.depth*0.3f);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_fsuspensionRestLength, m_fwheelRadius, m_tuning, isFrontWheel);

    connectionPointCS0 = btVector3(-chassisBbox.width*0.4f, -chassisBbox.height*0.25f, chassisBbox.depth*0.3f);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_fsuspensionRestLength, m_fwheelRadius, m_tuning, isFrontWheel);

    isFrontWheel = false;
    connectionPointCS0 = btVector3(-chassisBbox.width*0.4f, -chassisBbox.height*0.25f, -chassisBbox.depth*0.3f);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_fsuspensionRestLength, m_fwheelRadius, m_tuning, isFrontWheel);

    connectionPointCS0 = btVector3(chassisBbox.width*0.4f, -chassisBbox.height*0.25f, -chassisBbox.depth*0.3f);
    m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, m_fsuspensionRestLength, m_fwheelRadius, m_tuning, isFrontWheel);
	unsigned long int numWheels = m_vehicle->getNumWheels();
    for (unsigned long int i = 0; i < numWheels; ++i){
        btWheelInfo & wheel = m_vehicle->getWheelInfo(i);
        wheel.m_rollInfluence = m_frollInfluence;	
	}

    m_vehicle->resetSuspension();

    for (unsigned long int i = 0; i < numWheels; ++i){
        // create wheels
		MeshEntity * wheelNode = new MeshEntity(_wheelMesh, _wheelShader); // TODO: Wheel mesh, shader
		childTransform->addChild(wheelNode);
		wheels.push_back(wheelNode);

		// flip the left wheels
		wheelNode->meshTransform->scale((m_vehicle->getWheelInfo(i).m_chassisConnectionPointCS.x() >= 0.0 ? -1 : 1), 1, 1);
    }
	// realign the wheels after creation
	realignWheels();

   
	// setup orbital camera
	orbitalCamera = new MousePerspectiveCamera();
	orbitalCamera->nearClip = 0.001f;
	orbitalCamera->farClip = 1000;
	childTransform->addChild(orbitalCamera);
}


void BulletVehicle::update(Step * _step){
    handleInput();
    updateWheels();
	realignWheels();
	
	Entity::update(_step);

	// update orbital camera
	if(orbitalCamera->pitch > -10){
		orbitalCamera->pitch = -10;
	}else if(orbitalCamera->pitch < -80){
		orbitalCamera->pitch = -80;
	}
	orbitalCamera->firstParent()->translate(chassis->meshTransform->getWorldPos() - orbitalCamera->forwardVectorRotated * orbitalCameraRadius, false);
}

void BulletVehicle::handleInput(){
	float newSteering = 0.0f;
    float accelerator = 0.0f;

    // Read controls
	if (keyboard->keyDown(GLFW_KEY_A)){
        newSteering = 1.0f;
	}if (keyboard->keyDown(GLFW_KEY_D)){
        newSteering = -1.0f;
	}

    // calculate steering angle
	if (glm::abs(newSteering) > FLT_EPSILON){
		// pull towards target steering angle
		currentSteeringAngle = currentSteeringAngle * 0.95f + newSteering * 0.05f;
    }else{
		// if we're not steering, pull the wheels back to zero
        currentSteeringAngle *= 0.8f;
    }
	m_fVehicleSteering = currentSteeringAngle;

    // calculate forces
    if (keyboard->keyDown(GLFW_KEY_W)){
        accelerator = 1.0f;
	}if (keyboard->keyDown(GLFW_KEY_S)){
        accelerator = -0.5f;
	}
	m_fEngineForce = m_fmaxEngineForce * accelerator;
    m_fBreakingForce = 0.f;
	
	// wake up the rigid body if needed
    if( glm::abs(newSteering) >= FLT_EPSILON || glm::abs(accelerator) != FLT_EPSILON ){
		chassis->body->activate();
    }
}

void BulletVehicle::updateWheels(){
	// update wheels with steering angle and engine/breaking forces
    int wheelIndex;
	switch(steeringMode){
	case kFOUR_WHEEL:
		updateWheel(3, true);
		updateWheel(2, true);
		updateWheel(1, false);
		updateWheel(0, false);
		break;
	case kTWO_WHEEL:
		updateWheel(1, false);
		updateWheel(0, false);
		break;
	}
}

void BulletVehicle::updateWheel(unsigned long int _index, bool _reverse){
	m_vehicle->setSteeringValue(_reverse ? -m_fVehicleSteering : m_fVehicleSteering, _index);
    m_vehicle->applyEngineForce(m_fEngineForce, _index);
	if(m_fEngineForce < 0){
		m_vehicle->setBrake(m_fBreakingForce, _index);
	}
}

void BulletVehicle::realignWheels(){
	unsigned long int numWheels = m_vehicle->getNumWheels();
    for (unsigned long int i = 0; i < m_vehicle->getNumWheels(); ++i){
		// get wheel properties
		m_vehicle->updateWheelTransform( i, false );
		btTransform transform = m_vehicle->getWheelTransformWS(i);
		btVector3 origin = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();

		// update wheel mesh to reflect physics simulation
		MeshEntity * pWheel = wheels[i];
		pWheel->firstParent()->translate( glm::vec3(origin.x(), origin.y(), origin.z()), false );
		pWheel->firstParent()->setOrientation(glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    }
}