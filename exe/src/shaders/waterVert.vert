#version 430 core
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec2 aSpriteCoords;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aLight;
layout (location = 4) in float aLevel;

layout (location = 4) uniform mat4 transform;

out vec2 fSpriteCoords;
out vec2 fTexCoords;
out vec3 fLight;
out float fLevel;

void main() {
	vec4 pos = transform * vec4(aPosition, 0 , 1.0f);
	gl_Position = pos;
	fSpriteCoords = aSpriteCoords;
	fTexCoords = aTexCoords;
	fLight = aLight;
	fLevel = aLevel;
}