#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

out vec2 t1;
out vec2 fPos;

void main() {
	gl_Position = vec4(aPosition, 1.0f);
	t1 = aTexCoords;
	fPos = aPosition.xy;
}