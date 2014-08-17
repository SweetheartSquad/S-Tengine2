#include "cinder/MayaCamUI.h"
#include "cinder/TriMesh.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/params/Params.h"

#include <sstream>

#include "Joint.h"


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
	void render(const Camera & cam);
	//! loads an OBJ file, writes it to a much faster binary file and loads the mesh
	void loadMesh( const std::string &objFile, const std::string &meshFile, TriMesh *mesh);
	//! loads the shaders
	void loadShaders();
	//! initializes empty single channel fbos
	void initFbo(gl::Fbo & _fbo, Area _area);
	//! initializes empty 3-channel fbos
	void initMultiChannelFbo(gl::Fbo & _fbo, Area _area);

	//! draws a grid on the floor
	void drawGrid(float size=100.0f, float step=10.0f);

	void newJoint(Vec3d pos, Joint * parent = NULL);
	Vec3d getCameraCorrectedPos();

	void pickJoint(const Vec2i &pos);
	void switchMode();

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
	//params
	bool drawParams;
	params::InterfaceGlRef params;

	//! our cameras
	MayaCamUI camMayaPersp;
	CameraOrtho camTop;
	CameraOrtho camRight;
	CameraOrtho camFront;

	//! our Phong shader, which supports multiple targets
	gl::GlslProg	jointShader;

	//! our little picking framebuffer (non-AA) 
	//gl::Fbo			mPickingFboVoxel;
	//gl::Fbo			mPickingFboFace;
	
	//framebuffers for cameras
	gl::Fbo fboTop;
	gl::Fbo fboRight;
	gl::Fbo fboFront;
	gl::Fbo	fboPersp; //! our main framebuffer (AA, containing 2 color buffers)

	//framebuffer for selecting joints
	gl::Fbo mPickingFboJoint;
	
	//rect definitions for cameras sizes
	Rectf rectTop;
	Rectf rectRight;
	Rectf rectFront;
	Rectf rectPersp;
	//rect definitions for cameras sizes
	Rectf boundsTop;
	Rectf boundsRight;
	Rectf boundsFront;
	//Rectf boundsPersp;

	//! keeping track of our cursor position
	Vec2i			mMousePos;

	//! background color
	Color			mColorBackground;

	//! colour channel to draw to the main viewport
	unsigned long int channel;
	
	gl::Material JointMaterial;
	
	std::vector<Joint *> Joints;
	Joint * selectedJoint;

	enum UImode{
		CREATE,
		SELECT
	} mode;
	
};

void CinderApp::prepareSettings(Settings *settings){
	settings->setWindowSize(900, 600);
	settings->setFrameRate(100.0f);
	settings->setTitle("Picking using multiple targets and color coding");
}

void CinderApp::setup(){
	drawParams = true;
	params = params::InterfaceGl::create( getWindow(), "Params", toPixels( Vec2i( 200, 400 ) ) );
	params->addButton( "Switch Mode", std::bind( &CinderApp::switchMode, this ) );
	params->addText( "Interaction Mode", "label=`CREATE`" );
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

	selectedJoint = NULL;
	

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
	
	rectTop.set(0,0,w2,h2);
	rectRight.set(w2,0,w,h2);
	rectFront.set(0,h2,w2,h);
	rectPersp.set(w2,h2,w,h);
	
	boundsTop.set(-r, -1, r, 1);
	boundsRight.set(-r, -1, r, 1);
	boundsFront.set(-r, -1, r, 1);
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
	for(int i = 0; i < Joints.size(); ++i){
		delete Joints.at(i);
	}
	Joints.clear();
	selectedJoint = NULL;
}

void CinderApp::update(){}

void CinderApp::draw(){
	fboPersp.bindFramebuffer();
	render(camMayaPersp.getCamera());
	fboPersp.unbindFramebuffer();
	
	fboTop.bindFramebuffer();
	render(camTop);
	fboTop.unbindFramebuffer();

	fboRight.bindFramebuffer();
	render(camRight);
	fboRight.unbindFramebuffer();

	fboFront.bindFramebuffer();
	render(camFront);
	fboFront.unbindFramebuffer();

	// draw the scene
	gl::color( Color::white() );
	
	gl::draw( fboTop.getTexture(channel), rectTop );
	gl::drawStrokedRect(rectTop);
	
	gl::draw( fboRight.getTexture(channel), rectRight );
	gl::drawStrokedRect(rectRight);
	
	gl::draw( fboFront.getTexture(channel), rectFront );
	gl::drawStrokedRect(rectFront);
	
	gl::draw( fboPersp.getTexture(channel), rectPersp );
	gl::drawStrokedRect(rectPersp);

	// draw the picking framebuffer in the upper right corner
	if(mPickingFboJoint){
		Rectf rct = (Rectf) mPickingFboJoint.getBounds() * 5.0f;
		rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
		gl::draw( mPickingFboJoint.getTexture(), Rectf(rct.x1, rct.y1, rct.x2, rct.y2) );
		gl::drawStrokedRect(Rectf(rct.x1, rct.y1, rct.x2, rct.y2));
	}

	if(drawParams){
		params->draw();
	}
	// draw the colour channels in the upper left corner
	/*windowBounds *=  0.2f;
	gl::draw( fboPersp.getTexture(0), windowBounds);
	windowBounds.y1 += windowBounds.y2;
	windowBounds.y2 += windowBounds.y2;
	gl::draw( fboPersp.getTexture(1), windowBounds);
	windowBounds.y2 += windowBounds.y1;
	windowBounds.y1 += windowBounds.y1;
	gl::draw( fboPersp.getTexture(2), windowBounds);*/

	
}

void CinderApp::render(const Camera & cam){
	
	// clear background
	gl::clear( mColorBackground );

	// specify the camera matrices
	gl::pushMatrices();
	gl::setMatrices(cam);

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
			if(j->parent == nullptr){
				j->draw(&jointShader);
			}
			if(selectedJoint == j){
				gl::drawColorCube(j->getPos(false), Vec3f(0.1,0.1,0.1));
			}
		}

		// unbind shader
		jointShader.unbind();
	
		//gl::drawColorCube(Vec3f(0,0,0),Vec3f(5,5,5));

	// restore matrices
	gl::popMatrices();

	// restore render states
	glPopAttrib();
}

void CinderApp::mouseMove( MouseEvent event )
{
	mMousePos = event.getPos();

	//pickJoint(mMousePos);
}

void CinderApp::mouseDown( MouseEvent event ){
	// handle the camera
	camMayaPersp.mouseDown( event.getPos() );

	if(!event.isAltDown() && event.isLeft()){
		
		if(mode == CREATE){
			Vec3d pos = getCameraCorrectedPos();
		
			if(selectedJoint != NULL){
				if(selectedJoint->building){
					newJoint(pos, selectedJoint);
				}else{
					newJoint(pos);
				}
			}else{
				newJoint(pos);
			}
		}else if(mode == SELECT){
			pickJoint(mMousePos);
		}
	}
}

void CinderApp::mouseDrag( MouseEvent event )
{
	mMousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		camMayaPersp.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}else{
		if(selectedJoint != NULL){
			if(selectedJoint->building){
				selectedJoint->setPos(getCameraCorrectedPos());
			}
		}
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
	case KeyEvent::KEY_F1:
		drawParams = !drawParams;
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
	case KeyEvent::KEY_4:
		channel = 3;
		break;
	case KeyEvent::KEY_RETURN:
		if(selectedJoint != NULL){
			if(selectedJoint->building){
				selectedJoint->building = false;
			}
		}
		break;
	}
}

void CinderApp::keyUp( KeyEvent event )
{
}

void CinderApp::loadShaders()
{
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

	// enable multi-sampling for better quality 
	//  (if this sample does not work on your computer, try lowering the number of samples to 2 or 0)
	fmt.setSamples(4);

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

void CinderApp::newJoint(Vec3d pos, Joint * parent){
	Joint * b;
	if(parent != NULL){
		b = new Joint(parent);
		b->parent = parent;
		parent->building = false;
		parent->children.push_back(b);
	}else{	
		b = new Joint();
	}
	b->setPos(pos);
	selectedJoint = b;
	Joints.push_back(b);
}

Vec2d fromRectToRect(Vec2d _p, Rectf _r1, Rectf _r2){
	Vec2d res;
	res.x = ((_p.x-_r1.x1)/_r1.getWidth())*_r2.getWidth() + _r2.x1;
	res.y = ((_p.y-_r1.y1)/_r1.getHeight())*_r2.getHeight() + _r2.y1;
	console() << _p << std::endl << _r1 << std::endl << _r2 << std::endl << res << std::endl;
	return res;
}

Vec3d CinderApp::getCameraCorrectedPos(){
	Vec3d res;
	float w = getWindowWidth();
	float h = getWindowHeight();
	float x = (mMousePos.x);
	float y = (mMousePos.y);
	if(rectTop.contains(mMousePos)){
		Vec2d t = fromRectToRect(Vec2f(x,y), rectTop, boundsTop);
		res.x = t.x;
		res.y = 0;
		res.z = t.y;
	}else if(rectRight.contains(mMousePos)){
		x = rectRight.x2 - x + rectRight.x1;
		y = rectRight.y2 - y + rectRight.y1;
		Vec2d t = fromRectToRect(Vec2f(x,y), rectRight, boundsRight);
		res.x = 0;
		res.y = t.y;
		res.z = t.x;
	}else if(rectFront.contains(mMousePos)){
		y = rectFront.y2 - y + rectFront.y1;
		Vec2d t = fromRectToRect(Vec2f(x,y), rectFront, boundsFront);
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


void CinderApp::pickJoint( const Vec2i &pos ){
	// this is the main section of the demo:
	//  here we sample the second color target to find out
	//  which color is under the cursor.
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
			selectedJoint = Joint::jointMap.at(color);
			selectedJoint->building = true;
		}else{
			selectedJoint = nullptr;
		}
	}else{
		// we can't be sure about the color, we probably are on an object's edge
		selectedJoint = nullptr;
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

CINDER_APP_BASIC( CinderApp, RendererGl )