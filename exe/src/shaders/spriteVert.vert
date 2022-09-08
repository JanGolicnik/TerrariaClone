#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aSpriteCoords;

layout (location = 1) uniform mat4 transform;
layout (location = 3) uniform mat4 camTrans;


out vec2 fSpriteCoords;

void main() {
	gl_Position = camTrans * transform * vec4(aPosition, 1.0f);
	fSpriteCoords = aSpriteCoords;
}