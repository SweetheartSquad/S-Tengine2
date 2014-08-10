#include "cinder/app/AppBasic.h"
#include "Resources.h"

#include "cinder/Vector.h"
#include "cinder/Utilities.h"
#include "cinder/params/Params.h"
#include "cinder/Camera.h"
#include "cinder/gl/Fbo.h"
#include "cinder/TriMesh.h"
#include "cinder/ImageIo.h"

#include "cinder/gl/GlslProg.h"

#include "globals.h"

using namespace ci;
//using namespace ci::app;
//using namespace std;

class CinderApp : public app::AppBasic {
  public:
	void prepareSettings( Settings *settings );
	void setup();
	/*void mouseUp( app::MouseEvent event );
	void mouseDown( app::MouseEvent event );
	void mouseMove( app::MouseEvent event );*/
	void mouseDrag( app::MouseEvent event );
	void keyDown( app::KeyEvent event );
	void update();
	void renderScene();
	void draw();

	Quatf camRot;
	float camZoom;
	params::InterfaceGlRef myParams;

	float displayScale;
	int loop;
	CameraPersp camera;
	Vec3f camAim, camUp;
	float camFOV, camNearClip, camFarClip;
	
	gl::Fbo depthFbo;
	
	gl::GlslProg depthShader;

	bool rotating;

	float resolution;
	Vec3f center;

	std::vector<Vec3f> voxels;

	bool drawing;
};

void CinderApp::prepareSettings( Settings *settings ){
	displayScale = 10;
	loop = 0;
	settings->setWindowSize( SIM_SIZE*displayScale, SIM_SIZE*displayScale );
	settings->setFrameRate( 60.0f );
}

void CinderApp::setup(){
	voxels.push_back(Vec3f(SIM_SIZE*displayScale/2-resolution/2,SIM_SIZE*displayScale/2-resolution/2,SIM_SIZE*displayScale/2-resolution/2));

	rotating = false;
	drawing = false;
	camRot = Quatf(0.01,0.0,0.0,0.99);
	camZoom = 700.0f;

	camAim = Vec3f(0, 0, 0);
	camUp = Vec3f::yAxis();
	
	camFOV = 75;
	camFarClip = 5000;
	camNearClip = 5;

	resolution = 5;
	center = Vec3f(SIM_SIZE*displayScale/2,SIM_SIZE*displayScale/2,SIM_SIZE*displayScale/2);
	// PARAMS
	myParams = params::InterfaceGl::create( "Behavioural Animation", Vec2i( 200, 400 ) );
	myParams->addParam( "Scene Rotation", &camRot, "opened=1" );
	myParams->addSeparator();
	myParams->addParam( "Zoom", &camZoom, "min=5.0 max=1500.0 step=10.0 keyIncr=s keyDecr=w" );
	myParams->addParam( "FOV", &camFOV, "min=0 max=360 step=1" );
	myParams->addParam( "Near Clip", &camNearClip, "min=0 max=200 step=1" );
	myParams->addParam( "Far Clip", &camFarClip, "min=0 max=10000 step=10" );
	myParams->addSeparator();
	myParams->addParam( "Frame", &loop,"",true);
	myParams->addSeparator();
	myParams->addParam( "Resolution", &resolution,"min=1");

	
	depthFbo = gl::Fbo(SIM_SIZE*displayScale, SIM_SIZE*displayScale);

	//LOAD SHADERS
	try {
        depthShader = ci::gl::GlslProg( loadFile("../resources/vert.glsl"), loadFile("../resources/frag.glsl") );
	}catch( gl::GlslProgCompileExc &exc ) {
		console() << "Cannot compile depthShader: " << exc.what() << std::endl;
		exit(1);
	}catch( Exception &exc ){
		console() << "Cannot load depthShader: " << exc.what() << std::endl;
		exit(1);
	}
}
/*void CinderApp::mouseDown(app::MouseEvent event){
	console() << "down" <<std::endl;
	drawing = true;
}
void CinderApp::mouseUp(app::MouseEvent event){
	console() << "up" <<std::endl;
	drawing = false;
}*/
void CinderApp::mouseDrag( app::MouseEvent event ){
	console() << "move" <<std::endl;
	//if(drawing){
		//rotating = !rotating;
		Vec3f pos = Vec3f(event.getPos().x,event.getPos().y,0);

		ci::Ray ray = camera.generateRay(pos.x, pos.y, camera.getAspectRatio());

		for(int i = 0; i < 100; i++){
			pos = ray.calcPosition(i);

			pos.x -= fmod(pos.x, resolution);
			pos.y -= fmod(pos.y, resolution);
			pos.z -= fmod(pos.z, resolution);

			voxels.push_back(pos);
		}
	//}
}
void CinderApp::keyDown( app::KeyEvent event ){
	console() << event.getChar() << std::endl;

}

//The cylinder is defined by an axis from 'pt1' to 'pt2',
// the axis having a length squared of 'lengthsq' (pre-compute for each cylinder
// to avoid repeated work!), and radius squared of 'radius_sq'.
bool pointInObj(Vec3f testpt, Vec3f center, float size, float rot){
	//testpt.rotate(Vec3f(0,1,0), rot);
	testpt-=center;
	Quatd q(rot,1-rot,1-rot,rot);
	q.normalize();
	testpt.rotate(q.getAxis(), q.getAngle());
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
	gl::pushMatrices();
	gl::translate(SIM_SIZE*displayScale/2,SIM_SIZE*displayScale/2,SIM_SIZE*displayScale/2);

	gl::color(1,1,1,1);
	for(int x = -10*resolution; x < 10*resolution; x+=resolution){
		for(int y = -10*resolution; y < 10*resolution; y+=resolution){
			gl::drawStrokedRect(Rectf(x,y,x+resolution,y+resolution));
		}
	}
	gl::popMatrices();

	for(int i = 0; i < voxels.size(); ++i){
		gl::drawColorCube(voxels.at(i), Vec3f(resolution,resolution,resolution));
	}
	
	/*
	Vec3f bb1(-SIM_SIZE*displayScale,-SIM_SIZE*displayScale,-SIM_SIZE*displayScale);
	bb1 += center;
	Vec3f bb2(SIM_SIZE*displayScale,SIM_SIZE*displayScale,SIM_SIZE*displayScale);
	bb2 += center;

	float size = 25*displayScale;
	float rot = (float)(loop%100)/100;
	Vec3f aabb;
	float r = displayScale*resolution;
	for(aabb.x = bb1.x; aabb.x <= bb2.x; aabb.x += r){
		for(aabb.y = bb1.y; aabb.y <= bb2.y; aabb.y += r){
			for(aabb.z = bb1.z; aabb.z <= bb2.z; aabb.z += r){
				if(pointInObj(aabb, center, size, rot)){
					gl::enableDepthRead();
					gl::drawColorCube(aabb, Vec3f(r-camZoom/250, r-camZoom/250, r-camZoom/250));
				}
			}
		}
	}*/
}


void CinderApp::update(){
	loop += 1;

	camera.setPerspective(camFOV, getWindowAspectRatio(), camNearClip, camFarClip);

	if(rotating){
		camRot.set(Vec3f(0,1,0), fmod(app::getElapsedSeconds()/10, 2*PI) );
	}

	Vec3f look = Vec3f(SIM_SIZE/2, SIM_SIZE/2, SIM_SIZE/2);
	look *= displayScale;

	Vec3f dir(camZoom, camZoom, camZoom);
	dir.rotate(camRot.getAxis(), camRot.getAngle());
	Vec3f pos = look + dir;
	camera.lookAt(pos, look, camUp);

	gl::setMatrices(camera);

	//gl::enableAlphaBlending();

	depthFbo.bindFramebuffer();
	//depthShader.bind();
	gl::clear(Color(0, 0, 0));
	gl::color(Color(1, 1, 1));
	
	renderScene();
	//depthShader.unbind();
	depthFbo.unbindFramebuffer();

}

void CinderApp::draw(){
	gl::enableAlphaBlending();
	gl::enableDepthWrite();
	gl::enableDepthRead();

	CameraOrtho cam2;
	cam2 = CameraOrtho(0, SIM_SIZE*displayScale, 0, SIM_SIZE*displayScale, -5, 5);
	gl::setMatrices(cam2);
	
	gl::clear(Color(0, 0, 0));
	gl::color(Color(1.0, 1.0, 1.0));
	gl::draw(depthFbo.getTexture());
	
	//draw parameters window
	myParams->draw();
	//renderScene();

	

	//save window to img file
	
	//writeImage("../renderData/img0_"+intToStr(loop)+".png", this->copyWindowSurface());
}

CINDER_APP_BASIC( CinderApp, app::RendererGl )