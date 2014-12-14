#pragma once

#include "CinderApp.h"
#include "UI.h"
#include "ButtonFunctions.hpp"

#include "Commands/CMD_CreateJoint.h"
#include "Commands/CMD_DeleteJoints.h"
#include "Commands/CMD_SelectNodes.h"
#include "Commands/CMD_TranslateSelectedTransformables.h"
#include "Commands/CMD_ScaleSelectedTransformables.h"
#include "Commands/CMD_RotateSelectedTransformables.h"
#include "Commands/CMD_KeyProperty.h"
#include "Commands/CMD_KeyAllProperties.h"
#include "Commands/CMD_ClearAllProperties.h"
#include "Commands/CMD_ParentSelectedNodes.h"
#include "Commands/CMD_PlaceVoxel.h"
#include "Commands/CMD_DeleteVoxel.h"
#include "Commands/CMD_SetTime.h"
#include "Commands/CMD_UpdateTrackbar.h"

#include "Transform.h"
#include "node/NodeTransformable.h"

#include "Tween.h"
#include "Step.h"

#include "MatrixStack.h"
#include "CinderRenderOptions.h"

#include "ToolBar.h"
#include "ToolSet.h"
#include "ToolButton.h"
#include "ParamTextBox.h"
#include "TrackBar.h"

#include "ConsoleGUI.h"

void CinderApp::prepareSettings(Settings *settings){
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup(){

	TriMesh test;
	ObjLoader objLoader(loadFile("../assets/sphere.vox"));
	objLoader.load(&test, true, false, true);
	sphere = gl::VboMesh(test);

	ObjLoader objLoader2(loadFile("../assets/cube.vox"));
	objLoader2.load(&test, true, false, true);
	cube = gl::VboMesh(test);

	sceneRoot = new SceneRoot();
	cmdProc = new CommandProcessor();

	sourceCam = nullptr;
	sourceRect = nullptr;
	sourceFbo = nullptr;
	sourceBounds = nullptr;
	activeButton = nullptr;

	uiColour = 0;
	clickedUiColour = 0;
	
	translateSpace = kWORLD;
	rotateSpace = kOBJECT;
	scaleSpace = kOBJECT;
	
	channel = 0;

	mode = kCREATE;

	drawParams = true;

	viewJointsOnly = false;
	voxelSelectMode = false;
	voxelPreviewMode = false;
	voxelPreviewResolution = 0.1f;
	voxelSphereRadius = 0.1f;
	voxelPaintSpacing = 1.f;

	params = params::InterfaceGl::create( getWindow(), "General", toPixels( Vec2i( 175, 360 ) ), ColorA(0.6f, 0.3f, 0.3f, 0.4f));
	
	params->addText("Animation");
	params->addParam("Time", &UI::time, "", true);
	params->addParam("Interpolation", UI::interpolationNames, &UI::interpolationValue);
	params->addButton("Add/Edit Keyframe", std::bind(&CinderApp::setKeyframe, this));
	params->addButton("Clear Keyframe", std::bind(&CinderApp::clearKeyframe, this));
	
	params->addSeparator();
	params->addText("Voxels");

	params->addParam("Resolution", &voxelPreviewResolution, "min=0.01, step=0.01");
	params->addParam("Radius", &voxelSphereRadius, "min=0.01, step=0.01");
	params->addParam("Paint Spacing", &voxelPaintSpacing, "min=0.01, step=0.5");

	params->addSeparator();
	params->addText("Save Menu");

	params->addParam("Directory", &directory);
	params->addParam("File Name", &fileName);
	params->addButton("Save", std::bind( &CinderApp::saveSkeleton, this ));
	params->addSeparator();
	params->addText("Load Menu");
	params->addParam("Bones File", &filePath);
	params->addButton("Load", std::bind(&CinderApp::loadSkeleton, this));
	params->addSeparator();
	params->addParam("Message", &message, "", true);

	stringstream paramsOptions;
	paramsOptions << "position='" << getWindowWidth() - 175 - 5 << " " << getWindowHeight() / 2.4f << "'";
	string blah = paramsOptions.str();
	params->setOptions("", paramsOptions.str());
	
	consoleGUI = new ConsoleGUI(15, &cmdProc->consoleEntries, 3);

	// note: we will setup our camera in the 'resize' function,
	// because it is called anyway so we don't have to set it up twice

	// load shaders
	loadShaders();

	// set background color
	mColorBackground = Color(0.0f, 0.0f, 0.0f);
	


	//// test animation
	/*vector<Tween *> tweens;
	tweens.push_back(new Tween(2, 0.1, Easing::Type::kNONE));
	tweens.push_back(new Tween(2, -0.5, Easing::Type::kNONE));
	
	cmdProc->executeCommand(new CMD_CreateJoint(&joints, Vec3f(0.f, 0.f, 0.f), nullptr));
	cmdProc->executeCommand(new CMD_CreateJoint(&joints, Vec3f(0,1,0), joints.at(0)));

	joints.at(0)->translateZ.tweens = tweens;*/
	
	timelineTrackbar = new TrackBar(&UI::time, Vec2i(0, 5), Vec2i(getWindowWidth(), 15), Vec2i(10, 25), 0, 24, 1, &mMousePos);

	play = false;
	previousTime = 0;
	toolbar = new ToolBar(Vec2i(5, 30), true);
	// Selecting, Creating, Painting, and Transformations
	toolbar->addSet(new ToolSet());

	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/select.png", nullptr, &ButtonFunctions::MODE_Select));
	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/translate.png", nullptr, &ButtonFunctions::MODE_Translate));
	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/rotate.png", nullptr, &ButtonFunctions::MODE_Rotate));
	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/scale.png", nullptr, &ButtonFunctions::MODE_Scale));
	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/joint.png", nullptr, &ButtonFunctions::MODE_CreateJoints));
	toolbar->addButton(0, new ToolButton("", ToolButton::Type::kRADIO, Vec2i(40, 40), "../assets/icons/voxel.png", nullptr, &ButtonFunctions::MODE_PaintVoxels));

	timelineBar = new ToolBar(Vec2i(50, 30), false);
	
	// timeline controls
	timelineBar->addSet(new ToolSet());
	// channel
	timelineBar->addSet(new ToolSet());
	// View Joints, Voxels, or All
	timelineBar->addSet(new ToolSet());
	// coordinate-space
	timelineBar->addSet(new ToolSet());

    timelineBar->addButton(0, new ToolButton("", ToolButton::Type::kNORMAL, Vec2i(30, 30), "../assets/icons/prev.png", nullptr, &ButtonFunctions::TIME_Decrement));
	timelineBar->addButton(0, new ToolButton("", ToolButton::Type::kTOGGLE, Vec2i(30, 30), "../assets/icons/play.png", nullptr, &ButtonFunctions::TIME_PlayPause));
	timelineBar->addButton(0, new ToolButton("", ToolButton::Type::kNORMAL, Vec2i(30, 30), "../assets/icons/next.png", nullptr, &ButtonFunctions::TIME_Increment));
								 
	timelineBar->addButton(1, new ToolButton("Joints Only", ToolButton::Type::kTOGGLE, Vec2i(50, 30), "../assets/icons/null.png", nullptr, &ButtonFunctions::VOXEL_ShowHide));
	timelineBar->addButton(1, new ToolButton("Voxel Preview", ToolButton::Type::kTOGGLE, Vec2i(50, 30), "../assets/icons/null.png", nullptr, &ButtonFunctions::VOXEL_Preview));
	timelineBar->addButton(1, new ToolButton("Voxel Preview", ToolButton::Type::kTOGGLE, Vec2i(50, 30), "../assets/icons/null.png", nullptr, &ButtonFunctions::VOXEL_Selectable));

	timelineBar->addButton(2, new ToolButton("Object", ToolButton::Type::kRADIO, Vec2i(40, 30), "../assets/icons/null.png", nullptr, &ButtonFunctions::SPACE_Object));
	timelineBar->addButton(2, new ToolButton("World", ToolButton::Type::kRADIO, Vec2i(40, 30), "../assets/icons/null.png", nullptr, &ButtonFunctions::SPACE_World));
	
	timelineBar->addButton(3, new ToolButton("1", ToolButton::Type::kRADIO, Vec2i(20, 20), "../assets/icons/null.png", nullptr, &ButtonFunctions::CHANNEL_0));
	timelineBar->addButton(3, new ToolButton("2", ToolButton::Type::kRADIO, Vec2i(20, 20), "../assets/icons/null.png", nullptr, &ButtonFunctions::CHANNEL_1));
	timelineBar->addButton(3, new ToolButton("3", ToolButton::Type::kRADIO, Vec2i(20, 20), "../assets/icons/null.png", nullptr, &ButtonFunctions::CHANNEL_2));
	timelineBar->addButton(3, new ToolButton("4", ToolButton::Type::kRADIO, Vec2i(20, 20), "../assets/icons/null.png", nullptr, &ButtonFunctions::CHANNEL_3));
	
	//timeTextBox = new ParamTextBox(ParamTextBox::Type::NUMBER, Vec2i(60, 40), Vec2i(30,20));

	code = 0;
	active = false;

	// press active buttons
	dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(4))->pressProgrammatically(this);
	dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(2))->children.at(0))->pressProgrammatically(this);
	dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(3))->children.at(0))->pressProgrammatically(this);
}

void CinderApp::resize(){
	float w = (float)max(1, getWindowWidth());
	float h = (float)max(1, getWindowHeight());
	float r = getWindowAspectRatio();
	Area b = getWindowBounds();
	rectWindow = b;
	
	float w2 = w/2.f;
	float h2 = h/2.f;
	float w3 = w/3.f;
	float h3 = h/3.f;
	
	rectTop.set(0.f, 0.f, w3, h3);
	rectRight.set(w3, 0.f, w3+w3, h3);
	rectFront.set(w3+w3, 0.f, w3+w3+w3, h3);

	rectPersp.set(0.f, h3, w, h);
	
	boundsTop.set(	-r,	-1.f,	r,	1.f);
	boundsRight.set(-r,	-1.f,	r,	1.f);
	boundsFront.set(-r,	-1.f,	r,	1.f);
	//boundsPersp.set(-1, -h/w, 1, h/w);
	
	// setup the camera
	CameraPersp cam = camMayaPersp.getCamera();
	cam.setPerspective( 60.0f, rectPersp.getAspectRatio(), 0.1f, 10000.0f );
	camMayaPersp.setCurrentCam( cam );

	camTop.setOrtho(boundsTop.x1, boundsTop.x2, boundsTop.y1, boundsTop.y2, -10000.f, 10000.f);
	camRight.setOrtho(boundsRight.x1, boundsRight.x2, boundsRight.y1, boundsRight.y2, -10000.f, 10000.f);
	camFront.setOrtho(boundsFront.x1, boundsFront.x2, boundsFront.y1, boundsFront.y2, -10000.f, 10000.f);
	
	/*camTop.setEyePoint(Vec3f(0.f, 0.f, 0.f));
	camRight.setEyePoint(Vec3f(0.f, 0.f, 0.f));
	camFront.setEyePoint(Vec3f(0.f, 0.f, 0.f));*/
	camTop.lookAt(Vec3f(0.f, 1.f, 0.f), Vec3f(0.f, 0.f, 0.f), Vec3f(1.f, 0.f, 0.f));
	camRight.lookAt(Vec3f(1.f, 0.f, 0.f), Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f));
	camFront.lookAt(Vec3f(0.f, 0.f, 1.f), Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f));

	camTop.setCenterOfInterestPoint(Vec3f(0.f, 0.f, 0.f));
	camRight.setCenterOfInterestPoint(Vec3f(0.f, 0.f, 0.f));
	camFront.setCenterOfInterestPoint(Vec3f(0.f, 0.f, 0.f));
	
	camTop.setViewDirection(Vec3f(0.f, -1.f, 0.f));
	camRight.setViewDirection(Vec3f(-1.f, 0.f, 0.f));
	camFront.setViewDirection(Vec3f(0.f, 0.f, -1.f));

	// create or resize framebuffer if needed
	if(!fboUI || fboUI.getWidth() != w || fboUI.getHeight() != h){
		initMultiChannelFbo(fboUI, b, 2);
	}
	
	if(!fboPersp || fboPersp.getWidth() != rectPersp.getWidth() || fboPersp.getHeight() != rectPersp.getHeight()) {
		initMultiChannelFbo(fboPersp, rectPersp.getInteriorArea(), 4);
	}if(!fboTop || fboTop.getWidth() != rectTop.getWidth() || fboTop.getHeight() != rectTop.getHeight()){
		initMultiChannelFbo(fboTop, rectTop.getInteriorArea(), 4);
	}if(!fboRight || fboRight.getWidth() != rectRight.getWidth() || fboRight.getHeight() != rectRight.getHeight()){
		initMultiChannelFbo(fboRight, rectRight.getInteriorArea(), 4);
	}if(!fboFront || fboFront.getWidth() != rectFront.getWidth() || fboFront.getHeight() != rectFront.getHeight()){
		initMultiChannelFbo(fboFront, rectFront.getInteriorArea(), 4);
	}

	consoleGUI->resize(this);
	timelineTrackbar->resize(this);
}


void CinderApp::shutdown(){
	UI::selectedNodes.clear();
	delete toolbar;
	toolbar = nullptr;
	delete timelineBar;
	timelineBar = nullptr;
   // delete timeTextBox;
   // timeTextBox = nullptr;
	delete sceneRoot;
	sceneRoot = nullptr;
	delete cmdProc;
	cmdProc = nullptr;
	delete consoleGUI;
	consoleGUI = nullptr;
	delete timelineTrackbar;
	timelineTrackbar = nullptr;
}

void CinderApp::update(){
	// play animation or edit
	if(timelineTrackbar->isDown){
		cmdProc->executeCommand(new CMD_UpdateTrackbar(timelineTrackbar));
	}else{
		timelineTrackbar->update(nullptr);
	}

	step.time = getElapsedSeconds();

	if(play){
		step.targetFrameDuration = static_cast<double>(1) / 60;
		step.setDeltaTime(step.time - step.lastTimestamp);
		step.deltaTimeCorrection = step.getDeltaTime()/step.targetFrameDuration;

		if(UI::time + step.getDeltaTime() > timelineTrackbar->max){
			step.setDeltaTime(step.getDeltaTime() - timelineTrackbar->max);
		}else if(UI::time + step.getDeltaTime() < timelineTrackbar->min){
			step.setDeltaTime(step.getDeltaTime() + timelineTrackbar->min);
		}
		cmdProc->executeCommand(new CMD_SetTime(&UI::time, UI::time+(float)step.getDeltaTime(), false));
		app::console() << "CinderApp.update() play UI::time: " << UI::time << endl;
		console() << "previousTime: " << previousTime << endl;
		for(unsigned long int i = 0; i < sceneRoot->children.size(); ++i){
			NodeUpdatable * nu = dynamic_cast<NodeUpdatable *>(sceneRoot->children.at(i));
			nu->update(&step);
		}

		//UI::time += (float)s.getDeltaTime();
		previousTime = UI::time;
	}else{
		if(UI::time != previousTime){
			console() << "CinderApp.update() !play UI::time: " << UI::time << endl;
			console() << "previousTime: " << previousTime << endl;
			Step s;
			s.setDeltaTime((UI::time - previousTime));
			for(unsigned long int i = 0; i < sceneRoot->children.size(); ++i){
				NodeUpdatable * nu = dynamic_cast<NodeUpdatable *>(sceneRoot->children.at(i));
				nu->update(&s);
			}
			previousTime = UI::time;
		}
	}
	step.lastTimestamp = step.time;
	
	float r = getWindowAspectRatio();

	Vec3f dif = camMayaPersp.getCamera().getEyePoint() - camMayaPersp.getCamera().getCenterOfInterestPoint();
	float zoom = dif.length();

	boundsTop.set(	-r,	-1.f,	r,	1.f);
	boundsRight.set(-r,	-1.f,	r,	1.f);
	boundsFront.set(-r,	-1.f,	r,	1.f);
	
	boundsTop.scale(zoom);
	boundsRight.scale(zoom);
	boundsFront.scale(zoom);

	camTop.setOrtho(-boundsTop.x1, -boundsTop.x2, boundsTop.y2, boundsTop.y1, -10000, 10000);
	camFront.setOrtho(boundsFront.x1, boundsFront.x2, -boundsFront.y2, -boundsFront.y1, -10000, 10000);
	camRight.setOrtho(boundsRight.x1, boundsRight.x2, -boundsRight.y2, -boundsRight.y1, -10000, 10000);
	
	camTop.setEyePoint(camMayaPersp.getCamera().getEyePoint());
	camFront.setEyePoint(camMayaPersp.getCamera().getEyePoint());
	camRight.setEyePoint(camMayaPersp.getCamera().getEyePoint());
}

void CinderApp::draw(){
	renderScene(fboPersp, camMayaPersp.getCamera());
	renderScene(fboTop, camTop);
	renderScene(fboRight, camRight);
	renderScene(fboFront, camFront);

	fboUI.bindFramebuffer();
		// set viewport to the size of the FBO
		gl::setViewport( fboUI.getBounds() );
		gl::enableDepthRead();
		gl::enableDepthWrite();
		gl::enableAlphaBlending();

		uiShader.bind();
		uiShader.uniform("tex", false);
		gl::clear(ColorA(0.f, 0.f, 0.f, 0.f));

		renderUI(camMayaPersp.getCamera(), rectPersp);
		renderUI(camTop, rectTop);
		renderUI(camRight, rectRight);
		renderUI(camFront, rectFront);
		
		gl::disableDepthRead();
		gl::disableDepthWrite();

		gl::setViewport(fboUI.getBounds());

		vox::MatrixStack t;
		CinderRenderOptions t2(nullptr, nullptr);
		t2.ciShader = &uiShader;

		timelineTrackbar->render(&t, &t2);
		
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			// draw keyframes
			NodeAnimatable * na = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
			if(na != nullptr){
				glLineWidth(2.f);
				gl::color(1.f, (float)i/(float)UI::selectedNodes.size(), 0.f);

				// should check each animation object individually
				if(na->translateX->hasStart){
					glBegin(GL_LINES);
					float time = (UI::time - na->translateX->currentAnimationTime);
					float timeStart = time;
					glVertex2f(timelineTrackbar->pos.x + time*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y);
					glVertex2f(timelineTrackbar->pos.x + time*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y+timelineTrackbar->size.y);
				
					for(unsigned long int anim = 0; anim < na->translateX->tweens.size(); ++anim){
						time += na->translateX->tweens.at(anim)->deltaTime;

						glVertex2f(timelineTrackbar->pos.x + time*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y);
						glVertex2f(timelineTrackbar->pos.x + time*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y+timelineTrackbar->size.y);
					}

					glVertex2f(timelineTrackbar->pos.x + timeStart*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y+timelineTrackbar->size.y/2+i);
					glVertex2f(timelineTrackbar->pos.x + time*((float)timelineTrackbar->size.x/timelineTrackbar->max), timelineTrackbar->pos.y+timelineTrackbar->size.y/2+i);
					
					glEnd();
				}
				glLineWidth(1.f);
			}
		}
		uiShader.uniform("pickingColor", Vec3f(0.f, 0.f, 0.f));

		uiShader.uniform("tex", true);
		uiShader.uniform("pickingColor", Color(0.f, 0.f, 0.f));
		consoleGUI->render(&t, &t2);
		toolbar->render(&t, &t2);
		timelineBar->render(&t, &t2);
        //timeTextBox->render(&t, &t2);
		uiShader.unbind();

		params->draw();

		gl::disableAlphaBlending();
	fboUI.unbindFramebuffer();

	

	// draw the scene
	gl::enableAlphaBlending();
	
	gl::color( ColorA(1.f, 1.f, 1.f, 1.f) );
	
	gl::draw( fboTop.getTexture(channel), fboTop.getBounds(), rectTop );
	gl::drawStrokedRect(rectTop);
	
	gl::draw( fboRight.getTexture(channel), fboRight.getBounds(), rectRight );
	gl::drawStrokedRect(rectRight);
	
	gl::draw( fboFront.getTexture(channel), fboFront.getBounds(), rectFront );
	gl::drawStrokedRect(rectFront);
	
	gl::draw( fboPersp.getTexture(channel), fboPersp.getBounds(), rectPersp );
	gl::drawStrokedRect(rectPersp);
	
	gl::draw( fboUI.getTexture(0), getWindowBounds() );
	
	// draw the picking framebuffer in the upper right corner
	if(mPickingFboJoint){
		Rectf rct((Rectf)mPickingFboJoint.getBounds() * 5.f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0.f) );
		gl::draw( mPickingFboJoint.getTexture(0), rct );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1, rct.x2, rct.y2));
	}
	
	// draw the picking framebuffers in the corner
	/*if(pickingFboUI){
		Rectf rct((Rectf)pickingFboUI.getBounds() * 5.f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0.f) );
		gl::draw( pickingFboUI.getTexture(0), Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2));
	}
	if(pixelFbo){
		Rectf rct((Rectf)pixelFbo.getBounds() * 5.0f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0.f) );
		gl::draw( pixelFbo.getTexture(0), Rectf(rct.x1, rct.y1+rct.y2+rct.y2, rct.x2, rct.y2+rct.y2+rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1+rct.y2+rct.y2, rct.x2, rct.y2+rct.y2+rct.y2));
	}*/
}

void CinderApp::renderScene(gl::Fbo & fbo, const Camera & cam){
	fbo.bindFramebuffer();

	// clear background
	gl::clear( ColorA(mColorBackground.r, mColorBackground.g, mColorBackground.b, active ? 0.05f : 1.f) );

	// specify the camera matrices
	gl::pushMatrices();
	gl::setMatrices(cam);

		// set viewport to the size of the FBO
		gl::setViewport( fbo.getBounds() );

		drawGrid(10, 1);

		// setup the light
		gl::Light light( gl::Light::POINT, 0 );
		light.setAmbient( Color::white()/*black()*/ );
		light.setDiffuse( Color::white() );
		light.setSpecular( Color::white() );
		light.setPosition( camMayaPersp.getCamera().getEyePoint() );

		// specify render states
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
		gl::enableDepthRead();
		gl::enableDepthWrite();


		// bind phong shader, which renders to both our color targets.
		//  See 'shaders/phong.frag'
		jointShader.bind();

		jointShader.uniform("availableForVoxel", true);

		// draw joints:
		vox::MatrixStack mStack;
		jointShader.uniform("viewMatrix", cam.getModelViewMatrix());
		jointShader.uniform("projectionMatrix", cam.getProjectionMatrix());

		CinderRenderOptions cro(nullptr, nullptr);
		cro.ciCam = &cam;
		cro.ciShader = &jointShader;
		cro.voxelPreviewMode = voxelPreviewMode;
		cro.voxelPreviewResolution = voxelPreviewResolution;
		cro.voxelSphereRadius = voxelSphereRadius;
		cro.viewJointsOnly = viewJointsOnly;
		cro.sphere = &sphere;
		cro.cube = &cube;

		for(unsigned long int i = 0; i < sceneRoot->children.size(); ++i){
			NodeRenderable * nr = dynamic_cast<NodeRenderable *>(sceneRoot->children.at(i));
			if(nr != nullptr){
				nr->render(&mStack, &cro);
			}
		}

		// unbind shader
		jointShader.unbind();

	// restore matrices
	gl::popMatrices();

	// restore render states
	glPopAttrib();

	fbo.unbindFramebuffer();
}

void CinderApp::renderUI(const Camera & cam, const Rectf & rect){
	gl::pushMatrices();

	// set viewport to the size of the FBO
	
	gl::setViewport(Area(rect.x1, fboUI.getHeight()-rect.y1, rect.x2, fboUI.getHeight()-rect.y2));

	gl::setMatrices(cam);
	
	gl::color(ColorA(1.f, 1.f, 1.f, 1.f));

	uiShader.uniform("pickingColor", Color(0.f, 0.f, 0.f));
	if(mode == kCREATE){
		Ray ray = camMayaPersp.getCamera().generateRay((float)mMousePos.x/rectPersp.getWidth(), 1.f-((float)(mMousePos.y-rect.y1)/rectPersp.getHeight()), camMayaPersp.getCamera().getAspectRatio());			
		float distance = 0.f;
		if(ray.calcPlaneIntersection(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f), &distance)){
			Vec3f jointWillGoHere = ray.calcPosition(distance);
			
			gl::enableWireframe();
			gl::pushMatrices();
			gl::translate(jointWillGoHere);
			gl::scale(0.06f, 0.06f, 0.06f);
			gl::draw(sphere);
			gl::popMatrices();
			gl::disableWireframe();
		}
	}

	if(UI::selectedNodes.size() != 0){
		gl::pushMatrices();
		
			glPointSize(5);
			// Draw spheres around the selected joints
			gl::enableWireframe();
			for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){

				if (i == UI::selectedNodes.size() - 1){
					gl::color(0.f, 1.f, 1.f);
				}
				Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
				if(j != nullptr){
					gl::pushMatrices();
						glm::vec3 absPos = j->getPos(false);
						gl::translate(absPos.x, absPos.y, absPos.z);
						gl::scale(0.06f, 0.06f, 0.06f);
						gl::draw(sphere);
					gl::popMatrices();

					if(j->voxels.size() > 0){
						//glBegin(GL_POINTS);
						Vec3f * verts = (Vec3f *) malloc(sizeof(Vec3f) * j->voxels.size());
						uint32_t * indices = (uint32_t *) malloc(sizeof(uint32_t) * j->voxels.size());
						for(unsigned long int i = 0; i < j->voxels.size(); ++i){
							Voxel * v = dynamic_cast<Voxel*>(j->voxels.at(i));
							if(v != nullptr){
								glm::vec3 pos = v->getPos(false);
								verts[i] = Vec3f(pos.x, pos.y, pos.z);
								indices[i] = i;
								//glVertex3f(pos.x, pos.y, pos.z);
							}
						}
						//glEnd();
						glVertexPointer(3, GL_FLOAT, 0, verts);

						glEnableClientState( GL_VERTEX_ARRAY );
						glDisableClientState( GL_NORMAL_ARRAY );
						glDisableClientState( GL_COLOR_ARRAY );	
						glDisableClientState( GL_TEXTURE_COORD_ARRAY );

						glDrawElements(GL_POINTS, j->voxels.size(), GL_UNSIGNED_INT, indices);
						free(indices);
						free(verts);

						glDisableClientState( GL_VERTEX_ARRAY );
						glDisableClientState( GL_NORMAL_ARRAY );
						glDisableClientState( GL_COLOR_ARRAY );
						glDisableClientState( GL_TEXTURE_COORD_ARRAY );
					}

				}else{
					Voxel * v = dynamic_cast<Voxel *>(UI::selectedNodes.at(i));
					if(v != nullptr){
						glBegin(GL_POINTS);
						glm::vec3 pos = v->getPos(false);
						glVertex3f(pos.x, pos.y, pos.z);
						glEnd();
					}
				}
			}
			gl::disableWireframe();

			switch(mode){
			case kTRANSLATE:
				gl::translate(UI::displayHandlePos);
				break;
			case kROTATE:
			case kSCALE:
				gl::translate(UI::handlePos);
				break;
			}

			gl::lineWidth(2);
			if(cam.isPersp()){
				float zoom = cam.getEyePoint().distance(UI::handlePos);
				gl::scale(zoom, zoom, zoom);
				gl::scale(-1,-1,-1);
			}

			switch(mode){
			case kTRANSLATE:
				if(translateSpace == kOBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != nullptr){
						std::vector<glm::quat> rotateStack;
						while(j != nullptr){
							rotateStack.push_back(j->transform->orientation);
							j = dynamic_cast<Joint *>(j->parent);
						}
						while(rotateStack.size() > 1){
							glm::quat t = rotateStack.at(rotateStack.size()-1);
							gl::rotate(Quatf(t.w, t.x, t.y, t.z));
							rotateStack.pop_back();
						}
					}
				}
				// Draw the axes
				gl::lineWidth((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.f, 0.3f), 0.15f, 0.03f);
				
				gl::lineWidth((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.3f, 0.f, 0.f), 0.15f, 0.03f);
				
				gl::lineWidth((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.3f, 0.f), 0.15f, 0.03f);
				
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.f, 0.f, 0.));
				gl::color((clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::scale(0.05f, 0.05f, 0.05f);
				gl::draw(sphere);
				break;

			case kROTATE:
				if(rotateSpace == kOBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != nullptr){
						std::vector<glm::quat> rotateStack;
						while(j != nullptr){
							rotateStack.push_back(j->transform->orientation);
							j = dynamic_cast<Joint *>(j->parent);
						}
						while(rotateStack.size() > 0){
							glm::quat t = rotateStack.at(rotateStack.size()-1);
							gl::rotate(Quatf(t.w, t.x, t.y, t.z));
							rotateStack.pop_back();
						}
					}
				}

				// Draw the three circles
				gl::lineWidth((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawStrokedCircle(Vec2f(0.f, 0.f), 0.3f, 32);
				
				gl::scale(0.9f, 0.9f, 0.9f);
				gl::rotate(Vec3f(0.f, 90.f, 0.f));
				gl::lineWidth((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawStrokedCircle(Vec2f(0.f, 0.f), 0.3f, 32);

				gl::scale(0.9f, 0.9f, 0.9f);
				gl::rotate(Vec3f(90.f, 0.f, 0.f));
				gl::lineWidth((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawStrokedCircle(Vec2f(0.f, 0.f), 0.3f, 32);
				
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.f, 0.f, 0.));
				gl::color((clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::scale(0.05f, 0.05f, 0.05f);
				gl::draw(sphere);
				break;

			case kSCALE:
				if(scaleSpace == kOBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != nullptr){
						std::vector<glm::quat> rotateStack;
						while(j != nullptr){
							rotateStack.push_back(j->transform->orientation);
							j = dynamic_cast<Joint *>(j->parent);
						}
						while(rotateStack.size() > 0){
							glm::quat t = rotateStack.at(rotateStack.size()-1);
							gl::rotate(Quatf(t.w, t.x, t.y, t.z));
							rotateStack.pop_back();
						}
					}
				}

				// Draw the axes
				gl::lineWidth((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x0000FF || clickedUiColour == 0) ? Color(0.f, 0.f, 1.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.f, 0.3f), 0.15f, 0.03f);
				
				gl::lineWidth((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0xFF0000 || clickedUiColour == 0) ? Color(1.f, 0.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.3f, 0.f, 0.f), 0.15f, 0.03f);
				
				gl::lineWidth((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? 5.f : 2.5f);
				uiShader.uniform("pickingColor", (clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.f, 0.f, 0.f));
				gl::color((clickedUiColour == 0x00FF00 || clickedUiColour == 0) ? Color(0.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::drawVector(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 0.3f, 0.f), 0.15f, 0.03f);
				
				uiShader.uniform("pickingColor", (clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.f, 0.f, 0.));
				gl::color((clickedUiColour == 0xFFFF00 || clickedUiColour == 0) ? Color(1.f, 1.f, 0.f) : Color(0.25f, 0.25f, 0.25f));
				gl::scale(0.05f, 0.05f, 0.05f);
				gl::draw(sphere);
				break;
			}// end switch
			gl::lineWidth(1);
		gl::popMatrices();
	}

	// restore matrices
	gl::popMatrices();
}

void CinderApp::mouseMove( MouseEvent event ){
	mMousePos = event.getPos();

	if(!event.isLeftDown() && !event.isRightDown()){
		pickColour(&uiColour, &fboUI, &rectWindow, &pickingFboUI, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
		if(NodeSelectable::pickingMap.count(uiColour) == 1){
			UiInteractable * newButt = dynamic_cast<UiInteractable *>(NodeSelectable::pickingMap.at(uiColour));
			if(activeButton != nullptr){
				if(activeButton != newButt){
					if(activeButton->isHovered){
						activeButton->out();
						
					}
				}
			}
			activeButton = newButt;
			if(activeButton != nullptr){
				if(!activeButton->isHovered){
					activeButton->in();
				}
			}
		}else{
			if(activeButton != nullptr){
				if(activeButton->isHovered){
					activeButton->out();
                    if(dynamic_cast<ParamTextBox *>(activeButton) == nullptr){
					    activeButton = nullptr;
					}
				}
			}
		}
	}
}

void CinderApp::pickColour(void * res, const gl::Fbo * _sourceFbo, const Rectf *  _sourceRect, gl::Fbo * _destFbo, Vec2i _pos, Area _area, unsigned long int _channel, GLenum _type){
	if(res != nullptr){
		if(_type == GL_FLOAT){
			*((GLfloat *)res) = 0;
		}else{
			*((unsigned long int *)res) = 0;
		}
		if(_sourceFbo != nullptr && _destFbo != nullptr){
			// first, specify a small region around the current cursor position 
			float scaleX = _sourceFbo->getWidth() / (float) _sourceRect->getWidth();
			float scaleY = _sourceFbo->getHeight() / (float) _sourceRect->getHeight();
			Vec2i pixel((int)((_pos.x - _sourceRect->x1) * scaleX), (int)((_sourceRect->y2 - _pos.y) * scaleY));

			//pixel = fromRectToRect(pixel, sourceFbo->getBounds(), *sourceRect);

			//Area	area(pixel.x-5, pixel.y-5, pixel.x+5, pixel.y+5);
			_area.moveULTo(pixel-Vec2i(_area.getWidth()/2, _area.getHeight()/2));

			// next, we need to copy this region to a non-anti-aliased framebuffer
			//  because sadly we can not sample colors from an anti-aliased one. However,
			//  this also simplifies the glReadPixels statement, so no harm done.
			//  Here, we create that non-AA buffer if it does not yet exist.
			if(!*_destFbo || _destFbo->getWidth() != _area.getWidth() || _destFbo->getHeight() != _area.getHeight()) {
				initFbo(*_destFbo, _area);
			}
	
			// bind the picking framebuffer, so we can clear it and then read its pixels later
			_destFbo->bindFramebuffer();
			gl::clear();

			glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
			// (Cinder does not yet provide a way to handle multiple color targets in the blitTo function, 
			//  so we have to make sure the correct target is selected before calling it)
			glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, _sourceFbo->getId() );
			glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, _destFbo->getId() );
		
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT+_channel);
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

			_sourceFbo->blitTo(*_destFbo, _area, _destFbo->getBounds());
		
			// read pixel value(s) in the area
			glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		
			// calculate the total number of pixels
			unsigned long int total = (_area.getWidth() * _area.getHeight());
			void * buffer = nullptr;
			switch(_type){
			case GL_FLOAT:
				buffer = malloc(sizeof(GLfloat) * total*4); // make sure this is large enough to hold 4 bytes for every pixel!
				break;
			case GL_UNSIGNED_BYTE:
			default:
				buffer = malloc(sizeof(GLubyte) * total*4); // make sure this is large enough to hold 4 bytes for every pixel!
				break;
			}
			glReadPixels(0, 0, _area.getWidth(), _area.getHeight(), GL_RGBA, _type, (void *)buffer);

			// unbind the picking framebuffer
			_destFbo->unbindFramebuffer();
		
			// now that we have the color information, count each occuring color
			unsigned int max = 0;
			if(_type == GL_FLOAT){
				Color color(0,0,0);
				std::map<Color, unsigned long int> occurences;
				for(size_t i = 0; i < total; ++i) {
					color = Color(((GLfloat *)buffer)[i],((GLfloat *)buffer)[i+1],((GLfloat *)buffer)[i+2]);
					occurences[color]++;
				}
			
				// find the most occuring color
				std::map<Color, unsigned long int>::const_iterator itr;
				for(itr = occurences.begin(); itr != occurences.end(); ++itr) {
					//console() << itr->first << "\t" << itr->second << std::endl;
					if(itr->second > max) {
						color = itr->first;
						max = itr->second;
					}
					total ++;
				}

				// if this color is present in at least 50% of the pixels, we can safely assume that it is indeed belonging to one object
				if(max >= (total / 2)) {
					*((Color *)res) = color;
				}
			}else{
				unsigned long int color = 0;
				std::map<unsigned long int, unsigned long int> occurences;
				for(size_t i = 0; i < total; ++i) {
					color = charToInt( ((GLubyte *)buffer)[(i*4)+0], ((GLubyte *)buffer)[(i*4)+1], ((GLubyte *)buffer)[(i*4)+2] );
					occurences[color]++;
				}
			
				// find the most occuring color
				std::map<unsigned long int, unsigned long int>::const_iterator itr;
				for(itr = occurences.begin(); itr != occurences.end(); ++itr) {
					//console() << itr->first << "\t" << itr->second << std::endl;
					if(itr->second > max) {
						color = itr->first;
						max = itr->second;
					}
					total ++;
				}

				// if this color is present in at least 50% of the pixels, we can safely assume that it is indeed belonging to one object
				if(max >= (total / 2)) {
					*((unsigned long int *)res) = color;
				}
			}
			if(buffer != nullptr){
				free(buffer);
				buffer = nullptr;
			}
		}
	}else{
		// Error: no result variable provided
	}
}

void CinderApp::mouseDown( MouseEvent event ){
	isMouseDown = true;
	mMousePos = event.getPos();
	
	cmdProc->startCompressing();
	console() << "startCompressing" << endl;
	
	// handle the camera
	camMayaPersp.mouseDown( mMousePos );
	
	if(rectTop.contains(mMousePos)){
		sourceCam = &camTop;
		sourceRect = &rectTop;
		sourceFbo = &fboTop;
		sourceBounds = &boundsTop;
	}else if(rectFront.contains(mMousePos)){
		sourceCam = &camFront;
		sourceRect = &rectFront;
		sourceFbo = &fboFront;
		sourceBounds = &boundsFront;
	}else if(rectRight.contains(mMousePos)){
		sourceCam = &camRight;
		sourceRect = &rectRight;
		sourceFbo = &fboRight;
		sourceBounds = &boundsRight;
	}else if(rectPersp.contains(mMousePos)){
		sourceCam = &camMayaPersp.getCamera();
		sourceRect = &rectPersp;
		sourceFbo = &fboPersp;
		sourceBounds = nullptr;
	}else{
		sourceCam = nullptr;
		sourceRect = nullptr;
		sourceFbo = nullptr;
		sourceBounds = nullptr;
	}
	
	
	// Get the selected UI colour
	pickColour(&clickedUiColour, &fboUI, &rectWindow, &pickingFboUI, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
	uiColour = clickedUiColour;

	if(event.isLeft()){
		oldMousePos = mMousePos;
	}

	if(!event.isAltDown() && clickedUiColour == 0 && !play){
		if(sourceCam == &camMayaPersp.getCamera()){
			if(mode == kPAINT_VOXELS){
				if(UI::selectedNodes.size() == 1 && (dynamic_cast<Joint *>(UI::selectedNodes.at(0)) != NULL)){
					if(event.isLeft()){
						// place voxel
						Color voxel;
						pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 3, GL_FLOAT);
						if(voxel != Color(0.f, 0.f, 0.f)){
							Vec3f voxelPos = Vec3f(voxel.r, voxel.g, voxel.b);
							console() << "voxelPos: " << voxelPos << endl;
							cmdProc->executeCommand(new CMD_PlaceVoxel(voxelPos, dynamic_cast<Joint *>(UI::selectedNodes.back())));
							lastVoxelPaintPos = voxelPos;
							currentSpacingDistance = 0;
						}else{
							console() << "bg" << std::endl;
						}
					}else{
						// delete voxel
						unsigned long int voxel;
						pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
						if(NodeSelectable::pickingMap.count(voxel) != 0){
							Voxel * t = dynamic_cast<Voxel *>(NodeSelectable::pickingMap.at(voxel));
							if(t != nullptr){
								cmdProc->executeCommand(new CMD_DeleteVoxel(t));
							}
						}
					}
				}
			}else if(mode == kCREATE){
				if(event.isLeft()){
					Ray ray = camMayaPersp.getCamera().generateRay((float)mMousePos.x/rectPersp.getWidth(), 1.f-((float)(mMousePos.y-rectPersp.y1)/rectPersp.getHeight()), camMayaPersp.getCamera().getAspectRatio());			
					float distance = 0.f;
					if(ray.calcPlaneIntersection(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f), &distance)){
						Vec3f pos = ray.calcPosition(distance);
						if(UI::selectedNodes.size() == 1){
							cmdProc->executeCommand(new CMD_CreateJoint(sceneRoot, pos, dynamic_cast<Joint *>(UI::selectedNodes.at(0))));
						}else{
							cmdProc->executeCommand(new CMD_CreateJoint(sceneRoot, pos, nullptr));
						}
					}
				}
			}else if(mode == kSELECT){
				if(event.isLeft()){
					unsigned long int pickedColour;
					pickColour(&pickedColour, sourceFbo, sourceRect, &mPickingFboJoint, mMousePos, Area(0,0,5,5), 1, GL_UNSIGNED_BYTE);
					NodeSelectable * selection = nullptr;
					if(NodeSelectable::pickingMap.count(pickedColour) == 1){
						selection = NodeSelectable::pickingMap.at(pickedColour);
					}

					if(!voxelSelectMode){
						if(dynamic_cast<Voxel *>(selection) != nullptr){
							selection = dynamic_cast<Joint *>(dynamic_cast<Voxel *>(selection)->parent);
						}
					}

					bool additive = event.isShiftDown();
					bool subtractive = event.isControlDown() != event.isShiftDown();

					if(selection != nullptr || (!additive && !subtractive)){
						cmdProc->executeCommand(new CMD_SelectNodes((Node *)selection, additive, subtractive));
					}
				}
			}
		}

		UI::updateHandlePos(false);
	}else{
		if(NodeSelectable::pickingMap.count(clickedUiColour) == 1){
			activeButton = dynamic_cast<UiInteractable *>(NodeSelectable::pickingMap.at(clickedUiColour));
			if(activeButton != nullptr){
				activeButton->down(this);
			}
		}
	}
}

void CinderApp::mouseDrag( MouseEvent event ){
	mMousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		camMayaPersp.mouseDrag( mMousePos, event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}else if(!play){
		if(event.isLeftDown()){
			if(clickedUiColour != 0){
				if(sourceCam != nullptr && sourceRect != nullptr){
					Vec2f handlePosInScreen = sourceCam->worldToScreen(UI::handlePos, sourceRect->getWidth(), sourceRect->getHeight());

					Vec2f deltaMousePos = mMousePos - oldMousePos;
					console() << "Mouse:\t" << deltaMousePos << std::endl;

					if(mode == kTRANSLATE){
						if(UI::selectedNodes.size() > 0){
							if(sourceCam->isPersp()){

								Ray ray = sourceCam->generateRay((handlePosInScreen.x + deltaMousePos.x)/sourceRect->getWidth(), 1.f-(handlePosInScreen.y + deltaMousePos.y)/sourceRect->getHeight(), sourceCam->getAspectRatio());
							
								console() << "Ray:\t" << ray.getOrigin() << ",\t" << ray.getDirection() << std::endl;
							
								float distance = 0.f;
								if(ray.calcPlaneIntersection(UI::handlePos, sourceCam->getViewDirection(), &distance)){
									Vec3f newPos = ray.calcPosition(distance);
									Vec3f dif = newPos-UI::handlePos;

									switch(clickedUiColour){
										case 0xFF0000: dif.y = 0; dif.z = 0; break;
										case 0x00FF00: dif.x = 0; dif.z = 0; break;
										case 0x0000FF: dif.x = 0; dif.y = 0; break;
										case 0xFFFF00: break;
										default: dif.x = 0; dif.y = 0;  dif.z = 0; break;
									}
									console() << "distance:\t" << distance << std::endl;
									console() << "newPos:\t" << newPos << std::endl;
									console() << "handlePos:\t" << UI::handlePos << std::endl;
									console() << "Move:\t" << dif << std::endl << std::endl;

									cmdProc->executeCommand(new CMD_TranslateSelectedTransformables(dif, true, translateSpace));
								}
							}
						}
					}else if(mode == kROTATE){
						glm::vec3 axis(0,0,0);
						switch(clickedUiColour){
							case 0xFF0000: axis.x -= 1; break;
							case 0x00FF00: axis.y -= 1; break;
							case 0x0000FF: axis.z -= 1; break;
							case 0xFFFF00: axis = glm::vec3(sourceCam->getViewDirection().x, sourceCam->getViewDirection().y, sourceCam->getViewDirection().z); break;
						}
						if(UI::selectedNodes.size() > 0){
							Vec2i dif1 = oldMousePos - handlePosInScreen-sourceRect->getUpperLeft();
							Vec2i dif2 = mMousePos - handlePosInScreen-sourceRect->getUpperLeft();
							float angle1 = atan2f(dif1.y, dif1.x);
							float angle2 = atan2f(dif2.y, dif2.x);
							float angle = (angle2 - angle1);
					
							console() << angle << std::endl;

							glm::vec3 eulerAngles(angle,angle,angle);
							eulerAngles.x *= axis.x;
							eulerAngles.y *= axis.y;
							eulerAngles.z *= axis.z;

							cmdProc->executeCommand(new CMD_RotateSelectedTransformables(glm::quat(eulerAngles), true, rotateSpace));
						}
					}else if(mode == kSCALE){
						if(UI::selectedNodes.size() > 0){
							//nothing
							Vec3i dir(0,0,0);
							switch(clickedUiColour){
								case 0xFF0000: dir.x -= 1; break;
								case 0x00FF00: dir.y -= 1; break;
								case 0x0000FF: dir.z -= 1; break;
								case 0xFFFF00: dir.x -= 1; dir.y -= 1; dir.z -= 1; break;
							}
						
							Vec2i end = sourceCam->worldToScreen(UI::handlePos + dir, sourceRect->getWidth(), sourceRect->getHeight());
							Vec2f mouseAxis = end - handlePosInScreen;
							float dif = deltaMousePos.dot(mouseAxis) / sqrtf(getWindowHeight()*getWindowHeight() + getWindowWidth()*getWindowWidth());
							
							console() << dif << std::endl;

							cmdProc->executeCommand(new CMD_ScaleSelectedTransformables(Vec3f(1.f, 1.f, 1.f) + Vec3f(dir)*dif/100.f, true, scaleSpace));
						}
					}
				}
			}else{
				if(mode == kSELECT){
					if(event.isLeftDown() && !event.isAltDown() && !event.isShiftDown()){
						unsigned long int pickedColour;
						pickColour(&pickedColour, sourceFbo, sourceRect, &mPickingFboJoint, mMousePos, Area(0,0,5,5), 1, GL_UNSIGNED_BYTE);
						NodeSelectable * selection = nullptr;
						if(NodeSelectable::pickingMap.count(pickedColour) == 1){
							selection = NodeSelectable::pickingMap.at(pickedColour);
						}

						if(!voxelSelectMode){
							if(dynamic_cast<Voxel *>(selection) != nullptr){
								selection = dynamic_cast<Joint *>(dynamic_cast<Voxel *>(selection)->parent);
							}
						}

						bool additive = !event.isControlDown();
						bool subtractive = event.isControlDown();

						if(selection != nullptr || (!additive && !subtractive)){
							cmdProc->executeCommand(new CMD_SelectNodes((Node *)selection, additive, subtractive));
						}
					}

				}else if(mode == kPAINT_VOXELS){
					if(UI::selectedNodes.size() == 1 && (dynamic_cast<Joint *>(UI::selectedNodes.at(0)) != NULL)){
						if(event.isLeftDown()){
							// place voxel
							Color voxel;
							pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 3, GL_FLOAT);
								
							if(voxel != Color(0.f, 0.f, 0.f)){
								Vec3f voxelPos = Vec3f(voxel.r, voxel.g, voxel.b);
								float spacingDistance = voxelPaintSpacing * (voxelPreviewMode ? voxelPreviewResolution : voxelSphereRadius ) * 2;
								currentSpacingDistance += lastVoxelPaintPos.distance(voxelPos);
								console() << "lastVoxelPaintPos: " << lastVoxelPaintPos << endl;
								console() << " voxelPos: " << voxelPos << endl;
								console() << "currentSpacingDistance: " << currentSpacingDistance << " spacingDistance: " << spacingDistance << endl;
								if(currentSpacingDistance >= spacingDistance){
									console() << "blah distance: " << currentSpacingDistance << " spacing: " << voxelPaintSpacing << endl;
									cmdProc->executeCommand(new CMD_PlaceVoxel(voxelPos, dynamic_cast<Joint *>(UI::selectedNodes.back())));
									currentSpacingDistance = 0;
								}
								lastVoxelPaintPos = voxelPos;
							}else{
								console() << "bg" << std::endl;
							}
						}
					}
				}
			}
			oldMousePos = mMousePos;
		}else{
			// delete voxel
			unsigned long int voxel;
			pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
			if(NodeSelectable::pickingMap.count(voxel) != 0){
				Voxel * t = dynamic_cast<Voxel *>(NodeSelectable::pickingMap.at(voxel));
				if(t != nullptr){
					cmdProc->executeCommand(new CMD_DeleteVoxel(t));
				}
			}
		}
	}

	// Get the selected UI colour
	if(activeButton != nullptr){
		pickColour(&uiColour, &fboUI, &rectWindow, &pickingFboUI, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
		if(NodeSelectable::pickingMap.count(uiColour) == 1){
			if(activeButton != dynamic_cast<UiInteractable *>(NodeSelectable::pickingMap.at(uiColour))){
				if(activeButton->isHovered){
					activeButton->out();
				}
			}else if(!activeButton->isHovered){
				activeButton->in();
			}
		}else{
			if(activeButton->isHovered){
				activeButton->out();
			}
		}
	}

	UI::updateHandlePos(true);
}

void CinderApp::mouseUp( MouseEvent event ){
	isMouseDown = false;
	UI::updateHandlePos(false);
	
	if(activeButton != nullptr){
		activeButton->up(this);
	}
	
	if(!play){
		cmdProc->endCompressing();
		console() << "endCompressing" << endl;
	}

	uiColour = 0;
	clickedUiColour = 0;
}

void CinderApp::keyDown( KeyEvent event ){
	handleCode(event.getCode());
	if(!play){
		if(!isMouseDown){
			ParamTextBox * activeTextBox = dynamic_cast<ParamTextBox *>(activeButton);
			if (activeTextBox != nullptr && activeTextBox->isActive){
				activeTextBox->setText(event);
			}else{
				if(!event.isAltDown()){
					if(event.isControlDown()){
						if(!event.isShiftDown()){
							// Ctrl + key combinations
							switch( event.getCode() ){
							case KeyEvent::KEY_z:
								cmdProc->undo();
								break;
							case KeyEvent::KEY_y:
								cmdProc->redo();
								break;
							case KeyEvent::KEY_d:
								// Deselect all
								if(UI::selectedNodes.size() != 0){
									cmdProc->executeCommand(new CMD_SelectNodes(nullptr));
								}
								break;
							}
						}else{
							// Ctrl + Shift + key combinations
						}
					}else if(event.isShiftDown()){
						// Shift + key combinations
						switch (event.getCode() ){
						case KeyEvent::KEY_p:
							if(UI::selectedNodes.size() > 0){
								cmdProc->executeCommand(new CMD_ParentSelectedNodes(sceneRoot, sceneRoot));
							}
							break;
						}
					}else{
						// Simple key
						switch (event.getCode() ){
						case KeyEvent::KEY_ESCAPE:
							//shutdown();
							quit();
							break;
						case KeyEvent::KEY_f:
							setFullScreen( !isFullScreen() );
							break;
						case KeyEvent::KEY_F1:
							drawParams = !drawParams;
							if(drawParams){
								params->maximize();
							}else{
								params->minimize();
							}
							break;
						case KeyEvent::KEY_1:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(3))->children.at(0))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_2:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(3))->children.at(1))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_3:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(3))->children.at(2))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_4:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(3))->children.at(3))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_DELETE:
							if(UI::selectedNodes.size() > 0){
								cmdProc->executeCommand(new CMD_DeleteJoints());
							}
							break;
						case KeyEvent::KEY_p:
							if(UI::selectedNodes.size() > 0){
								cmdProc->executeCommand(new CMD_ParentSelectedNodes(sceneRoot, dynamic_cast<NodeParent *>(UI::selectedNodes.back())));
							}
							break;
						case KeyEvent::KEY_q:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(0))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_w:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(1))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_e:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(2))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_r:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(3))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_b:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(4))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_v:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(5))->pressProgrammatically(this);
							break;
						case KeyEvent::KEY_SPACE:
							dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(0))->children.at(1))->pressProgrammatically(this);
							break;
						}
					}
				}else{
					if(event.isControlDown()){
						if(!event.isShiftDown()){
							// Alt + Ctrl + key combinations
						}
					}else if(event.isShiftDown()){
						// Alt + Shift + key combinations
					}else{
						// Alt + key combination
					}
				}
		
			}
			UI::updateHandlePos(false);
		}
	}else{
		if(event.getCode() == KeyEvent::KEY_SPACE){
			dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(timelineBar->children.at(0))->children.at(1))->pressProgrammatically(this);			
		}
	}
}

void CinderApp::keyUp( KeyEvent event ){
}

void CinderApp::loadShaders(){
	try{
		jointShader = gl::GlslProg( loadFile("../assets/shaders/joint.vert"), loadFile("../assets/shaders/joint.frag") );
		uiShader = gl::GlslProg( loadFile("../assets/shaders/ui.vert"), loadFile("../assets/shaders/ui.frag") );
	}catch( const std::exception &e ) {
		console() << e.what() << std::endl;
		quit();
	}
}

void CinderApp::initFbo(gl::Fbo & _fbo, Area _area){
	gl::Fbo::Format fmt;
	// make sure the framebuffer is not anti-aliased
	fmt.setSamples(0);
	fmt.setCoverageSamples(0);
	fmt.setColorInternalFormat(GL_RGBA32F);
	// you can omit these lines if you don't intent to display the picking framebuffer
	fmt.setMagFilter(GL_NEAREST);
	fmt.setMinFilter(GL_LINEAR);
	
	unsigned int w = max(1, _area.getWidth());
	unsigned int h = max(1, _area.getHeight());
	_fbo = gl::Fbo(w, h, fmt);

	// work-around for an old Cinder issue
	_fbo.getTexture(0).setFlipped(true);
}

void CinderApp::initMultiChannelFbo(gl::Fbo & _fbo, Area _area, unsigned long int _numChannels){
	gl::Fbo::Format fmt;

	// we create multiple color targets:
	//  -one for the scene as we will view it
	//  -one to contain a color coded version of the scene that we can use for picking
	fmt.enableColorBuffer( true, _numChannels );
	fmt.setColorInternalFormat(GL_RGBA32F);

	// anti-aliasing samples
	fmt.setSamples(0);
	fmt.setCoverageSamples(0);
	// you can omit these lines if you don't intent to display the picking framebuffer
	fmt.setMagFilter(GL_NEAREST);
	fmt.setMinFilter(GL_LINEAR);

	// create the buffer
	unsigned int w = max(1, _area.getWidth());
	unsigned int h = max(1, _area.getHeight());
	_fbo = gl::Fbo( w, h, fmt );

	// work-around for an old Cinder issue
	for(unsigned long int i = 0; i < _numChannels; ++i){
		_fbo.getTexture(i).setFlipped(true);
	}
}

void CinderApp::drawGrid(float size, float step){
	jointShader.bind();
	gl::color( Colorf(0.2f, 0.2f, 0.2f) );
	jointShader.uniform("pickingColor", Color(0.f, 0.f, 0.f));
	jointShader.uniform("availableForVoxel", false);
	for(float i=-size;i<=size;i+=step) {
		gl::drawLine( Vec3f(i, 0.f, -size), Vec3f(i, 0.f, size) );
		gl::drawLine( Vec3f(-size, 0.f, i), Vec3f(size, 0.f, i) );
	}
	jointShader.uniform("availableForVoxel", true);
	jointShader.unbind();
}

void CinderApp::saveSkeleton() {
	try{
		console() << "saveSkeleton" << endl;
		SkeletonData::SaveSkeleton(directory, fileName, sceneRoot);
		message = "Saved skeleton";
	}catch (exception ex){
		message = string(ex.what());
	}
}

void CinderApp::loadSkeleton() {
	try{
		// Deselect everything
		cmdProc->executeCommand(new CMD_SelectNodes(nullptr));
		console() << "loadSkeleton" << endl;

		std::vector<Joint *> joints = SkeletonData::LoadSkeleton(filePath);

		// If SkeletonData loads without any exceptions, clear and load joints into sceneRoot's children
		sceneRoot->children.clear();
		for(unsigned long int i = 0; i < joints.size(); ++i){
			sceneRoot->addChild(joints.at(i));
		}

		message = "Loaded skeleton";

		// Clear the undo/redo history
		cmdProc->reset();
	}catch (exception ex){
		message = string(ex.what());
	}
}

void CinderApp::setKeyframe(){
	if(UI::selectedNodes.size() != 0){
		cmdProc->startCompressing();
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
			if (_node != nullptr){
				cmdProc->executeCommand(new CMD_KeyAllProperties(_node, UI::time));
			}
		}
		cmdProc->endCompressing();
	}
}

void CinderApp::clearKeyframe(){
	if(UI::selectedNodes.size() != 0){
		cmdProc->startCompressing();
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
			if (_node != nullptr){
				cmdProc->executeCommand(new CMD_ClearAllProperties(_node, UI::time));
			}
		}
		cmdProc->endCompressing();
	}
}

void CinderApp::handleCode(unsigned long int _code){
	switch(code){
	case 0:
		if(_code == KeyEvent::KEY_UP){
			code += 1;
		}else{
			code = 0;
		}
		break;
	case 1:
		if(lastKey == KeyEvent::KEY_UP){
			if(_code == KeyEvent::KEY_UP){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 2:
		if(lastKey == KeyEvent::KEY_UP){
			if(_code == KeyEvent::KEY_DOWN){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 3:
		if(lastKey == KeyEvent::KEY_DOWN){
			if(_code == KeyEvent::KEY_DOWN){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 4:
		if(lastKey == KeyEvent::KEY_DOWN){
			if(_code == KeyEvent::KEY_LEFT){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 5:
		if(lastKey == KeyEvent::KEY_LEFT){
			if(_code == KeyEvent::KEY_RIGHT){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 6:
		if(lastKey == KeyEvent::KEY_RIGHT){
			if(_code == KeyEvent::KEY_LEFT){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 7:
		if(lastKey == KeyEvent::KEY_LEFT){
			if(_code == KeyEvent::KEY_RIGHT){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 8:
		if(lastKey == KeyEvent::KEY_RIGHT){
			if(_code == KeyEvent::KEY_b){
				code += 1;
			}else{
				code = 0;
			}
		}
		break;
	case 9:
		if(lastKey == KeyEvent::KEY_b){
			if(_code == KeyEvent::KEY_a){
				active = !active;
			}
			code = 0;
		}
	}
	lastKey = _code;
}

CINDER_APP_BASIC( CinderApp, RendererGl )