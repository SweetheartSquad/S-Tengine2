/*
 Copyright (c) 2010-2012, Paul Houx - All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/ObjLoader.h"
#include "cinder/MayaCamUI.h"
#include "cinder/TriMesh.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"

#include <sstream>

#include "Voxel.h"


using namespace ci;
using namespace ci::app;
using namespace std;

class CinderApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);

	void setup();
	void shutdown();
	void update();
	void draw();

	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseUp( MouseEvent event );

	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );

	void resize();

	//! renders the scene
	void render();
	//! samples the color buffer to determine which object is under the mouse
	void pickVoxel( const Vec2i &position );
	void pickFace( const Vec2i &position );
	//! loads an OBJ file, writes it to a much faster binary file and loads the mesh
	void loadMesh( const std::string &objFile, const std::string &meshFile, TriMesh *mesh);
	//! loads the shaders
	void loadShaders();
	//! initializes empty fbos
	void initFbo(gl::Fbo & _fbo, Area _area);
	//! draws a grid on the floor
	void drawGrid(float size=100.0f, float step=10.0f);
public:
	// utility functions to translate colors to and from ints or chars 
	static Color charToColor( unsigned char r, unsigned char g, unsigned char b ){
		return Color(r / 255.0f, g / 255.0f, b / 255.0f);
	};

	static unsigned int charToInt( unsigned char r, unsigned char g, unsigned char b ){
		return b + (g << 8) + (r << 16);
	};

	static Color intToColor( unsigned int i ){
		unsigned char r = (i >> 16) & 0xFF;
		unsigned char g = (i >> 8) & 0xFF;
		unsigned char b = (i >> 0) & 0xFF;
		return Color(r / 255.0f, g / 255.0f, b / 255.0f);
	};

	static unsigned int colorToInt( const Color &color ){
		unsigned char r = (unsigned char)(color.r * 255);
		unsigned char g = (unsigned char)(color.g * 255);
		unsigned char b = (unsigned char)(color.b * 255);
		return b + (g << 8) + (r << 16);
	};
protected:
	//! our camera
	MayaCamUI		mCamera;

	//! our Phong shader, which supports multiple targets
	gl::GlslProg	mPhongShader;

	//! our main framebuffer (AA, containing 2 color buffers)
	gl::Fbo			mFbo;
	//! our little picking framebuffer (non-AA) 
	gl::Fbo			mPickingFboVoxel;
	gl::Fbo			mPickingFboFace;

	//! framebuffer for UI elements
	gl::Fbo			mUIFbo;

	//! keeping track of our cursor position
	Vec2i			mMousePos;

	//! fancy font fizzle
	Font			mFont;

	//! background color
	Color			mColorBackground;

	//! colour channel to draw to the main viewport
	unsigned long int channel;
	
	gl::Material voxelMaterial;
	std::vector<Voxel *> voxels;

	//! pointer to currently selected voxel
	Voxel * selectedVoxel;
	//! currently selected face of selected voxel (Top, Bottom, Left, Right, Front, or Back)
	std::string selectedFace;
};

void CinderApp::prepareSettings(Settings *settings)
{
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup()
{
	// note: we will setup our camera in the 'resize' function,
	//  because it is called anyway so we don't have to set it up twice

	// create materials
	voxelMaterial.setAmbient(Color::black());
	voxelMaterial.setDiffuse(Color::white());
	voxelMaterial.setSpecular(Color::white());
	voxelMaterial.setShininess( 20.0f );

	// load shaders
	loadShaders();

	// load font
	mFont = Font(loadAsset("font/b2sq.ttf"), 32);	

	// set background color
	mColorBackground = Color(0.1f, 0.1f, 0.1f);

	for(int x = 0; x < 100; ++x){
		for(int z = 0; z < 10; ++z){
			Voxel *t = new Voxel(x*Voxel::resolution,0,z*Voxel::resolution,false);
			voxels.push_back(t);
		}
	}
	channel = 0;
}

void CinderApp::shutdown()
{
	for(int i = 0; i < voxels.size(); ++i){
		delete voxels.at(i);
	}
	voxels.clear();
	Voxel::voxelMap.clear();
}

void CinderApp::update()
{
}

void CinderApp::draw()
{
	// bind framebuffer
	mFbo.bindFramebuffer();

	// render the scene
	render();

	// unbind framebuffer
	mFbo.unbindFramebuffer();

	// draw the scene
	Rectf windowBounds = Rectf(getWindowBounds());
	gl::color( Color::white() );
	gl::draw( mFbo.getTexture(channel), windowBounds );
	// draw the colour channels in the upper left corner
	windowBounds *=  0.2f;
	gl::draw( mFbo.getTexture(0), windowBounds);
	windowBounds.y1 += windowBounds.y2;
	windowBounds.y2 += windowBounds.y2;
	gl::draw( mFbo.getTexture(1), windowBounds);
	windowBounds.y2 += windowBounds.y1;
	windowBounds.y1 += windowBounds.y1;
	gl::draw( mFbo.getTexture(2), windowBounds);
	
	/*// perform picking and display the results
	//  (alternatively you can do it in the 'mouseMove' or 'mouseDown' function)
	gl::enableAlphaBlending();
	gl::drawStringCentered( pick( mMousePos ), Vec2f(0.5f * getWindowWidth(), getWindowHeight() - 50.0f), Color::white(), mFont );
	gl::disableAlphaBlending();*/
	if(selectedVoxel != 0){
		std::ostringstream s;
		s << *selectedVoxel;
		gl::drawStringCentered(s.str(), Vec2f(0.5f * getWindowWidth(), getWindowHeight() - 50.0f), Color::white(), mFont );
	}

	// draw the picking framebuffer in the upper right corner
	Rectf rct = (Rectf) mPickingFboVoxel.getBounds() * 5.0f;
	rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
	if(mPickingFboVoxel){
		gl::draw( mPickingFboVoxel.getTexture(), Rectf(rct.x1, rct.y1, rct.x2, rct.y2) );
	}if(mPickingFboVoxel){
		gl::draw( mPickingFboFace.getTexture(), Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2) );
	}

	// draw the UI over the scene
	if(mPickingFboFace){
		gl::enableAlphaBlending();
		windowBounds.y2 += windowBounds.y1;
		windowBounds.y1 += windowBounds.y1;
		gl::draw(mUIFbo.getTexture(), windowBounds);
	}
}

void CinderApp::mouseMove( MouseEvent event )
{
	mMousePos = event.getPos();
	pickVoxel(mMousePos);
}

void CinderApp::mouseDown( MouseEvent event )
{
	// handle the camera
	mCamera.mouseDown( event.getPos() );
}

void CinderApp::mouseDrag( MouseEvent event )
{
	mMousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		mCamera.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}else{
		pickFace(mMousePos);
		pickVoxel(mMousePos);
		Vec3f pos;
		if(selectedVoxel != 0){
			pos = selectedVoxel->pos;
			if(selectedFace == "Right"){
				pos.x += Voxel::resolution;
			}else if(selectedFace == "Left"){
				pos.x -= Voxel::resolution;
			}else if(selectedFace == "Front"){
				pos.z += Voxel::resolution;
			}else if(selectedFace == "Back"){
				pos.z -= Voxel::resolution;
			}else if(selectedFace == "Top"){
				pos.y += Voxel::resolution;
			}else if(selectedFace == "Bottom"){
				pos.y -= Voxel::resolution;
			}else{
				//face is uncertain
			}
		}else{
			pos = Vec3f(mMousePos.x, 0, -mMousePos.y);
		}
		Voxel * t = new Voxel(pos, false);
		voxels.push_back(t);
	}
}

void CinderApp::mouseUp( MouseEvent event )
{
}

void CinderApp::keyDown( KeyEvent event )
{
	switch( event.getCode() ) {
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	case KeyEvent::KEY_f:
		setFullScreen( !isFullScreen() );
		break;
	case KeyEvent::KEY_1:
		channel = 0;
		break;
	case KeyEvent::KEY_2:
		channel = 1;
		break;
	case KeyEvent::KEY_3:
		channel = 2;
		break;
	}
}

void CinderApp::keyUp( KeyEvent event )
{
}

void CinderApp::resize()
{
	// setup the camera
	CameraPersp cam = mCamera.getCamera();
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 0.1f, 1000.0f );
	mCamera.setCurrentCam( cam );

	// create or resize framebuffer if needed
	if(!mFbo || mFbo.getWidth() != getWindowWidth() || mFbo.getHeight() != getWindowHeight()) {
		gl::Fbo::Format fmt;

		// we create multiple color targets:
		//  -one for the scene as we will view it
		//  -one to contain a color coded version of the scene that we can use for picking
		fmt.enableColorBuffer( true, 3 );

		// enable multi-sampling for better quality 
		//  (if this sample does not work on your computer, try lowering the number of samples to 2 or 0)
		fmt.setSamples(4);

		// create the buffer
		mFbo = gl::Fbo( getWindowWidth(), getWindowHeight(), fmt );
		mUIFbo = gl::Fbo( getWindowWidth(), getWindowHeight(), fmt );

		// work-around for an old Cinder issue
		mFbo.getTexture(0).setFlipped(true);
		mFbo.getTexture(1).setFlipped(true);
		mFbo.getTexture(2).setFlipped(true);

		mUIFbo.getTexture(0).setFlipped(true);
	}
	pickFace(mMousePos);
	pickVoxel(mMousePos);
}

//

void CinderApp::render()
{
	// clear background
	gl::clear( mColorBackground );

	// specify the camera matrices
	gl::pushMatrices();
	gl::setMatrices( mCamera.getCamera() );

		// setup the light
		gl::Light light( gl::Light::POINT, 0 );
		light.setAmbient( Color::black() );
		light.setDiffuse( Color::white() );
		light.setSpecular( Color::white() );
		light.setPosition( mCamera.getCamera().getEyePoint() );

		// specify render states
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
		gl::enableDepthRead();
		gl::enableDepthWrite();

		// bind phong shader, which renders to both our color targets.
		//  See 'shaders/phong.frag'
		mPhongShader.bind();

		// draw meshes:
		gl::color( Color::white() );

		// -each mesh should have a unique picking color that we can use to 
		//  find out which object is under the cursor. 
		//voxelMaterial.apply();
		for(unsigned long int i = 0; i < voxels.size(); ++i){
			gl::pushModelView();
				mPhongShader.uniform("pickingColor", voxels.at(i)->color);
				gl::translate(voxels.at(i)->pos);
			
				if(selectedVoxel == voxels.at(i)){
					voxelMaterial.setDiffuse(Color::black());
					voxelMaterial.apply();
				}
				gl::drawColorCube(Vec3f(0,0,0), Vec3f(Voxel::resolution*0.9,Voxel::resolution*0.9,Voxel::resolution*0.9));
			
				if(selectedVoxel == voxels.at(i)){
					voxelMaterial.setDiffuse(Color::white());
					voxelMaterial.apply();
				}
			gl::popModelView();
		}

		// unbind shader
		mPhongShader.unbind();
	


	// restore matrices
	gl::popMatrices();
	

	// draw UI axes
	mUIFbo.bindFramebuffer();
	gl::enableAlphaBlending();
	gl::clear(ColorA(0,0,0,0));
	gl::pushModelView();
		gl::setMatrices(mCamera.getCamera());
			Vec3f cof = mCamera.getCamera().getCenterOfInterestPoint();
			float scale = mCamera.getCamera().getEyePoint().distance(cof);
			gl::translate(cof);
			gl::scale(scale/5, scale/5, scale/5);

			gl::lineWidth(5);
			gl::color(0,0,1);
			gl::drawVector(Vec3f(0,0,0),Vec3f(0,0,1),0.4,0.1);
			gl::color(0,1,0);
			gl::drawVector(Vec3f(0,0,0),Vec3f(0,1,0),0.4,0.1);
			gl::color(1,0,0);
			gl::drawVector(Vec3f(0,0,0),Vec3f(1,0,0),0.4,0.1);
		gl::popMatrices();
	gl::popModelView();
	mUIFbo.unbindFramebuffer();
	
	// restore render states
	glPopAttrib();
}

void CinderApp::pickVoxel( const Vec2i &position ){
	// this is the main section of the demo:
	//  here we sample the second color target to find out
	//  which color is under the cursor.

	// prevent errors if framebuffer does not exist
	if(!mFbo){
		console() << "Error: mFbo doesn't exist";
	}

	// first, specify a small region around the current cursor position 
	float scaleX = mFbo.getWidth() / (float) getWindowWidth();
	float scaleY = mFbo.getHeight() / (float) getWindowHeight();
	Vec2i	pixel((int)(position.x * scaleX), (int)((getWindowHeight() - position.y) * scaleY));
	Area	area(pixel.x-5, pixel.y-5, pixel.x+5, pixel.y+5);

	// next, we need to copy this region to a non-anti-aliased framebuffer
	//  because sadly we can not sample colors from an anti-aliased one. However,
	//  this also simplifies the glReadPixels statement, so no harm done.
	//  Here, we create that non-AA buffer if it does not yet exist.
	if(!mPickingFboVoxel) {
		initFbo(mPickingFboVoxel, area);
	}

	// (Cinder does not yet provide a way to handle multiple color targets in the blitTo function, 
	//  so we have to make sure the correct target is selected before calling it)
	glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, mFbo.getId() );
	glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, mPickingFboVoxel.getId() );
	glReadBuffer(GL_COLOR_ATTACHMENT1_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	mFbo.blitTo(mPickingFboVoxel, area, mPickingFboVoxel.getBounds());

	// bind the picking framebuffer, so we can read its pixels
	mPickingFboVoxel.bindFramebuffer();

	// read pixel value(s) in the area
	GLubyte buffer[400]; // make sure this is large enough to hold 4 bytes for every pixel!
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, mPickingFboVoxel.getWidth(), mPickingFboVoxel.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer);

	// unbind the picking framebuffer
	mPickingFboVoxel.unbindFramebuffer();

	// calculate the total number of pixels
	unsigned int total = (mPickingFboVoxel.getWidth() * mPickingFboVoxel.getHeight());

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
		if(Voxel::voxelMap.count(color) == 1){
			selectedVoxel = Voxel::voxelMap.at(color);
		}else{
			selectedVoxel = 0;
		}
	}else{
		// we can't be sure about the color, we probably are on an object's edge
		selectedVoxel = 0;
	}
}

void CinderApp::pickFace( const Vec2i &position )
{
	// this is the main section of the demo:
	//  here we sample the second color target to find out
	//  which color is under the cursor.

	// prevent errors if framebuffer does not exist
	if(!mFbo){
		console() << "Error: mFbo doesn't exist";
	}

	// first, specify a small region around the current cursor position 
	float scaleX = mFbo.getWidth() / (float) getWindowWidth();
	float scaleY = mFbo.getHeight() / (float) getWindowHeight();
	Vec2i	pixel((int)(position.x * scaleX), (int)((getWindowHeight() - position.y) * scaleY));
	Area	area(pixel.x-5, pixel.y-5, pixel.x+5, pixel.y+5);

	// next, we need to copy this region to a non-anti-aliased framebuffer
	//  because sadly we can not sample colors from an anti-aliased one. However,
	//  this also simplifies the glReadPixels statement, so no harm done.
	//  Here, we create that non-AA buffer if it does not yet exist.
	if(!mPickingFboFace) {
		initFbo(mPickingFboFace, area);
	}

	// (Cinder does not yet provide a way to handle multiple color targets in the blitTo function, 
	//  so we have to make sure the correct target is selected before calling it)
	glBindFramebufferEXT( GL_READ_FRAMEBUFFER_EXT, mFbo.getId() );
	glBindFramebufferEXT( GL_DRAW_FRAMEBUFFER_EXT, mPickingFboFace.getId() );
	glReadBuffer(GL_COLOR_ATTACHMENT2_EXT);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	mFbo.blitTo(mPickingFboFace, area, mPickingFboFace.getBounds());

	// bind the picking framebuffer, so we can read its pixels
	mPickingFboFace.bindFramebuffer();

	// read pixel value(s) in the area
	GLubyte buffer[400]; // make sure this is large enough to hold 4 bytes for every pixel!
	glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glReadPixels(0, 0, mPickingFboFace.getWidth(), mPickingFboFace.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, (void*)buffer);

	// unbind the picking framebuffer
	mPickingFboFace.unbindFramebuffer();

	// calculate the total number of pixels
	unsigned int total = (mPickingFboFace.getWidth() * mPickingFboFace.getHeight());

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
		switch(color){
		case 8388479:	selectedFace = "Top"; break;
		case 16744319:	selectedFace = "Right"; break;
		case 8355839:	selectedFace = "Front"; break;
		case 8323199:	selectedFace = "Bottom"; break;
		case 32639:		selectedFace = "Left"; break;
		case 8355584:	selectedFace = "Back"; break;
		default:		selectedFace = "Unknown"; break;
		}
	}else{
		// we can't be sure about the color, we probably are on an object's edge
		selectedFace = "Uncertain";
	}
}





void CinderApp::loadMesh(const std::string &objFile, const std::string &meshFile, TriMesh *mesh)
{
	try {
		// try to load a binary mesh file - sadly the following line does not work:
		// mMesh.read( loadAsset(meshfile) );

		// note: instead of throwing an exception, Cinder crashes if the
		//   file does not exist. Hopefully this will be changed in future versions.
		DataSourceRef file = loadAsset(meshFile);
		if(!file->createStream()) throw std::exception();

		mesh->read( file );
	}
	catch( ... ) {
		// if it failed or didn't exist, load an obj file...
		DataSourceRef file = loadAsset(objFile);
		if(file->createStream()) {
			ObjLoader obj( file );
			// ...create a mesh...
			obj.load( mesh, true, true, true );
			// ...and write it to a binary mesh file for future use
			mesh->write( writeFile( getAssetPath("") / meshFile ) );
		}
	}
}

void CinderApp::loadShaders()
{
	try {
		mPhongShader = gl::GlslProg( loadFile("../assets/shaders/phong.vert"), loadFile("../assets/shaders/phong.frag") );
	}
	catch( const std::exception &e ) {
		console() << e.what() << std::endl;
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

	_fbo = gl::Fbo(_area.getWidth(), _area.getHeight(), fmt);

	// work-around for an old Cinder issue
	_fbo.getTexture(0).setFlipped(true);
}

CINDER_APP_BASIC( CinderApp, RendererGl )