#include "Rendering/Camera.h"

Camera::Camera(int wnd_x, int wnd_y) {
	this->window_width = wnd_x;
	this->window_height = wnd_y;
	//Init Vectors
	this->camera_position = glm::vec3(0.0f, 0.0f, 1.0f);   //Camera at (0, 0, -10)
	this->camera_look_at = glm::vec3(0.0f, 0.0f, 0.0f);	   //And looks at Origin
	this->camera_up = glm::vec3(0.0f, 1.0f, 0.0f);		   //Head is up
	//Init Camera Mats
	this->view_matrix = glm::lookAt(camera_position, camera_look_at, camera_up);//Building Look-At-Camera
	this->projection_matrix = glm::ortho(0.0f, float(this->window_width), float(this->window_height), 0.0f, 1.0f, 0.0f); //World Space: x ~ [0, width], y ~ [height, 0](top to bottom)
}

Camera::~Camera() {}

void Camera::Update(float dt) {
	//Static Camera! -> We don't move the Camera
}

void Camera::Resize(int s_x, int s_y) {
	this->window_width = s_x;
	this->window_height = s_y;
	this->projection_matrix = glm::ortho(0.0f, float(this->window_width), float(this->window_height), 0.0f, 1.0f, 0.0f); 
}

glm::mat4 Camera::getView() {
	return this->view_matrix;
}

glm::mat4 Camera::getProjection() {
	return this->projection_matrix;
}

void Camera::getSize(int& x, int& y) {
	x = this->window_width;
	y = this->window_height;
}
