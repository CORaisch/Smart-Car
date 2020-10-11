#include "Managers/Models_Manager.h"

using namespace Managers;
using namespace Rendering;

Models_Manager::Models_Manager(EventHandler* eh, Camera* cam) {
	//Get Screen Size
	int sx, sy;
	cam->getSize(sx, sy);
	//Create Car-Model
	Models::Car* car = new Models::Car();
	car->SetProgram(Shader_Manager::GetShader("textureShader"));
	car->setEventHandler(eh);
	car->setCamera(cam);
	car->Create();
	gameModelList["car"] = car;
	//Create four Obstacle-Models
	char name[20];
	Models::Obstacle* obs[4];
	for (int i = 0; i < 4; i++) {
		obs[i] = new Models::Obstacle();
		obs[i]->SetProgram(Shader_Manager::GetShader("generalShader"));
		obs[i]->setCamera(cam);
		obs[i]->Create();
		sprintf(name, "obstacle_%i", i);
		gameModelList[name] = obs[i];
	}
	//Transform Model Matrices
	glm::vec3 center(float(sx / 2), float(sy / 2), 0.0f);
	obs[0]->setModelMatrix(glm::translate(glm::translate(glm::mat4(1.), glm::vec3(-float(sx / 4), 0.0f, 0.0f)), center));
	obs[1]->setModelMatrix(glm::translate(glm::translate(glm::mat4(1.), glm::vec3(float(sx / 4), 0.0f, 0.0f)), center));
	obs[2]->setModelMatrix(glm::translate(glm::translate(glm::mat4(1.), glm::vec3(0.0f, -float(sx / 4), 0.0f)), center));
	obs[3]->setModelMatrix(glm::translate(glm::translate(glm::mat4(1.), glm::vec3(0.0f, float(sx / 4), 0.0f)), center));
}

Models_Manager::~Models_Manager() {
	for (auto model : gameModelList) {
		delete model.second;
	}
	gameModelList.clear();
}

void Models_Manager::DeleteModel(const std::string& gameModelName) {
	IGameObject* model = gameModelList[gameModelName];
	model->Destroy();
	gameModelList.erase(gameModelName);
}

const IGameObject& Models_Manager::GetModel(const std::string& gameModelName) const {
	return (*gameModelList.at(gameModelName));
}

std::map<std::string, IGameObject*>& Models_Manager::getModelList() {
	return this->gameModelList;
}

void Models_Manager::Update(float dt) {
	//auto -it's a map iterator
	for (auto model : gameModelList) {
		model.second->Update(dt);
	}
}

void Models_Manager::Draw() {
	//auto -it's a map iterator
	for (auto model : gameModelList) {
		model.second->Draw();
	}
}
