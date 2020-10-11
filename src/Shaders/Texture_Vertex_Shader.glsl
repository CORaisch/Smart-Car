#version 400 core 
layout(location = 0) in vec4 in_position;  
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec2 in_texcoord;
 
out vec4 color;
out vec2 texcoord;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main() {
	color = in_color;
	texcoord = in_texcoord;
	gl_Position = P * V * M * in_position;
}