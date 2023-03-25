#pragma once

#include <core.h>
#include <fstream>
#include <blocks.h>
#include <items.h>

struct Block {
	int id;
	glm::vec3 light;
	int state;
	float breaking;
	int timealive = 0;
};

struct Layer {
	std::string mname;
	Block* mblocks;
	float mdarkness;
	int mdrawingOffset;
};

class blockRelationship {
public:
	Layer* l;
	std::string layerName;
	std::unordered_map<int, glm::vec2> subs;
	std::unordered_map<int, std::set<int>> doms;
	blockRelationship(std::string LAYERNAME, std::unordered_map<int, glm::vec2> SUBS = {}, std::unordered_map<int, std::set<int>> DOMS = {});
	blockRelationship();
	void addSub(glm::vec2 pos, glm::vec2 dom, bool removeprev = true);
	void addDom(glm::vec2 pos, std::set<int> subs, bool removeprev = true);
	void clearRelationships(glm::vec2 pos, bool Break = true);
	void breakRelationships(glm::vec2 pos);
	void clearSubs(glm::vec2 pos, bool Break = true);
	void clearDoms(glm::vec2 pos, bool Break = true, bool breakdom = false);
	bool isSub(glm::vec2 pos);
	bool isDom(glm::vec2 pos);
	void save(std::ofstream* file);
	void load(std::ifstream* file);
};

struct Chest {
	std::vector<std::shared_ptr<InventoryItem>> items;
	std::string name;
};

struct Biome {
	int numNeededBlocks;
	std::string bg;
	std::pair<int, int> dayspawns = std::make_pair(600, 5);
	std::pair<int, int> nightspawns = std::make_pair(600, 5);
	std::pair<int, int> undergroundspawns = std::make_pair(600, 5);
	std::pair<int, int> cavernspawns = std::make_pair(600, 5);
	std::pair<int, int> underworldspawns = std::make_pair(600, 5);
	std::unordered_map<std::string, float> daymobs;
	std::unordered_map<std::string, float> nightmobs;
	std::unordered_map<std::string, float> undergroundmobs;
	std::unordered_map<std::string, float> cavernmobs;
	std::unordered_map<std::string, float> underworldmobs;
};

namespace Layers
{
	extern BlockVertex* drawnVertices;

	extern glm::vec2 blocksOnScreen;
	extern int nQuads;
	extern int nDrawnIndices;
	extern glm::vec2 trueBsOnScr;

	extern int nDrawnBlocks;

	extern std::vector<Layer> layers;

	extern blockRelationship* childParent;

	extern std::string currentBiome;
	extern std::set<std::string> allBiomes;
	extern GLuint layersVA;
	extern GLuint layersVB;

	extern std::unordered_map<std::string, Biome> biomes;
	extern std::unordered_map<std::string, int> biomeCounter;

	extern std::unordered_map<int, Chest> chests;

	extern std::unordered_map<int, glm::vec3> lights;

	void init();
	void setUp();
	void clean();
	void reschanged();

	void addLayer(const char* name, float darkness, int drawingOffset = 0.0f);

	Layer* getLayer(std::string_view l);

	void fastPlaceBlock(glm::vec2 pos, std::string type, Layer* layer = nullptr, int size = 1);
	bool placeBlock(Layer* l, glm::vec2 pos, std::string type, int size = 1, std::vector<std::function<bool(BlockConditionArgs)>>* conditions = nullptr, glm::vec3 setlight = glm::vec3(0.0f), bool update = true);
	bool placeBlock(glm::vec2 pos, std::string type, int size = 1, std::vector<std::function<bool(BlockConditionArgs)>>* conditions = nullptr, glm::vec3 setlight = glm::vec3(0.0f));
	void breakBlock(Layer* l, glm::vec2 pos, int size = 1, bool dropitem = false, bool checkforchildren = false, bool execute = true, bool particles = false);
	const std::string* queryBlockName(Layer* l, glm::vec2 pos, bool checkForChildren = true);
	Block* queryBlock(Layer* l, glm::vec2 pos, bool checkForChildren = true);
	Block* fastQueryBlock(Layer* l, glm::vec2 pos);
	blocks::BlockInfo* queryBlockInfo(Layer* l, glm::vec2 pos, bool checkForChildren = true);
	blocks::BlockInfo* queryBlockInfo(Layer* l, uint32_t c);
	blocks::BlockInfo* fastQueryBlockInfo(Layer* l, glm::vec2 pos);
	bool canLiquidGoThru(Layer* l, glm::vec2 pos);
	bool canLiquidGoThru(Layer* l, int c);
	void moveBlockTo(Layer* l, glm::vec2 from, glm::vec2 to);
	void swapBlocks(Layer* l, glm::vec2 from, glm::vec2 to);

	glm::vec3 getLight(glm::vec2 pos, glm::vec3 iffail = glm::vec3(1));
	void setLight(Layer* l, glm::vec2 pos, glm::vec3 light);
	void fastSetLight(Layer* l, glm::vec2 pos, glm::vec3 light);
	void addLight(glm::vec2 pos, glm::vec3 light);
	void keepLightAbove(Layer* l, glm::vec2 pos, glm::vec3 light);

	void updateOnScreen();
	void updateBlock(Layer* l, glm::vec2 pos);
	bool evaluateCondition(BlockRuleCond* cond, glm::vec2 pos);
	bool damageBlock(Layer* l, glm::vec2 pos, float strength, int size = 1, bool dropitem = true, itemFamily fam = if_ANY);

	void renderLights();
	void renderLayers();

	void autoSprite(Layer* l, glm::vec2 pos);

	int vecToInt(glm::vec2 pos);
	glm::vec2 intToVec(int pos);

	bool verifyBlock(Layer* l, glm::vec2* pos, bool checkForChildren = true);
		
	bool doBlockFunction(glm::vec2 pos);

	void clearRelationships(Layer* l, glm::vec2 pos, bool Break = true);

	void calculateLight();

	void saveChests(std::ofstream* file);
	void loadChests(std::ifstream* file);

	bool isAreaEmpty(glm::vec2 pos, glm::vec2 size);
	void spawnMobs();
	glm::vec2 findEmptySpot();

	void spawnParticlesFromBlock(glm::vec2 pos, blocks::BlockInfo* info);

	bool canDamage(Layer* l, glm::vec2 pos);

};

