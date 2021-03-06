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
	orbitalCameraRadius(3.f),
	steeringMode(kTWO_WHEEL),
	chassisMesh(nullptr)
{

    m_fEngineForce = 0.0f;
	m_fBrakingForce = 0.0f;

    m_fmaxEngineForce = 2500.f;//this should be engine/velocity dependent
    m_fmaxBrakingForce = 100.f;

    m_fVehicleSteering = 0.0f;
	m_frollInfluence = 0.01f;//1.0f;

	m_tuning.m_suspensionStiffness = 14.f;//20.f;
    m_tuning.m_suspensionDamping = 2.0f;//2.3f;
    m_tuning.m_suspensionCompression = 4.0f;//4.4f;
	m_tuning.m_frictionSlip = 1000;// (wheel friction) BT_LARGE_FLOAT;
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

void BulletVehicle::addWheelDefinition(glm::vec3 _pos, MeshInterface * _mesh, bool _front){
	BulletVehicleWheelDefinition def;
	def.pos = _pos;
	def.mesh = _mesh;
	def.isFrontWheel = _front;

	sweet::Box bbox = _mesh->calcBoundingBox();
	def.radius = bbox.height * 0.5f;

	
	def.steeringMultiplier = 1.f;
	def.engineMultiplier = 1.f;
	def.brakingMultiplier= 1.f;

	wheelDefinitions.push_back(def);
}

void BulletVehicle::addChassisDefinition(MeshInterface * _chassisMesh){
	assert(chassisMesh == nullptr);
	chassisMesh = _chassisMesh;
}

void BulletVehicle::init(BulletWorld * _world, Shader * _shader){
	float mass = 500;
	m_tuning.m_suspensionStiffness = mass*0.1f;//20.f;  
	float linearDamping = 0.2f;
	float angularDamping = 0.5f;

	chassis = new BulletMeshEntity(_world, chassisMesh, _shader);
	childTransform->addChild(chassis);
	
	sweet::Box chassisBbox = chassisMesh->calcBoundingBox();
	m_tuning.m_suspensionCompression = chassisBbox.height*0.25f;

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

	for(auto d : wheelDefinitions){
		// add wheels to bullet vehicle
		btVector3 connectionPointCS0(chassisBbox.width*d.pos.x, 0, chassisBbox.depth*d.pos.z);
		btWheelInfo & wheel = m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, chassisBbox.height*d.pos.y, d.radius, m_tuning, d.isFrontWheel);
        wheel.m_rollInfluence = m_frollInfluence;

		MeshEntity * wheelNode = new MeshEntity(d.mesh, _shader); // TODO: Wheel mesh, shader
		childTransform->addChild(wheelNode);
		wheels.push_back(wheelNode);

		// flip the left wheels
		wheelNode->meshTransform->scale((chassisBbox.width*d.pos.x >= 0.0 ? -1 : 1), 1, 1);
	}

	// realign the wheels after creation
    m_vehicle->resetSuspension();
	realignWheels();

   
	// setup orbital camera
	MousePerspectiveCamera * c = new MousePerspectiveCamera();
	c->nearClip = 0.001f;
	c->farClip = 1000;
	orbitalCameraController = c->controller;
	orbitalCamera = c;
	childTransform->addChild(orbitalCamera);
}


void BulletVehicle::update(Step * _step){
	BulletController::update(_step);
	Entity::update(_step);

	// update orbital camera
	if(orbitalCamera->pitch > -10){
		orbitalCamera->pitch = -10;
	}else if(orbitalCamera->pitch < -80){
		orbitalCamera->pitch = -80;
	}
	orbitalCamera->firstParent()->translate(chassis->meshTransform->getWorldPos() - orbitalCamera->forwardVectorRotated * orbitalCameraRadius, false);
}

void BulletVehicle::handleInputs(Step * _step, glm::vec3 _inputs){
	// parse inputs
	float steering = _inputs.x, accelerator = _inputs.z, brake = 0.f;


    // calculate steering angle
	if (glm::abs(steering) > FLT_EPSILON){
		// pull towards target steering angle
		currentSteeringAngle = currentSteeringAngle * 0.95f + steering * 0.05f;
    }else{
		// if we're not steering, pull the wheels back to zero
        currentSteeringAngle *= 0.8f;
    }
	m_fVehicleSteering = currentSteeringAngle;
	
	m_fEngineForce = m_fmaxEngineForce * accelerator;
	m_fBrakingForce = m_fmaxBrakingForce * brake;
	
	// if we're turning or accelerating, wake up the rigid body
    if( glm::abs(steering) + glm::abs(accelerator) >= FLT_EPSILON){
		chassis->body->activate();
    }

	// deal with wheels
    updateWheels();
	realignWheels();
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
	const BulletVehicleWheelDefinition & w = wheelDefinitions.at(_index);
	m_vehicle->setSteeringValue((_reverse ? -m_fVehicleSteering : m_fVehicleSteering) * w.steeringMultiplier, _index);
	m_vehicle->applyEngineForce(m_fEngineForce * w.engineMultiplier, _index);
	if(m_fEngineForce < 0){
		m_vehicle->setBrake(m_fBrakingForce * w.brakingMultiplier, _index);
	}
}

void BulletVehicle::realignWheels(){
	unsigned long int numWheels = m_vehicle->getNumWheels();
    for (unsigned long int i = 0; i < m_vehicle->getNumWheels(); ++i){
		// get wheel properties
		m_vehicle->updateWheelTransform(i, false);
		btTransform transform = m_vehicle->getWheelTransformWS(i);
		btVector3 origin = transform.getOrigin();
		btQuaternion rotation = transform.getRotation();

		// update wheel mesh to reflect physics simulation
		MeshEntity * pWheel = wheels[i];
		pWheel->firstParent()->translate( glm::vec3(origin.x(), origin.y(), origin.z()), false );
		pWheel->firstParent()->setOrientation(glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z()));
    }
}

void BulletVehicle::enable(){
	BulletController::enable();
}
void BulletVehicle::disable(){
	BulletController::disable();
}