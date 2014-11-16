#pragma once
#include "cinder/MayaCamUI.h"
#include "cinder/TriMesh.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "cinder/params/Params.h"

#include <string>
#include <sstream>
#include <set>

#include "SkeletonData.h"
#include "Joint.h"
#include "CommandProcessor.h"
#include "Transform.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderApp : public AppBasic {
public:
	void prepareSettings(Settings * settings);

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

	// Renders the scene
	void renderScene(gl::Fbo & fbo, const Camera & cam);
	void renderUI(const Camera & cam, const Rectf & rect);

	// Loads an OBJ file, writes it to a much faster binary file and loads the mesh
	void loadMesh( const std::string &objFile, const std::string &meshFile, TriMesh *mesh);
	// Loads the shaders
	void loadShaders();
	// Initializes empty single channel fbos
	void initFbo(gl::Fbo & _fbo, Area _area);
	// Initializes empty 3-channel fbos
	void initMultiChannelFbo(gl::Fbo & _fbo, Area _area);

	// Draws a grid on the floor
	void drawGrid(float size=100.0f, float step=10.0f);

	void newJoint(Vec3d pos, Joint * parent = NULL);
	Vec3d getCameraCorrectedPos();

	Joint * pickJoint(const Vec2i &pos);

	void saveSkeleton();
	void loadSkeleton();

	void setKeyframe();

	void handleUI(const Vec2i &pos);
	void getPixelThing();
public:
	// Utility functions to translate colors to and from ints or chars 
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

	void togglePlay();
protected:
	//params
	bool drawParams;
	params::InterfaceGlRef params;
	params::InterfaceGlRef timelineParams;

	string directory;
	string fileName;
	string filePath;
	string message;

	//! our cameras
	MayaCamUI camMayaPersp;
	CameraOrtho camTop;
	CameraOrtho camRight;
	CameraOrtho camFront;

	gl::GlslProg jointShader;

	//! our little picking framebuffer (non-AA) 
	//gl::Fbo			mPickingFboVoxel;
	//gl::Fbo			mPickingFboFace;
	
	// Framebuffers for cameras
	gl::Fbo fboTop;
	gl::Fbo fboRight;
	gl::Fbo fboFront;
	gl::Fbo	fboPersp; //! our main framebuffer (AA, containing 2 color buffers)

	gl::Fbo	fboUI; //! our main framebuffer (AA, containing 2 color buffers)
	gl::Fbo pickingFboUI;
	
	// Framebuffer for selecting joints
	gl::Fbo mPickingFboJoint;

	// Framebuffer for getpixelting
	gl::Fbo pixelFbo;
	
	// Rect definitions for cameras sizes
	Rectf rectTop;
	Rectf rectRight;
	Rectf rectFront;
	Rectf rectPersp;
	// Rect definitions for cameras sizes
	Rectf boundsTop;
	Rectf boundsRight;
	Rectf boundsFront;
	//Rectf boundsPersp;

	//! keeping track of our cursor position
	Vec2i mMousePos;

	//! background color
	Color mColorBackground;

	//! colour channel to draw to the main viewport
	unsigned long int channel;
	
	gl::Material JointMaterial;
	
	std::vector<Joint *> joints;


	

	//Vec3i dir;
	//Vec2i mouseAxis;
	Vec2i oldMousePos;
	unsigned long int uiColour;
	const Camera * sourceCam;
	const Rectf * sourceRect;
	const gl::Fbo * sourceFbo;

	enum UImode{
		CREATE,
		SELECT,
		TRANSLATE,
		ROTATE,
		SCALE,
		PAINT_VOXELS
	} mode;

	// Commands
	CommandProcessor cmdProc;

	// Animation
	bool play;
};