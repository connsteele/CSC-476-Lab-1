#pragma once

#include <iostream>
#include "BaseCode/Shape.h"
#include "BaseCode/Program.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "BaseCode/MatrixStack.h"


class GameObject
{
public:
	std::string nameObj;
	std::shared_ptr<Shape> objModel;
	std::shared_ptr<Program> curShaderProg;

	glm::vec3 position, orientation;
	float velocity;

	GameObject(const std::string& name, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg, glm::vec3 pos, float vel, glm::vec3 orient);
	void DrawGameObj();
	void step(float dt, std::shared_ptr<MatrixStack> &M, std::shared_ptr<MatrixStack> &P);
	~GameObject(); // Destroyer
};

