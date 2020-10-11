#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

class Camera {

public:
	Camera(int wnd_x, int wnd_y);
	~Camera();

	void Update(float dt);
	void Resize(int s_x, int s_y);
	glm::mat4 getView();
	glm::mat4 getProjection();
	void getSize(int& x, int& y);

private:
	glm::mat4 view_matrix, projection_matrix;
	glm::vec3 camera_position, camera_up, camera_look_at;
	int window_width, window_height;
};
