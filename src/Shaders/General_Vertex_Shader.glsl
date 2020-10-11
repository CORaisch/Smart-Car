#version 400 core 
layout(location = 0) in vec4 in_position;  
layout(location = 1) in vec4 in_color;
 
out vec4 color;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
	color = in_color;
	gl_Position = P * V * M * in_position;
}