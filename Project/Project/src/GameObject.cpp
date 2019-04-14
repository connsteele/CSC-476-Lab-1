#include "GameObject.h"



GameObject::GameObject(const std::string& gameObjName, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg)
{
	this->nameObj = gameObjName;
	//-- Setup the Model Geometry
	objModel = std::make_shared<Shape>();
	objModel->loadMesh(resourceDirectory + fileName);
	objModel->resize();
	objModel->init();

	this->curShaderProg = curShaderProg;
}

void GameObject::DrawGameObj()
{
	objModel->draw(curShaderProg);
}


GameObject::~GameObject()
{
}
