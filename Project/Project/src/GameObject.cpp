#include "GameObject.h"

// Connor Steele and Chris Gix Game Object Implementation


GameObject::GameObject(const std::string& gameObjName, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg, glm::vec3 pos, float vel, glm::vec3 orient, bool isTerrain)
{
	this->nameObj = gameObjName;
	//-- Setup the Model Geometry
	objModel = std::make_shared<Shape>();
	objModel->loadMesh(resourceDirectory + fileName);
	objModel->resize();
	objModel->init();
	//-- Setup geometry of the bounding box
	initBbox();


	//Initialize globals
	this->curShaderProg = curShaderProg;
	this->position = pos;
	this->velocity = vel;
	this->orientation = orient;
	this->isTerrain = isTerrain;

	elapsedTime = 0.0f;
}

void GameObject::DrawGameObj()
{
	objModel->draw(curShaderProg);
	updateBbox();
}

void GameObject::step(float dt, std::shared_ptr<MatrixStack> &M, std::shared_ptr<MatrixStack> &P, glm::vec3 camLoc, glm::vec3 center, glm::vec3 up)
{ 
	// elapsedTime += dt;

	DoCollisions(M);

	//-- Calculate new position and translate the model
	position += velocity * orientation * dt;
	//printf("Obj Current Pos: x: %f y: %f z: %f\n", position.x, position.y, position.z);
	M->translate(position);
	//updateBbox(M, P, camLoc, center, up);
	
	GLfloat min_x, max_x,
		min_y, max_y,
		min_z, max_z;
	// Get the position buffer from the model
	std::vector<float> modelVertPosBuf = objModel->getPosBuf();
	// Set up initial Values
	min_x = max_x = modelVertPosBuf[0];
	min_y = max_y = modelVertPosBuf[1];
	min_z = max_z = modelVertPosBuf[2];

	// Walk thru all models vertices and fit mins and maxes
	for (size_t i = 0; i < modelVertPosBuf.size() / 3; i++)
	{
		if (modelVertPosBuf[3 * i + 0] < min_x) min_x = modelVertPosBuf[3 * i + 0];
		if (modelVertPosBuf[3 * i + 0] > max_x) max_x = modelVertPosBuf[3 * i + 0];

		if (modelVertPosBuf[3 * i + 1] < min_y) min_y = modelVertPosBuf[3 * i + 1];
		if (modelVertPosBuf[3 * i + 1] > max_y) max_y = modelVertPosBuf[3 * i + 1];

		if (modelVertPosBuf[3 * i + 2] < min_z) min_z = modelVertPosBuf[3 * i + 2];
		if (modelVertPosBuf[3 * i + 2] > max_z) max_z = modelVertPosBuf[3 * i + 2];
	}
	// If the game object is terrain move the bounding box up
	switch (isTerrain)
	{
		case true:
			bboxSize = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z) * 40.0f; // Fix this magic number, is tied to scale that is set in main for this
			bboxCenter = position + glm::vec3(0.0, 39, 0.0); // used to be glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2)
			bboxTransform = glm::translate(glm::mat4(1), bboxCenter) * glm::scale(glm::mat4(1), bboxSize);
			break;
		case false:
			bboxSize = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
			bboxCenter = position; // used to be glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2)
			bboxTransform = glm::translate(glm::mat4(1), bboxCenter) * glm::scale(glm::mat4(1), bboxSize);
			break;
		/*default:
			break;*/
	}
	
	
}

void GameObject::DoCollisions(std::shared_ptr<MatrixStack> &M) //std::shared_ptr<GameObject> world
{

	if (position.z > 40.0f) 
	{
		orientation = glm::vec3(0, 0, -1);
	}
	else if (position.z < -40.0f)
	{
		orientation = glm::vec3(0, 0, 1);
	}

	if (position.x > 40.0f)
	{
		orientation = glm::vec3(0, 0, -1);
	}
	else if (position.x < -40.0f)
	{
		orientation = glm::vec3(0, 0, 1);
	}
	
	//TODO: Add for loop that loops through all other game objects so we can check for collisions with each

	/*bool collisionX = bboxCenter.x + bboxSize.x >= world->bboxCenter.x && world->bboxCenter.x + world->bboxSize.x >= bboxCenter.x;
	bool collisionY = bboxCenter.y + bboxSize.y >= world->bboxCenter.y && world->bboxCenter.y + world->bboxSize.y >= bboxCenter.y;
	bool collisionZ = bboxCenter.z + bboxSize.z >= world->bboxCenter.z && world->bboxCenter.z + world->bboxSize.z >= bboxCenter.z;

	if (!isTerrain && collisionX && collisionY && collisionZ) {
		orientation = orientation * -1.0f;
	}*/
}

// Update the center of the bounding box for the model
void GameObject::updateBbox()
{
	glUniformMatrix4fv(curShaderProg->getUniform("M"), 1, GL_FALSE, value_ptr(bboxTransform));
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,  // attribute
		4,                  // number of elements per vertex, here (x,y,z,w)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void GameObject::initBbox()
{
	// Cube 1x1x1, centered on origin
	GLfloat vertices[] = {
	  -0.5, -0.5, -0.5, 1.0,
	   0.5, -0.5, -0.5, 1.0,
	   0.5,  0.5, -0.5, 1.0,
	  -0.5,  0.5, -0.5, 1.0,
	  -0.5, -0.5,  0.5, 1.0,
	   0.5, -0.5,  0.5, 1.0,
	   0.5,  0.5,  0.5, 1.0,
	  -0.5,  0.5,  0.5, 1.0,
	};
	//GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind

	GLushort elements[] = {
	  0, 1, 2, 3,
	  4, 5, 6, 7,
	  0, 4, 1, 5, 2, 6, 3, 7
	};
	//GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind

	GLfloat min_x, max_x,
		min_y, max_y,
		min_z, max_z;

	// Get the position buffer from the model
	std::vector<float> modelVertPosBuf = objModel->getPosBuf();

	// Set up initial Values
	min_x = max_x = modelVertPosBuf[0];
	min_y = max_y = modelVertPosBuf[1];
	min_z = max_z = modelVertPosBuf[2];

	// Fit mins and maxes to the models vertices
	for (size_t i = 0; i < modelVertPosBuf.size() / 3; i++)
	{
		if (modelVertPosBuf[3 * i + 0] < min_x) min_x = modelVertPosBuf[3 * i + 0];
		if (modelVertPosBuf[3 * i + 0] > max_x) max_x = modelVertPosBuf[3 * i + 0];

		if (modelVertPosBuf[3 * i + 1] < min_y) min_y = modelVertPosBuf[3 * i + 1];
		if (modelVertPosBuf[3 * i + 1] > max_y) max_y = modelVertPosBuf[3 * i + 1];

		if (modelVertPosBuf[3 * i + 2] < min_z) min_z = modelVertPosBuf[3 * i + 2];
		if (modelVertPosBuf[3 * i + 2] > max_z) max_z = modelVertPosBuf[3 * i + 2];
	}

	bboxSize = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
	bboxCenter = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	bboxTransform = glm::translate(glm::mat4(1), bboxCenter) * glm::scale(glm::mat4(1), bboxSize);

}


GameObject::~GameObject()
{
}
