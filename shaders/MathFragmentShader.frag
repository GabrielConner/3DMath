#version 460 core
out vec4 FragColor;
uniform vec4 color;
uniform float xDist;

in vec4 vertPos;
void main() {
	FragColor = vec4(color.xyz * (xDist - length(vertPos.xz)) / xDist, 1);
}