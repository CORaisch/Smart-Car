#version 400 core

layout(location = 0) out vec4 out_color;

in vec4 color;
in vec2 texcoord;

uniform sampler2D tex; 

void main(void) {

  out_color = texture(tex, texcoord);
}