#pragma once

#include <core.h>
#include <Layers.h>

namespace Player
{
    extern std::string name;

    extern glm::vec2 pos;
    extern glm::vec2 vel;
    extern glm::vec2 tvel;
    extern float ms;
    extern float jumpms;
    extern int dir;
    extern glm::vec2 center;

    extern float pickupRange;
    extern float editRange;
    extern int editsize;
    extern int invBlock;
    
    extern bool leftClick;
    extern bool rightClick;

    extern float width;
    extern float height;

    extern const int numsprites;
    extern GLuint playerVB;
    extern GLuint playerVA;
    extern GLuint playerIB;

    extern bool wantstoskip;

    extern float craftingDist;

    extern float hp;
    extern float currmaxhp;
    extern float maxhp;
    extern int defense;

    extern float mana;
    extern float currmaxmana;
    extern float maxmana;

    extern int cursoritem;

    extern int inventoryC;
    extern int chestInvC;
    extern int crafting;

    extern int entity;

    extern int jumpDuration;
    extern float jumpSpeed;
    extern int currJumpTime;

    extern bool friction;

    extern int iframes;

    extern int regentimer;

    extern int boomerangsout;

    extern int tool;
    
    extern float maxsummons;
    extern float currsummons;

    extern int heartcrystals;
    extern int manacrystals;

    extern std::array<ActiveBuff, 20> buffs;
    extern int manaregentimer;
    extern float regeneration;

    extern float enemyChance;
    extern bool spawnCap;

    extern float meeleDamage;
    extern float thorns;

    extern std::string curritem;

    extern std::string activeSetBonus;

    extern float breathtimer;

    extern bool dead;

    extern std::string timeToRespawn;

    extern int hairid;

    extern std::string headarmor;
    extern std::string bodyarmor;
    extern std::string legsarmor;

    extern hsv hairclr_hsv;
    extern hsv shirtclr_hsv;
    extern hsv skinclr_hsv;
    extern hsv pantsclr_hsv;
    extern hsv eyeclr_hsv;
    extern hsv shoeclr_hsv;

    extern bool isinwater;

    void create();
    void update();
    void render();
    void clean();
    void calculateStats();
    bool save();
    bool load();
    playerData loadToData(std::string name);
    void jump();
    void stop();

    void useItem(std::string item, itemInfo* info);

    void setPos(glm::vec2 val);
    void setVel(glm::vec2 val);
    glm::vec2* position();
    glm::vec2* velocity();

    void doPrimary(std::string item);
    void doSecondary(std::string item);
    void doPrimaryHold(std::string item);

    void addStatsFromItem(std::string item);

    void addBuff(std::string name, int timer);
    bool hasBuff(std::string name);

    int pickUp(std::string  item, int num);

    void dropCoins();
    
    void setupBodyparts();

    void renderAs(glm::vec2 pos, float size, playerData* data);

    void cycleHair(uiC* p);

    bool hasEnoughCoins(int price);
    bool takeCoinsIfEnough(int price);
};

