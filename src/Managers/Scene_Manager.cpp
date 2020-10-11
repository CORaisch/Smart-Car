#include "Managers/Scene_Manager.h"

using namespace Managers;

Scene_Manager::Scene_Manager(int size_x, int size_y) {

	this->fullscreen = false;
	this->input = new EventHandler();
	this->camera = new Camera(size_x, size_y);

	glEnable(GL_DEPTH_TEST);

	//Init Shader Manager
	shader_manager = new Shader_Manager();
	shader_manager->CreateProgram("generalShader", "../src/Shaders/General_Vertex_Shader.glsl", "../src/Shaders/General_Fragment_Shader.glsl");
	shader_manager->CreateProgram("textureShader", "../src/Shaders/Texture_Vertex_Shader.glsl", "../src/Shaders/Texture_Fragment_Shader.glsl");

	//Init Models Manager
	models_manager = new Models_Manager(this->input, this->camera);

	//Init Game Logic Routine
	this->logic_routine = new Game_Logic();
	this->logic_routine->setModels(this->models_manager->getModelList());
	this->logic_routine->Init(size_x, size_y);
	this->logic_routine->setEventHandler(*this->input);

	//Init Timer
	this->lastTime = glutGet(GLUT_ELAPSED_TIME);
}

Scene_Manager::~Scene_Manager() {
	delete shader_manager;
	delete models_manager;
}

void Scene_Manager::notifyBeginFrame() {
	this->currentTime = glutGet(GLUT_ELAPSED_TIME);
	this->deltaTime = this->currentTime - this->lastTime;
	this->models_manager->Update(this->deltaTime);
	this->logic_routine->compute(this->deltaTime);
}

void Scene_Manager::notifyDisplayFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	models_manager->Draw();
}

void Scene_Manager::notifyEndFrame() {
	this->lastTime = this->currentTime;
}

void Scene_Manager::keyDown(unsigned char key, int x, int y) {
	this->input->keyDown(key, x, y);
}

void Scene_Manager::keyUp(unsigned char key, int x, int y) {
	this->input->keyUp(key, x, y);
}

void Scene_Manager::notifyReshape(int width, int height, int previous_width, int previous_height) {
	//nothing here for the moment
}
