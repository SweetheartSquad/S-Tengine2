#pragma once

#include "CinderApp.h"
#include "UI.h"
#include "CMD_DeleteJoint.h"
#include "CMD_SelectNodes.h"
#include "CMD_MoveSelectedJoints.h"
#include "CMD_SetParent.h"

void CinderApp::prepareSettings(Settings *settings){
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup(){
	drawParams = true;
	params = params::InterfaceGl::create( getWindow(), "Params", toPixels( Vec2i( 150, 100 ) ) );
	params->addButton( "Switch Mode", std::bind( &CinderApp::switchMode, this ) );
	params->addText( "Interaction Mode", "label=`CREATE`" );

	params->addSeparator();

	params->addParam("Directory", &directory);
	params->addParam("File Name", &fileName);
	params->addButton("Save", std::bind( &CinderApp::saveSkeleton, this ));
	params->addSeparator();
	params->addParam("Bones File", &filePath);
	params->addButton("Load", std::bind(&CinderApp::loadSkeleton, this));
	params->addSeparator();
	params->addParam("Message", &message, "", true);
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
	mColorBackground = Color(0.1f, 0.1f, 0.1f);
	
	channel = 0;

	mode = CREATE;
}


void CinderApp::resize(){
	unsigned int w = max(1, getWindowWidth());
	unsigned int h = max(1, getWindowHeight());
	double r = getWindowAspectRatio();
	Area b = getWindowBounds();
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
		initFbo(fboUI, b);
	}
	
	w /= 2;
	h /= 2;
	b.x1 /= 2;
	b.x2 /= 2;
	b.y1 /= 2;
	b.y2 /= 2;

	if(!fboPersp || fboPersp.getWidth() != w || fboPersp.getHeight() != h) {
		initMultiChannelFbo(fboPersp, b);
	}if(!fboTop || fboTop.getWidth() != w || fboTop.getHeight() != h){
		initMultiChannelFbo(fboTop, b);
	}if(!fboRight || fboRight.getWidth() != w || fboRight.getHeight() != h){
		initMultiChannelFbo(fboRight, b);
	}if(!fboFront || fboFront.getWidth() != w || fboFront.getHeight() != h){
		initMultiChannelFbo(fboFront, b);
	}
}


void CinderApp::shutdown(){
	for(unsigned long int i = 0; i < Joints.size(); ++i){
		Joint::deleteJoints(Joints.at(i));
	}
	Joints.clear();
	UI::selectedNodes.clear();
}

void CinderApp::update(){
	
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

		// draw joints:
		for(Joint * j : Joints){
			j->draw(&jointShader);
		}

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
			Vec3f avg(0, 0, 0);	
			for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
				avg += ((Joint *)UI::selectedNodes.at(i))->getPos(false);
			}
			avg /= UI::selectedNodes.size();

			gl::translate(avg);
			gl::lineWidth(2);
			if(cam.isPersp()){
				// If the camera is a perspective view, scale the coordinate frame proportionally to the distance from camera
				gl::scale(cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()),cam.worldToEyeDepth(cam.getCenterOfInterestPoint()));
			}
			gl::drawCoordinateFrame(0.3, 0.15, 0.03);
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

void CinderApp::mouseDown( MouseEvent event ){
	mMousePos = event.getPos();

	// handle the camera
	camMayaPersp.mouseDown( mMousePos );

	if(event.isRight()){
		handleUI(mMousePos);
	}

	if(!event.isAltDown() && event.isLeft()){
		
		if(mode == CREATE){
			Vec3d pos = getCameraCorrectedPos();
			
			if(UI::selectedNodes.size() == 1){
				cmdProc.executeCommand(new CMD_CreateJoint(&Joints, pos, (Joint *)UI::selectedNodes.at(0)));
			}else{
				cmdProc.executeCommand(new CMD_CreateJoint(&Joints, pos, nullptr));
			}
		}else if(mode == SELECT){
			pickJoint(mMousePos);
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
			//cmdProc.executeCommand(new CMD_MoveSelectedJoints(getCameraCorrectedPos(), false));
		}else if(event.isRightDown()){
			if(UI::selectedNodes.size() > 0){

				Vec2i delta = mMousePos - oldMousePos;

				float dif = delta.dot(mouseAxis);

				dif /= sqrtf(getWindowHeight()*getWindowHeight() + getWindowWidth()*getWindowWidth());
				
				cmdProc.executeCommand(new CMD_MoveSelectedJoints(Vec3d(dir.x*dif/100.f, dir.y*dif/100.f, dir.z*dif/100.f), true));
				
				oldMousePos = mMousePos;
			}
		}
	}
}

void CinderApp::mouseUp( MouseEvent event ){
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
			cmdProc.executeCommand(new CMD_DeleteJoint(&Joints));
		}
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
	}
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

void CinderApp::initFbo(gl::Fbo & _fbo, Area _area){
	gl::Fbo::Format fmt;
	// make sure the framebuffer is not anti-aliased
	fmt.setSamples(0);
	fmt.setCoverageSamples(0);

	// you can omit these lines if you don't intent to display the picking framebuffer
	fmt.setMagFilter(GL_NEAREST);
	fmt.setMinFilter(GL_LINEAR);
	
	unsigned int w = max(1, _area.getWidth());
	unsigned int h = max(1, _area.getHeight());
	_fbo = gl::Fbo(w, h, fmt);

	// work-around for an old Cinder issue
	_fbo.getTexture(0).setFlipped(true);
}

void CinderApp::initMultiChannelFbo(gl::Fbo & _fbo, Area _area){
	gl::Fbo::Format fmt;

	// we create multiple color targets:
	//  -one for the scene as we will view it
	//  -one to contain a color coded version of the scene that we can use for picking
	fmt.enableColorBuffer( true, 3 );

	// anti-aliasing samples
	fmt.setSamples(0);
	fmt.setCoverageSamples(0);

	// create the buffer
	unsigned int w = max(1, _area.getWidth());
	unsigned int h = max(1, _area.getHeight());
	_fbo = gl::Fbo( w, h, fmt );

	// work-around for an old Cinder issue
	_fbo.getTexture(0).setFlipped(true);
	_fbo.getTexture(1).setFlipped(true);
	_fbo.getTexture(2).setFlipped(true);
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


void CinderApp::handleUI( const Vec2i &pos ){
	// this is the main section of the demo:
	//  here we sample the second color target to find out
	//  which color is under the cursor.
	gl::Fbo * sourceFbo = &fboUI;
	Rectf * sourceRect;
	const Camera * sourceCam;
	if(rectTop.contains(pos)){
		sourceCam = &camTop;
		sourceRect = &rectTop;
	}else if(rectRight.contains(pos)){
		sourceCam = &camRight;
		sourceRect = &rectRight;
	}else if(rectFront.contains(pos)){
		sourceCam = &camFront;
		sourceRect = &rectFront;
	}else if(rectPersp.contains(pos)){
		sourceCam = &camMayaPersp.getCamera();
		sourceRect = &rectPersp;
	}else{
		return;
	}



	// first, specify a small region around the current cursor position 
	float scaleX = sourceFbo->getWidth() / (float) getWindowWidth();
	float scaleY = sourceFbo->getHeight() / (float) getWindowHeight();
	Vec2i	pixel((int)((pos.x) * scaleX), (int)((getWindowHeight() - pos.y) * scaleY));

	//pixel = fromRectToRect(pixel, sourceFbo->getBounds(), *sourceRect);

	Area	area(pixel.x-5, pixel.y-5, pixel.x+5, pixel.y+5);

	// next, we need to copy this region to a non-anti-aliased framebuffer
	//  because sadly we can not sample colors from an anti-aliased one. However,
	//  this also simplifies the glReadPixels statement, so no harm done.
	//  Here, we create that non-AA buffer if it does not yet exist.
	if(!pickingFboUI) {
		initFbo(pickingFboUI, area);
	}
	
	// bind the picking framebuffer, so we can clear it and then read its pixels later
	pickingFboUI.bindFramebuffer();
	gl::clear();

	// (Cinder does not yet provide a way to handle multiple color targets in the blitTo function, 
	//  so we have to make sure the correct target is selected before calling it)
	glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, sourceFbo->getId() );
	glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, pickingFboUI.getId() );
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	sourceFbo->blitTo(pickingFboUI, area, pickingFboUI.getBounds());


	// read pixel value(s) in the area
	GLubyte buffer[400]; // make sure this is large enough to hold 4 bytes for every pixel!
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, pickingFboUI.getWidth(), pickingFboUI.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer);

	// unbind the picking framebuffer
	mPickingFboJoint.unbindFramebuffer();

	// calculate the total number of pixels
	unsigned int total = (pickingFboUI.getWidth() * pickingFboUI.getHeight());

	// now that we have the color information, count each occuring color
	unsigned int color;

	std::map<unsigned int, unsigned int> occurences;
	for(size_t i=0;i<total;++i) {
		color = charToInt( buffer[(i*4)+0], buffer[(i*4)+1], buffer[(i*4)+2] );
		occurences[color]++;
	}

	// find the most occuring color
	unsigned int max = 0;
	std::map<unsigned int, unsigned int>::const_iterator itr;
	for(itr=occurences.begin();itr!=occurences.end();++itr) {
		if(itr->second > max) {
			color = itr->first;
			max = itr->second;
		}
	}

	// if this color is present in at least 50% of the pixels, we can safely assume that it is indeed belonging to one object
	if(max >= (total / 2)) {
		console() << color << std::endl;
		
		
		dir.set(Vec3i(0,0,0));

		switch(color){
			case 16711680: dir.x -= 10; break;	//r
			case 65280: dir.y -= 10; break;		//g
			case 255: dir.z -= 10; break;		//b
			/*
			case 65535: dir.x += 10; break;		//m
			case 16711935: dir.y += 10; break;	//c
			case 16776960: dir.z += 10; break;	//y
			*/
		}

		if(UI::selectedNodes.size() != 0){
			oldMousePos = mMousePos;
			
			Vec3f avg(0, 0, 0);	
			for(unsigned long int i = 0; i < UI::selectedNodes.size(); ++i){
				avg += ((Joint *)UI::selectedNodes.at(i))->getPos(false);
			}
			avg /= UI::selectedNodes.size();
			
			Vec2i start = sourceCam->worldToScreen(avg, sourceRect->getWidth(), sourceRect->getHeight());
			Vec2i end = sourceCam->worldToScreen(avg + dir, sourceRect->getWidth(), sourceRect->getHeight());

			mouseAxis = end - start;

			console() << dir << " " << mouseAxis << std::endl;
		}
	}else{
		// we can't be sure about the color, we probably are on an object's edge
		
	}
}

void CinderApp::pickJoint( const Vec2i &pos ){
	// get the corresponding camera and screen area
	gl::Fbo * sourceFbo;
	Rectf * sourceRect;
	if(rectTop.contains(pos)){
		sourceFbo = &fboTop;
		sourceRect = &rectTop;
	}else if(rectRight.contains(pos)){
		sourceFbo = &fboRight;
		sourceRect = &rectRight;
	}else if(rectFront.contains(pos)){
		sourceFbo = &fboFront;
		sourceRect = &rectFront;
	}else if(rectPersp.contains(pos)){
		sourceFbo = &fboPersp;
		sourceRect = &rectPersp;
	}else{
		return;
	}


	// first, specify a small region around the current cursor position 
	float scaleX = sourceFbo->getWidth() / (float) sourceRect->getWidth();
	float scaleY = sourceFbo->getHeight() / (float) sourceRect->getHeight();
	Vec2i	pixel((int)((pos.x - sourceRect->x1) * scaleX), (int)((sourceRect->y2 - pos.y) * scaleY));

	//pixel = fromRectToRect(pixel, sourceFbo->getBounds(), *sourceRect);

	Area	area(pixel.x-5, pixel.y-5, pixel.x+5, pixel.y+5);

	// next, we need to copy this region to a non-anti-aliased framebuffer
	//  because sadly we can not sample colors from an anti-aliased one. However,
	//  this also simplifies the glReadPixels statement, so no harm done.
	//  Here, we create that non-AA buffer if it does not yet exist.
	if(!mPickingFboJoint) {
		initFbo(mPickingFboJoint, area);
	}
	
	// bind the picking framebuffer, so we can clear it and then read its pixels later
	mPickingFboJoint.bindFramebuffer();
	gl::clear();

	// (Cinder does not yet provide a way to handle multiple color targets in the blitTo function, 
	//  so we have to make sure the correct target is selected before calling it)
	glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, sourceFbo->getId() );
	glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, mPickingFboJoint.getId() );
	glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	sourceFbo->blitTo(mPickingFboJoint, area, mPickingFboJoint.getBounds());


	// read pixel value(s) in the area
	GLubyte buffer[400]; // make sure this is large enough to hold 4 bytes for every pixel!
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, mPickingFboJoint.getWidth(), mPickingFboJoint.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer);

	// unbind the picking framebuffer
	mPickingFboJoint.unbindFramebuffer();

	// calculate the total number of pixels
	unsigned int total = (mPickingFboJoint.getWidth() * mPickingFboJoint.getHeight());

	// now that we have the color information, count each occuring color
	unsigned int color;

	std::map<unsigned int, unsigned int> occurences;
	for(size_t i=0;i<total;++i) {
		color = charToInt( buffer[(i*4)+0], buffer[(i*4)+1], buffer[(i*4)+2] );
		occurences[color]++;
	}

	// find the most occuring color
	unsigned int max = 0;
	std::map<unsigned int, unsigned int>::const_iterator itr;
	for(itr=occurences.begin();itr!=occurences.end();++itr) {
		if(itr->second > max) {
			color = itr->first;
			max = itr->second;
		}
	}

	// if this color is present in at least 50% of the pixels, 
	//  we can safely assume that it is indeed belonging to one object
	if(max >= (total / 2)) {
		if(Joint::jointMap.count(color) == 1){
			if(UI::selectedNodes.size() == 1){
				if(UI::selectedNodes.at(0) != (Node *)Joint::jointMap.at(color)){
					cmdProc.executeCommand(new CMD_SelectNodes((Node *)Joint::jointMap.at(color)));
				}
			}else{
				cmdProc.executeCommand(new CMD_SelectNodes((Node *)Joint::jointMap.at(color)));
			}
		}else{
			if(UI::selectedNodes.size() != 0){
				cmdProc.executeCommand(new CMD_SelectNodes(nullptr));
			}
		}
	}else{
		// we can't be sure about the color, we probably are on an object's edge
		if(UI::selectedNodes.size() != 0){
			cmdProc.executeCommand(new CMD_SelectNodes(nullptr));
		}
	}
}

void CinderApp::switchMode(){
	if(mode == CREATE){
		mode = SELECT;
		params->setOptions( "Interaction Mode", "label=`SELECT`" );
	}else{
		mode = CREATE;
		params->setOptions( "Interaction Mode", "label=`CREATE`" );
	}
}

void CinderApp::saveSkeleton() {
	try{
		console() << "saveSkeleton" << endl;
		SkeletonData::SaveSkeleton(directory, fileName, Joints);
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
		Joints.clear();

		Joints = SkeletonData::LoadSkeleton(filePath);
		message = "Loaded skeleton";

		// Clear the undo/redo history
		cmdProc.reset();
	}catch (exception ex){
		message = string(ex.what());
	}
}


CINDER_APP_BASIC( CinderApp, RendererGl )