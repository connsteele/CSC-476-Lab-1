#include "GameObject.h"



GameObject::GameObject(const std::string& gameObjName, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg, glm::vec3 pos, float vel, glm::vec3 orient)
{
	this->nameObj = gameObjName;
	//-- Setup the Model Geometry
	objModel = std::make_shared<Shape>();
	objModel->loadMesh(resourceDirectory + fileName);
	objModel->resize();
	objModel->init();


	//Initialize globals
	this->curShaderProg = curShaderProg;
	this->position = pos;
	this->velocity = vel;
	this->orientation = orient;
}

void GameObject::DrawGameObj()
{
	objModel->draw(curShaderProg);
}

void GameObject::step(float dt, std::shared_ptr<MatrixStack> &M, std::shared_ptr<MatrixStack> &P)
{
	// Simple move
	/*static float z = 0.0f;
	M->translate(glm::vec3(0.0f, 0.0f, z += 0.0025f));*/

	static float elapsedTime = 0.0f;
	elapsedTime += dt;
	position = velocity * orientation * elapsedTime;
	M->translate(position);
	//M->rotate(orientation);
	
}


GameObject::~GameObject()
{
}
