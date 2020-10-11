#include "Rendering/Models/Car.h"

using namespace Rendering;
using namespace Models;

Car::Car() {}

Car::~Car() {
	//is going to be deleted in Models.cpp
}

void Car::Create() {

	GLuint vao;
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Get Size of Screen to Scale Model
	int screen_x, screen_y;
	this->camera->getSize(screen_x, screen_y);

	//Init Texture
	glGenTextures(1, &(this->texID));
	glBindTexture(GL_TEXTURE_2D, this->texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Load Image using SOIL
	unsigned char* image = SOIL_load_image("../data/car.png", &(this->tex_width), &(this->tex_height), 0, SOIL_LOAD_RGBA);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tex_width, this->tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//Load Vertex Data
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x / 64, -screen_y / 24, 0.0, 1.0), glm::vec2(1, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x / 64, -screen_y / 24, 0.0, 1.0), glm::vec2(1, 0)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x / 64, screen_y / 24, 0.0, 1.0), glm::vec2(0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(-screen_x / 64, screen_y / 24, 0.0, 1.0), glm::vec2(0, 1)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x / 64, -screen_y / 24, 0.0, 1.0), glm::vec2(1, 0)));
	this->vertices.push_back(VertexFormat(glm::vec4(screen_x / 64, screen_y / 24, 0.0, 1.0), glm::vec2(0, 0)));

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * 6, &vertices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);

	//here we assign the values
	this->vao = vao;
	this->vbos.push_back(vbo);
	this->model_matrix = glm::translate(glm::mat4(1.), glm::vec3(float(screen_x / 2), float(screen_y / 2), 0.0f));
	this->modelID = glGetUniformLocation(program, "M");
	this->viewID = glGetUniformLocation(program, "V");
	this->projectID = glGetUniformLocation(program, "P");
}

void Car::Update(float dt) {
	//Update Camera
	viewMatrix = this->camera->getView();
	projectMatrix = this->camera->getProjection();
}

void Car::Draw() {
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texcoord)));

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
