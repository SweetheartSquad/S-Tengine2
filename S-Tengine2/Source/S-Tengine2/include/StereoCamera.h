#pragma once

#include <PerspectiveCamera.h>
#include <Sweet.h>
#include <functional>
#include <Extras/OVR_StereoProjection.h>


class Scene;

// A stereo-camera setup designed for use with an Oculus Rift (tested on DK2 hardware with SDK v0.8)
// TODO: implement eye buffers as extensions of our FBO class
class StereoCamera : public PerspectiveCamera {
private:
	// inter-pupillary distance, i.e. the distance between the two cameras
	float ipd;

	
	struct OculusEye{
		PerspectiveCamera * camera;
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
	OVR::StereoEye activeCam;

	OVR::Quatf hmdOrientation;
public:
	bool useHeadTrackingPosition;
	bool useHeadTrackingOrientation;
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

	// To render to an Oculus, the _renderFunction is called twice (once for each eye camera)
	// and then the buffers are submitted to the mirrorTexture as a single frame
	// If an hmd is not connected, calls renderStandard
	void renderStereo(std::function<void()> _renderFunction);
	// calls _renderFunction from the central camera's point of view without any additional changes
	void renderStandard(std::function<void()> _renderFunction);
	// renders the active camera by calling _renderFunction
	// if the active camera is one of the eye cams, it renders to the eye framebuffer
	// if the active camera is the center camera, nothing special happens
	void renderActiveCamera(std::function<void()> _renderFunction);
	// uses positional data and eye buffers to submit a frame to the oculus
	// also calls glfwSwapBuffers on the current context
	void renderFrame();

	// Copies the mirror texture to the provided fbo 
	void blitTo(GLint _targetFbo = 0);
};