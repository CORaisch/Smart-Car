#include "GameLogic/Game_Logic.h"
#include<iomanip>

//Macros
#define RAD(A)	(((2.0f * PI) / 360.0f) * A)

Game_Logic::Game_Logic() {}

Game_Logic::~Game_Logic() {
	delete input;
}

/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/
void Game_Logic::compute(float dt) {
	//Check Collisions
	if (this->checkCollision()) {
		/*#####################################*/
		/*### !! React on Collision here !! ###*/
		/*#####################################*/
		//Reset Car Position
		this->reset();
		//Let Agent React on Crash
		this->agent.onCrash();
	}
	//Proceed Agent if Computer deserves Control
	if (!this->keyboard_control)
		this->agent.proceed(dt);
	//Else just update Sensor Data
	else
		this->agent.updateSensors();
	//Update Console Output
	this->updateConsole();
	//Proceed Keyboard Inputs
	this->proceedInputs(dt);
	//Move Obstacles
	this->transformObstacles(dt);
}
/*########################################*/
/*###### !! Implement Logic Here !! ######*/
/*########################################*/

void Game_Logic::Init(int screen_x, int screen_y) {
	/*Set Screen Size*/
	this->screen_x = screen_x;
	this->screen_y = screen_y;

  /*Init Obstacle Motion Parameters */
  this->dist = 0.0f; this->sign = 1.0f;

	/*disable Keyboard Control*/
	this->keyboard_control = false;

	/*Create Border Triangles*/
	////Top
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), 0.0f, 0.0f, 1.0f)));
	this->border_top.push_back(Rendering::VertexFormat(glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)));
	////Left
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y), 0.0f, 1.0f)));
	this->border_left.push_back(Rendering::VertexFormat(glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f)));
	////Bottom
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y), 0.0f, 1.0f)));
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), float(screen_y), 0.0f, 1.0f)));
	this->border_bottom.push_back(Rendering::VertexFormat(glm::vec4(0.0f, float(screen_y) + 1.0f, 0.0f, 1.0f)));
	////Right
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), 0.0f, 0.0f, 1.0f)));
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x), float(screen_y), 0.0f, 1.0f)));
	this->border_right.push_back(Rendering::VertexFormat(glm::vec4(float(screen_x) + 1.0f, 0.0f, 0.0f, 1.0f)));

	/*Init Agent*/
	this->agent.Init(this->models, border_top, border_left, border_bottom, border_right);

	/*Init Console Data*/
	////Print Initial Telemetry
	this->printTelemetry();
}

void Game_Logic::updateConsole() {
    this->printTelemetry();
}

void Game_Logic::printTelemetry() {
  std::cout
    << std::fixed << std::setprecision(2) << std::setfill('0')\
    << "\033[s"\
    << "\r******************** Start Running Smart - Car Framework *********************\n"\
    << "\r******************************************************************************\r\n\n"\
    << "\rEpoche: " << std::setw(5) << this->agent.epoche << " | Time: " << std::setw(5) << this->agent.get_time_since_start()\
    << "\n\n"\
    << "\rSensor Data:\n"\
    << "\rS4 " << std::setw(5) << this->agent.sensors[4].distance << " | S2 " << std::setw(5) << this->agent.sensors[2].distance\
    << " | S0 " << std::setw(5) << this->agent.sensors[0].distance << " | S1 " << std::setw(5) << this->agent.sensors[1].distance << " | S3 " << std::setw(5) << this->agent.sensors[3].distance\
    << "     \r\n\n"\
    << "\rSelected Action: " << this->agent.selected_action << " | Reward: " << std::setw(5) << this->agent.reward << " | MSE of ANN: " << std::setw(5) << this->agent.MSE\
    << "\r\n\n"\
    << "\rQ-Function (3x3 Sample of current Q-Values):"\
    << "\r\n\n"\
    << "\rA             Q(S', A)"\
    << "\r\n"\
    << "\rLeft          " << std::setw(5) << this->agent.Q_t[0] << "\n"\
    << "\rNeutral       " << std::setw(5) << this->agent.Q_t[1] << "\n"\
    << "\rRight         " << std::setw(5) << this->agent.Q_t[2] << "\n"\
    << "\r\n******************************************************************************\r\n\n\033[u" << std::flush;
}

void Game_Logic::reset() {
	//Reset Cars' Model Matrix to Initial State
	glm::vec3 center(float(screen_x / 2), float(screen_y / 2), 0.0f);
	this->models["car"]->setModelMatrix(glm::translate(glm::mat4(1.), glm::vec3(float(screen_x / 2), float(screen_y / 2), 0.0f)));
}

void Game_Logic::proceedInputs(float dt) {
	//Check Basic Inputs
	if (this->input->getKeyState('1')) //Press 1 for keyboard control
      this->keyboard_control = true;
	if (this->input->getKeyState('2')) //Press 2 for agent control
      this->keyboard_control = false;
	if (this->input->getKeyState((char)27)) { //Exit on ESC
		//Call Agents' onExit Method to perform some important Tasks before shutting down
		this->agent.onExit();
		glutLeaveMainLoop();
	}
	//Check Car Control Input if Keyboard Control is Enabled
	if (this->keyboard_control) {
		//Update Movement
		float movespeed = 0.3;
		float rotspeed = 0.005;
		if (this->input->getKeyState('w'))
			this->models["car"]->setModelMatrix(glm::translate(this->models["car"]->getModelMatrix(), glm::vec3(0, -dt*movespeed, 0)));
		if (this->input->getKeyState('a'))
			this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), -dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f)));
		if (this->input->getKeyState('s'))
			this->models["car"]->setModelMatrix(glm::translate(this->models["car"]->getModelMatrix(), glm::vec3(0, dt*movespeed, 0)));
		if (this->input->getKeyState('d'))
			this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f)));
	}
}

bool Game_Logic::checkCollision() {
	std::vector<Rendering::VertexFormat>& verts = this->models["car"]->getVertices();
	glm::mat4& carMat = this->models["car"]->getModelMatrix();
	//Check Intersection with Car and any Obstacle
	return intersectTriangles(verts, carMat, this->models["obstacle_0"]->getVertices(), this->models["obstacle_0"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_1"]->getVertices(), this->models["obstacle_1"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_2"]->getVertices(), this->models["obstacle_2"]->getModelMatrix())
		|| intersectTriangles(verts, carMat, this->models["obstacle_3"]->getVertices(), this->models["obstacle_3"]->getModelMatrix())
		|| checkBorders(this->models["car"]->getVertices(), this->models["car"]->getModelMatrix());
}

bool Game_Logic::checkBorders(std::vector<Rendering::VertexFormat>& verts, glm::mat4& modelMat) {
	//Check Car against Border-Triangles
	return intersectTriangles(verts, modelMat, this->border_top, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_left, glm::mat4(1.0f))
		|| intersectTriangles(verts, modelMat, this->border_bottom, glm::mat4(1.0f)) || intersectTriangles(verts, modelMat, this->border_right, glm::mat4(1.0f));
}

void Game_Logic::transformObstacles(float dt) {
  float rotspeed = 0.000075;
	glm::vec3 center(float(screen_x / 2), float(screen_y / 2), 0.0f);
	//Translate to Origin
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), center);
	//Rotate around Origin
	transform = glm::rotate(transform, -dt*rotspeed, glm::vec3(0.0f, 0.0f, 1.0f));
	//Translate back to Center
	transform = glm::translate(transform, -center);
  //Move Obstacles sidewards
  float movespeed = 0.00075f; glm::vec3 side_dir(1.0f,0.0f,0.0f);
  transform = glm::translate(transform, center*this->sign*movespeed);
  //Update sidewards Direction
  this->dist += movespeed;
  if (this->dist>250.0f*movespeed) { this->sign = -1.0f*this->sign; this->dist = 0.0f; }
  //Apply Transformation on Obstacles
  this->models["obstacle_0"]->setModelMatrix(transform * this->models["obstacle_0"]->getModelMatrix());
	this->models["obstacle_1"]->setModelMatrix(transform * this->models["obstacle_1"]->getModelMatrix());
	this->models["obstacle_2"]->setModelMatrix(transform * this->models["obstacle_2"]->getModelMatrix());
	this->models["obstacle_3"]->setModelMatrix(transform * this->models["obstacle_3"]->getModelMatrix());
}

void Game_Logic::setModels(std::map<std::string, Rendering::IGameObject*>& gameModelList) {
	this->models = gameModelList;
}

void Game_Logic::setEventHandler(EventHandler& evHndl) {
	this->input = &evHndl;
}
