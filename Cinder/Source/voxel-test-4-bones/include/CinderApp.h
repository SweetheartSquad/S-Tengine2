#pragma once

#include <cinder\MayaCamUI.h>
#include <cinder\TriMesh.h>
#include <cinder\app\AppBasic.h>
#include <cinder\gl\Fbo.h>
#include <cinder\gl\GlslProg.h>
#include <cinder\gl\Light.h>
#include <cinder\gl\Material.h>
#include <cinder\params\Params.h>
#include <cinder\Plane.h>
#include <cinder\Ray.h>

#include <string>
#include <sstream>
#include <set>

#include "SkeletonData.h"
#include "Joint.h"
#include "CommandProcessor.h"
#include "Transform.h"
#include "SceneRoot.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ToolBar;
class ToolButton;
class ConsoleGUI;

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
	void loadMesh( const std::string & objFile, const std::string & meshFile, TriMesh * mesh);
	// Loads the shaders
	void loadShaders();
	// Initializes empty single channel fbos
	void initFbo(gl::Fbo & _fbo, Area _area);
	// Initializes empty 3-channel fbos
	void initMultiChannelFbo(gl::Fbo & _fbo, Area _area, unsigned long int _numChannels);

	// Draws a grid on the floor
	void drawGrid(float size = 100.f, float step = 10.f);

	void snapParams();

	void saveSkeleton();
	void loadSkeleton();

	void setKeyframe();
	void togglePlay();

	/** Copies an area of size _area centered around _pos from (GL_COLOR_ATTACHMENENT0 + _channel) of _sourceFbo into _destFbo.
	* If the most-occurring colour in _destFbo takes up more than 50% of the pixels,
	* returns the most-occurring colour. Otherwise, returns 0.
	* _res:			pointer to result (Color if _type is GL_FLOAT, unsigned long int otherwise)
	* _sourceFbo:	fbo to read pixels from
	* _sourceRect:	rectangle describing _sourceFbo
	* _destFbo:		fbo to use as intermediary (pixels are copied in from _sourceFbo and then read)
	* _pos:			pixel to use as reference point
	* _area:		area describing width/height of pixels to copy
	* _channel:		channel of _sourceFbo to copy pixels from
	* _type:		GL_FLOAT or GL_UNSIGNED_BYTE
	*/
	void pickColour(void * res, const gl::Fbo * _sourceFbo, const Rectf *  _sourceRect, gl::Fbo * _destFbo, Vec2i _pos, Area _area, unsigned long int _channel, GLenum _type);
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

public:
	//params
	bool drawParams;
	params::InterfaceGlRef params;
	params::InterfaceGlRef timelineParams;
	params::InterfaceGlRef voxelParams;

	ToolBar * toolbar;

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
	gl::GlslProg uiShader;
	
	// Framebuffers for cameras
	gl::Fbo fboTop;
	gl::Fbo fboRight;
	gl::Fbo fboFront;
	gl::Fbo	fboPersp; //! our main framebuffer (AA, containing 2 color buffers)

	gl::Fbo	fboUI; //! our main framebuffer (AA, containing 2 color buffers)
	gl::Fbo pickingFboUI;
	
	// Framebuffer for selecting joints
	gl::Fbo mPickingFboJoint;

	// Framebuffer for getpixelthing
	gl::Fbo pixelFbo;
	
	// Rect definitions for cameras sizes
	Rectf rectTop;
	Rectf rectRight;
	Rectf rectFront;
	Rectf rectPersp;
	Rectf rectWindow;
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
	
	// The root node of the scene; contains the "root" nodes for each skeleton
	SceneRoot * sceneRoot;

	ToolButton * activeButton;
	
	// Whether voxels can be selected (if false, their parent joint is selected instead)
	bool voxelSelectMode;
	// Whether to draw the voxels as voxels
	bool voxelPreviewMode;
	// Grid resolution for voxel preview
	float voxelPreviewResolution;
	// Sphere radius for voxel placing
	float voxelSphereRadius;

	//Vec3i dir;
	//Vec2i mouseAxis;
	Vec2i oldMousePos;
	unsigned long int uiColour;
	unsigned long int clickedUiColour;
	const Camera * sourceCam;
	const Rectf * sourceRect;
	const Rectf * sourceBounds;
	const gl::Fbo * sourceFbo;

	enum UImode{
		kCREATE,
		kSELECT,
		kTRANSLATE,
		kROTATE,
		kSCALE,
		kPAINT_VOXELS
	} mode;
	
	CoordinateSpace translateSpace;
	CoordinateSpace rotateSpace;
	CoordinateSpace scaleSpace;

	// Commands
	CommandProcessor * cmdProc;

	// Animation
	bool play;
	float previousTime;

	// UI restriction check variables
	bool isMouseDown;

	// VoxelPaint spacing
	Vec3f lastVoxelPaintPos;
	float currentSpacingDistance;
	float voxelPaintSpacing;

	ConsoleGUI * consoleGUI;
};