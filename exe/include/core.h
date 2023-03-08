#ifndef CORE_H
#define CORE_H
#define PI 3.14159265359

#define BlockRuleArgs BlockRule *rule, glm::vec2 pos

// Glad and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library stuff
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <map>
#include <functional>
#include <set>
#include <string_view>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

enum AI {
    ai_GREENSLIME = 0,
    ai_TEST = 1,
    ai_ZOMBIE = 2,
    ai_SAND = 3,
    ai_DEMONEYE = 4,
    ai_BUNNY = 6,
    ai_ARROW = 7,
    ai_BULLET = 8,
    ai_TOOL = 9,
    ai_PARTICLE = 10,
    ai_FALLENSTAR = 11,
    ai_BOMB = 12,
    ai_GUIDE = 13,
    ai_TWINSSUMMON = 14,
    ai_IMP = 15,
};

enum UIFunction {
    ui_TEST,
    ui_ITEMSLOT,
    ui_CURSORITEM,
    ui_CRAFTABLEITEM,
    ui_DISPLAY,
    ui_BODY,
    ui_TEXT,
    ui_CONTAINER,
    ui_TOOLTIP,
    ui_RESOURCEBAR,
    ui_DRAGFLOAT,
    ui_TOGGLE,
    ui_BUTTON,
    ui_DRAGINT,
    ui_EMPTY,
    ui_FAKESLOT,
    ui_CURSOR,
    ui_HPBAR,
    ui_PICKUPTEXT,
    ui_TEXTBOX,
    ui_BACK,
    ui_BUFF,
    ui_BOSSBAR,
    ui_RADIAL
    
};

struct OverlayVertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct SpriteVertex {
    glm::vec3 Position;
    glm::vec2 SpriteCoords;
    glm::vec4 color;
    float spritesheet;
};

struct BlockVertex {
    glm::vec2 Position;
    glm::vec2 SpriteCoords;
    glm::vec2 TexCoords;
    glm::vec3 light;
    float breaking;
};

enum SpriteType {
    st_SINGLE,
    st_BLOCK,
    st_WALL,
    st_TORCH,
    st_GRASS,
    st_STALAKTIT,
    st_STALAGMIT,
    st_SMALLROCK,
    st_MEDIUMROCK,
    st_PLATFORM
};

struct light {
    glm::vec2 pos;
    glm::vec3 light;
};

enum frametype {
    ft_ABSOLUTE,
    ft_OFFSET1,
    ft_OFFSET8,
    ft_OFFSET16,
};

enum BlockRuleT {
	br_IS,
	br_ISNT,
	br_ALWAYS,
	br_RANDOM
};

enum BlockRuleArg {
    bs_STAT,
    bs_BLOCK
};

struct BlockRuleCond {
    BlockRuleT type;
    BlockRuleArg arg;
    glm::vec2 pos;
    std::string block;
    std::string layer;
    float randomInterval;
};

struct BlockRule {
    std::vector<BlockRuleCond> conditions;
    const char* block;
    const char* layer;
    bool exBool;
    std::function<void(BlockRuleArgs)> func;
};

struct InventoryItem {
    std::string item;
    int num;
    int pos;
};

#endif 
