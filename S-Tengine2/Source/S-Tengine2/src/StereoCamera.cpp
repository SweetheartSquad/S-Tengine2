#pragma once

#include <StereoCamera.h>
#include <Keyboard.h>

StereoCamera::StereoCamera() :
	leftEye(new PerspectiveCamera()),
	rightEye(new PerspectiveCamera()),
	ipd(0),
	ipdScale(1),
	ipdScale_ui(1.8),
	ftiming(0),
	sensorSampleTime(0),
	activeCam(this),
	mirrorTexture(nullptr),
	mirrorFBO(0),
	swapTextureSet(nullptr)
{
	leftEye->interpolation = 0;
	rightEye->interpolation = 0;
	
	childTransform->addChild(leftEye);
	childTransform->addChild(rightEye);


	if(sweet::ovrInitialized){
		// Setup Window and Graphics
		// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
		ovrSizei windowSize = { sweet::hmdDesc.Resolution.w / 2, sweet::hmdDesc.Resolution.h / 2 };
		std::cout << windowSize.w << " x " << windowSize.h << std::endl;

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
 
		EyeRenderDesc[0] = ovr_GetRenderDesc(*sweet::hmd, ovrEye_Left, sweet::hmdDesc.DefaultEyeFov[0]);
		EyeRenderDesc[1] = ovr_GetRenderDesc(*sweet::hmd, ovrEye_Right, sweet::hmdDesc.DefaultEyeFov[1]);

		// Turn off vsync to let the compositor do its magic
		//glSwapIntervalEXT(0);
		glfwSwapInterval(0);
	}




	if(sweet::ovrInitialized){
		// setup for oculus rendering
		for(unsigned long int eye = 0; eye < 2; ++eye){
			//eyes[eye].size.w = sweet::hmdDesc.Resolution.w/2;
			//eyes[eye].size.h = sweet::hmdDesc.Resolution.h/2;
			eyes[eye].size = ovr_GetFovTextureSize(*sweet::hmd, ovrEyeType(eye), sweet::hmdDesc.DefaultEyeFov[eye], 1);
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
	}else{
		// setup for non-oculus stereo rendering
		for(unsigned long int eye = 0; eye < 2; ++eye){
			eyes[eye].size = ovr_GetFovTextureSize(*sweet::hmd, ovrEyeType(eye), sweet::hmdDesc.DefaultEyeFov[eye], 1);

			glGenTextures(1, &eyes[eye].tbuffer.texId);
			glBindTexture(GL_TEXTURE_2D, eyes[eye].tbuffer.texId);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, eyes[eye].size.w, eyes[eye].size.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

			glGenFramebuffers(1, &eyes[eye].fboId);
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
	if(activeCam == this){
		return PerspectiveCamera::getViewMatrix();
	}
	return activeCam->getViewMatrix();
}

glm::mat4 StereoCamera::getProjectionMatrix() const{
	return activeCam->getViewMatrix();
}

void StereoCamera::update(Step * _step){
	Keyboard * keyboard = &Keyboard::getInstance();

	if(sweet::ovrInitialized){
		// Get eye poses, feeding in correct IPD offset
		ViewOffset[0] = EyeRenderDesc[0].HmdToEyeViewOffset;
		ViewOffset[1] = EyeRenderDesc[1].HmdToEyeViewOffset;
		ftiming = ovr_GetPredictedDisplayTime(*sweet::hmd, 0);
		ovrTrackingState hmdState = ovr_GetTrackingState(*sweet::hmd, ftiming, ovrTrue);
		ovr_CalcEyePoses(hmdState.HeadPose.ThePose, ViewOffset, EyeRenderPose);
		sensorSampleTime = ovr_GetTimeInSeconds();

		//player->playerCamera->firstParent()->translate(EyeRenderPose[0].Position.x, EyeRenderPose[0].Position.y, EyeRenderPose[0].Position.z);
		//player->playerCamera->childTransform->setOrientation(glm::quat(EyeRenderPose[0].Orientation.w, EyeRenderPose[0].Orientation.z, EyeRenderPose[0].Orientation.y, EyeRenderPose[0].Orientation.x));
	
		if(keyboard->keyJustDown(GLFW_KEY_R)){
			ovr_RecenterPose(*sweet::hmd);
		}

		if(keyboard->keyJustDown(GLFW_KEY_EQUAL)){
			ipdScale += 0.1;
		}else if(keyboard->keyJustDown(GLFW_KEY_MINUS)){
			ipdScale -= 0.1;
		}
	}


	PerspectiveCamera::update(_step);
	
	leftEye->firstParent()->translate(glm::vec3(EyeRenderDesc[0].HmdToEyeViewOffset.x, EyeRenderDesc[0].HmdToEyeViewOffset.y, EyeRenderDesc[0].HmdToEyeViewOffset.z) * -ipdScale, false);
	rightEye->firstParent()->translate(glm::vec3(EyeRenderDesc[1].HmdToEyeViewOffset.x, EyeRenderDesc[1].HmdToEyeViewOffset.y, EyeRenderDesc[1].HmdToEyeViewOffset.z) * -ipdScale, false);

	leftEye->update(_step);
	rightEye->update(_step);
}

void StereoCamera::render(std::function<void()> _renderFunction){
	if(sweet::ovrInitialized){
		// render oculus eyes
		for(unsigned long int eye = 0; eye < 2; ++eye){
			// Increment to use next texture, just before writing
			eyes[eye].tbuffer.TextureSet->CurrentIndex = (eyes[eye].tbuffer.TextureSet->CurrentIndex + 1) % eyes[eye].tbuffer.TextureSet->TextureCount;

			// Switch to eye render target
			// set render surface
			auto tex = reinterpret_cast<ovrGLTexture*>(&eyes[eye].tbuffer.TextureSet->Textures[eyes[eye].tbuffer.TextureSet->CurrentIndex]);

			glBindFramebuffer(GL_FRAMEBUFFER, eyes[eye].fboId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->OGL.TexId, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, eyes[eye].dbuffer.texId, 0);
	
			glViewport(0, 0, eyes[eye].size.w, eyes[eye].size.h);
			glEnable(GL_FRAMEBUFFER_SRGB);
	
			// render scene
			
			if(eye == 0){
				activeCam = leftEye;
			}else{
				activeCam = rightEye;
			}

			_renderFunction();


			/*
			_matrixStack->pushMatrix();
		
			activeCamera->firstParent()->translate(glm::vec3(sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.x, sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.y, sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.z) * -ipdScale);
			uiLayer.childTransform->translate(glm::vec3(sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.x*sweet::getWindowWidth(), sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.y*sweet::getWindowHeight(), sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.z) * -ipdScale_ui);
			
			_renderOptions->clear();
			Scene::render(_matrixStack, _renderOptions);
			uiLayer.render(_matrixStack, _renderOptions);
			_matrixStack->popMatrix();
		
			uiLayer.childTransform->translate(glm::vec3(sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.x*sweet::getWindowWidth(), sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.y*sweet::getWindowHeight(), sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.z) * ipdScale_ui);
			activeCamera->firstParent()->translate(glm::vec3(sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.x, sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.y, sweet::EyeRenderDesc[eye].HmdToEyeViewOffset.z) * ipdScale);
			*/

			// unset render surface
			glBindFramebuffer(GL_FRAMEBUFFER, eyes[eye].fboId);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
			//glDisable(GL_FRAMEBUFFER_SRGB);
		}



		 // Set up positional data.
		ovrViewScaleDesc viewScaleDesc;
		viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
		viewScaleDesc.HmdToEyeViewOffset[0] = ViewOffset[0];
		viewScaleDesc.HmdToEyeViewOffset[1] = ViewOffset[1];

		ovrLayerEyeFov ld;
		ld.Header.Type  = ovrLayerType_EyeFov;
		ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.
	
		ovrRecti recti;
		for (int eye = 0; eye < 2; ++eye)
		{
			recti.Size = eyes[eye].size;
			recti.Pos.x = 0;
			recti.Pos.y = 0;
			ld.ColorTexture[eye] = eyes[eye].tbuffer.TextureSet;
			ld.Viewport[eye]     = recti;
			ld.Fov[eye]          = sweet::hmdDesc.DefaultEyeFov[eye];
			ld.RenderPose[eye]   = EyeRenderPose[eye];
			ld.SensorSampleTime  = sensorSampleTime;
		}
		//ld.Viewport[1].Pos.x = windowSize.w;
		//ld.Viewport[1].Pos.y = windowSize.h;

		ovrLayerHeader* layers = &ld.Header;
		checkForOvrError(ovr_SubmitFrame(*sweet::hmd, 0, &viewScaleDesc, &layers, 1));
		//glfwSwapBuffers(sweet::currentContext);



		
		glfwSwapBuffers(sweet::currentContext);
	}else{
		// standard scene rendering
		_renderFunction();
	}
}

void StereoCamera::blitBack(GLint _targetFbo){
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