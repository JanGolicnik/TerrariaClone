#pragma once
#include <core.h>
#include <componentsystems.h>
#include <items.h>

#define BlockFuncArgs glm::vec2 pos, blocks::BlockInfo* info
#define BlockConditionArgs Layer* l, Layer* bg, Layer* bs, glm::vec2 pos, blocks::BlockInfo* info

struct Layer;

namespace blocks {
	struct PremadeBlock {
		BlockVertex v0;
		BlockVertex v1;
		BlockVertex v2;
		BlockVertex v3;
	};

	struct Decor {
		std::string block;
		float chance;
		std::set<std::string> ontop;
		std::set<std::string> onright;
		std::set<std::string> onleft;
		std::set<std::string> onbot;
		glm::vec2 size;
	};

	struct BlockInfo {
		float life;
		bool collidableTop;
		bool collidableBot;
		bool collidableLeft;
		bool collidableRight;
		bool notReplacable;
		bool updates;
		bool updatesBot;
		bool updatesTop;
		bool updatesLeft;
		bool updatesRight;
		bool emitskylight = false;
		float friction;
		std::string layer;
		float minPickaxeStrength = 30;
		std::string damageParticle = "";
		SpriteType spriteType;
		glm::vec2 size;
		glm::vec3 light = glm::vec3(0.0f);
		std::string biome;
		std::string drops;
		std::string texture;
		std::set<std::string> craftingStations;
		std::vector<BlockRule> rules;
		std::vector<std::function<bool(BlockConditionArgs)>> conditions;
		std::function<bool(BlockFuncArgs)> function;
		std::function<bool(BlockFuncArgs)> onPlace;
		std::function<bool(BlockFuncArgs)> onBreak;
		std::function<bool(BlockFuncArgs)> onUpdate;
		bool skippable = false;
		bool animated = false;
		int numframes = 0;
		int animrate = 0;
		int animoffset = 0;
		bool emitsparticle = false;
		particleEmmiterC emmiter;
		itemFamily damagableWith = if_PICKAXE;
		bool canBreakBelow = true;
		std::string buff = "nothing";
		bool canpassliquid = false;
	};

}

namespace BFuncs {
	bool nothing(BlockFuncArgs);
	bool breakSelf(BlockFuncArgs);
	bool toggleDoor(BlockFuncArgs);
	bool chestFunction(BlockFuncArgs);
	bool chestOnPlace(BlockFuncArgs);
	bool chestOnBreak(BlockFuncArgs);
	bool potOnBreak(BlockFuncArgs);
	bool craftingStationOnUpdate(BlockFuncArgs);
	bool chairOnPlace(BlockFuncArgs);
	bool saplingOnUpdate(BlockFuncArgs);
	bool buffBlockOnUpdate(BlockFuncArgs);
	bool shadoworbOnBreak(BlockFuncArgs);
	bool hellstoneOnBreak(BlockFuncArgs);
}

namespace BRules {
	void breakSelf(BlockRuleArgs);
	void breakSelfAndUpdateBelow(BlockRuleArgs);
	void replaceWith(BlockRuleArgs);
	void growNormalTree(BlockRuleArgs);
	void growCorruptTree(BlockRuleArgs);
	void growJungleTree(BlockRuleArgs);
	void growWinterTree(BlockRuleArgs);
	void test(BlockRuleArgs);
	void spawnSand(BlockRuleArgs);
}

namespace BConditions {
	bool connected(BlockConditionArgs);
	bool isreplacable(BlockConditionArgs);
	bool haswall(BlockConditionArgs);
	bool isntreplacablebelow(BlockConditionArgs);
	bool nowall(BlockConditionArgs);
	bool solidontop(BlockConditionArgs);
	bool isntempty(BlockConditionArgs);
	bool issolidbelow(BlockConditionArgs);
	bool bottle(BlockConditionArgs);
}


namespace blocks
{
	
	extern std::unordered_map<std::string, BlockInfo> nameToInfo;
	extern std::unordered_map<int, BlockInfo> idToInfo;
	extern std::unordered_map<int, std::string> idToName;
	extern std::unordered_map<std::string, int> nameToID;

	extern std::vector<std::vector<PremadeBlock>> blockBuffer;
	extern std::vector<Decor> decorations;

	void addBlock(std::string name, std::string texture, bool collidable,
		bool solid, std::string itemname, std::string layer, bool updates = true,
		SpriteType spriteType = st_SINGLE, glm::vec2 size = glm::vec2(1.0f),
		float friction = 1,
		std::vector<std::function<bool(BlockConditionArgs)>> placeConditions = { BConditions::isreplacable, BConditions::connected },
		bool updateLeft = true, bool updateTop = true, bool updateBot = true, bool updateRight = true);
	void addRule(std::string name, std::vector<BlockRuleCond> conditions, std::function<void(BlockRuleArgs)> func, const char* block = "empty", const char* layer = "empty", bool exBool = false);
	void addFunction(std::string name, std::function<bool(BlockFuncArgs)> func);
	void addOnPlace(std::string name, std::function<bool(BlockFuncArgs)> func);
	void addOnBreak(std::string name, std::function<bool(BlockFuncArgs)> func);
	void addOnUpdate(std::string name, std::function<bool(BlockFuncArgs)> func);
	void modifyCollidable(std::string name, bool collidableTop, bool collidableBot, bool collidableLeft, bool collidableRight);
	void addLife(std::string name, float val);
	void addCraftingStation(std::string name, std::string stationName);
	void addDamageParticle(std::string name, std::string particle);
	void addBiome(std::string name, std::string biome);
	void addSkippable(std::string name);
	void addMinPickaxe(std::string name, float val);
	void addParticle(std::string name, particleEmmiterC pec);
	void addDamagableWith(std::string name, itemFamily fam);
	void addBuff(std::string name, std::string buff);
	void canPassLiquid(std::string name);
	void emitSkyLight(std::string name);
	void makeUnbreakableBelow(std::string name);
	void animate(std::string name, glm::vec4 frameoffset, int rate, int numframes);
	void clean();
	int addToBuffer(std::string texture, glm::vec2 size, SpriteType st);
	void addDecor(std::string block, float chance, std::set<std::string> ontop = {}, std::set<std::string> onbot = {}, std::set<std::string> onleft = {}, std::set<std::string> onright = {});
	float potCoinMod(glm::vec2);
};
