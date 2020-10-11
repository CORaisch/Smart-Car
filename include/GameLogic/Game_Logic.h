#pragma once

#include <map>
#include <iostream>
#include "Rendering/IGameObject.h"
#include "CollisionDetection/Collision_Detection.h"
#include "GameLogic/Agent.h"

#define PI 3.14159265359

class Game_Logic {

public:
	Game_Logic();
	~Game_Logic();

	void Init(int screen_x, int screen_y);
	bool checkBorders(std::vector<Rendering::VertexFormat>& verts, glm::mat4& modelMat);
	bool checkCollision();
	void reset();
	void setModels(std::map<std::string, Rendering::IGameObject*>& gameModelList);
	void setEventHandler(EventHandler& evHndl);
	void compute(float dt);
	void transformObstacles(float dt);
	void proceedInputs(float dt);
	void updateConsole();
	void printTelemetry();

	std::map<std::string, Rendering::IGameObject*> models;
	std::vector<Rendering::VertexFormat> border_top, border_left, border_bottom, border_right;

private:
	int screen_x, screen_y, cursor_x_start, cursor_y_start;
  float dist, sign;
	float Q_temp[3];
	bool keyboard_control;
	Agent agent;
	EventHandler* input;
};
