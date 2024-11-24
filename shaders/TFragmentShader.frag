#version 460 core
out vec4 fragColor;
in vec2 uvCoord;
uniform sampler2D texTarget;
uniform vec4 color;

void main() {
	fragColor = color * texture(texTarget, vec2(uvCoord.x, uvCoord.y));
}