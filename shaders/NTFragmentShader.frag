#version 460 core
out vec4 FragColor;
uniform vec4 color;

in vec4 vertPos;
void main() {
	FragColor = color;
}