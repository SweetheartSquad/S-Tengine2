#include "Game.h"
#include "Cube.h"

double lastTime = glfwGetTime();
int nbFrames = 0;

Cube *cube;

GLfloat verts[] ={
    0.5f, 0.5f, 0.f,
	0.75f,  0.75f, 0.f,
	0.0f,  0.75f,  0.f
};
//VertexBuffer* vertexBuffer;

Game::Game(bool isRunning)
{
	GLUtils::checkForError(true,__FILE__,__LINE__);
	this->isRunning = isRunning;
	this->glfwWindow = glfwGetCurrentContext();
	GLUtils::checkForError(true,__FILE__,__LINE__);
	this->renderSystem = &RenderSystem::getInstance();
	GLUtils::checkForError(true,__FILE__,__LINE__);
	this->children = new std::vector<Entity*>();
	cube = new Cube(gmtl::Vec3f(0.f, 0.f, 0.5f),0.2f);
	Cube* cube2 = new Cube(gmtl::Vec3f(-0.2f, 0.5f, 0.5f), 0.1f);
	children->push_back(cube);
	children->push_back(cube2);
	//vertexBuffer = new VertexBuffer(verts, sizeof(verts), GL_STATIC_DRAW, 3, sizeof(GLfloat)*3);
}

Game::~Game(void)
{
	RenderSystem::destroyRenderSystem();
}

void Game::update(void)
{
	printFps();
}

void Game::draw(void)
{
	renderSystem->render(children);
}

void Game::printFps(){
	// Measure speed
	double currentTime = glfwGetTime();
	nbFrames++;
	if ( currentTime - lastTime >= 1.0 ){ 
		// If last prinf() was more than 1 sec ago
		// printf and reset timer
		printf("%f FPS\n", double(nbFrames));
		nbFrames = 0;
		lastTime += 1.0;
	}
}

