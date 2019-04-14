#pragma once

#include <iostream>
#include "BaseCode/Shape.h"
#include "BaseCode/Program.h"


class GameObject
{
public:
	std::string nameObj;
	std::shared_ptr<Shape> objModel;
	std::shared_ptr<Program> curShaderProg;

	GameObject(const std::string& name, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg);
	void DrawGameObj();
	~GameObject(); // Destroyer
};

