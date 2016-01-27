#pragma once

#include <StereoCamera.h>
#include <Keyboard.h>
#include <Extras/OVR_Math.h>

StereoCamera::StereoCamera() :
	ipd(0),
	ipdScale(1),
	ipdScale_ui(1.8),
	ftiming(0),
	sensorSampleTime(0),
	mirrorTexture(nullptr),
	mirrorFBO(0),
	swapTextureSet(nullptr),
	activeCam(OVR::StereoEye_Center),
	hmdOrientation(0,0,0,1)
{
	interpolation = 0; // interpolation screws stuff up

	for(unsigned long int eye = 0; eye < 2; ++eye){
		PerspectiveCamera * cam = new PerspectiveCamera();
		childTransform->addChild(cam);
		eyes[eye].camera = cam;
	}


	if(sweet::ovrInitialized){
		// Setup Window and Graphics
		// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
		ovrSizei windowSize = { sweet::hmdDesc.Resolution.w, sweet::hmdDesc.Resolution.h};
		std::cout << windowSize.w << " x " << windowSize.h << std::endl;

		//fieldOfView = glm::degrees(sweet::hmdDesc.CameraFrustumVFovInRadians);
		//std::cout << "TEST: " << fieldOfView << std::endl;

			/*checkForOvrError(ovr_CreateSwapTextureSetGL(*hmd, GL_RGBA, windowSize.w, windowSize.h, &swapTextureSet));
		Platform.InitDevice(windowSize.w, windowSize.h, reinterpret_cast<LUID*>(&luid));*/

		// Make eye render buffers
		/*for (int eye = 0; eye < 2; ++eye)
		{
			ovrSizei idealTextureSize = ovr_GetFovTextureSize(*hmd, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
			//eyeRenderTexture[eye] = new TextureBuffer(*hmd, true, true, idealTextureSize, 1, NULL, 1);
			//eyeDepthBuffer[eye]   = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);
		}*/

		// Create mirror texture and an FBO used to copy mirror texture to back buffer
		checkForOvrError(ovr_CreateMirrorTextureGL(*sweet::hmd, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture)));

		// Configure the mirror read buffer
		glGenFramebuffers(1, &mirrorFBO);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
		glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	
		for(unsigned long int eye = 0; eye < 2; ++eye){
			EyeRenderDesc[eye] = ovr_GetRenderDesc(*sweet::hmd, (ovrEyeType)eye, sweet::hmdDesc.MaxEyeFov[eye]);
		}

		// Turn off vsync to let the compositor do its magic
		//glSwapIntervalEXT(0);
		glfwSwapInterval(0);
	}




	if(sweet::ovrInitialized){
		// setup for oculus rendering
		for(unsigned long int eye = 0; eye < 2; ++eye){
			eyes[eye].size.w = sweet::hmdDesc.Resolution.w/2;
			eyes[eye].size.h = sweet::hmdDesc.Resolution.h/2;
			//eyes[eye].size = ovr_GetFovTextureSize(*sweet::hmd, ovrEyeType(eye), sweet::hmdDesc.MaxEyeFov[eye], 1);
			checkForOvrError(ovr_CreateSwapTextureSetGL(*sweet::hmd, GL_SRGB8_ALPHA8, eyes[eye].size.w, eyes[eye].size.h, &eyes[eye].tbuffer.TextureSet));

			for (int i = 0; i < eyes[eye].tbuffer.TextureSet->TextureCount; ++i){
				ovrGLTexture* tex = (ovrGLTexture*)&eyes[eye].tbuffer.TextureSet->Textures[i];
				glBindTexture(GL_TEXTURE_2D, tex->OGL.TexId);
		
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glGenFramebuffers(1, &eyes[eye].fboId);


			// depth
			glGenTextures(1, &eyes[eye].dbuffer.texId);
			glBindTexture(GL_TEXTURE_2D, eyes[eye].dbuffer.texId);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			GLenum internalFormat = GL_DEPTH_COMPONENT24;
			GLenum type = GL_UNSIGNED_INT;
			/*if (GLE_ARB_depth_buffer_float)
			{
				internalFormat = GL_DEPTH_COMPONENT32F;
				type = GL_FLOAT;
			}*/

			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, eyes[eye].size.w, eyes[eye].size.h, 0, GL_DEPTH_COMPONENT, type, NULL);
		}
	}
}

StereoCamera::~StereoCamera(){

	if(sweet::ovrInitialized){
		for(unsigned long int eye = 0; eye < 2; ++eye){

			if(sweet::ovrInitialized){
				ovr_DestroySwapTextureSet(*sweet::hmd, eyes[eye].tbuffer.TextureSet);
				eyes[eye].tbuffer.TextureSet = nullptr;
			}else{
				glDeleteTextures(1, &eyes[eye].tbuffer.texId);
			}

			glDeleteFramebuffers(1, &eyes[eye].fboId);
			eyes[eye].fboId = 0;
			glDeleteTextures(1, &eyes[eye].dbuffer.texId);
			eyes[eye].dbuffer.texId = 0;
		}
	}

	if (mirrorFBO){
		glDeleteFramebuffers(1, &mirrorFBO);
	}
	if (mirrorTexture){
		ovr_DestroyMirrorTexture(*sweet::hmd, reinterpret_cast<ovrTexture*>(mirrorTexture));
	}
	for (int eye = 0; eye < 2; ++eye){
		//delete eyeRenderTexture[eye];
		//delete eyeDepthBuffer[eye];
	}
}

glm::mat4 StereoCamera::getViewMatrix() const{
	if(activeCam == OVR::StereoEye_Center){
		return PerspectiveCamera::getViewMatrix();
	}else{
		return eyes[activeCam].camera->getViewMatrix();
	}
}

glm::mat4 StereoCamera::getProjectionMatrix() const{
	if(activeCam == OVR::StereoEye_Center){
		return PerspectiveCamera::getProjectionMatrix();
	}else{
		return eyes[activeCam].camera->getProjectionMatrix();
	}
}

void StereoCamera::update(Step * _step){
	Keyboard * keyboard = &Keyboard::getInstance();

	if(sweet::ovrInitialized){
		// Get eye poses, feeding in correct IPD offset
		ViewOffset[0] = EyeRenderDesc[0].HmdToEyeViewOffset;
		ViewOffset[1] = EyeRenderDesc[1].HmdToEyeViewOffset;
		ftiming = ovr_GetPredictedDisplayTime(*sweet::hmd, 0);

		// Query the HMD for the current tracking state.
		ovrTrackingState hmdState = ovr_GetTrackingState(*sweet::hmd, ftiming, ovrTrue);
		if(hmdState.StatusFlags & (ovrStatus_PositionTracked | ovrStatus_OrientationTracked)){
			ovrPoseStatef pose = hmdState.HeadPose;
			ovr_CalcEyePoses(pose.ThePose, ViewOffset, EyeRenderPose);
			hmdOrientation = pose.ThePose.Orientation;
		}
		sensorSampleTime = ovr_GetTimeInSeconds();
	
		if(keyboard->keyJustDown(GLFW_KEY_R)){
			ovr_RecenterPose(*sweet::hmd);
		}

		/*if(keyboard->keyJustDown(GLFW_KEY_EQUAL)){
			ipdScale += 0.1;
		}else if(keyboard->keyJustDown(GLFW_KEY_MINUS)){
			ipdScale -= 0.1;
		}
		*/
	}

	PerspectiveCamera::update(_step);
	
	for(unsigned long int eye = 0; eye < 2; ++eye){
		PerspectiveCamera * cam = eyes[eye].camera;
		// copy all properties of the central camera to the eye camera
		cam->yaw = yaw;
		cam->pitch = pitch;
		cam->fieldOfView = fieldOfView;
		cam->lastOrientation = lastOrientation;
		cam->forwardVectorLocal = forwardVectorLocal;
		cam->rightVectorLocal = rightVectorLocal;
		cam->upVectorLocal = upVectorLocal;
		cam->nearClip = nearClip;
		cam->farClip = farClip;
		cam->interpolation = interpolation;
		cam->lookAtOffset = lookAtOffset;
		cam->childTransform->setOrientation(childTransform->getOrientationQuat());
		
		float y,p,r;
		if(sweet::ovrInitialized){
			// account for eye position relative to central camera
			cam->firstParent()->translate(glm::vec3(EyeRenderDesc[eye].HmdToEyeViewOffset.x, EyeRenderDesc[eye].HmdToEyeViewOffset.y, EyeRenderDesc[eye].HmdToEyeViewOffset.z) * -ipdScale, false);
			cam->firstParent()->translate(glm::vec3(EyeRenderPose[eye].Position.x, EyeRenderPose[eye].Position.y, EyeRenderPose[eye].Position.z));
			
			// we swap roll and yaw for some reason
			hmdOrientation.GetYawPitchRoll(&y, &p, &r);
			
			cam->yaw   += glm::degrees(y);
			cam->pitch += glm::degrees(p);
			cam->roll  -= glm::degrees(r); // the roll value from the oculus SDK is opposite what we need

			cam->childTransform->setOrientation(cam->calcOrientation());

			cam->rotateVectors(cam->childTransform->getOrientationQuat());
			/*cam->childTransform->rotate(glm::degrees(r), 0, 0, 1, kWORLD);
			cam->childTransform->rotate(glm::degrees(p), 1, 0, 0, kWORLD);
			cam->childTransform->rotate(glm::degrees(y), 0, 1, 0, kWORLD);*/
		}

		cam->update(_step);

		if(sweet::ovrInitialized){
			cam->yaw   -= glm::degrees(y);
			cam->pitch -= glm::degrees(p);
			cam->roll  += glm::degrees(r);
		}
	}
}

void StereoCamera::render(std::function<void()> _renderFunction){
	if(sweet::ovrInitialized){
		// render oculus eyes
		activeCam = OVR::StereoEye_Left;
		renderActiveCamera(_renderFunction);
		activeCam = OVR::StereoEye_Right;
		renderActiveCamera(_renderFunction);
		activeCam = OVR::StereoEye_Center;

		renderFrame();
	}else{
		// standard scene rendering
		activeCam = OVR::StereoEye_Center;
		renderActiveCamera(_renderFunction);
	}
}

void StereoCamera::renderActiveCamera(std::function<void()> _renderFunction){
	// if we're rendering the center cam, just run the render code and return early
	if(activeCam == OVR::StereoEye_Center){
		_renderFunction();
		return;
	}


	// Increment to use next texture, just before writing
	OculusEye & eye = eyes[activeCam];
	eye.tbuffer.TextureSet->CurrentIndex = (eye.tbuffer.TextureSet->CurrentIndex + 1) % eye.tbuffer.TextureSet->TextureCount;
			
	// Switch to eye render target
	// set render surface
	auto tex = reinterpret_cast<ovrGLTexture*>(&eye.tbuffer.TextureSet->Textures[eye.tbuffer.TextureSet->CurrentIndex]);

	glBindFramebuffer(GL_FRAMEBUFFER, eye.fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, eye.dbuffer.texId, 0);
	
	glViewport(0, 0, eye.size.w, eye.size.h);
	glEnable(GL_FRAMEBUFFER_SRGB);
	
	// render scene
	_renderFunction();

	glDisable(GL_FRAMEBUFFER_SRGB);

	// unset render surface
	glBindFramebuffer(GL_FRAMEBUFFER, eye.fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
}

void StereoCamera::renderFrame(){
	 // Set up positional data.
	ovrViewScaleDesc viewScaleDesc;
	viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
	viewScaleDesc.HmdToEyeViewOffset[0] = ViewOffset[0];
	viewScaleDesc.HmdToEyeViewOffset[1] = ViewOffset[1];

	ovrLayerEyeFov ld;
	ld.Header.Type  = ovrLayerType_EyeFov;
	ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft | ovrLayerFlag_HighQuality;   // Because OpenGL.
	
	ovrRecti recti;
	for (int eye = 0; eye < 2; ++eye)
	{
		recti.Size = eyes[eye].size;
		recti.Pos.x = 0;
		recti.Pos.y = 0;
		ld.ColorTexture[eye] = eyes[eye].tbuffer.TextureSet;
		ld.Viewport[eye]     = recti;
		ld.Fov[eye]          = sweet::hmdDesc.MaxEyeFov[eye];
		ld.RenderPose[eye]   = EyeRenderPose[eye];
		ld.SensorSampleTime  = sensorSampleTime;
	}

	ovrLayerHeader* layers = &ld.Header;
	checkForOvrError(ovr_SubmitFrame(*sweet::hmd, 0, &viewScaleDesc, &layers, 1));
}

void StereoCamera::blitTo(GLint _targetFbo){
	if(sweet::ovrInitialized){
		// get the current fbo bindings
		GLint readFboId = 0, drawFboId = 0;
		glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
		glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
		
		// Blit mirror texture to back buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _targetFbo);
		GLint w = mirrorTexture->OGL.Header.TextureSize.w;
		GLint h = mirrorTexture->OGL.Header.TextureSize.h;
		glBlitFramebuffer(0, h, w, 0,
							0, 0, w, h,
							GL_COLOR_BUFFER_BIT, GL_NEAREST);
		
		// reassign the fbo bindings to what they were
		glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFboId);
	}
}