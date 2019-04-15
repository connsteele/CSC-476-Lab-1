#include "GameObject.h"




GameObject::GameObject(const std::string& gameObjName, const std::string& fileName, const std::string& resourceDirectory, std::shared_ptr<Program> curShaderProg, glm::vec3 pos, float vel, glm::vec3 orient)
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


}

void GameObject::DrawGameObj()
{
	objModel->draw(curShaderProg);
}

void GameObject::step(float dt, std::shared_ptr<MatrixStack> &M, std::shared_ptr<MatrixStack> &P, glm::vec3 camLoc, glm::vec3 center, glm::vec3 up)
{
	static float elapsedTime = 0.0f;
	elapsedTime += dt;
	position = velocity * orientation * elapsedTime;
	M->translate(position);
	//updateBbox(M, P, camLoc, center, up);
	
}

void GameObject::updateBbox(std::shared_ptr<MatrixStack> &M, std::shared_ptr<MatrixStack> &P, glm::vec3 camLoc, glm::vec3 center, glm::vec3 up)
{
	/*glm::vec3 size = glm::vec3(max_x - min_x, max_y - min_y, max_z - min_z);
	glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	M->translate(transform);
	*/




	//curShaderProg->bind();
	//glUniformMatrix4fv(curShaderProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
	//glUniformMatrix4fv(curShaderProg->getUniform("M"), 1, GL_FALSE, glm::value_ptr(M->topMatrix()));
	//glUniformMatrix4fv(curShaderProg->getUniform("V"), 1, GL_FALSE, value_ptr(lookAt(camLoc, center, up)));
	//glUniform3f(curShaderProg->getUniform("lightSource"), 0, 88, 10);
	////SetMaterial(1);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	//glEnableVertexAttribArray(0); // Enable location 0 to send to shader
	//glVertexAttribPointer(
	//	0,  // attribute
	//	4,                  // number of elements per vertex, here (x,y,z,w)
	//	GL_FLOAT,           // the type of each element
	//	GL_FALSE,           // take our values as-is
	//	0,                  // no extra data between each position
	//	(void*)0                   // offset of first element
	//);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	//glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4 * sizeof(GLushort)));
	//glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8 * sizeof(GLushort)));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDeleteBuffers(1, &vbo_vertices);
	//glDeleteBuffers(1, &ibo_elements);
	//curShaderProg->unbind();

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
	for (int i = 0; i < modelVertPosBuf.size(); i += 3)
	{
		if (modelVertPosBuf[i] < min_x) min_x = modelVertPosBuf[i];
		if (modelVertPosBuf[i] > max_x) max_x = modelVertPosBuf[i];
		if (modelVertPosBuf[i] < min_y) min_y = modelVertPosBuf[i + 1];
		if (modelVertPosBuf[i] > max_y) max_y = modelVertPosBuf[i + 1];
		if (modelVertPosBuf[i] < min_z) min_z = modelVertPosBuf[i + 2];
		if (modelVertPosBuf[i] > max_z) max_z = modelVertPosBuf[i + 2];
	}


}


GameObject::~GameObject()
{
}
