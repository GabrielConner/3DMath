#version 460 core
out vec4 FragColor;
uniform vec4 color;
uniform float xDist;
uniform bool colorChange;

in vec4 vertPos;
void main() {
	if (colorChange)
		FragColor = vec4(color.xyz * (xDist - length(vertPos.xz)) / xDist, 1);
	else
		FragColor = color;
}