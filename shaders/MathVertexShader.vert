#version 460 core
layout(location=0) in vec3 pos;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

out vec4 vertPos;

void main() {
	gl_Position = projection * view * transform * vec4(pos, 1);
	vertPos = transform * vec4(pos, 1);
}