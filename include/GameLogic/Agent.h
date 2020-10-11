#pragma once

#include "glm/glm.hpp"
#include "GameLogic/Sensor.h"
#include "CollisionDetection/Collision_Detection.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "floatfann.h"
#include "fann_cpp.h"

//Macros
#define PI 3.14159265359
#define RAD(A)	(((2.0f * PI) / 360.0f) * A)

struct Agent {

	/********************/
	/*MEMBERS DEFINITION*/
	/********************/

	/*ANN Parameters*/
	struct fann *ann;
	fann_type *input, *input_temp;
	fann_type *output;
	/*Q-Learning Parameters*/
	float Q_t[3];
	float action[3] = { 0.0f, 0.5f, 1.0f };
	float reward, learning_rate, discount_factor, Q_old, Q_new, MSE;
	float temperature, max_temperature, epsilon;
	float max_view;
	int selected_action, epoche;
	bool crashed;
	/*Model Parameters*/
	std::map<std::string, Rendering::IGameObject*> models;
	std::vector<Rendering::VertexFormat> border_top, border_left, border_bottom, border_right;
	Sensor sensors[5];
	/*Time Parameters*/
	time_t time_at_start, current_time;
	char *return_buffer;
	int timer[3] = {0, 0, 0};

	/************************/
	/*end MEMBERS DEFINITION*/
	/************************/

	//Empty Default Constructor
	Agent() {}

	//Initialize Agents Data
	void Init(std::map<std::string, Rendering::IGameObject*>& mods, std::vector<Rendering::VertexFormat>& border_top, std::vector<Rendering::VertexFormat>&border_left, std::vector<Rendering::VertexFormat>& border_bottom, std::vector<Rendering::VertexFormat>& border_right) {
		/*Init Parameters for Q-Learning*/
		this->input = new fann_type[6];
		this->input_temp = new fann_type[6];
		this->output = new fann_type[1];
		this->max_temperature = 5000.0f;
		this->temperature = max_temperature;
		this->epsilon = 1.0f;	//Initially start with 100% Probability of selecting Actions randomly (Parameter will be decreased over time)
		this->learning_rate = 0.01f;
		this->discount_factor = 0.5f;
		this->epoche = 1;
		this->max_view = 200.0f;		//Ursprünglich 200
		this->crashed = false;
		this->MSE = 0.0f;

		/*Init Rands*/
		srand(time(NULL));

		//Init Time
		this->return_buffer = new char[30];
		time(&this->time_at_start);

		/*Init Sensors*/
		this->models = mods;
		this->border_top = border_top;
		this->border_bottom = border_bottom;
		this->border_left = border_left;
		this->border_right = border_right;
		//Get Vetices of Car
		std::vector<Rendering::VertexFormat>& carVerts = this->models["car"]->getVertices();
		glm::vec4 p0 = carVerts[0].position;
		glm::vec4 p1 = carVerts[1].position;
		glm::vec4 p2 = carVerts[2].position;
		//Init Center Point c
		glm::vec4 df = (p1 - p0) / 2.0f;
		glm::vec4 ds = (p2 - p0) / 4.0f;
		glm::vec4 c = p0 + df + ds;
		//Init Sensor s0
		glm::vec4 d0 = glm::normalize(p0 - p2);
		sensors[0] = { c, d0 };
		//Init Angles
		float inner_angle = 22.5f;
		float outter_angle = 45.0f;
		//Init Sensor s1
		float deg = RAD(inner_angle);
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec4 d1 = glm::normalize(rotation * d0);
		this->sensors[1] = { c, d1 };
		//Init Sensor s2
		rotation = glm::rotate(glm::mat4(1.0f), -deg, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec4 d2 = glm::normalize(rotation * d0);
		this->sensors[2] = { c, d2 };
		//Init Sensor s3
		deg = RAD(outter_angle);
		rotation = glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec4 d3 = glm::normalize(rotation * d0);
		this->sensors[3] = { c, d3 };
		//Init Sensor s4
		rotation = glm::rotate(glm::mat4(1.0f), -deg, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec4 d4 = glm::normalize(rotation * d0);
		this->sensors[4] = { c, d4 };

		/*Init FANN*/
		//Setup Topology
		ann = fann_create_standard( 3,	//Setup ANN with 3 Layers
			  		  			    6,	//Setup 6 Neurons on Input-Layer
									7,	//Setup 7 Neurons on Hidden-Layer
									1);//Setup 1 Neuron as Output-Neuron
		//Setup Learningrate
		fann_set_learning_rate(ann, 0.7f);
		//Setup Activation-Functions
		fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
		fann_set_activation_function_output(ann, FANN_LINEAR);
		//Setup Training Algorithm
		fann_set_training_algorithm(ann, FANN_TRAIN_INCREMENTAL);
		//Setup Steepness
		//fann_set_activation_steepness_hidden(ann, 1.0);
		//fann_set_activation_steepness_output(ann, 1.0);
		//Init Weights Randomly
		fann_type min = 0.0f, max = 1.0f;
		fann_randomize_weights(ann, min, max);

		/*Get initial Sensor States 'x_i' and Q-Values*/
		//Get initial Sensor State
		this->updateSensors();	//Sensor State 'x_i' updated in Senores-Array
		//Get initial Q-Values Q(x_i, 0.0f),  Q(x_i, 0.5f) and Q(x_i, 1.0f)
		assign_Q_Values();
		//Save Initial Version of ANN for Debug Reasons
		fann_save(ann, "fann_after_init.dat");
	}

	//Proceed one Step of the Agent
	void proceed(float dt) {
		/*Update Timer*/
		this->updateTimer();

		/*Select optimal Action 'a' depending on wether Agent is in Exploration or Exploitation Phase*/
		this->selected_action = select_epsilon_greedy();

		/*Perform optimal Action a*/
		//Switch for Action
		if (selected_action == 0)
			turnCar(0.1f, dt);
		else if (selected_action == 2)
			turnCar(-0.1f, dt);
		//[If selected_action == 1 -> Do Nothing (Neutral)]
		//Move Car Forward
		moveCar(0.08f, dt);

		/*Synchronize Sensors*/
		this->updateSensors();

		/*Update Q(x, a)*/
		//Backup Q(x, a)
		Q_old = Q_t[selected_action];
		//Backup old input
		assign_input_temp(selected_action);
		//Get Q(x', 0.0f), Q(x', 0.5f), Q(x', 1.0f)
		assign_Q_Values();
		//Get Reward
		reward = receive_local_reward();
		//Get Max Q-Value 'Q_max'
		float Q_max = get_max_Q();
		//Calculate: Q'(x, a) = Q(x, a) +  alpha * (r + gamma * Q_max - Q(x, a))
		Q_new = Q_old + learning_rate * (reward + discount_factor * Q_max - Q_old);

		/*Train ANN*/
		//Backpropagate Error: Err = Q(x, a) - Q'(x, a)
		this->output[0] = Q_new;
		//Train ANN with old Input Data and desired Q-Value-Output
		fann_train(ann, input_temp, output);

		//Update MSE
		this->MSE = fann_get_MSE(ann);
	}

	void assign_Q_Values() {
		//Declare Locals
		float *out;
		//Assign Sensor Input
		this->input[0] = this->sensors[0].distance;
		this->input[1] = this->sensors[1].distance;
		this->input[2] = this->sensors[2].distance;
		this->input[3] = this->sensors[3].distance;
		this->input[4] = this->sensors[4].distance;
		//Assign and Feed Action 'turn_left'
		this->input[5] = this->action[0];
		out = fann_run(ann, input);
		this->Q_t[0] = out[0];
		//Assign and Feed Action 'neutral'
		this->input[5] = this->action[1];
		out = fann_run(ann, input);
		this->Q_t[1] = out[0];
		//Assign and Feed Action 'turn_right'
		this->input[5] = this->action[2];
		out = fann_run(ann, input);
		this->Q_t[2] = out[0];
	}

	void assign_input_temp(int action) {
		this->input_temp[0] = this->input[0];
		this->input_temp[1] = this->input[1];
		this->input_temp[2] = this->input[2];
		this->input_temp[3] = this->input[3];
		this->input_temp[4] = this->input[4];
		this->input_temp[5] = this->action[action];
	}

	float get_max_Q() {
		float q_temp = Q_t[0];
		for (int i = 1; i < 3; i++) {
			if (q_temp <= Q_t[i]) {
				q_temp = Q_t[i];
			}
		}
		//Return Max Q-Value
		return q_temp;
	}

	void onCrash() {
		//Increment Number of Epoches
		this->epoche++;

		//Reducing Temperature when using Softmax-Action-Selection
		if (this->temperature <= 1.0f)
			this->temperature = 0.0f;
		else
      this->temperature -= 1.0f; //Probability to choose an Action randomly decreases linearly

		//Reducing Epsilon when using Epsilon-Greedy-Selection
		this->epsilon = this->temperature / this->max_temperature;

		//Set Crash Indicator
		this->crashed = true;
	}

	void onExit() {
		//Save ANN into File
		fann_save(ann, "fann_after_exit.dat");
		//Destroy ANN
		fann_destroy(ann);
	}

	float receive_temporal_difference_reward() {
		if (!this->crashed) {
			//Calculate Squared Mean of (d(t+1)-d(t))^2
			float sum = 0.0f;
			float pitch_factor = 500.0f; //pitch_factor = 1.0f for standard
			for (int i = 0; i < 5; i++) {
				sum += (1.0f/5.0f) * pitch_factor*(this->input[i] - this->input_temp[i]);
			}
			//Return Squared Mean
			return sum;
		} else {
			this->crashed = false;
			return -2.0f;
		}
	}

	float receive_local_reward() {
		if (!this->crashed) {
			//If one Sensor is to close => r = 1
			if (!((this->sensors[0].distance >= 0.4f) && (this->sensors[4].distance >= 0.4f)
				&& (this->sensors[3].distance >= 0.4f) && (this->sensors[2].distance >= 0.4f)
				&& (this->sensors[1].distance >= 0.4f))) {
				return -10.0f; //REWARD = -2
			} else {
				//Calculate Discrete Reward Table to score mean distance of Sensor
				/*r = -4 if car crashes
				/*r = 0 if d_t is in (0, 0.33]
				/*r = 1 if d_t is in (0.33, 0.66]
				/*r = 2 if d_t is in (0.66, 1]
				*/
				float d_t = calculate_mean_distance();
				if (d_t >= 0.0f && d_t <= 0.33f)
					return 0.0f; //REWARD = 0
				else if (d_t <= 0.66f)
					return 1.0f; //REWARD = 1
				else
					return 2.0f; //REWARD = 2
			}
		} else {
			this->crashed = false;
			return -100.0f; //REWARD = -4
		}
	}

	float receive_local_reward_with_spatial_consideration() {
		if (!this->crashed) {
			//Define Spatial Means
			float s_l = (this->sensors[2].distance + this->sensors[4].distance) / 2.0f;
			float s_r = (this->sensors[1].distance + this->sensors[3].distance) / 2.0f;
			float s_f = this->sensors[0].distance;
			//Evaluate Spatial Means
			float evaluation_limit = 0.7f; //Set evaluation_limit to 0.7 (i.e., if mean sensor length is beyond 0.7, its bad)
			bool eval_l = true, eval_f = true, eval_r = true; //Declare Eval Bools, where true = "good", false = "bad"
			if (s_l <= evaluation_limit)
				eval_l = false;
			if (s_f <= evaluation_limit)
				eval_f = false;
			if (s_r <= evaluation_limit)
				eval_r = false;
			//Switch for Cases
			if (eval_l && eval_f && eval_r)
				return 3.0f; //Return +3 for all 3 Sensors be "Good"
			if ((eval_l && eval_f && !eval_r) || (!eval_l && eval_f && eval_r))
				return 2.0f; //Return +2 for 2 Side Senosors be "Good"
			if ((!eval_l && !eval_f && eval_r) || (eval_l && !eval_f && !eval_r))
				return 0.0f; //Return 0 for jusst 1 Side Sensor be "Good"
			if (eval_l && !eval_f && eval_r)
				return -1.0f; //If just Centered Sensor is "Bad" return -1, because car could still escape
			else
				return -2.0f; //If just Centered Sensor is "Good" or all are "Bad" return -2, because car will not escape with high probability
		} else {
			this->crashed = false;
			return -3.0f; //If Car crashed return -3
		}
	}

	float calculate_mean_distance() {
		float sum = 0.0f;
		for (int i = 0; i < 5; i++) {
			sum += this->sensors[i].distance;
		}
		//Return Mean Sensor Distance
		return sum/5.0f;
	}

	/*Selects Action with highest Q-Value (Optimal Action)*/
	int select_greedy() {
		float q_temp = this->Q_t[0];
		int i_temp	 = 0;
		for (int i = 1; i < 3; i++) {
			if (q_temp <= this->Q_t[i]) {
				q_temp = this->Q_t[i];
				i_temp = i;
			}
		}
		return i_temp;
	}

	int select_epsilon_greedy() {
		//Roll Number in Range [0.0, 1.0]
		float dice = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
		if (dice < this->epsilon)
			return select_random();
		else
			return select_greedy();
	}

	/*Randomly (iid) Selects one Action*/
	int select_random() {
		return rand() % 3;
	}

	/*Randomly Selects one Action							*/
	/*Boltzman Distribution is used as Distrubution Function*/
	int select_softmax() {
		//Find Probability Coefficients
		float p_c[3], denominator = 0.0f;
		for (int i = 0; i < 3; i++) {
			p_c[i] = exp(this->Q_t[i] / this->temperature);
			denominator += p_c[i];
		}
		//Roll Number in Range [0.0, denominator]
		float dice = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / denominator));
		//Select winning Action
		float prob_sum = 0.0f;
		for (int i = 0; i < 3; i++) {
			prob_sum += p_c[i];
			if ((dice <= prob_sum) && (dice >= (prob_sum - p_c[i]))) //If dice is in Range [prob_sum(i-1), prob_sum(i)] -> action(i) is winner!
				return i;
		}
	}

	float feed_forward_in_q_function(float* in) {
		fann_type* input = in;
		fann_type* out = fann_run(ann, input);
		return out[0];
	}

	char* get_time_since_start() {
		//Some Cosmetics...
		char t_0[3], t_1[3], t_2[3];
		if (this->timer[0] < 10)
			sprintf(t_0, "0%i", this->timer[0]);
		else
			sprintf(t_0, "%i", this->timer[0]);
		if (this->timer[1] < 10)
			sprintf(t_1, "0%i", this->timer[1]);
		else
			sprintf(t_1, "%i", this->timer[1]);
		if (this->timer[2] < 10)
			sprintf(t_2, "0%i", this->timer[2]);
		else
			sprintf(t_2, "%i", this->timer[2]);
		//Build 'pretty' Timer String
		sprintf(this->return_buffer, "%s:%s:%s ", t_2, t_1, t_0);
		return this->return_buffer;
	}

	/*Moves Car one Step Forward*/
	void moveCar(float move_speed, float dt) {
		this->models["car"]->setModelMatrix(glm::translate(this->models["car"]->getModelMatrix(), glm::vec3(0.0f, -dt*move_speed, 0.0f)));
	}

	/* If angleDec positive => Turn Car Counter Clock Wise */
	/* If angleDec negative => Turn Car Clock Wise		   */
	void turnCar(float angleDec, float dt) {
		float angleRad = angleDec*(PI / 180.0f);
		this->models["car"]->setModelMatrix(glm::rotate(this->models["car"]->getModelMatrix(), dt*angleRad, glm::vec3(0.0f, 0.0f, 1.0f)));
	}

	void updateTimer() {
		//Get Current Time
		time(&this->current_time);
		int time_since_start = this->current_time - this->time_at_start;
		//Store in Time Table
		this->timer[0] = time_since_start % 60; //Seconds
		this->timer[1] = ((int)(time_since_start / 60)) % 60; //Minutes
		this->timer[2] = ((int)(time_since_start / (3600))); // Hours
	}

	void updateSensors() {
		//Transform Sensors in Worldspace
		glm::vec4 d_ws[5];
		float t_nearest, t_temp[8];
		glm::vec4 c_ws = this->models["car"]->getModelMatrix() * this->sensors[0].position;
		d_ws[0] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[0].direction);
		d_ws[1] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[1].direction);
		d_ws[2] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[2].direction);
		d_ws[3] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[3].direction);
		d_ws[4] = glm::normalize(this->models["car"]->getModelMatrix() * this->sensors[4].direction);
		//Itetate through all S	ensors
		for (int i = 0; i < 5; i++) {
			//Check Collision: Ray with Border Triangles
			t_temp[0] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_top);
			t_temp[1] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_left);
			t_temp[2] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_bottom);
			t_temp[3] = intersectRayTriangles(c_ws, d_ws[i], glm::mat4(1.0f), border_right);
			//Check Collision: Ray with Moving Obstacles
			t_temp[4] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_0"]->getModelMatrix(), this->models["obstacle_0"]->getVertices());
			t_temp[5] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_1"]->getModelMatrix(), this->models["obstacle_1"]->getVertices());
			t_temp[6] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_2"]->getModelMatrix(), this->models["obstacle_2"]->getVertices());
			t_temp[7] = intersectRayTriangles(c_ws, d_ws[i], this->models["obstacle_3"]->getModelMatrix(), this->models["obstacle_3"]->getVertices());
			//Get nearest Intersection
			t_nearest = FLT_MAX;
			for (int j = 0; j < 8; j++) {
				if (t_temp[j] < t_nearest)
					t_nearest = t_temp[j];
			}
			t_nearest = (t_nearest > this->max_view ? this->max_view : t_nearest);
			this->sensors[i].distance = t_nearest / this->max_view;
		}
	}

};
