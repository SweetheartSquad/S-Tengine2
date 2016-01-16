#pragma once

#include <Sweet.h>
#include <Log.h>

ovrHmd * sweet::hmd = nullptr;
bool sweet::ovrInitialized = false;
ovrGLTexture * sweet::mirrorTexture = nullptr;
GLuint sweet::mirrorFBO = 0;
ovrSwapTextureSet * sweet::swapTextureSet = nullptr;
ovrEyeRenderDesc sweet::EyeRenderDesc[2];
ovrGraphicsLuid sweet::luid;
ovrHmdDesc sweet::hmdDesc;

void sweet::initOVR(){
	// OVR init (oculus support)
	
	/*Initialize LibOVR through ovr_Initialize.
	Call ovr_Create and check the return value to see if it succeeded. You can periodically poll for the presence of an HMD with ovr_GetHmdDesc(nullptr).
	Integrate head-tracking into your application’s view and movement code. This involves:
	Obtaining predicted headset orientation for the frame through a combination of the GetPredictedDisplayTime and ovr_GetTrackingState calls.
	Applying Rift orientation and position to the camera view, while combining it with other application controls.
	Modifying movement and game play to consider head orientation.
	Initialize rendering for the HMD.
	Select rendering parameters such as resolution and field of view based on HMD capabilities.
	See: ovr_GetFovTextureSize andovr_GetRenderDesc.
	Configure rendering by creating D3D/OpenGL-specific swap texture sets to present data to the headset.
	See: ovr_CreateSwapTextureSetD3D11 andovr_CreateSwapTextureSetGL.
	Modify application frame rendering to integrate HMD support and proper frame timing:
	Make sure your engine supports rendering stereo views.
	Add frame timing logic into the render loop to obtain correctly predicted eye render poses.
	Render each eye’s view to intermediate render targets.
	Submit the rendered frame to the headset by calling ovr_SubmitFrame.
	Customize UI screens to work well inside of the headset.
	Destroy the created resources during shutdown.
	See: ovr_DestroySwapTextureSet, ovr_Destroy, and ovr_Shutdown.*/
	
	
	checkForOvrError(ovr_Initialize(nullptr));
	hmd = new ovrHmd();
	checkForOvrError(ovr_Create(hmd, &luid));
	hmdDesc = ovr_GetHmdDesc(*hmd);
	ovrInitialized = hmdDesc.Type != 0;
}

void sweet::initOVR2(){
	if(ovrInitialized){
		// Setup Window and Graphics
		// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
		ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };
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
		checkForOvrError(ovr_CreateMirrorTextureGL(*hmd, GL_SRGB8_ALPHA8, windowSize.w, windowSize.h, reinterpret_cast<ovrTexture**>(&mirrorTexture)));

		// Configure the mirror read buffer
		glGenFramebuffers(1, &mirrorFBO);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mirrorTexture->OGL.TexId, 0);
		glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
 
		EyeRenderDesc[0] = ovr_GetRenderDesc(*hmd, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
		EyeRenderDesc[1] = ovr_GetRenderDesc(*hmd, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

		// Turn off vsync to let the compositor do its magic
		//glSwapIntervalEXT(0);
		glfwSwapInterval(0);
	}
}

void sweet::destructOVR(){
	if(ovrInitialized){
		if (mirrorFBO){
			glDeleteFramebuffers(1, &mirrorFBO);
		}
		if (mirrorTexture){
			ovr_DestroyMirrorTexture(*hmd, reinterpret_cast<ovrTexture*>(mirrorTexture));
		}
		for (int eye = 0; eye < 2; ++eye)
		{
			//delete eyeRenderTexture[eye];
			//delete eyeDepthBuffer[eye];
		}
		//Platform.ReleaseDevice();


		ovr_Destroy(*hmd);
		ovr_Shutdown();
		hmd = nullptr;
		ovrInitialized = false;
	}
}