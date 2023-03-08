#include "BlockManager.h"

#include <Layers.h>

#include <componentsystems.h>

#include <globals.h>

#include <Player.h>

#include <Inventory.h>
#include <DroppedItems.h>

#include <utils.h>

#include <textures.h>

#include <enemies.h>

namespace blocks {
	std::unordered_map<std::string, BlockInfo> nameToInfo;
	std::unordered_map<int, BlockInfo> idToInfo;
	std::unordered_map<int, std::string> idToName;
	std::unordered_map<std::string, int> nameToID;
	std::vector<std::vector<PremadeBlock>> blockBuffer;
	std::vector<Decor> decorations;

	glm::vec2 blockSc[16] = {
		{0,   0}, //nobene okol 0
		{0,  -3}, //ena zgori 1
		{-3,  0}, //ena desno 2
		{-3, -3}, //ena zgori ena desno 3
		{0,  -1}, //ena spodi 4
		{0,  -2}, // ena zgori ena spodi 5
		{-3, -1}, //ena spodi ena desno 6
		{-3, -2}, //ena zgori ena spodi ena desno 7
		{-1,  0}, //ena levo 8
		{-1, -3}, //ena levo ena zgori 9
		{-2,  0}, //ena levo ena desno 10
		{-2, -3}, // ena zgori ena levo ena desno 11
		{-1, -1}, // ena spodi ena desno 12
		{-1, -2}, //levo zgori spodi 13
		{-2, -1}, //levo spodi desno 14
		{-2, -2} //vse okol 15
	};

	glm::vec4 wallSc[16] = {
		{0, 0, 0, 0}, //nobene okol 0
		{0,  -42, 0, -42}, //ena zgori 1
		{-42,  0, -42, 0}, //ena desno 2
		{-42, -42, -42, -42}, //ena zgori ena desno 3
		{0,  -16, 0, -16}, //ena spodi 4
		{0,  -29, 0, -29}, // ena zgori ena spodi 5
		{-42, -16, -42, -16}, //ena spodi ena desno 6
		{-42, -29, -42, -29}, //ena zgori ena spodi ena desno 7
		{-16,  0, -16, 0}, //ena levo 8
		{-16, -42, -16, -42}, //ena levo ena zgori 9
		{-29,  0, -29, 0}, //ena levo ena desno 10
		{-29, -42, -29, -42}, // ena zgori ena levo ena desno 11
		{-16, -16, -16 ,-16}, // ena spodi ena levo 12
		{-16, -29, -16, -29}, //levo zgori spodi 13
		{-29, -16, -29, -16}, //levo spodi desno 14
		{-25, -25, -33, -33} //vse okol 15
	};

	glm::vec4 torchSc[4] = {
		{0, 0, 0, 0}, //skos razn :tm
		{9, 0, 8, -4}, // kocka levo
		{17, 0, 16, -4}, // kocka desno
		{25, 0, 25, 0}, // kocki desno pa levo
	};

	glm::vec4 grassSc[5] = {
		{0,0,0,0},
		{8,0,8,0},
		{16,0,16,0},
		{24,0,24,0},
		{32,0,32,0}
	};

	glm::vec4 stalaktitSc[5] = {
	{0,0,0,0},
	{8,0,8,0},
	{16,0,16,0},
	{24,0,24,0}
	};

	glm::vec4 smallrockSc[6] = {
		{0,0,0,0},
		{8,0,8,0},
		{16,0,16,0},
		{24,0,24,0},
		{32,0,32,0},
		{40,0,40,0}
	};

	glm::vec4 mediumrockSc[3] = {
		{0,0,0,0},
		{16,0,16,0},
		{32,0,32,0}
	};

	glm::vec4 platformSc[8] = {
	{0,0,0,0},
	{-8,0,-8,0},
	{-16,0,-16,0},
	{-24,0,-24,0},
	{-32,0,-32,0},
	{-40,0,-40,0},
	{8,0,8,0},
	{16,0,16,0}
	};

	void addBlock(std::string name, std::string texture, bool collidable, bool solid, std::string itemname, const char* layer, bool updates, SpriteType spriteType, glm::vec2 size, float friction, std::vector<BlockPlaceCondition> placeConditions, bool updateLeft, bool updateTop, bool updateBot, bool updateRight) {
		if (nameToInfo.count(name) == 0) {
			int id = addToBuffer(texture, size, spriteType);
			BlockInfo tmp;
			tmp.life = 100;
			tmp.collidableTop = collidable;
			tmp.collidableBot = collidable;
			tmp.collidableLeft = collidable;
			tmp.collidableRight = collidable;
			tmp.notReplacable = solid;
			tmp.texture = texture;
			tmp.drops = itemname;
			tmp.layer = layer;
			tmp.light = glm::vec3(0);
			tmp.size = size;
			tmp.spriteType = spriteType;
			tmp.friction = friction;
			tmp.updatesBot = updateBot;
			tmp.updatesLeft = updateLeft;
			tmp.updatesTop = updateTop;
			tmp.updatesRight = updateRight;
			tmp.conditions = placeConditions;
			tmp.function = BFuncs::nothing;
			tmp.onPlace = BFuncs::nothing;
			tmp.onBreak = BFuncs::nothing;
			tmp.onUpdate = BFuncs::nothing;
			tmp.updates = updates;
			tmp.craftingStation = "none";
			tmp.biome = "none";
			nameToInfo.insert(std::make_pair(name, tmp));
			idToInfo.insert(std::make_pair(id, tmp));
			idToName.insert(std::make_pair(id, name));
			nameToID.insert(std::make_pair(name, id));
		}
	}
	void addRule(std::string name, std::vector<BlockRuleCond> conditions, std::function<void(BlockRuleArgs)> func, const char* block, const char* layer, bool exBool)
	{
		if (blocks::nameToInfo.count(name) < 1)
		{
			std::cout << "adding rule on " << name << " failed cuy block dont existance" << "\n";
		}
		BlockRule tmp;
		tmp.conditions = conditions;
		tmp.func = func;
		tmp.block = block;
		tmp.layer = layer;
		tmp.exBool = exBool;
		blocks::nameToInfo[name].rules.push_back(tmp);
		blocks::idToInfo[nameToID[name]].rules.push_back(tmp);
	}
	void addFunction(std::string name, std::function<bool(BlockFuncArgs)> func)
	{
		nameToInfo[name].function = func;
		idToInfo[nameToID[name]].function = func;
	}
	void addOnPlace(std::string name, std::function<bool(BlockFuncArgs)> func)
	{
		nameToInfo[name].onPlace = func;
		idToInfo[nameToID[name]].onPlace = func;
	}
	void addOnBreak(std::string name, std::function<bool(BlockFuncArgs)> func)
	{
		nameToInfo[name].onBreak = func;
		idToInfo[nameToID[name]].onBreak = func;
	}
	void addOnUpdate(std::string name, std::function<bool(BlockFuncArgs)> func)
	{
		nameToInfo[name].onUpdate = func;
		idToInfo[nameToID[name]].onUpdate = func;
	}
	void modifyCollidable(std::string name, bool collidableTop, bool collidableBot, bool collidableLeft, bool collidableRight)
	{
		nameToInfo[name].collidableTop = collidableTop;
		idToInfo[nameToID[name]].collidableTop = collidableTop;
		nameToInfo[name].collidableBot = collidableBot;
		idToInfo[nameToID[name]].collidableBot = collidableBot;
		nameToInfo[name].collidableLeft = collidableLeft;
		idToInfo[nameToID[name]].collidableLeft = collidableLeft;
		nameToInfo[name].collidableRight = collidableRight;
		idToInfo[nameToID[name]].collidableRight = collidableRight;
	}
	void addLife(std::string name, float val)
	{
		nameToInfo[name].life = val;
		idToInfo[nameToID[name]].life = val;
	}
	void addCraftingStation(std::string name, std::string stationName)
	{
		nameToInfo[name].craftingStation = stationName;
		idToInfo[nameToID[name]].craftingStation = stationName;
	}
	void addDamageParticle(std::string name, std::string particle)
	{
		nameToInfo[name].damageParticle = particle;
		idToInfo[nameToID[name]].damageParticle = particle;
	}
	void addBiome(std::string name, std::string biome)
	{
		nameToInfo[name].biome = biome;
		idToInfo[nameToID[name]].biome = biome;
	}
	void addSkippable(std::string name)
	{
		nameToInfo[name].skippable = true;
		idToInfo[nameToID[name]].skippable = true;
	}
	void addMinPickaxe(std::string name, float val)
	{
		nameToInfo[name].minPickaxeStrength = val;
		idToInfo[nameToID[name]].minPickaxeStrength = val;
	}
	void addParticle(std::string name, particleEmmiterC pec)
	{
		nameToInfo[name].emitsparticle = true;
		idToInfo[nameToID[name]].emitsparticle= true;
		nameToInfo[name].emmiter = pec;
		idToInfo[nameToID[name]].emmiter = pec;
	}
	void addDamagableWith(std::string name, itemFamily fam)
	{
		nameToInfo[name].damagableWith = fam;
		idToInfo[nameToID[name]].damagableWith = fam;
	}
	void animate(std::string name, glm::vec4 frameoffset, int rate, int numframes)
	{
		frameoffset.r /= globals::spritesheetWH.x;
		frameoffset.b /= globals::spritesheetWH.x;
		frameoffset.g /= globals::spritesheetWH.y;
		frameoffset.a /= globals::spritesheetWH.y;
		int id = nameToID[name];
		auto bb = &blockBuffer[id];
		int numBaseSprites = bb->size();
		for (int j = 0; j < numframes; j++) {
			for (int i = numBaseSprites * j; i < numBaseSprites * (j + 1); i++) {
				PremadeBlock tmp = bb->at(i);
				tmp.v0.SpriteCoords += glm::vec2(frameoffset.r, frameoffset.g);
				tmp.v1.SpriteCoords += glm::vec2(frameoffset.b, frameoffset.g);
				tmp.v2.SpriteCoords += glm::vec2(frameoffset.b, frameoffset.a);
				tmp.v3.SpriteCoords += glm::vec2(frameoffset.r, frameoffset.a);
				bb->push_back(tmp);
			}
		}

		nameToInfo[name].animated = true;
		idToInfo[nameToID[name]].animated = true;

		nameToInfo[name].numframes = numframes;
		idToInfo[nameToID[name]].numframes = numframes;

		nameToInfo[name].animrate = rate;
		idToInfo[nameToID[name]].animrate = rate;

		nameToInfo[name].animoffset = numBaseSprites;
		idToInfo[nameToID[name]].animoffset = numBaseSprites;
	}
	void clean()
	{
		nameToInfo.clear();
		idToInfo.clear();
		idToName.clear();
		nameToID.clear();
	}
	int addToBuffer(std::string texture, glm::vec2 size, SpriteType st)
	{
		std::vector<PremadeBlock> vec;
		switch (st) {
			case st_SINGLE:
			{
				PremadeBlock tmp;
				for (int x = 0; x < size.x; x++) {
					for(int y = 0; y < size.y; y++) {
						glm::vec4 sC = textures::nametocoords[texture]->coords;

						float difx = (sC.b - sC.r) * x;
						float dify = (sC.a - sC.g) * y;
						sC.r += difx;
						sC.b += difx;
						sC.g += dify;
						sC.a += dify;

						auto vertices = utils::CreateQuad(0, 0, sC, { 0,0,0 }, textures::nametocoords[texture]->size);
						tmp.v0 = vertices[0];
						tmp.v1 = vertices[1];
						tmp.v2 = vertices[2];
						tmp.v3 = vertices[3];

						vec.push_back(tmp);
					}
				}
			}	break;
			case st_BLOCK:
				PremadeBlock tmp;
				auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords, { 0,0,0 }, textures::nametocoords[texture]->size);
				for (int i = 0; i < 16; i++) {
					glm::vec4 sC = (glm::vec4(blockSc[i].x, blockSc[i].y, blockSc[i].x, blockSc[i].y) * glm::vec4(8.0f)) / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					tmp.v0.SpriteCoords += glm::vec2(sC.r, sC.g);
					tmp.v1.SpriteCoords += glm::vec2(sC.b, sC.g);
					tmp.v2.SpriteCoords += glm::vec2(sC.b, sC.a);
					tmp.v3.SpriteCoords += glm::vec2(sC.r, sC.a);
					
					vec.push_back(tmp);
				}
				break;
			case st_TORCH: {
				PremadeBlock tmp;
				for (int i = 0; i < 4; i++) {
					glm::vec4 sC = torchSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
				break;
			}
				break;
			case st_WALL:
			{
				PremadeBlock tmp;
				for (int i = 0; i < 15; i++) {
					glm::vec4 sC = glm::vec4(wallSc[i].x, wallSc[i].y, wallSc[i].x, wallSc[i].y) / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}

				glm::vec4 sC = glm::vec4(wallSc[15].x, wallSc[15].y, wallSc[15].x, wallSc[15].y) / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
				
				tmp.v0 = vertices[0];
				tmp.v1 = vertices[1];
				tmp.v2 = vertices[2];
				tmp.v3 = vertices[3];

				vec.push_back(tmp);
			}break;
			case st_GRASS: {
				PremadeBlock tmp;
				for (int i = 0; i < 5; i++) {
					glm::vec4 sC = grassSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);

					sC = textures::nametocoords[texture]->coords + sC;
					float dify = (sC.a - sC.g);
					sC.g += dify;
					sC.a += dify;
					vertices = utils::CreateQuad(0, 0, sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}	
			break;
			case st_STALAKTIT: {
				PremadeBlock tmp;
				for (int i = 0; i < 3; i++) {
					glm::vec4 sC = stalaktitSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);

					sC = textures::nametocoords[texture]->coords + sC;
					float dify = (sC.a - sC.g);
					sC.g += dify;
					sC.a += dify;
					vertices = utils::CreateQuad(0, 0, sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}
			break;
			case st_STALAGMIT: {
				PremadeBlock tmp;
				for (int i = 0; i < 3; i++) {
					glm::vec4 sC = stalaktitSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);

					sC = textures::nametocoords[texture]->coords + sC;
					float dify = (sC.a - sC.g);
					sC.g += dify;
					sC.a += dify;
					vertices = utils::CreateQuad(0, 0, sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}
			break;
			case st_MEDIUMROCK: {
				PremadeBlock tmp;
				for (int i = 0; i < 3; i++) {
					glm::vec4 sC = stalaktitSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);

					sC = textures::nametocoords[texture]->coords + sC;
					float difx = (sC.b - sC.r);
					sC.r += difx;
					sC.b += difx;
					vertices = utils::CreateQuad(0, 0, sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}
			break;
			case st_SMALLROCK: {
				PremadeBlock tmp;
				for (int i = 0; i < 6; i++) {
					glm::vec4 sC = smallrockSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}
			break;
			case st_PLATFORM: {
				PremadeBlock tmp;
				for (int i = 0; i < 8; i++) {
					glm::vec4 sC = platformSc[i] / glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
					auto vertices = utils::CreateQuad(0, 0, textures::nametocoords[texture]->coords + sC, { 0,0,0 }, textures::nametocoords[texture]->size);

					tmp.v0 = vertices[0];
					tmp.v1 = vertices[1];
					tmp.v2 = vertices[2];
					tmp.v3 = vertices[3];

					vec.push_back(tmp);
				}
			}
		 break;
		}
		blockBuffer.push_back(vec);
		return blockBuffer.size() - 1;
	}
	void addDecor(std::string block, float chance, std::set<std::string> ontop, std::set<std::string> onbot, std::set<std::string> onleft, std::set<std::string> onright)
	{
		Decor tmp;
		tmp.block = block;
		tmp.chance = chance;
		tmp.onbot = onbot;
		tmp.ontop = ontop;
		tmp.onleft = onleft;
		tmp.onright = onright;
		tmp.size = nameToInfo[block].size;
		decorations.push_back(tmp);
	}
	float potCoinMod(glm::vec2)
	{
		return 1.0f;
	}
}

namespace BRules {
	void breakSelf(BlockRuleArgs)
	{
		Layers::breakBlock(Layers::getLayer(rule->layer), pos, 1, rule->exBool, false, true, true);
	}
	void replaceWith(BlockRuleArgs) {
		Layers::placeBlock(pos, rule->block);
	}
	void growNormalTree(BlockRuleArgs) {
		std::vector<BlockPlaceCondition> tmpCond = { pc_ISNTREPLACABLEBELOW };
		int height = rand() % 5 + 10;
		Layers::placeBlock(pos, "trunkbase1", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x - 1, pos.y }, "trunkbase2", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({pos.x + 1, pos.y}, "trunkbase3", glm::vec3(0.0f), 1, &tmpCond);
		int branchesright = 0;
		int branchesleft = 0;
		for (int y2 = 1; y2 < height; y2++) {
			Layers::placeBlock({pos.x, y2 + pos.y}, "trunk");
			if (y2 > 3 && rand() % 12 == 0) {
				if (rand() % 2 == 0) {
					if (branchesright < 3) {
						Layers::placeBlock({ pos.x - 2, y2 + pos.y }, "normalbranchr");
						branchesright++;
					}
				}
				else {
					if (branchesleft < 3) {
						Layers::placeBlock({ pos.x + 1, y2 + pos.y }, "normalbranchl");
						branchesleft++;
					}
				}
			}
		}
		Layers::placeBlock({ pos.x, pos.y + height }, "leaves", glm::vec3(0.0f), 1.0f);
		Layers::placeBlock({ pos.x, pos.y - 1 }, "dirt");
		Layers::placeBlock({ pos.x + 1, pos.y - 1 }, "dirt");
		Layers::placeBlock({ pos.x - 1, pos.y - 1 }, "dirt");
	}
	void growCorruptTree(BlockRuleArgs) {
		std::vector<BlockPlaceCondition> tmpCond = { pc_ISNTREPLACABLEBELOW };
		int height = rand() % 9 + 10;
		Layers::placeBlock(pos, "corrupttrunkbase1", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x - 1, pos.y }, "corrupttrunkbase2", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x + 1, pos.y }, "corrupttrunkbase3", glm::vec3(0.0f), 1, &tmpCond);
		int branchesright = 0;
		int branchesleft = 0;
		for (int y2 = 1; y2 < height; y2++) {
			Layers::placeBlock({ pos.x, y2 + pos.y }, "corrupttrunk");
			if (y2 > 4 && rand() % 10 == 0) {
				if (rand() % 2 == 0) {
					if (branchesright < 3) {
						Layers::placeBlock({ pos.x - 2, y2 + pos.y }, "corruptbranchr");
						branchesright++;
					}
				}
				else {
					if (branchesleft < 3) {
						Layers::placeBlock({ pos.x + 1, y2 + pos.y }, "corruptbranchl");
						branchesleft++;
					}
				}
			}
		}
		Layers::placeBlock({ pos.x, pos.y + height }, "corruptleaves", glm::vec3(0.0f), 1.0f);
		Layers::placeBlock({ pos.x, pos.y - 1 }, "dirt");
		Layers::placeBlock({ pos.x + 1, pos.y - 1 }, "dirt");
		Layers::placeBlock({ pos.x - 1, pos.y - 1 }, "dirt");
	}

	void growJungleTree(BlockRuleArgs)
	{
		std::vector<BlockPlaceCondition> tmpCond = { pc_ISNTREPLACABLEBELOW };
		int height = rand() % 10 + 8;
		Layers::placeBlock(pos, "mahoganytrunkbase1", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x - 1, pos.y }, "mahoganytrunkbase2", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x + 1, pos.y }, "mahoganytrunkbase3", glm::vec3(0.0f), 1, &tmpCond);
		int branchesright = 0;
		int branchesleft = 0;
		for (int y2 = 1; y2 < height; y2++) {
			Layers::placeBlock({ pos.x, y2 + pos.y }, "mahoganytrunk");
			if (y2 > 4 && rand() % 10 == 0) {
				if (rand() % 2 == 0) {
					if (branchesright < 3) {
						Layers::placeBlock({ pos.x - 3, y2 + pos.y }, "junglebranchr");
						branchesright++;
					}
				}
				else {
					if (branchesleft < 3) {
						Layers::placeBlock({ pos.x + 1, y2 + pos.y }, "junglebranchl");
						branchesleft++;
					}
				}
			}
		}
		Layers::placeBlock({ pos.x, pos.y + height }, "mahoganyleaves", glm::vec3(0.0f), 1.0f);
		Layers::placeBlock({ pos.x, pos.y - 1 }, "mud");
		Layers::placeBlock({ pos.x + 1, pos.y - 1 }, "mud");
		Layers::placeBlock({ pos.x - 1, pos.y - 1 }, "mud");
	}

	void growWinterTree(BlockRuleArgs)
	{
		std::vector<BlockPlaceCondition> tmpCond = { pc_ISNTREPLACABLEBELOW };
		int height = rand() % 4 + 15;
		Layers::placeBlock(pos, "borealtrunkbase1", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x - 1, pos.y }, "borealtrunkbase2", glm::vec3(0.0f), 1, &tmpCond);
		Layers::placeBlock({ pos.x + 1, pos.y }, "borealtrunkbase3", glm::vec3(0.0f), 1, &tmpCond);
		int branchesright = 0;
		int branchesleft = 0;
		for (int y2 = 1; y2 < height; y2++) {
			Layers::placeBlock({ pos.x, y2 + pos.y }, "borealtrunk");
			if (y2 > 4 && rand() % 10 == 0) {
				if (rand() % 2 == 0) {
					if (branchesright < 3) {
						Layers::placeBlock({ pos.x - 3, y2 + pos.y }, "borealbranchr");
						branchesright++;
					}
				}
				else {
					if (branchesleft < 3) {
						Layers::placeBlock({ pos.x + 1, y2 + pos.y }, "borealbranchl");
						branchesleft++;
					}
				}
			}
		}
		Layers::placeBlock({ pos.x, pos.y + height }, "borealleaves", glm::vec3(0.0f), 1.0f);
		Layers::placeBlock({ pos.x, pos.y - 1 }, "snow");
		Layers::placeBlock({ pos.x + 1, pos.y - 1 }, "snow");
		Layers::placeBlock({ pos.x - 1, pos.y - 1 }, "snow");
	}

	void test(BlockRuleArgs)
	{
		static int a = 0;
		std::cout << "test " << a << "\n";
		a++;
	}
	void spawnSand(BlockRuleArgs)
	{
		enemies::spawnEnemy("sand", { pos.x, pos.y });
	}
}
namespace BFuncs {
	bool nothing(BlockFuncArgs)
	{
		return true;
	}
	bool breakSelf(BlockFuncArgs)
	{
		Layers::breakBlock(Layers::getLayer("blocks"), pos, 1, true);
		return false;
	}
	bool toggleDoor(BlockFuncArgs)
	{
		std::string b = *Layers::queryBlockName(Layers::getLayer("blocks"), pos);
		
		std::vector<BlockPlaceCondition> tmp = { pc_ISREPLACABLE };
		if (b == "door") {
			if (Player::dir > 0) {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock(pos, "doorright", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "door");
				}
			}
			else {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock({ pos.x - 1, pos.y }, "doorleft", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "door");
				}
			}
			return false;
		}
		else if (b == "doorright") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock(pos, "door", { 0,0,0 }, 1, &tmp);
			return false;
		}
		else if (b == "doorleft") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock({ pos.x + 1, pos.y }, "door", { 0,0,0 }, 1, &tmp);
			return false;
		}

		if (b == "borealdoor") {
			if (Player::dir > 0) {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock(pos, "borealdoorright", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "borealdoor");
				}
			}
			else {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock({ pos.x - 1, pos.y }, "borealdoorleft", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "borealdoor");
				}
			}
			return false;
		}
		else if (b == "borealdoorright") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock(pos, "borealdoor", { 0,0,0 }, 1, &tmp);
			return false;
		}
		else if (b == "borealdoorleft") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock({ pos.x + 1, pos.y }, "borealdoor", { 0,0,0 }, 1, &tmp);
			return false;
		}

		if (b == "mahoganydoor") {
			if (Player::dir > 0) {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock(pos, "mahoganydoorright", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "mahoganydoor");
				}
			}
			else {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock({ pos.x - 1, pos.y }, "mahoganydoorleft", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "mahoganydoor");
				}
			}
			return false;
		}
		else if (b == "mahoganydoorright") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock(pos, "mahoganydoor", { 0,0,0 }, 1, &tmp);
			return false;
		}
		else if (b == "mahoganydoorleft") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock({ pos.x + 1, pos.y }, "mahoganydoor", { 0,0,0 }, 1, &tmp);
			return false;
		}

		if (b == "ebonwooddoor") {
			if (Player::dir > 0) {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock(pos, "ebonwooddoorright", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "ebonwooddoor");
				}
			}
			else {
				Layers::breakBlock(Layers::getLayer("blocks"), pos);
				if (!Layers::placeBlock({ pos.x - 1, pos.y }, "ebonwooddoorleft", { 0,0,0 }, 1, &tmp)) {
					Layers::placeBlock(pos, "ebonwooddoor");
				}
			}
			return false;
		}
		else if (b == "ebonwooddoorright") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock(pos, "ebonwooddoor", { 0,0,0 }, 1, &tmp);
			return false;
		}
		else if (b == "ebonwooddoorleft") {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			Layers::placeBlock({ pos.x + 1, pos.y }, "ebonwooddoor", { 0,0,0 }, 1, &tmp);
			return false;
		}

		return false;
	}
	bool chestFunction(BlockFuncArgs)
	{
		UI::openInventory();
		UI::chest->setTo(&Layers::chests[Layers::vecToInt(pos)].items);
		UI::openChest = Layers::vecToInt(pos);
		return false;
	}
	bool chestOnPlace(BlockFuncArgs)
	{
		Layers::chests[Layers::vecToInt(pos)] = { {}, "Chest" };
		Inventory::setupInventory(&Layers::chests[Layers::vecToInt(pos)].items, {10, 4});
		return true;
	}
	bool chestOnBreak(BlockFuncArgs)
	{
		int c = Layers::vecToInt(pos);
		if (Layers::chests.count(c) >= 1) {
			for (auto& i : Layers::chests[c].items) {
				for (int j = 0; j < i->num; j++) {
					DroppedItems::dropItem(pos.x, pos.y, i->item, i->num);
				}
			}
			Layers::chests.erase(c);
		}
		if (UI::openChest == c) {
			UI::chest->toggle(1);
			UI::chest->setTo(&UI::defaultChest);
			UI::openChest = -1;
		}
		return false;
	}
	bool potOnBreak(BlockFuncArgs)
	{

		for (auto& i: items::potItems) {
			if (rand() % int(1 / i.second.chance) == 0) {
				int randAmount = ((rand() % 100) / 50.0f - 1) * i.second.random;
				DroppedItems::dropItem(pos.x, pos.y, i.first, i.second.num + randAmount);
				return false;
			}
		}

		int coins = (rand() % 1000) / 1000.0f * 280 + 80;
		coins *= blocks::potCoinMod(pos);

		int numPlatinum = coins / 1000000;
		coins %= 1000000;
		int numGold = coins / 10000;
		coins %= 10000;
		int numSilver = coins / 100;
		coins %= 100;
		for (int i = 0; i < numPlatinum; i++) {
			DroppedItems::dropItem(pos.x, pos.y, "platinumcoin", 1);
		}
		for (int i = 0; i < numGold; i++) {
			DroppedItems::dropItem(pos.x, pos.y, "goldcoin", 1);
		}
		for (int i = 0; i < numSilver; i++) {
			DroppedItems::dropItem(pos.x, pos.y, "silvercoin", 1);
		}
		for (int i = 0; i < coins; i++) {
			DroppedItems::dropItem(pos.x, pos.y, "coppercoin", 1);
		}

		return false;
	}
	bool craftingStationOnUpdate(BlockFuncArgs)
	{
		if (glm::distance(Player::pos, pos) <= Player::craftingDist) {
			UI::craftingStationsInRange.insert(blocks::nameToInfo[*Layers::queryBlockName(Layers::getLayer("blocks"), pos)].craftingStation);
		}
		return true;
	}
	bool chairOnPlace(BlockFuncArgs)
	{
		std::string b = *Layers::queryBlockName(Layers::getLayer("blocks"), pos);
		if (Player::dir < 0) {
			Layers::breakBlock(Layers::getLayer("blocks"), pos);
			if (b == "chairright") {
				Layers::placeBlock(pos, "chairleft");
			}
			else if (b == "ebonwoodchairright") {
				Layers::placeBlock(pos, "ebonwoodchairleft");
			}
			else if (b == "borealchairright") {
				Layers::placeBlock(pos, "borealchairleft");
			}
			else if (b == "mahoganychairright") {
				Layers::placeBlock(pos, "mahoganychairleft");
			}
		}
		return false;
	}
	bool saplingOnUpdate(BlockFuncArgs)
	{
		if (rand() % 100 != 0) return true;
		auto bs = Layers::getLayer("blocks");
		auto block = Layers::queryBlockName(bs, pos);
		Layers::breakBlock(bs, pos, 1, false, true);
		Layers::breakBlock(bs, pos + glm::vec2(0, 1));
		if (*block == "sapling") {
			BRules::growNormalTree(nullptr, pos);
		}
		else if (*block == "corruptsapling") {
			BRules::growCorruptTree(nullptr, pos);
		}
		else if (*block == "mahoganysapling") {
			BRules::growJungleTree(nullptr, pos);
		}
		else if (*block == "borealsapling") {
			BRules::growWinterTree(nullptr, pos);
		}
		return false;
	}
}
