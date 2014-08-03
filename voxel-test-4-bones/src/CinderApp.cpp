#include "cinder/ObjLoader.h"
#include "cinder/MayaCamUI.h"
#include "cinder/TriMesh.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"

#include <sstream>

#include "Bone.h"


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
	//! loads an OBJ file, writes it to a much faster binary file and loads the mesh
	void loadMesh( const std::string &objFile, const std::string &meshFile, TriMesh *mesh);
	//! loads the shaders
	void loadShaders();
	//! initializes empty fbos
	void initFbo(gl::Fbo & _fbo, Area _area);
	//! draws a grid on the floor
	void drawGrid(float size=100.0f, float step=10.0f);

	void newBone(Bone * parent = NULL);

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

	//! keeping track of our cursor position
	Vec2i			mMousePos;

	//! fancy font fizzle
	Font			mFont;

	//! background color
	Color			mColorBackground;

	//! colour channel to draw to the main viewport
	unsigned long int channel;
	
	gl::Material boneMaterial;
	
	std::vector<Bone *> bones;
	Bone * selectedBone;
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
	boneMaterial.setAmbient(Color::black());
	boneMaterial.setDiffuse(Color::white());
	boneMaterial.setSpecular(Color::white());
	boneMaterial.setShininess( 20.0f );

	// load shaders
	loadShaders();

	// load font
	mFont = Font(loadAsset("font/b2sq.ttf"), 32);	

	// set background color
	mColorBackground = Color(0.1f, 0.1f, 0.1f);
	
	channel = 0;

	selectedBone = NULL;
}

void CinderApp::shutdown()
{
	for(int i = 0; i < bones.size(); ++i){
		delete bones.at(i);
	}
	bones.clear();
	selectedBone = NULL;
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
	/*windowBounds *=  0.2f;
	gl::draw( mFbo.getTexture(0), windowBounds);
	windowBounds.y1 += windowBounds.y2;
	windowBounds.y2 += windowBounds.y2;
	gl::draw( mFbo.getTexture(1), windowBounds);
	windowBounds.y2 += windowBounds.y1;
	windowBounds.y1 += windowBounds.y1;
	gl::draw( mFbo.getTexture(2), windowBounds);*/

	// draw the picking framebuffer in the upper right corner
	/*Rectf rct = (Rectf) mPickingFboVoxel.getBounds() * 5.0f;
	rct.offset( Vec2f((float) getWindowWidth() - rct.getWidth(), 0) );
	if(mPickingFboVoxel){
		gl::draw( mPickingFboVoxel.getTexture(), Rectf(rct.x1, rct.y1, rct.x2, rct.y2) );
	}if(mPickingFboVoxel){
		gl::draw( mPickingFboFace.getTexture(), Rectf(rct.x1, rct.y1+rct.y2, rct.x2, rct.y2+rct.y2) );
	}*/
}

void CinderApp::mouseMove( MouseEvent event )
{
	mMousePos = event.getPos();
	if(selectedBone != NULL){
		if(selectedBone->building){
			selectedBone->end = Vec3d(mMousePos.x,mMousePos.y,0);
		}
	}
}

void CinderApp::mouseDown( MouseEvent event )
{
	// handle the camera
	mCamera.mouseDown( event.getPos() );

	if(!event.isAltDown() && event.isLeft()){
		if(selectedBone != NULL){
			if(selectedBone->building){
				newBone(selectedBone);
			}else{
				newBone();
			}
		}else{
			newBone();
		}
	}
}

void CinderApp::mouseDrag( MouseEvent event )
{
	mMousePos = event.getPos();

	// move the camera
	if(event.isAltDown()){
		mCamera.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
	}else{
		
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
	case KeyEvent::KEY_RETURN:
		if(selectedBone != NULL){
			if(selectedBone->building){
				selectedBone->building = false;
			}
		}
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
	cam.setPerspective( 60.0f, getWindowAspectRatio(), 0.1f, 10000.0f );
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

		// work-around for an old Cinder issue
		mFbo.getTexture(0).setFlipped(true);
		mFbo.getTexture(1).setFlipped(true);
		mFbo.getTexture(2).setFlipped(true);
	}
}

//

void CinderApp::render(){
	// clear background
	gl::clear( mColorBackground );

	// specify the camera matrices
	gl::pushMatrices();
	gl::setMatrices( mCamera.getCamera() );

		// setup the light
		gl::Light light( gl::Light::POINT, 0 );
		light.setAmbient( Color::white()/*black()*/ );
		light.setDiffuse( Color::white() );
		light.setSpecular( Color::white() );
		light.setPosition( mCamera.getCamera().getEyePoint() );

		// specify render states
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
		gl::enableDepthRead();
		gl::enableDepthWrite();

		// bind phong shader, which renders to both our color targets.
		//  See 'shaders/phong.frag'
		//mPhongShader.bind();

		// draw meshes:
		gl::color( Color::white() );

		// -each mesh should have a unique picking color that we can use to 
		//  find out which object is under the cursor. 
		boneMaterial.apply();
		for(Bone * b : bones){
			b->draw();
		}

		// unbind shader
		//mPhongShader.unbind();
	


	// restore matrices
	gl::popMatrices();

	// restore render states
	glPopAttrib();
}

void CinderApp::loadMesh(const std::string &objFile, const std::string &meshFile, TriMesh *mesh){
	try{
		// try to load a binary mesh file - sadly the following line does not work:
		// mMesh.read( loadAsset(meshfile) );

		// note: instead of throwing an exception, Cinder crashes if the
		//   file does not exist. Hopefully this will be changed in future versions.
		DataSourceRef file = loadAsset(meshFile);
		if(!file->createStream()) throw std::exception();

		mesh->read( file );
	}catch( ... ){
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
	try{
		mPhongShader = gl::GlslProg( loadFile("../assets/shaders/phong.vert"), loadFile("../assets/shaders/phong.frag") );
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

	_fbo = gl::Fbo(_area.getWidth(), _area.getHeight(), fmt);

	// work-around for an old Cinder issue
	_fbo.getTexture(0).setFlipped(true);
}

void CinderApp::newBone(Bone * parent){
	Bone * b = new Bone();
	b->start = Vec3d(mMousePos.x,mMousePos.y,0);
	b->end = Vec3d(mMousePos.x,mMousePos.y,0);
	if(parent != NULL){
		parent->building = false;
		b->parent = selectedBone;
		selectedBone->children.push_back(b);
	}
	selectedBone = b;
	bones.push_back(b);
}

CINDER_APP_BASIC( CinderApp, RendererGl )