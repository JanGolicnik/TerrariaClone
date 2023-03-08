#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aSpriteCoords;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aSpritesheet;

layout (location = 0) uniform mat4 camTrans;

out vec2 fSpriteCoords;
out vec4 fColor;
out float fSpritesheet;

void main() {
	gl_Position = camTrans * vec4(aPosition, 1.0f);
	fSpriteCoords = aSpriteCoords;
	fColor = aColor;
	fSpritesheet = aSpritesheet;
}