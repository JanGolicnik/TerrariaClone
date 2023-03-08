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

    extern bool ignorebot;

    extern int anim;

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

    extern std::array<ActiveBuff, 20> buffs;
    extern int manaregentimer;
    extern float regeneration;

    extern float enemyChance;
    extern float critterChance;

    extern float meeleDamage;
    extern float thorns;

    void create();
    void update();
    void render();
    void clean();
    void calculateStats();
    bool save();
    bool load();
    void jump();
    void stop();

    void useItem(std::string item, itemInfo* info);

    void setPos(glm::vec2 val);
    void setVel(glm::vec2 val);
    glm::vec2* position();
    glm::vec2* velocity();

    void addStatsFromItem(std::string item);

    void addBuff(std::string name, int timer);
    bool hasBuff(std::string name);

    int pickUp(std::string  item, int num);
};

