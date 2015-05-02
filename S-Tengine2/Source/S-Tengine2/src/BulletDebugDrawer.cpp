#pragma once

#include <BulletDebugDrawer.h>

#include <glew\glew.h>

#include <MatrixStack.h>
#include <RenderOptions.h>
#include <shader\Shader.h>
#include <shader\BaseComponentShader.h>
#include <shader\ShaderComponentTexture.h>

BulletDebugDrawer::BulletDebugDrawer(btCollisionWorld * _world) :
	NodeChild(nullptr),
	NodeTransformable(new Transform()),
	m_debugMode(0),
	world(_world),
	shader(new BaseComponentShader(false))
{
	shader->addComponent(new ShaderComponentTexture(shader));
	shader->compileShader();
}

BulletDebugDrawer::~BulletDebugDrawer(){
}

void BulletDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor){
	glBegin(GL_LINES);
		glColor3f(fromColor.getX(), fromColor.getY(), fromColor.getZ());
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glColor3f(toColor.getX(), toColor.getY(), toColor.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void BulletDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color){
	drawLine(from,to,color,color);
}

void BulletDebugDrawer::drawSphere(const btVector3& p, btScalar radius, const btVector3& color){
	glColor4f (color.getX(), color.getY(), color.getZ(), btScalar(1.0f));
	glPushMatrix ();
	glTranslatef (p.getX(), p.getY(), p.getZ());

	int lats = 5;
	int longs = 5;

	int i, j;
	for(i = 0; i <= lats; i++) {
		btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
		btScalar z0  = radius*sin(lat0);
		btScalar zr0 =  radius*cos(lat0);

		btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
		btScalar z1 = radius*sin(lat1);
		btScalar zr1 = radius*cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= longs; j++) {
			btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
			btScalar x = cos(lng);
			btScalar y = sin(lng);

			glNormal3f(x * zr0, y * zr0, z0);
			glVertex3f(x * zr0, y * zr0, z0);
			glNormal3f(x * zr1, y * zr1, z1);
			glVertex3f(x * zr1, y * zr1, z1);
		}
		glEnd();
	}

	glPopMatrix();
}



void BulletDebugDrawer::drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha){
//	if (m_debugMode > 0)
	{
		const btVector3	n=btCross(b-a,c-a).normalized();
		glBegin(GL_TRIANGLES);		
		glColor4f(color.getX(), color.getY(), color.getZ(),alpha);
		glNormal3d(n.getX(),n.getY(),n.getZ());
		glVertex3d(a.getX(),a.getY(),a.getZ());
		glVertex3d(b.getX(),b.getY(),b.getZ());
		glVertex3d(c.getX(),c.getY(),c.getZ());
		glEnd();
	}
}

void BulletDebugDrawer::setDebugMode(int debugMode){
	m_debugMode = debugMode;
}

void BulletDebugDrawer::draw3dText(const btVector3& location,const char* textString){
	glRasterPos3f(location.x(),  location.y(),  location.z());
	//BMF_DrawString(BMF_GetFont(BMF_kHelvetica10),textString);
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString){
	printf("%s\n",warningString);
}

void BulletDebugDrawer::drawContactPoint(const btVector3& pointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color){
	btVector3 to=pointOnB+normalOnB*1;//distance;
	const btVector3&from = pointOnB;
	glColor4f(color.getX(), color.getY(), color.getZ(),1.f);
	//glColor4f(0,0,0,1.f);
	glBegin(GL_LINES);
	glVertex3d(from.getX(), from.getY(), from.getZ());
	glVertex3d(to.getX(), to.getY(), to.getZ());
	glEnd();
}


void BulletDebugDrawer::render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	GLfloat oldWidth;
	glGetFloatv(GL_LINE_WIDTH, &oldWidth);
	glLineWidth(2.5f);

	_matrixStack->pushMatrix();
	_matrixStack->applyMatrix(transform->getModelMatrix());

	matrixStack = _matrixStack;
	renderOptions = _renderOptions;
	shader->clean(_matrixStack, _renderOptions, this); // remove this later, just here because everything is in immediate mode
	world->debugDrawWorld();
	matrixStack = nullptr;
	renderOptions = nullptr;

	_matrixStack->popMatrix();
	glLineWidth(oldWidth);
}