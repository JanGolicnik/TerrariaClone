#pragma once
#include <core.h>

#include <particleEmmiterSystem.h>

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
	int spawnEffect(std::string effectName, glm::vec2 pos, glm::vec2 dir = glm::vec2(-123,-123));
	void addEffect(std::string name, effectBase base);
	void dropGore(glm::vec2 pos, std::string name);
};
