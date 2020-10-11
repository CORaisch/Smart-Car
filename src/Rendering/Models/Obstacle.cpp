#include "Rendering/Models/Obstacle.h"

using namespace Rendering;
using namespace Models;

Obstacle::Obstacle() {}

Obstacle::~Obstacle() {
	//is going to be deleted in Models.cpp
}

void Obstacle::Create() {

	GLuint vao;
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Get Size of Screen to Scale Model
	int screen_x, screen_y;
	this->camera->getSize(screen_x, screen_y);

	//Load Vertex Data
	int q = 24;
	int h = 9;
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(0.0, -screen_y/h, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(0.0, screen_y/h, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x/h, 0.0, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, -screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/q, screen_y/q, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x/h, 0.0, 0.0, 1.0), glm::vec4(1, 0, 0, 1)));

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * this->vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);

	//here we assign the values
	this->vao = vao;
	this->vbos.push_back(vbo);
	this->modelID = glGetUniformLocation(program, "M");
	this->viewID = glGetUniformLocation(program, "V");
	this->projectID = glGetUniformLocation(program, "P");
}

void Obstacle::Update(float dt) {
	//Update Camera
	viewMatrix = this->camera->getView();
	projectMatrix = this->camera->getProjection();
}

void Obstacle::Draw() {
	glUseProgram(program);

	glUniformMatrix4fv(this->modelID, 1, GL_FALSE, &this->model_matrix[0][0]);
	glUniformMatrix4fv(this->viewID, 1, GL_FALSE, &this->viewMatrix[0][0]);
	glUniformMatrix4fv(this->projectID, 1, GL_FALSE, &this->projectMatrix[0][0]);

	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbos[0]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::position)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::color)));

	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
