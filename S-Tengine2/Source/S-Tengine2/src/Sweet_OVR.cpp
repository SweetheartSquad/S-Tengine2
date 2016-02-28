#pragma once

#include <Sweet.h>
#include <Log.h>

ovrHmd * sweet::hmd = nullptr;
bool sweet::ovrInitialized = false;
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

void sweet::destructOVR(){
	if(ovrInitialized){
		ovr_Destroy(*hmd);
		ovr_Shutdown();
		hmd = nullptr;
		ovrInitialized = false;
	}
}