#include "cinder/app/AppBasic.h"
#include "Resources.h"

#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/TriMesh.h"
#include "cinder/ImageIo.h"
#include "cinder/MayaCamUI.h"

#include "cinder/gl/GlslProg.h"

using namespace ci;
using namespace ci::app;
//using namespace std;

class CinderApp : public AppBasic {
  public:
	  
	void mouseMove( MouseEvent event );
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseUp( MouseEvent event );

	void keyDown( KeyEvent event );
	void keyUp( KeyEvent event );
	
	void prepareSettings( Settings *settings );
	void setup();
	void resize();

	void update();
	void draw();
	void renderScene();

	bool drawParams;
	params::InterfaceGlRef myParams;

	int loop;
	Vec2i mousePos;
	MayaCamUI camera;
	Vec3f camAim, camUp;
	float camFOV, camNearClip, camFarClip;
	
	gl::Fbo depthFbo;
	
	gl::GlslProg depthShader;

	float resolution;
	Vec3f center;
};

void CinderApp::prepareSettings( Settings *settings ){
	loop = 0;
	settings->setWindowSize( 720, 480 );
	settings->setFrameRate( 100.0f );
}

void CinderApp::setup(){
	drawParams = true;

	camFOV = 75;
	camFarClip = 5000;
	camNearClip = 5;

	resolution = 5;
	center = Vec3f(0,0,0);
	// PARAMS
	myParams = params::InterfaceGl::create( "voxel-test", Vec2i( 200, 400 ) );
	myParams->addParam( "FOV", &camFOV, "min=0 max=360 step=1" );
	myParams->addParam( "Near Clip", &camNearClip, "min=0 max=200 step=1" );
	myParams->addParam( "Far Clip", &camFarClip, "min=0 max=10000 step=10" );
	myParams->addSeparator();
	myParams->addParam( "Frame", &loop,"",true);
	myParams->addSeparator();
	myParams->addParam( "Resolution", &resolution,"min=1");
	//LOAD SHADERS
	/*try {
        depthShader = ci::gl::GlslProg( loadAsset("../resources/vert.glsl"), loadAsset("../resources/frag.glsl") );
	}catch( gl::GlslProgCompileExc &exc ) {
		console() << "Cannot compile depthShader: " << exc.what() << std::endl;
		exit(1);
	}catch( Exception &exc ){
		console() << "Cannot load depthShader: " << exc.what() << std::endl;
		exit(1);
	}*/
}

void CinderApp::resize(){
	// setup the camera
	CameraPersp cam = camera.getCamera();
	cam.setPerspective( camFOV, getWindowAspectRatio(), camNearClip, camFarClip);
	camera.setCurrentCam( cam );

	// create or resize framebuffer if needed
	if(!depthFbo || depthFbo.getWidth() != getWindowWidth() || depthFbo.getHeight() != getWindowHeight()) {
		gl::Fbo::Format fmt;

		fmt.enableColorBuffer( true, 1 );

		// enable multi-sampling for better quality 
		//  (if this sample does not work on your computer, try lowering the number of samples to 2 or 0)
		fmt.setSamples(16);

		// create the buffer
		depthFbo = gl::Fbo( getWindowWidth(), getWindowHeight(), fmt );

		// work-around for an old Cinder issue
		depthFbo.getTexture(0).setFlipped(true);
	}
}

void CinderApp::mouseMove( MouseEvent event ){
	mousePos = event.getPos();
}

void CinderApp::mouseDown( MouseEvent event ){
	// handle the camera
	camera.mouseDown( event.getPos() );
}

void CinderApp::mouseDrag( MouseEvent event ){
	mousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		camera.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}
}

void CinderApp::mouseUp( MouseEvent event ){
}

void CinderApp::keyDown( KeyEvent event ){
	switch( event.getCode() ) {
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	case KeyEvent::KEY_f:
		setFullScreen(!isFullScreen());
		break;
	case KeyEvent::KEY_F1:
		drawParams = !drawParams;
		break;
	case KeyEvent::KEY_s:
		center.z -= 1.5;
		break;
	case KeyEvent::KEY_a:
		center.x -= 1.5;
		break;
	case KeyEvent::KEY_w:
		center.z += 1.5;
		break;
	case KeyEvent::KEY_d:
		center.x += 1.5;
		break;
	}
}

void CinderApp::keyUp( KeyEvent event ){
}

//The cylinder is defined by an axis from 'pt1' to 'pt2',
// the axis having a length squared of 'lengthsq' (pre-compute for each cylinder
// to avoid repeated work!), and radius squared of 'radius_sq'.
bool pointInObj(Vec3f testpt, Vec3f center, float size, float rot){
	//testpt.rotate(Vec3f(0,1,0), rot);
	testpt -=center;
	testpt.rotate(Vec3f(1,0,0), rot);
	testpt += center;

	if(testpt.x < center.x - size){
		return false;
	}if(testpt.y < center.y - size){
		return false;
	}if(testpt.z < center.z - size){
		return false;
	}if(testpt.x > center.x + size){
		return false;
	}if(testpt.y > center.y + size){
		return false;
	}if(testpt.z > center.z + size){
		return false;
	}
	return true;
}
void CinderApp::renderScene(){
	gl::enableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();

	gl::pushMatrices();
	gl::setMatrices(camera.getCamera());

	gl::clear(Color(0, 0, 0));

	Vec3f bb1(-100, -100, -100);
	Vec3f bb2(100, 100, 100);
	bb1 += center;
	bb2 += center;

	float size = 25.f;
	float rot = sinf(getElapsedSeconds())*2.f*3.14159f;
	Vec3f aabb;
	float r = resolution;
	for(aabb.x = bb1.x; aabb.x <= bb2.x; aabb.x += r){
		for(aabb.y = bb1.y; aabb.y <= bb2.y; aabb.y += r){
			for(aabb.z = bb1.z; aabb.z <= bb2.z; aabb.z += r){
				if(pointInObj(aabb, center, size, rot)){
					gl::enableDepthRead();
					gl::drawColorCube(aabb, Vec3f(r*0.9f, r*0.9f, r*0.9f));
				}
			}
		}
	}

	gl::popMatrices();
}


void CinderApp::update(){
	loop += 1;

	CameraPersp cam = camera.getCamera();
	cam.setPerspective( camFOV, getWindowAspectRatio(), camNearClip, camFarClip);
	camera.setCurrentCam( cam );
}

void CinderApp::draw(){
	depthFbo.bindFramebuffer();
	//depthShader.bind();
	renderScene();
	//depthShader.unbind();
	depthFbo.unbindFramebuffer();
	
	gl::clear();
	gl::color(Color(1.0, 1.0, 1.0));
	gl::draw(depthFbo.getTexture(), getWindowBounds());
	
	//draw parameters window
	if(drawParams){
	myParams->draw();
	}

	//save window to img file
	//writeImage("../renderData/img0_"+intToStr(loop)+".png", this->copyWindowSurface());
}

CINDER_APP_BASIC( CinderApp, app::RendererGl )