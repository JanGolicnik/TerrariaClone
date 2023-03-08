#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aSpriteCoords;


layout (location = 1) uniform mat4 trans;


out vec2 fSpriteCoords;


void main() {
	gl_Position = trans * vec4(aPosition, 1.0f);
	fSpriteCoords = aSpriteCoords;
}