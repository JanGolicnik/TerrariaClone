#pragma once

#include <core.h>
#include <globals.h>
#include <enemyFunctions.h>
#include <componentsystems.h>

enum itemFamily {
    if_ANY = 0,
    if_NONE = 1,
    if_HEADGEAR,
    if_BODY,
    if_MEELE,
    if_RANGED,
    if_YOYO,
    if_ACCESORY,
    if_BLOCK,
    if_PICKAXE,
    if_BOW,
    if_GUN,
    if_SPELL,
    if_PROJECTILE,
    if_THROWABLE,
    if_ARROW,
    if_MISC,
    if_BULLET,
    if_WEAPON,
    if_AXE,
    if_ORE,
    if_BAR,
    if_HAMMER,
    if_GREAVES,
    if_HELMET,
    if_BREASTPLATE,
    if_MATERIAL,
    if_MAGIC,
    if_SUMMON,
};

enum itemMod {
    im_CRAZY,
    im_TRASH
};

struct recipeItem {
    const char* item;
    int num;
};

struct recipe {
    std::vector<recipeItem> items;
    int craftedAmount;
    const char* craftedItem;
    std::set<std::string> craftingStations;
};

union itemStat {
    int valueInt;
    float valueFloat;
    bool valueBool;
    glm::vec2 valueVec2;
    glm::vec3 valueVec3;
};

enum Rarity {
    r_GRAY = 0,
    r_WHITE = 1,
    r_BLUE = 2,
    r_GREEN = 3,
    r_ORANGE = 4,
    r_LIGHTED = 5,
    r_PINK = 6,
    r_LIGHTPURPLE = 7,
    r_LIME = 8,
    r_YELLOW = 9,
    r_CYAN = 10,
    r_RED = 11,
    r_PURPLE = 12
};

#define itemconditionargs std::string item
#define args std::string, int
struct itemInfo {
    std::string tex;
    std::string displayName;
    std::vector<std::function<bool(itemconditionargs)>> conditions;
    std::vector<std::function<void(args)>> onleftclick;
    std::set<itemFamily> families;
    std::unordered_map<std::string, itemStat> stats;
    std::unordered_map<std::string, std::string> textStats;
    std::vector<int> fromRecipes;
    std::vector<int> inRecipes;
    float useSpeed;
    itemAnim animType;
    float sizeMod;
    int maxStacks = 99;
    std::string anim = "";
    std::function<void()>onpickup;
    glm::vec3 light = glm::vec3(0);
    bool autouse = true;
    particleEmmiterC emmiter;
    std::function <void(mobCargs)> onhit = collisionFs::damage;
    Rarity rarity = r_WHITE;
    std::string set = "none";
    std::string description = "";
    void(*soundsfunc)() = &sounds::swing;
    float buyprice = 10;
    float sellprice = 0;
};

struct itemChance {
    float chance;
    int num;
    int random;
};
struct naturalChest {
    std::unordered_map<std::string, itemChance> items;
};

struct setBonus {
    std::function<void()> func;
    std::string description = "Click to edit text";
};

namespace itemFuncs {
    void nothing(args);
    void testLeft(args);
    void testRight(args);
    void place(args);
    void placeSapling(args);
    void dig(args);
    void shootArrow(args);
    void shootBoomerang(args);
    void throwSelf(args);
    void shootBullet(args);
    void removeAmmo(args);
    void coppercoinonpickup();
    void goldcoinonpickup();
    void silvercoinonpickup();
    void teleportToSpawn(args);
    void summonEye(args);
    void takeMana(args);
    void starfuryStar(args);
    void terraBlade(args);
    void vilethorn(args);
    void beegun(args);
    void opticStaff(args);
    void impStaff(args);
    void increaseMaxHealth(args);
    void heal(args);
    void bladeOfGrass(args);
    void manaCrystal(args);

    void nonearmorbonus();
    void ironarmorbonus();
    void silverarmorbonus();
    void goldarmorbonus();
    void copperarmorbonus();
    void moltenarmorbonus();
    void cactusarmorbonus();
}

namespace itemConditions {
    bool nothing(itemconditionargs);
    bool hasAmmo(itemconditionargs);
    bool hasMana(itemconditionargs);
    bool boomerang(itemconditionargs);
    bool lightdisc(itemconditionargs);
    bool hasSummonSpace(itemconditionargs);
    bool heartCrystal(itemconditionargs);
    bool manaCrystal(itemconditionargs);
    bool heal(itemconditionargs);
}

namespace items
{
    extern std::unordered_map<std::string, itemInfo> info;
    extern std::vector<recipe> recipes;
    extern std::unordered_map<std::string, projectileBase> projectiles;
    extern std::unordered_map<std::string, naturalChest> naturalChests;
    extern std::unordered_map<std::string, itemChance> potItems;
    extern std::unordered_map<std::string, setBonus> setBonuses;
    
    extern std::unordered_map<std::string, std::array<glm::vec4, 13>> armorsprites;
    
    void addItem(std::string itemName, std::string displayName,  std::string tex,
        std::vector<std::function<void(args)>> onleftclick = {}, float useSpeed = 60, std::set<itemFamily> families = {},
        std::unordered_map<std::string, itemStat> stats = {}, std::vector<std::function<bool(itemconditionargs)>> conditions = {itemConditions::nothing}, itemAnim animType = ia_DEFAULT, float sizeMod = 1.0f);
    void addRecipe(recipe rec);
    void addOnPickup(std::string item, std::function<void()> func);
    void addDroppedAnim(std::string item, std::string animName);
    void addStatI(const char* item, const char* statname, int value);
    void addStatF(const char* item, const char* statname, float value);
    void addStatB(const char* item, const char* statname, bool value);
    void addStatT(const char* item, const char* statname, std::string value);
    void addStatV2(const char* item, const char* statname, glm::vec2 value);
    void addStatV3(const char* item, const char* statname, glm::vec3 value);
    void addFamily(const char* item, itemFamily fam);
    void addLight(const char* item, glm::vec3 light);
    void addSound(const char* item, void(*func)());
    void addSizeMod(const char* item, float sizemod);
    void disableAutouse(const char* item);
    void addEmmiter(const char* item, particleEmmiterC pec);
    void addOnhit(const char* item, std::function<void(mobCargs)> func);
    void addRarity(const char* item, Rarity rarity);
    void addToSet(const char* item, std::string set);
    void addSet(std::string item, std::function<void()> func, std::string description);
    void printItems();
    glm::vec3 getRarityColor(Rarity rarity);
    std::string getRarityColorString(Rarity rarity);
    itemInfo* getInfo(std::string item);
    itemStat getStat(std::string item, const char* stat, itemStat def = { .valueFloat = 0.0f });
    std::string_view getStatT(std::string item, const char* stat, const char* def = "");
    void addItemToNaturalChest(std::string chestName, std::string item, float chance, int num, int random);
    void addItemToPot(std::string item, float chance, int num, int random);
    void clean();
    void addArmorSprites(std::string name, glm::vec4 coords, glm::vec4 legcoords);

};
#undef args