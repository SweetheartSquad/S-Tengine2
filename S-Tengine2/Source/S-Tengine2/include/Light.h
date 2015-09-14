#pragma once

#include <Entity.h>

#include <glm/glm.hpp>

enum LightType {
	kPOINT_LIGHT = 0,
	kDIRECTIONAL_LIGHT = 1,
	kSPOT_LIGHT = 2
};

struct LightData {
	LightType type;
	glm::vec3 intensities;
	float ambientCoefficient;
	float attenuation;
	float cutoff;
	float coneAngle;
	glm::vec3 coneDirection;
	
	LightData(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff, float _coneAngle, glm::vec3 _coneDirection);
	
	bool operator==(const LightData &other) const;
	bool operator!=(const LightData &other) const;
};

/**************************************************************************************************
*
* A basic light node. 
* Stores a reference to a transform and a struct containing data used by lighting shaders, including
* position, color, ambient coefficient, and attentuation. 
*
****************************************************************************************************/
class Light abstract : public Entity{
public:

	virtual void update(Step * _step) override;
	virtual void unload() override;

	explicit Light(LightType _type, glm::vec3 _intensities, float _ambientCoefficient, float _attenuation, float _cutoff, float _coneAngle, glm::vec3 _coneDirection);
	~Light();

	glm::vec3 lastPos;

	bool lightDirty;

    glm::vec3 getIntensities();
	
	void  setIntensities(const glm::vec3 _intensities);
	float getAmbientCoefficient();
	void  setAmbientCoefficient(float _ambientCoefficient);
	float getAttenuation();
	void  setAttenuation(float _attenuation);
	float getCutoff();
	void  setCutoff(float _cutoff);
	LightType getType();

protected:
	/**
	* The data struct which is passed to the shader
	* This should be copied in the shader
	*/
	LightData data;
};