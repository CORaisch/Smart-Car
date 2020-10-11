#include "Rendering/Models/Model.h"

using namespace Rendering;
using namespace Models;

Model::Model() {}

Model::~Model() {
	Destroy();
}

void Model::Draw() {
	//this will be again overridden
}

void Model::Update(float dt) {
	//this will be again overridden
}

void Model::SetProgram(GLuint program) {
	this->program = program;
}

GLuint Model::GetProgram() {
	return this->program;
}

void Model::setEventHandler(EventHandler* eh) {
	this->input = eh;
}

void Model::setCamera(Camera* cam) {
	this->camera = cam;
}

GLuint Model::GetVao() const {
	return vao;
}

const std::vector<GLuint>& Model::GetVbos() const {
	return vbos;
}

std::vector<VertexFormat>& Model::getVertices() {
	return this->vertices;
}

glm::mat4& Model::getModelMatrix() {
	return this->model_matrix;
}

void Model::setModelMatrix(const glm::mat4& model) {
	this->model_matrix = model;
}

void Model::Destroy() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(vbos.size(), &vbos[0]);
	vbos.clear();
}
