#pragma once

#include <ECS/ECS.h>
#include <core.h>
#include <functional>
#include <globals.h>
#include <sounds.h>

struct physicsC {
    std::shared_ptr<glm::vec2> position;
    glm::vec2 vel = { 0,0 };
    glm::vec2 size = { 0,0 };
    bool isstatic = false;
    bool dontcollide = false;
    bool stoponcollision = true;
    bool collideleft = false;
    bool collideright = false;
    bool collidebot = false;
    bool collidetop = false;
    bool bounce = false;
    bool friction = true;
    float weight = 1;
    glm::vec2 prevpos = glm::vec2(0);
    float bounciness = 0;
    bool ignorebot = false;
    bool ignoretop = false;
    bool ignoreleft = false;
    bool ignoreright = false;
    bool wantstoskip = false;
    bool affectedByLiquid = true;
    int isinliquid = -1;
};

struct drawC {
    std::shared_ptr<glm::vec2> position;
    std::string tex = "empty";
    glm::mat4 mat= glm::mat4(1.0f);
    glm::vec2 size = { 0,0 };
    bool hidden = false;
    int parent = -1;
    std::vector<int> children;
    int anim = 0;
    bool autolight = true;
    bool hasmat = false;
    bool flipX = false;
    bool flipY = false;
    std::string text = "";
    float textScale = globals::fontSize;
    glm::vec2 textoffset = glm::vec2(0);
    glm::vec3 color = glm::vec3(1,1,1);
    float opacity = 1;
    glm::vec4 spriteCoords = glm::vec4(-1, -1, -1, -1);
    bool autocorrect = true;
};

#define adargs aiC*, int, physicsC*, ComponentArray<aiC>*, aiAction*, int 
struct aiC;

struct aiAction {
    int timer;
    std::function<void(adargs)> func;
    int repeat;
    int lastUpdate = 0;
};

union aiStat {
    int intVal;
    float floatVal;
    bool boolVal = 0;
    int* intValp;
    float* floatValp;
    bool* boolValp;
    void* voidp;
    glm::vec2 vec2Val;
    glm::vec3 vec3Val;
    const char* charp;
};

struct aiC {
    AI ai;
    std::vector<aiAction> actions;
    std::function<void(adargs)> onupdate;
    std::function<void(adargs)> onclick;
    std::unordered_map<std::string, aiStat> stats;
    int state = 0;
};
#undef adargs

enum mobFam {
    mf_ENEMY,
    mf_NEUTRAL,
    mf_CRITTER,
    mf_PLAYERPROJECTILE,
    mf_WEAPON,
    mf_SUMMON
};


struct ActiveBuff {
    std::string name;
    int countdown;
    unsigned long long starttime;
};

#define mobCargs mobC* p1, mobC* p2, physicsC* phys1, physicsC* phys2, int p1_i, int p2_i, ComponentArray<mobC>* arr
#define mobCWPargs mobC* p, physicsC* phys, int i, ComponentArray<mobC>* arr
#define mobDeathargs mobC* p, physicsC* phys, int ent
struct mobC {
    int hp = 1;
    int maxhp;
    int damage;
    std::set<mobFam> families;
    std::unordered_map<mobFam, std::function<void(mobCargs)> > onCollision;
    std::function<void(mobCWPargs)> onPlayerCollision;
    std::function<void(mobDeathargs)> onDeath;
    int iframes = 0;
    float knockback = 0;
    float kbResist = 1;
    glm::vec3 light = glm::vec3(0);
    int killin = -1;
    int hpbar = -1;
    int defense = 0;
    bool destroydecor = false;
    std::string displayName = "";
    std::unordered_map<std::string, int> drops;
    std::vector<int>children;
    int despawntimer = -1;
    bool candespawn = true;
    float hitboxradius = 0;
    std::vector<ActiveBuff> buffs;
    int directDamageTo = -1;
    bool damageInLava = true;
    void(*damageSound)() = sounds::normalhit;
    void(*deathSound)() = sounds::normaldie;
    std::string gore = "";
    int slots = 1;
};

enum itemAnim {
    ia_DEFAULT,
    ia_STAB,
    ia_USE,
    ia_NONE
};

struct toolC {
    float frames;
    float maxframes;
    bool neki;
    itemAnim animType;
};

struct droppedItemC {
    std::string name;
    int amount;
    int timer = 60;
    bool rotate = true;
};

enum uiAnchor {
    anchorLEFT,
    anchorTOPLEFT,
    anchorTOP,
    anchorTOPRIGHT,
    anchorRIGHT,
    anchorBOTRIGHT,
    anchorBOT,
    anchorBOTLEFT,
    anchorMID,
    anchorNONE
};

struct uiC;

union uiStat {
    int intVal;
    float floatVal;
    bool boolVal = 0;
    int* intValp;
    unsigned int* uintValp;
    float* floatValp;
    bool* boolValp;
    void* voidp;
    InventoryItem* itemp;
    std::string* stringp;
    void(*funcp)(uiC*);
    glm::vec3* vec3p;
    ActiveBuff* buffp;
};

#define uiCfunctionArguments uiC*, drawC*, int, ComponentArray<uiC>*
struct uiC {
    bool hidden;
    glm::vec2 pos;
    glm::vec2 size;
    int parent = 0;
    UIFunction func;
    bool interactable = true;
    bool propagateClick = true;
    uiAnchor anchor = anchorLEFT;
    std::vector<int> children = {};
    std::unordered_map<std::string, uiStat> stats;
    std::unordered_map<std::string, std::string> textStats;
    std::function<void(uiCfunctionArguments)> onclick;
    std::function<void(uiCfunctionArguments)> onhover;
    std::function<void(uiCfunctionArguments)> onrender;
    std::function<void(uiCfunctionArguments)> onnothover;
    std::function<void(uiCfunctionArguments)> onrightclick;
    std::function<void(uiCfunctionArguments)> onhold;
    std::function<void(uiCfunctionArguments)> onrighthold;
    std::function<void(uiCfunctionArguments)> onenter;
    bool removed = false;
    int holdingtime = 0;
    bool hovering = false;
};

struct textC {
    std::string text;
    glm::vec2 pos;
    float size;
    bool hidden;
    glm::vec3 color = glm::vec3(1,1,1);
};

struct particleEmmiterC {
    float rate = 0;
    float countdown = 0;
    std::string tex = "err";
    float amount = 0;
    float ms = 0;
    glm::vec2 dir = { 0,0 };
    glm::vec2 size = { 1,1 };
    glm::vec2 velovertime = glm::vec2(0);
    int lifespan = 0;
    int particleLifespan = 0;
    float rotation = 0;
    float randrotation = 0;
    float sizeovertime = 0;
    float opacityovertime = 0;
    float randangle = 0;
    float randomizems = 0;
    float randomizeamount = 0;
    float randomizerate= 0;
    float randomizelifespan = 0;
    float randomizescale= 0;
    bool stoponcollision = true;
    bool smallerAsDie = false;
    bool disappearAsDie = false;
    float weight = 0;
    bool killentity = false;
    float opacity = 1;
    bool stopAsDie = false;
    bool setup = true;
    glm::vec3 light = glm::vec3(0);
    float radius = 0;
    float randradius = 0;
    float speedmultiplier = 0;
    int parent = -1;
    glm::vec3 color = glm::vec3(1);
    bool matchparticledir = false;
};

struct projectileBase {
    physicsC pc;
    mobC mc;
    aiC ac;
    drawC dc;
    particleEmmiterC pec;
};

class physicsSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    physicsSystem() {
        componentArray = std::make_shared<ComponentArray<physicsC>>();
    }
};

class aiSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    aiStat* getStat(aiC* p, std::string statName, aiStat def = { .floatVal = 0 });
    aiStat* getStatFast(aiC* p, std::string statName);
    aiSystem() {
        componentArray = std::make_shared<ComponentArray<aiC>>();
    }
};

class mobSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    mobSystem() {
        componentArray = std::make_shared<ComponentArray<mobC>>();
    }
};

class toolSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    static void makeTool(int ent, glm::vec2 pos, std::string item, float usespeed, glm::mat4 = glm::mat4(1.0f), float damagemult = 1);
    toolSystem() {
        componentArray = std::make_shared<ComponentArray<toolC>>();
    }
};

class drawSystem : public SystemBase {
public:
    GLuint VA;
    GLuint VB;
    GLuint IB;
    GLuint shaderID;
    SpriteVertex* vertices;
    const int bsize = 1000;
    int renderi = 0;
    static int behindBackground;
    static int behindBlocks;
    static int mainDrawable;
    static int front;

    void onRegister();

    void UpdateBehindBackground();
    void UpdateBehindBlocks();
    void Update();
    void UpdateFront();

    static void addComponent(int entity, drawC* dc, bool force = true);

    void clean() override;
    void deleteComponent(int entity) override;
    void updateChildren(drawC* parent, ComponentArray<drawC>* arr);
    drawSystem() {
        componentArray = std::make_shared<ComponentArray<drawC>>();
    }
};

class droppedItemSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};

    bool dropItem(glm::vec2 pos, std::string_view name, int amount, bool collectable = true, glm::vec2 vel = glm::vec2(0.0f));

    droppedItemSystem() {
        componentArray = std::make_shared<ComponentArray<droppedItemC>>();
    }
};

class uiSystem : public SystemBase {
public:
    static int body;
    static bool mouseClicked;
    static bool mouseRightClicked;
    void onRegister();

    void Update();

    void updateChildren(uiC* parent, ComponentArray<uiC>* arr, glm::vec2 mc);
    
    uiSystem() {
        componentArray = std::make_shared<ComponentArray<uiC>>();
    }
};

class particleEmmiterSystem : public SystemBase {
public:
    void Update();
    void onRegister() {};
    void spawn(particleEmmiterC* pec, ComponentArray<particleEmmiterC>* arr, int i, physicsC* phys);
    particleEmmiterSystem() {
        componentArray = std::make_shared<ComponentArray<particleEmmiterC>>();
    }
};

