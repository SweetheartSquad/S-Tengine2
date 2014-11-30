#pragma once

#include "CinderApp.h"
#include "UI.h"

#include "CMD_CreateJoint.h"
#include "CMD_DeleteJoint.h"
#include "CMD_SelectNodes.h"
#include "CMD_MoveSelectedJoints.h"
#include "CMD_ScaleSelectedTransformable.h"
#include "CMD_RotateSelectedTransformable.h"
#include "CMD_KeyProperty.h"
#include "CMD_KeyAll.h"
#include "CMD_Parent.h"
#include "CMD_PlaceVoxel.h"
#include "CMD_DeleteVoxel.h"

#include "Transform.h"
#include "NodeTransformable.h"

#include "Tween.h"
#include "Step.h"

#include "MatrixStack.h"
#include "CinderRenderOptions.h"


void CinderApp::prepareSettings(Settings *settings){
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup(){
	sourceCam = nullptr;
	sourceRect = nullptr;
	sourceFbo = nullptr;

	drawParams = true;
	params = params::InterfaceGl::create( getWindow(), "Params", toPixels( Vec2i( 150, 100 ) ) );
	params->addText( "UI Mode", "label=`CREATE`" );

	params->addSeparator();

	params->addParam("Directory", &directory);
	params->addParam("File Name", &fileName);
	params->addButton("Save", std::bind( &CinderApp::saveSkeleton, this ));
	params->addSeparator();
	params->addParam("Bones File", &filePath);
	params->addButton("Load", std::bind(&CinderApp::loadSkeleton, this));
	params->addSeparator();
	params->addParam("Message", &message, "", true);

	timelineParams = params::InterfaceGl::create( getWindow(), "Animation", toPixels( Vec2i(180,150) ));
	timelineParams->minimize();
	timelineParams->addParam("Time", &UI::time);

	timelineParams->addButton("Add/Edit Keyframe", std::bind(&CinderApp::setKeyframe, this));

	timelineParams->addSeparator();
	timelineParams->addButton("togglePlay", std::bind(&CinderApp::togglePlay, this));
	timelineParams->addText("Status: ", "label=`STOPPED`");
	

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
	vector<Tween *> tweens;
	tweens.push_back(new Tween(1, 0.1, Easing::Type::kNONE));
	tweens.push_back(new Tween(1, -0.5, Easing::Type::kNONE));
	
	cmdProc.executeCommand(new CMD_CreateJoint(&joints, Vec3f(0,0,0), nullptr));
	cmdProc.executeCommand(new CMD_CreateJoint(&joints, Vec3f(0,1,0), joints.at(0)));

	joints.at(0)->translateZ.tweens = tweens;
	play = false;
	previousTime = 0;
}


void CinderApp::resize(){
	unsigned int w = max(1, getWindowWidth());
	unsigned int h = max(1, getWindowHeight());
	double r = getWindowAspectRatio();
	Area b = getWindowBounds();
	rectWindow = b;
	// setup the camera
	CameraPersp cam = camMayaPersp.getCamera();
	cam.setPerspective( 60.0f, r, 0.1f, 10000.0f );
	camMayaPersp.setCurrentCam( cam );
	
	float w2 = w/2;
	float h2 = h/2;
	
	rectTop.set(0, 0, w2, h2);
	rectRight.set(w2, 0, w, h2);
	rectFront.set(0, h2, w2, h);
	rectPersp.set(w2, h2, w, h);
	
	boundsTop.set(	-r,	-1.f,	r,	1.f);
	boundsRight.set(-r,	-1.f,	r,	1.f);
	boundsFront.set(-r,	-1.f,	r,	1.f);
	//boundsPersp.set(-1, -h/w, 1, h/w);
	
	camTop.setOrtho(boundsTop.x1, boundsTop.x2, boundsTop.y1, boundsTop.y2, -10000, 10000);
	camRight.setOrtho(boundsRight.x1, boundsRight.x2, boundsRight.y1, boundsRight.y2, -10000, 10000);
	camFront.setOrtho(boundsFront.x1, boundsFront.x2, boundsFront.y1, boundsFront.y2, -10000, 10000);
	
	camTop.setEyePoint(Vec3f(0,0,0));
	camRight.setEyePoint(Vec3f(0,0,0));
	camFront.setEyePoint(Vec3f(0,0,0));
	
	camTop.setViewDirection(Vec3f(0,-1,0));
	camRight.setViewDirection(Vec3f(-1,0,0));
	camFront.setViewDirection(Vec3f(0,0,-1));

	// create or resize framebuffer if needed
	if(!fboUI || fboUI.getWidth() != w || fboUI.getHeight() != h){
		initFbo(&fboUI, b);
	}
	
	w /= 2;
	h /= 2;
	b.x1 /= 2;
	b.x2 /= 2;
	b.y1 /= 2;
	b.y2 /= 2;

	if(!fboPersp || fboPersp.getWidth() != w || fboPersp.getHeight() != h) {
		initMultiChannelFbo(fboPersp, b, 4);
	}if(!fboTop || fboTop.getWidth() != w || fboTop.getHeight() != h){
		initMultiChannelFbo(fboTop, b, 4);
	}if(!fboRight || fboRight.getWidth() != w || fboRight.getHeight() != h){
		initMultiChannelFbo(fboRight, b, 4);
	}if(!fboFront || fboFront.getWidth() != w || fboFront.getHeight() != h){
		initMultiChannelFbo(fboFront, b, 4);
	}
}


void CinderApp::shutdown(){
	for(unsigned long int i = 0; i < joints.size(); ++i){
		NodeHierarchical::deleteRecursively(joints.at(i));
	}
	joints.clear();
	UI::selectedNodes.clear();
}

void CinderApp::update(){
	// play animation or edit
	if(play){
		Step s;
		s.setDeltaTime(1 * UI::stepScale);
		for(unsigned long int i = 0; i < joints.size(); ++i){
			joints.at(i)->update(&s);
		}
		UI::time += s.getDeltaTime();
		previousTime = UI::time;
	}else{
		if(UI::time != previousTime){
			Step s;
			s.setDeltaTime((UI::time - previousTime));
			for(unsigned long int i = 0; i < joints.size(); ++i){
				joints.at(i)->update(&s);
			}
			previousTime = UI::time;
		}
	}

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

		gl::clear(ColorA(0.f, 0.f, 0.f, 0.f));

		renderUI(camMayaPersp.getCamera(), rectPersp);
		renderUI(camTop, rectTop);
		renderUI(camRight, rectRight);
		renderUI(camFront, rectFront);

		gl::setViewport(fboUI.getBounds());

		params->draw();
		timelineParams->draw();
		
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
		Rectf rct((Rectf)mPickingFboJoint.getBounds() * 5.0f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
		gl::draw( mPickingFboJoint.getTexture(0), rct );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1, rct.x2, rct.y2));
	}

	// draw the picking framebuffer in the upper right corner
	if(pickingFboUI){
		Rectf rct((Rectf)pickingFboUI.getBounds() * 5.0f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
		gl::draw( pickingFboUI.getTexture(0), Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2));
	}
	

	// draw the picking framebuffer in the upper right corner
	if(pixelFbo){
		Rectf rct((Rectf)pixelFbo.getBounds() * 5.0f);
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
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

		jointShader.uniform("offset", false);
		// draw joints:
		vox::MatrixStack mStack;
		jointShader.uniform("camEye", cam.getEyePoint());
		jointShader.uniform("viewMatrix", cam.getModelViewMatrix());
		jointShader.uniform("projectionMatrix", cam.getProjectionMatrix());

		CinderRenderOptions r(nullptr, nullptr);
		r.ciCam = &cam;
		r.ciShader = &jointShader;

		for(Joint * j : joints){
			j->render(&mStack, &r);
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
			gl::drawSphere(Vec3f(0,0,0), 0.1, 16);
			gl::popMatrices();
			vox::popMatrix();
		}*/

		// unbind shader
		jointShader.unbind();
	
		//gl::drawColorCube(Vec3f(0,0,0),Vec3f(5,5,5));

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
	
	gl::color(ColorA(1, 1, 1, 1));
	if(UI::selectedNodes.size() != 0){

		gl::pushMatrices();
			gl::enableWireframe();
			for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
				Joint * j = dynamic_cast<Joint *>(UI::selectedNodes.at(i));
				if(j != NULL){
					gl::pushMatrices();
						gl::translate(j->getPos(false));
						gl::drawSphere(Vec3f(0,0,0), 0.06f);
					gl::popMatrices();
				}else{
					Voxel * v = dynamic_cast<Voxel *>(UI::selectedNodes.at(i));
					if(v != NULL){
						gl::pushMatrices();
							gl::translate(dynamic_cast<Joint *>(v->parent)->getPos(false));
							gl::drawSphere(v->pos, 0.06f);
						gl::popMatrices();
					}
				}
			}
			gl::disableWireframe();

			gl::translate(UI::handlePos);
			gl::lineWidth(2);
			if(cam.isPersp()){
				// If the camera is a perspective view, scale the coordinate frame proportionally to the distance from camera
				gl::scale(cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()));
			}

			if(mode == TRANSLATE){
				gl::drawCoordinateFrame(0.3, 0.15, 0.03);
				gl::color(1,1,0);
				gl::drawSphere(Vec3f(0,0,0), 0.05);
			}else if(mode == ROTATE){
				gl::lineWidth(10);
				gl::color(0,0,1.f);
				gl::drawStrokedCircle(Vec2f(0,0), 0.3, 32);
				gl::color(1.f,0,0);
				gl::scale(0.9,0.9,0.9);
				gl::rotate(Vec3f(0,90,0));
				gl::drawStrokedCircle(Vec2f(0,0), 0.3, 32);
				gl::color(0,1.f,0);
				gl::scale(0.9,0.9,0.9);
				gl::rotate(Vec3f(90,0,0));
				gl::drawStrokedCircle(Vec2f(0,0), 0.3, 32);
				gl::color(1,1,0);
				gl::drawSphere(Vec3f(0,0,0), 0.05);
			}else if(mode == SCALE){
				gl::drawCoordinateFrame(0.3, 0.15, 0.03);
				gl::color(1,1,0);
				gl::drawSphere(Vec3f(0,0,0), 0.05);
			}

			gl::lineWidth(1);
		gl::popMatrices();
	}

	// restore matrices
	gl::popMatrices();
}

void CinderApp::mouseMove( MouseEvent event ){
	mMousePos = event.getPos();
	//pickJoint(mMousePos);
	//handleUI(mMousePos);
}

void CinderApp::pickColour(void * res, const gl::Fbo * _sourceFbo, const Rectf *  _sourceRect, gl::Fbo * _destFbo, Vec2i _pos, Area _area, unsigned long int _channel, GLenum _type){
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
			initFbo(_destFbo, _area);
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
		void * buffer;
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
				console() << itr->first << "\t" << itr->second << std::endl;
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
				console() << itr->first << "\t" << itr->second << std::endl;
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
	}
}

void CinderApp::mouseDown( MouseEvent event ){
	mMousePos = event.getPos();
	
	// handle the camera
	camMayaPersp.mouseDown( mMousePos );
	
	if(rectTop.contains(mMousePos)){
		sourceCam = &camTop;
		sourceRect = &rectTop;
		sourceFbo = &fboTop;
	}else if(rectFront.contains(mMousePos)){
		sourceCam = &camFront;
		sourceRect = &rectFront;
		sourceFbo = &fboFront;
	}else if(rectRight.contains(mMousePos)){
		sourceCam = &camRight;
		sourceRect = &rectRight;
		sourceFbo = &fboRight;
	}else if(rectPersp.contains(mMousePos)){
		sourceCam = &camMayaPersp.getCamera();
		sourceRect = &rectPersp;
		sourceFbo = &fboPersp;
	}else{
		sourceCam = nullptr;
		sourceRect = nullptr;
		sourceFbo = nullptr;
	}
	
	// Get the selected UI colour
	pickColour(&uiColour, &fboUI, &rectWindow, &pickingFboUI, mMousePos, Area(0,0,1,1), 0, GL_UNSIGNED_BYTE);

	if(event.isRight()){
		oldMousePos = mMousePos;
	}

	if(!event.isAltDown()){
		if(mode == PAINT_VOXELS){
			if(UI::selectedNodes.size() == 1 && (dynamic_cast<Joint *>(UI::selectedNodes.at(0)) != NULL)){
				if(event.isLeft()){
					// place voxel
					Color voxel;
					pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 3, GL_FLOAT);
					if(voxel != Color(0,0,0)){
							cmdProc.executeCommand(new CMD_PlaceVoxel(Vec3f(voxel.r, voxel.g, voxel.b)));
					}else{
						console() << "bg" << std::endl;
					}
				}else{
					// delete voxel
					unsigned long int voxel;
					pickColour(&voxel, sourceFbo, sourceRect, &pixelFbo, mMousePos, Area(0,0,1,1), 1, GL_UNSIGNED_BYTE);
					if(NodeSelectable::pickingMap.count(voxel) != 0){
						Voxel * t = dynamic_cast<Voxel *>(NodeSelectable::pickingMap.at(voxel));
						if(t != NULL){
							cmdProc.executeCommand(new CMD_DeleteVoxel(t));
						}
					}
				}
			}
		}else if(mode == CREATE){
			if(event.isLeft()){
				Vec3d pos = getCameraCorrectedPos();
			
				if(UI::selectedNodes.size() == 1){
					cmdProc.executeCommand(new CMD_CreateJoint(&joints, pos, dynamic_cast<Joint *>(UI::selectedNodes.at(0))));
				}else{
					cmdProc.executeCommand(new CMD_CreateJoint(&joints, pos, nullptr));
				}
			}
		}else if(mode == SELECT){
			if(event.isLeft()){
				unsigned long int jointColour;
				pickColour(&jointColour, sourceFbo, sourceRect, &mPickingFboJoint, mMousePos, Area(0,0,5,5), 1, GL_UNSIGNED_BYTE);
				NodeSelectable * selection = nullptr;
				if(NodeSelectable::pickingMap.count(jointColour) == 1){
					selection = dynamic_cast<NodeSelectable *>(NodeSelectable::pickingMap.at(jointColour));
				}
				cmdProc.executeCommand(new CMD_SelectNodes((Node *)selection, event.isShiftDown(), event.isControlDown() != event.isShiftDown()));
			}
		}
	}

	UI::updateHandlePos();
}

void CinderApp::mouseDrag( MouseEvent event ){
	mMousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		camMayaPersp.mouseDrag( mMousePos, event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}else{
		if(event.isLeftDown()){
			//cmdProc.executeCommand(new CMD_MoveSelectedJoints(getCameraCorrectedPos(), false));
		}else if(event.isRightDown()){
			if(uiColour != 0){
				if(sourceCam != nullptr && sourceRect != nullptr){
					Vec2i handlePosInScreen = sourceCam->worldToScreen(UI::handlePos, sourceRect->getWidth(), sourceRect->getHeight());

					Vec2i deltaMousePos = mMousePos - oldMousePos;
					
					if(mode == TRANSLATE){
						if(UI::selectedNodes.size() > 0){
							Vec3i dir(0,0,0);
							switch(uiColour){
								case 0xFF0000: dir.x -= 10; break;
								case 0x00FF00: dir.y -= 10; break;
								case 0x0000FF: dir.z -= 10; break;
								//case 0xFFFF00: dir.x -= 10; dir.y -= 10; dir.z -= 10; break;
								case 0xFFFF00: dir = sourceCam->getViewDirection()*-10; break;
							}
						
							Vec2i end = sourceCam->worldToScreen(UI::handlePos + dir, sourceRect->getWidth(), sourceRect->getHeight());
							Vec2f mouseAxis = end - handlePosInScreen;
							float dif = deltaMousePos.dot(mouseAxis) / sqrtf(getWindowHeight()*getWindowHeight() + getWindowWidth()*getWindowWidth());
				
							cmdProc.executeCommand(new CMD_MoveSelectedJoints(Vec3d(dir.x, dir.y, dir.z)*dif/100.f, true));
						}
					}else if(mode == ROTATE){
						glm::vec3 axis(0,0,0);
						switch(uiColour){
							case 0xFF0000: axis.x -= 1; break;
							case 0x00FF00: axis.y -= 1; break;
							case 0x0000FF: axis.z -= 1; break;
							case 0xFFFF00: axis = glm::vec3(sourceCam->getViewDirection().x, sourceCam->getViewDirection().y, sourceCam->getViewDirection().z); break;
						}
						if(UI::selectedNodes.size() > 0){
							Vec2i dif1 = oldMousePos - handlePosInScreen-sourceRect->getUpperLeft();
							Vec2i dif2 = mMousePos - handlePosInScreen-sourceRect->getUpperLeft();
							float angle1 = atan2(dif1.y, dif1.x);
							float angle2 = atan2(dif2.y, dif2.x);
							float angle = (angle2 - angle1);
					
							console() << angle << std::endl;

							glm::vec3 eulerAngles(angle,angle,angle);
							eulerAngles.x *= axis.x;
							eulerAngles.y *= axis.y;
							eulerAngles.z *= axis.z;

							cmdProc.executeCommand(new CMD_RotateSelectedTransformable(glm::quat(eulerAngles), true));
						}
					}else if(mode == SCALE){
						if(UI::selectedNodes.size() > 0){
							//nothing
							Vec3i dir(0,0,0);
							switch(uiColour){
								case 0xFF0000: dir.x -= 1; break;
								case 0x00FF00: dir.y -= 1; break;
								case 0x0000FF: dir.z -= 1; break;
								case 0xFFFF00: dir.x -= 1; dir.y -= 1; dir.z -= 1; break;
							}
						
							Vec2i end = sourceCam->worldToScreen(UI::handlePos + dir, sourceRect->getWidth(), sourceRect->getHeight());
							Vec2f mouseAxis = end - handlePosInScreen;
							float dif = deltaMousePos.dot(mouseAxis) / sqrtf(getWindowHeight()*getWindowHeight() + getWindowWidth()*getWindowWidth());
							
							console() << dif << std::endl;

							cmdProc.executeCommand(new CMD_ScaleSelectedTransformable(Vec3f(1,1,1) + Vec3d(dir.x, dir.y, dir.z)*dif/100.f));
						}
					}
				}
			}
			oldMousePos = mMousePos;
		}
	}
}

void CinderApp::mouseUp( MouseEvent event ){
	UI::updateHandlePos();
}

void CinderApp::keyDown( KeyEvent event ){
	
	switch( event.getCode() ) {
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
			params->show();
		}else{
			params->hide();
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
		if(UI::selectedNodes.size() != 0){
			std::vector<NodeHierarchical *> temp;
			for(unsigned long int i = 0; i < joints.size(); ++i){
				temp.push_back(joints.at(i));
			}
			cmdProc.executeCommand(new CMD_DeleteJoint(&temp));
			joints.clear();
			for(unsigned long int i = 0; i < temp.size(); ++i){
				joints.push_back(dynamic_cast<Joint *>(temp.at(i)));
			}
		}
		break;
	case KeyEvent::KEY_p:
		cmdProc.executeCommand(new CMD_Parent(&joints));
		break;
	case KeyEvent::KEY_d:
		if(event.isControlDown()){
			// Deselect all
			if(UI::selectedNodes.size() != 0){
				cmdProc.executeCommand(new CMD_SelectNodes(nullptr));
			}
		}
		break;
	case KeyEvent::KEY_z:
		if (event.isControlDown()){
			cmdProc.undo();
		}
		break;
	case KeyEvent::KEY_y:
		if (event.isControlDown()){
			cmdProc.redo();
		}
		break;
	case KeyEvent::KEY_q:
		mode = SELECT;
		params->setOptions( "UI Mode", "label=`SELECT`" );
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
	case KeyEvent::KEY_t:
		mode = CREATE;
		params->setOptions( "UI Mode", "label=`CREATE`" );
		break;
	case KeyEvent::KEY_v:
		mode = PAINT_VOXELS;
		params->setOptions( "UI Mode", "label=`PAINT_VOXELS`" );
		break;
	}
	UI::updateHandlePos();
}

void CinderApp::keyUp( KeyEvent event ){
}

void CinderApp::loadShaders(){
	try{
		jointShader = gl::GlslProg( loadFile("../assets/shaders/joint.vert"), loadFile("../assets/shaders/joint.frag") );
	}catch( const std::exception &e ) {
		console() << e.what() << std::endl;
		quit();
	}
}

void CinderApp::initFbo(gl::Fbo * _fbo, Area _area){
	gl::Fbo::Format fmt;
	// make sure the framebuffer is not anti-aliased
	fmt.setSamples(0);
	fmt.setCoverageSamples(0);
	//if(_fbo == pixelFbo){
		fmt.setColorInternalFormat(GL_RGBA32F);
	//}
	// you can omit these lines if you don't intent to display the picking framebuffer
	fmt.setMagFilter(GL_NEAREST);
	fmt.setMinFilter(GL_LINEAR);
	
	unsigned int w = max(1, _area.getWidth());
	unsigned int h = max(1, _area.getHeight());
	*_fbo = gl::Fbo(w, h, fmt);

	// work-around for an old Cinder issue
	_fbo->getTexture(0).setFlipped(true);
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
	gl::color( Colorf(0.2f, 0.2f, 0.2f) );
	for(float i=-size;i<=size;i+=step) {
		gl::drawLine( Vec3f(i, 0.0f, -size), Vec3f(i, 0.0f, size) );
		gl::drawLine( Vec3f(-size, 0.0f, i), Vec3f(size, 0.0f, i) );
	}
}

Vec2d fromRectToRect(Vec2d _p, Rectf _r1, Rectf _r2){
	Vec2d res;
	res.x = ((_p.x-_r1.x1)/_r1.getWidth())*_r2.getWidth() + _r2.x1;
	res.y = ((_p.y-_r1.y1)/_r1.getHeight())*_r2.getHeight() + _r2.y1;
	//console() << _p << std::endl << _r1 << std::endl << _r2 << std::endl << res << std::endl;
	return res;
}

Vec3d CinderApp::getCameraCorrectedPos(){
	Vec3d res;
	float x = (int)mMousePos.x;
	float y = (int)mMousePos.y;
	if(rectTop.contains(mMousePos)){
		Vec2d t = fromRectToRect(Vec2f(x, y), rectTop, boundsTop);
		res.x = t.x;
		res.y = 0;
		res.z = t.y;
	}else if(rectRight.contains(mMousePos)){
		x = rectRight.x2 - x + rectRight.x1;
		y = rectRight.y2 - y + rectRight.y1;
		Vec2d t = fromRectToRect(Vec2f(x, y), rectRight, boundsRight);
		res.x = 0;
		res.y = t.y;
		res.z = t.x;
	}else if(rectFront.contains(mMousePos)){
		y = rectFront.y2 - y + rectFront.y1;
		Vec2d t = fromRectToRect(Vec2f(x, y), rectFront, boundsFront);
		res.x = t.x;
		res.y = t.y;
		res.z = 0;
	}else if(rectPersp.contains(mMousePos)){
		res.x = 0;
		res.y = 0;
		res.z = 0;
	}
	return res;
}

void CinderApp::saveSkeleton() {
	try{
		console() << "saveSkeleton" << endl;
		SkeletonData::SaveSkeleton(directory, fileName, joints);
		message = "Saved skeleton";
	}catch (exception ex){
		message = string(ex.what());
	}
}

void CinderApp::loadSkeleton() {
	try{
		// Deselect everything
		cmdProc.executeCommand(new CMD_SelectNodes(nullptr));
		console() << "loadSkeleton" << endl;
		joints.clear();

		joints = SkeletonData::LoadSkeleton(filePath);
		message = "Loaded skeleton";

		// Clear the undo/redo history
		cmdProc.reset();
	}catch (exception ex){
		message = string(ex.what());
	}
}

void CinderApp::setKeyframe(){
	if(UI::selectedNodes.size() != 0){
		for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
			cmdProc.executeCommand(new CMD_KeyAll(UI::time));
		}
	}
}

void CinderApp::togglePlay(){
	if (!play){
		timelineParams->setOptions( "togglePlay", "label=`PLAYING`" );
	}else{
		timelineParams->setOptions( "togglePlay", "label=`STOPPED`" );
	}

	play = !play;
}


CINDER_APP_BASIC( CinderApp, RendererGl )