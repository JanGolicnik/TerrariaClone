#include "particles.h"
#include <animations.h>
#include <textures.h>
#include <globals.h>
#include <enemyFunctions.h>

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
	void dropGore(glm::vec2 pos, std::string name) {
		std::vector <std::pair<std::string, glm::vec2> >vec;
		if (name == "bunny") {
			vec.push_back(std::make_pair("bunnygore1", glm::vec2(1)));
			vec.push_back(std::make_pair("bunnygore2", glm::vec2(1)));
		}
		else if (name == "demoneye") {
			vec.push_back(std::make_pair("demoneyegore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("demoneyegore2", glm::vec2(1, 1)));
		}
		else if (name == "servantofcthulu") {
			vec.push_back(std::make_pair("servantofcthulugore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("servantofcthulugore2", glm::vec2(1, 1)));
		}
		else if (name == "zombie") {
			vec.push_back(std::make_pair("zombiegore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("zombiegore2", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("zombiegore3", glm::vec2(1, 1)));
		}
		else if (name == "eyeofcthulueye") {
			vec.push_back(std::make_pair("eyeofcthulugore1", glm::vec2(1, 1)));
		}
		else if (name == "eyeofcthulu") {
			vec.push_back(std::make_pair("eyeofcthulugore2", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("eyeofcthulugore3", glm::vec2(1, 1)));
		}
		else if (name == "eaterofworldshead") {
			vec.push_back(std::make_pair("eaterofworldsheadgore1", glm::vec2(1, 1)));
		}
		else if (name == "eaterofworldsbody") {
			switch (rand() % 4) {
			case 0:
				vec.push_back(std::make_pair("eaterofworldsbodygore1", glm::vec2(1, 1)));
				break;
			case 1:
				vec.push_back(std::make_pair("eaterofworldsbodygore2", glm::vec2(1, 1)));
				break;
			case 2:
				vec.push_back(std::make_pair("eaterofworldsbodygore3", glm::vec2(1, 1)));
				break;
			case 3:
				vec.push_back(std::make_pair("eaterofworldsbodygore4", glm::vec2(1, 1)));
				break;
			}
		}
		else if (name == "eaterofworldstail") {
			vec.push_back(std::make_pair("eaterofworldstailgore1", glm::vec2(1, 1)));
		}
		else if (name == "devourerhead") {
			vec.push_back(std::make_pair("devourerheadgore1", glm::vec2(1, 1)));
		}
		else if (name == "devourerbody") {
			if (rand() % 2) 
			vec.push_back(std::make_pair("devourerbodygore1", glm::vec2(1, 1)));
		}
		else if (name == "devourertail") {
			vec.push_back(std::make_pair("devourertailgore1", glm::vec2(1, 1)));
		}
		else if (name == "wormhead") {
			vec.push_back(std::make_pair("wormheadgore1", glm::vec2(1, 1)));
		}
		else if (name == "wormbody") {
			if(rand()%2)
			vec.push_back(std::make_pair("wormbodygore1", glm::vec2(1, 1)));
		}
		else if (name == "wormtail") {
			vec.push_back(std::make_pair("wormtailgore1", glm::vec2(1, 1)));
		}
		else if (name == "eaterofsouls") {
			vec.push_back(std::make_pair("eaterofsoulsgore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("eaterofsoulsgore2", glm::vec2(1, 1)));
		}
		else if (name == "skeleton") {
			vec.push_back(std::make_pair("skeletongore1", glm::vec2(1, 1)));
		}
		else if (name == "demon") {
			vec.push_back(std::make_pair("demongore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("demongore2", glm::vec2(1, 1)));
		}
		else if (name == "vulture") {
			vec.push_back(std::make_pair("vulturegore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("vulturegore2", glm::vec2(1, 1)));
		}
		else if (name == "pot") {
			vec.push_back(std::make_pair("potgore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("potgore2", glm::vec2(1, 1)));
		}
		else if (name == "antlion") {
			vec.push_back(std::make_pair("antliongore1", glm::vec2(1, 1)));
			vec.push_back(std::make_pair("antliongore2", glm::vec2(1, 1)));
		}

		for (int i = 0; i < vec.size(); i++) {

			int e = ECS::newEntity();
			
			auto ppos = std::make_shared<glm::vec2>(pos);

			glm::vec4 sC;
			sC = textures::nametocoords.at(vec[i].first)->coords;
			glm::vec2 siz;
			float mod;
			siz.x = (sC.b - sC.r);
			siz.y = (sC.a - sC.g);
			if (siz.x > siz.y) {
				mod = siz.x / (8.0f / globals::spritesheetWH.x);
			}
			else {
				mod = siz.y / (8.0f / globals::spritesheetWH.y);
			}

			drawC dc;
			dc.position = ppos;
			dc.tex = vec[i].first;
			dc.size = glm::vec2(mod) * vec[i].second;
			dc.parent = globals::particleLayer;

			physicsC pc;
			pc.position = ppos;
			float randomvelx = (rand() % 1000) / 500.0f - 1;
			pc.vel = glm::vec2(randomvelx * 0.2, 0.1);
			pc.size = glm::vec2(mod) * vec[i].second;
			pc.bounce = true;
			pc.bounciness = 0.1;
			pc.friction = false;
			pc.prevpos = *pc.position;
			
			aiC ac;
			ac.onupdate = mobFunctions::goreOnUpdate;

			drawSystem::addComponent(e, &dc, false);
			ECS::queueComponent<physicsC>(e, pc);
			ECS::queueComponent<aiC>(e, ac);
		}
	}
}