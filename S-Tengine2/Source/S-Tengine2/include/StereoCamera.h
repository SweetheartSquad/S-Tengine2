#pragma once

#include <PerspectiveCamera.h>
#include <Sweet.h>
#include <functional>


class Scene;

class StereoCamera : public PerspectiveCamera {
private:
	PerspectiveCamera * leftEye;
	PerspectiveCamera * rightEye;

	// inter-pupillary distance, i.e. the distance between the two cameras
	float ipd;

	
	struct OculusEye{
		// width and height of the texture and depth buffers
		ovrSizei			size;
		GLuint              fboId;
		struct TextureBuffer{
			// oculus target
			ovrSwapTextureSet*  TextureSet;
			// non-oculus target
			GLuint              texId;
		} tbuffer;
		struct DepthBuffer{
			GLuint        texId;
		} dbuffer;
	} eyes[2];
	ovrVector3f ViewOffset[2];
	ovrPosef    EyeRenderPose[2];
	double ftiming;
	double sensorSampleTime;
	float ipdScale;
	float ipdScale_ui;
	
	
	ovrGLTexture * mirrorTexture;
	GLuint mirrorFBO;
	ovrSwapTextureSet * swapTextureSet;
	ovrEyeRenderDesc EyeRenderDesc[2];
	

	// the activeCam points to either this camera, this camera's leftEye, or this camera's rightEye
	// when rendering, the view matrix is calculated based on the activeCam
	Camera * activeCam;
public:
	StereoCamera();
	~StereoCamera();

	/**
	* @return The view matrix of the active camera
	*/
	virtual glm::mat4 getViewMatrix() const override;

	/**
	* @return The projection matrix of the active camera 
	*/
	virtual glm::mat4 getProjectionMatrix() const override;

	/** Updates the left and right eye to match the main camera */
	virtual void update(Step * _step) override;


	void render(std::function<void()> _renderFunction);

		// Render what the oculus sees to the provided fbo 
	void blitBack(GLint _targetFbo = 0);
};