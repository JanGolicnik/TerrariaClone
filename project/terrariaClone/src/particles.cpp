#include "particles.h"
#include <animations.h>
#include <textures.h>
#include <globals.h>
namespace particles {

	std::unordered_map<std::string, effectBase> effects;

	glm::vec4 get(std::string name)
	{
		glm::vec4 tmp = textures::nametocoords[name]->coords;
		float height = tmp.g - tmp.a;
		float off = rand() % 3;
		off = off * (height - 1.0f / globals::spritesheetWH.y);
		tmp.g += off;
		tmp.a += off;
		return tmp;
	}
	void emitParticle(glm::vec2 pos, particleEmmiterC *pec, int ent)
	{
		if (pec != nullptr) {
			int numsimed = rand() % (int)(pec->rate + 1);
			pec->countdown = 1 + pec->rate;
			for (int i = 0; i < numsimed; i++) {
				pec->countdown--;
				while (pec->countdown <= 1) {
					pec->countdown += pec->rate;
				}
			}
			pec->setup = false;

			if (ent == -1) {
				ent = ECS::newEntity();
				physicsC pC;
				pC.weight = 0;
				pC.isstatic = true;
				pC.stoponcollision = false;
				pC.position = std::make_shared<glm::vec2>(pos);
				ECS::queueComponent<physicsC>(ent, pC);
			}
			ECS::queueComponent<particleEmmiterC>(ent, *pec);
		}
	}

	void spawnEffect(std::string effectName, glm::vec2 pos, glm::vec2 dir)
	{
		auto effect = effects[effectName];
		for (int i = 0; i < effect.size(); i++) {
			int ent = ECS::newEntity();
			physicsC pC;
			pC.weight = 0;
			pC.isstatic = true;
			pC.stoponcollision = false;
			pC.position = std::make_shared<glm::vec2>(pos + effect[i].offset);
			
			if (dir != glm::vec2(-123, -123)) {
				effect[i].peC.dir = dir;
			}
			
			ECS::queueComponent<physicsC>(ent, pC);
			ECS::queueComponent<particleEmmiterC>(ent, effect[i].peC);
		}	
	}
	void addEffect(std::string name, effectBase effect)
	{
		effects[name] = effect;
	}
}