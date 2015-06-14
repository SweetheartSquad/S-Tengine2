#pragma once

#include <btBulletCollisionCommon.h>
#include <LinearMath/btIDebugDraw.h>
#include <Entity.h>

class ComponentShaderBase;

class BulletDebugDrawer : public btIDebugDraw, public Entity{
private:
	int m_debugMode;
	btCollisionWorld * world;
public:
	BulletDebugDrawer(btCollisionWorld * _world);
	virtual ~BulletDebugDrawer(); 
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	virtual void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);
	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	virtual void reportErrorWarning(const char* warningString);
	virtual void draw3dText(const btVector3& location,const char* textString);
	virtual void setDebugMode(int debugMode);
	virtual int getDebugMode() const { return m_debugMode;}
	
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	virtual void load() override;
	virtual void unload() override;

	vox::MatrixStack * matrixStack;
	RenderOptions * renderOptions;
	ComponentShaderBase * shader;
};