#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aSpriteCoords;
layout (location = 2) in vec3 aColor;

layout (location = 1) uniform mat4 trans;

out vec2 fSpriteCoords;
out vec3 fColor;

void main() {
	vec4 pos = trans * vec4(aPosition, 1.0f);
	gl_Position = pos;
	fSpriteCoords = aSpriteCoords;
	fColor = aColor;
}