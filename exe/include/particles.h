#pragma once
#include <core.h>
#include <componentsystems.h>

namespace particles
{
	struct particleBase {
		particleEmmiterC peC;
		glm::vec2 offset;
	};
	typedef std::vector<particleBase> effectBase;

	extern std::unordered_map<std::string, effectBase> effects;

	glm::vec4 get(std::string name);
	void emitParticle(glm::vec2 pos, particleEmmiterC* pec = nullptr, int ent = -1);
	void spawnEffect(std::string effectName, glm::vec2 pos);
	void addEffect(std::string name, effectBase base);
};
