#pragma once

#include "CinderApp.h"
#include "UI.h"
#include "ButtonFunctions.hpp"

#include "CMD_CreateJoint.h"
#include "CMD_DeleteJoints.h"
#include "CMD_SelectNodes.h"
#include "CMD_MoveSelectedJoints.h"
#include "CMD_ScaleSelectedTransformable.h"
#include "CMD_RotateSelectedTransformable.h"
#include "CMD_KeyProperty.h"
#include "CMD_KeyAll.h"
#include "CMD_ParentSelectedNodes.h"
#include "CMD_PlaceVoxel.h"
#include "CMD_DeleteVoxel.h"
#include "CMD_SetTime.h"

#include "Transform.h"
#include "NodeTransformable.h"

#include "Tween.h"
#include "Step.h"

#include "MatrixStack.h"
#include "CinderRenderOptions.h"

#include "ToolBar.h"
#include "ToolSet.h"
#include "ToolButton.h"

void CinderApp::prepareSettings(Settings *settings){
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup(){
	sceneRoot = new SceneRoot();
	cmdProc = new CommandProcessor();

	sourceCam = nullptr;
	sourceRect = nullptr;
	sourceFbo = nullptr;
	sourceBounds = nullptr;
	activeButton = nullptr;

	uiColour = 0;
	clickedUiColour = 0;
	
	translateSpace = WORLD;
	rotateSpace = OBJECT;
	scaleSpace = OBJECT;

	drawParams = true;
	params = params::InterfaceGl::create( getWindow(), "General", toPixels( Vec2i( 175, 250 ) ), ColorA(0.6f, 0.3f, 0.3f, 0.4f));
	params->addText( "UI Mode", "label=`CREATE`" );
	
	params->addParam("Translate Space", (int *)&translateSpace, "min=0 max=1");
	params->addParam("Rotate Space", (int *)&rotateSpace, "min=0 max=1");
	params->addParam("Scale Space", (int *)&scaleSpace, "min=0 max=1");

	params->addSeparator();

	params->addParam("Directory", &directory);
	params->addParam("File Name", &fileName);
	params->addButton("Save", std::bind( &CinderApp::saveSkeleton, this ));
	params->addSeparator();
	params->addParam("Bones File", &filePath);
	params->addButton("Load", std::bind(&CinderApp::loadSkeleton, this));
	params->addSeparator();
	params->addParam("Message", &message, "", true);

	timelineParams = params::InterfaceGl::create( getWindow(), "Animation", toPixels(Vec2i(175,150)), ColorA(0.3f, 0.6f, 0.3f, 0.4f));
	timelineParams->minimize();
	timelineParams->addParam("Time", &UI::time, "", true);

	timelineParams->addParam("Interpolation", UI::interpolationNames, &UI::interpolationValue);
	timelineParams->addButton("Add/Edit Keyframe", std::bind(&CinderApp::setKeyframe, this));

	timelineParams->addSeparator();
	timelineParams->addButton("togglePlay", std::bind(&CinderApp::togglePlay, this));
	timelineParams->addText("Status: ", "label=`STOPPED`");
	timelineParams->maximize();
	
	
	voxelSelectMode = false;
	voxelPreviewMode = false;
	voxelPreviewResolution = 0.1;
	voxelSphereRadius = 0.1;
	voxelPaintSpacing = 1;

	viewJointsOnly = false;

	voxelParams = params::InterfaceGl::create(getWindow(), "Voxel", toPixels(Vec2i(180,150)), ColorA(0.3f, 0.3f, 0.6f, 0.4f));
	voxelParams->addParam("Selectable", &voxelSelectMode);
	voxelParams->addParam("Preview", &voxelPreviewMode);
	voxelParams->addParam("Resolution", &voxelPreviewResolution, "min=0.01, step=0.01");
	voxelParams->addParam("Radius", &voxelSphereRadius, "min=0.01, step=0.01");
	voxelParams->addParam("Paint Spacing", &voxelPaintSpacing, "min=0.01, step=0.5");
	voxelParams->maximize();

	// note: we will setup our camera in the 'resize' function,
	//  because it is called anyway so we don't have to set it up twice

	// create materials
	JointMaterial.setAmbient(Color::white());
	JointMaterial.setDiffuse(Color::black());
	JointMaterial.setSpecular(Color::black());
	JointMaterial.setShininess( 0.0f );

	// load shaders
	loadShaders();

	// set background color
	mColorBackground = Color(0.0f, 0.0f, 0.0f);
	
	channel = 0;

	mode = CREATE;


	//// test animation
	/*vector<Tween *> tweens;
	tweens.push_back(new Tween(2, 0.1, Easing::Type::kNONE));
	tweens.push_back(new Tween(2, -0.5, Easing::Type::kNONE));
	
	cmdProc->executeCommand(new CMD_CreateJoint(&joints, Vec3f(0.f, 0.f, 0.f), nullptr));
	cmdProc->executeCommand(new CMD_CreateJoint(&joints, Vec3f(0,1,0), joints.at(0)));

	joints.at(0)->translateZ.tweens = tweens;*/


	play = false;
	previousTime = 0;
	
	toolbar = new ToolBar(Vec2i(5,5), false);

	// Selecting, Creating, Painting, and Transformations
	toolbar->addSet(new ToolSet(Area(0,0,30,30)));
	// Channels
	toolbar->addSet(new ToolSet(Area(0,0,20,20)));
	// View Joints, Voxels, or All
	toolbar->addSet(new ToolSet(Area(0,0,25,25)));


	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Select", nullptr, &ButtonFunctions::MODE_Select));
	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Translate", nullptr, &ButtonFunctions::MODE_Translate));
	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Rotate", nullptr, &ButtonFunctions::MODE_Rotate));
	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Scale", nullptr, &ButtonFunctions::MODE_Scale));
	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Joint", nullptr, &ButtonFunctions::MODE_CreateJoints));
	toolbar->addButton(0, new ToolButton(ToolButton::Type::RADIO, "Voxels", nullptr, &ButtonFunctions::MODE_PaintVoxels));

	toolbar->addButton(1, new ToolButton(ToolButton::Type::RADIO, "Channel 1", nullptr, &ButtonFunctions::CHANNEL_0));
	toolbar->addButton(1, new ToolButton(ToolButton::Type::RADIO, "Channel 2", nullptr, &ButtonFunctions::CHANNEL_1));
	toolbar->addButton(1, new ToolButton(ToolButton::Type::RADIO, "Channel 3", nullptr, &ButtonFunctions::CHANNEL_2));
	toolbar->addButton(1, new ToolButton(ToolButton::Type::RADIO, "Channel 4", nullptr, &ButtonFunctions::CHANNEL_3));

	toolbar->addButton(2, new ToolButton(ToolButton::Type::RADIO, "View Joints Only", nullptr, &ButtonFunctions::VIEW_JointsOnly));

	timelineBar = new ToolBar(Vec2i(5,40), false);

	timelineBar->addSet(new ToolSet(Area(0,0,20,20)));

	timelineBar->addButton(0, new ToolButton(ToolButton::Type::NORMAL, "Next", nullptr, &ButtonFunctions::TIME_Increment));
	timelineBar->addButton(0, new ToolButton(ToolButton::Type::NORMAL, "Prev", nullptr, &ButtonFunctions::TIME_Decrement));

}

void CinderApp::resize(){
	unsigned int w = max(1, getWindowWidth());
	unsigned int h = max(1, getWindowHeight());
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
}


void CinderApp::shutdown(){
	UI::selectedNodes.clear();
	delete toolbar;
	toolbar = nullptr;
	delete sceneRoot;
	sceneRoot = nullptr;
	delete cmdProc;
	cmdProc = nullptr;
}

void CinderApp::update(){
	// play animation or edit
	if(play){
		Step s;
		s.setDeltaTime(1 * UI::stepScale);
		cmdProc->executeCommand(new CMD_SetTime(&UI::time, UI::time+(float)s.getDeltaTime(), false));
		app::console() << "CinderApp.update() play UI::time: " << UI::time << endl;
		console() << "previousTime: " << previousTime << endl;
		for(unsigned long int i = 0; i < sceneRoot->children.size(); ++i){
			NodeUpdatable * nu = dynamic_cast<NodeUpdatable *>(sceneRoot->children.at(i));
			nu->update(&s);
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

	snapParams();
}

void CinderApp::draw(){
	renderScene(fboPersp, camMayaPersp.getCamera());
	renderScene(fboTop, camTop);
	renderScene(fboRight, camRight);
	renderScene(fboFront, camFront);

	fboUI.bindFramebuffer();
		uiShader.bind();
		// set viewport to the size of the FBO
		gl::setViewport( fboUI.getBounds() );
		gl::enableDepthRead();
		gl::enableDepthWrite();
		gl::enableAlphaBlending();

		gl::clear(ColorA(0.f, 0.f, 0.f, 0.f));

		renderUI(camMayaPersp.getCamera(), rectPersp);
		renderUI(camTop, rectTop);
		renderUI(camRight, rectRight);
		renderUI(camFront, rectFront);

		gl::setViewport(fboUI.getBounds());

		vox::MatrixStack t;
		CinderRenderOptions t2(nullptr, nullptr);
		t2.ciShader = &uiShader;
		toolbar->render(&t, &t2);
		timelineBar->render(&t, &t2);
		uiShader.unbind();

		params->draw();
		timelineParams->draw();
		voxelParams->draw();
		
		gl::disableAlphaBlending();
		gl::disableDepthRead();
		gl::disableDepthWrite();
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

	// draw the picking framebuffer in the upper right corner
	if(pickingFboUI){
		Rectf rct((Rectf)pickingFboUI.getBounds() * 5.f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0.f) );
		gl::draw( pickingFboUI.getTexture(0), Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2));
	}
	

	// draw the picking framebuffer in the upper right corner
	if(pixelFbo){
		Rectf rct((Rectf)pixelFbo.getBounds() * 5.0f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0.f) );
		gl::draw( pixelFbo.getTexture(0), Rectf(rct.x1, rct.y1+rct.y2+rct.y2, rct.x2, rct.y2+rct.y2+rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1+rct.y2+rct.y2, rct.x2, rct.y2+rct.y2+rct.y2));
	}
}

void CinderApp::renderScene(gl::Fbo & fbo, const Camera & cam){
	fbo.bindFramebuffer();

	// clear background
	gl::clear( mColorBackground );

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

		CinderRenderOptions r(nullptr, nullptr);
		r.ciCam = &cam;
		r.ciShader = &jointShader;
		r.voxelPreviewMode = voxelPreviewMode;
		r.voxelPreviewResolution = voxelPreviewResolution;
		r.voxelSphereRadius = voxelSphereRadius;
		r.viewJointsOnly = viewJointsOnly;

		for(unsigned long int i = 0; i < sceneRoot->children.size(); ++i){
			NodeRenderable * nr = dynamic_cast<NodeRenderable *>(sceneRoot->children.at(i));
			if(nr != nullptr){
				nr->render(&mStack, &r);
			}
		}

		//jointShader.uniform("offset", true);
		/*for(unsigned long int i = 0; i < paintPoints.size(); ++i){
			gl::pushMatrices();
			vox::pushMatrix();
			Transform t;
			t.translate(paintPoints.at(i).x, paintPoints.at(i).y, paintPoints.at(i).z);
			vox::translate(t.getTranslationMatrix());
			gl::translate(paintPoints.at(i));
			
			glUniformMatrix4fv(jointShader.getUniformLocation("modelMatrix"), 1, GL_FALSE, &vox::currentModelMatrix[0][0]);
			gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.1, 16);
			gl::popMatrices();
			vox::popMatrix();
		}*/

		// unbind shader
		jointShader.unbind();
	
		//gl::drawColorCube(Vec3f(0.f, 0.f, 0.f),Vec3f(5,5,5));

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
	if(mode == CREATE){
		Ray ray = camMayaPersp.getCamera().generateRay((float)mMousePos.x/rectPersp.getWidth(), 1.f-((float)(mMousePos.y-rect.y1)/rectPersp.getHeight()), camMayaPersp.getCamera().getAspectRatio());			
		float distance = 0.f;
		if(ray.calcPlaneIntersection(Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f), &distance)){
			Vec3f jointWillGoHere = ray.calcPosition(distance);
			
			gl::enableWireframe();
			gl::drawSphere(jointWillGoHere, 0.06f);
			gl::disableWireframe();
		}
	}

	if(UI::selectedNodes.size() != 0){
		gl::pushMatrices();

			// Draw spheres around the selected joints
			gl::enableWireframe();
			for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
				if (i == UI::selectedNodes.size() - 1){
					gl::color(1.f, 0.25f, 0.1f);
				}
				Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
				if(j != NULL){
					gl::pushMatrices();
						glm::vec3 absPos = j->getPos(false);
						gl::translate(absPos.x, absPos.y, absPos.z);
						gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.06f);
					gl::popMatrices();
				}else{
					Voxel * v = dynamic_cast<Voxel *>(UI::selectedNodes.at(i));
					if(v != NULL){
						gl::pushMatrices();
							glm::vec3 absPos = dynamic_cast<Joint *>(v->parent)->getPos(false);
							gl::translate(absPos.x, absPos.y, absPos.z);
							gl::drawSphere(Vec3f(v->getPos().x, v->getPos().y, v->getPos().z), 0.06f);
						gl::popMatrices();
					}
				}
			}
			gl::disableWireframe();

			switch(mode){
			case TRANSLATE:
				gl::translate(UI::displayHandlePos);
				break;
			case ROTATE:
			case SCALE:
				gl::translate(UI::handlePos);
				break;
			}

			gl::lineWidth(2);
			if(cam.isPersp()){
				// If the camera is a perspective view, scale the coordinate frame proportionally to the distance from camera
				gl::scale(cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()));
				gl::scale(-1,-1,-1);
			}

			switch(mode){
			case TRANSLATE:
				if(translateSpace == OBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != NULL){
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
				gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);
				break;

			case ROTATE:
				if(rotateSpace == OBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != NULL){
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
				gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);
				break;

			case SCALE:
				if(scaleSpace == OBJECT){
					// Rotate to match the object orientation
					Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(UI::selectedNodes.size()-1));
					if(j != NULL){
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
				gl::drawSphere(Vec3f(0.f, 0.f, 0.f), 0.05f);
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
			ToolButton * newButt = dynamic_cast<ToolButton *>(NodeSelectable::pickingMap.at(uiColour));
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
					activeButton = nullptr;
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

	switch (mode){
		case CinderApp::CREATE:
			break;
		case CinderApp::SELECT:
		case CinderApp::TRANSLATE:
		case CinderApp::ROTATE:
		case CinderApp::SCALE:
		case CinderApp::PAINT_VOXELS:
			cmdProc->startCompressing();
			console() << "startCompressing" << endl;
			break;
		default:
			break;
	}
	
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

	if(!event.isAltDown() && (clickedUiColour == 0)){
		if(sourceCam == &camMayaPersp.getCamera()){
			if(mode == PAINT_VOXELS){
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
			}else if(mode == CREATE){
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
			}else if(mode == SELECT){
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
			activeButton = dynamic_cast<ToolButton *>(NodeSelectable::pickingMap.at(clickedUiColour));
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
	}else{
		if(event.isLeftDown()){
			if(clickedUiColour != 0){
				if(sourceCam != nullptr && sourceRect != nullptr){
					Vec2f handlePosInScreen = sourceCam->worldToScreen(UI::handlePos, sourceRect->getWidth(), sourceRect->getHeight());

					Vec2f deltaMousePos = mMousePos - oldMousePos;
					console() << "Mouse:\t" << deltaMousePos << std::endl;

					if(mode == TRANSLATE){
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
										default: break;
									}
									console() << "distance:\t" << distance << std::endl;
									console() << "newPos:\t" << newPos << std::endl;
									console() << "handlePos:\t" << UI::handlePos << std::endl;
									console() << "Move:\t" << dif << std::endl << std::endl;

									cmdProc->executeCommand(new CMD_MoveSelectedJoints(dif, true, translateSpace));
								}
							}
						}
					}else if(mode == ROTATE){
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

							cmdProc->executeCommand(new CMD_RotateSelectedTransformable(glm::quat(eulerAngles), true, rotateSpace));
						}
					}else if(mode == SCALE){
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

							cmdProc->executeCommand(new CMD_ScaleSelectedTransformable(Vec3f(1.f, 1.f, 1.f) + Vec3f(dir)*dif/100.f, scaleSpace));
						}
					}
				}
			}else{
				if(mode == SELECT){
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

				}else if(mode == PAINT_VOXELS){
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
			if(activeButton != dynamic_cast<ToolButton *>(NodeSelectable::pickingMap.at(uiColour))){
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
	
	if(activeButton != NULL){
		activeButton->up(this);
	}
	
	if(mode == SELECT || mode == TRANSLATE || mode == ROTATE || mode == SCALE || mode == PAINT_VOXELS){
		cmdProc->endCompressing();
		console() << "endCompressing" << endl;
	}

	uiColour = 0;
	clickedUiColour = 0;
}

void CinderApp::keyDown( KeyEvent event ){
	if(!isMouseDown){
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
						timelineParams->maximize();
						voxelParams->maximize();
					}else{
						params->minimize();
						timelineParams->minimize();
						voxelParams->minimize();
					}
				case KeyEvent::KEY_1:
					channel = 0;
					break;
				case KeyEvent::KEY_2:
					channel = 1;
					break;
				case KeyEvent::KEY_3:
					channel = 2;
					break;
				case KeyEvent::KEY_4:
					channel = 3;
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
					/*mode = SELECT;
					params->setOptions( "UI Mode", "label=`SELECT`" );*/
					dynamic_cast<ToolButton *>(dynamic_cast<ToolSet *>(toolbar->children.at(0))->children.at(0))->pressProgrammatically(this);
					break;
				case KeyEvent::KEY_w:
					mode = TRANSLATE;
					params->setOptions( "UI Mode", "label=`TRANSLATE`" );
					break;
				case KeyEvent::KEY_e:
					mode = ROTATE;
					params->setOptions( "UI Mode", "label=`ROTATE`" );
					break;
				case KeyEvent::KEY_r:
					mode = SCALE;
					params->setOptions( "UI Mode", "label=`SCALE`" );
					break;
				case KeyEvent::KEY_b:
					mode = CREATE;
					params->setOptions( "UI Mode", "label=`CREATE`" );
					break;
				case KeyEvent::KEY_v:
					mode = PAINT_VOXELS;
					params->setOptions( "UI Mode", "label=`PAINT_VOXELS`" );
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
		UI::updateHandlePos(false);
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

void CinderApp::snapParams(){
	// Snap window to side. Using setOptions instead of setPosition, which is for Cinder for VS2013 and up only
	// Params options
	stringstream paramsOptions;
	paramsOptions << "position='" << getWindowWidth() - 175 - 5 << " " << getWindowHeight() / 2.5f << "'";
	string blah = paramsOptions.str();
	params->setOptions("", paramsOptions.str());
	//params->

	stringstream timelineParamsOptions;
	timelineParamsOptions << "position='5 " << getWindowHeight() / 2 << "'";
	timelineParams->setOptions("", timelineParamsOptions.str());

	stringstream voxelParamsOptions;
	voxelParamsOptions << "position='5 " << 2 * (getWindowHeight() / 2.5) << "'";
	voxelParams->setOptions("", voxelParamsOptions.str());
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
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			NodeAnimatable * _node = dynamic_cast<NodeAnimatable *>(UI::selectedNodes.at(i));
			if (_node != NULL){
				cmdProc->executeCommand(new CMD_KeyAll(_node, UI::time));
			}
		}
	}
}

void CinderApp::togglePlay(){
	if (!play){
		timelineParams->setOptions( "togglePlay", "label=`PLAYING`" );
		cmdProc->startCompressing();
	}else{
		timelineParams->setOptions( "togglePlay", "label=`STOPPED`" );
		cmdProc->endCompressing();
	}

	play = !play;
}

CINDER_APP_BASIC( CinderApp, RendererGl )