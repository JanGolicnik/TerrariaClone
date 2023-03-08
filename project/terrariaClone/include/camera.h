#pragma once

#include <core.h>

namespace camera
{
	struct cameraShake {
		float strength;
		int duration;
	};

	extern glm::mat4 base;
	extern glm::mat4 trans;
	extern glm::vec2 pos;
	extern glm::vec2 vel;
	extern glm::vec2 tvel;
	extern float accel;
	extern glm::vec2 limits;

	extern std::vector<cameraShake> shakes;

	void updateTowardsPlayer();
	void updateNormal();

	void shake(float strength, int duration);

};

