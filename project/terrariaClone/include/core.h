#ifndef CORE_H
#define CORE_H

#define PI 3.14159265359
#define halfblock (blocksize/2)

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

enum ToolFunc {
    NONE = 0,
    PICKAXE = 1,
    AXE = 2,
    SWORD = 3
};

struct OverlayVertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct SpriteVertex {
    glm::vec3 Position;
    glm::vec2 SpriteCoords;
};

struct BlockVertex {
    glm::vec3 Position;
    glm::vec2 SpriteCoords;
    glm::vec2 TexCoords;
    float light;
    float breaking;
};

struct recipeItem {
    const char* name;
    int num;
};

struct BlockInfo {
    const char* drops;
    std::vector<const char*> droppers;
    std::vector<const char*> recipeFor;
    std::vector<recipeItem> recipeFrom;
    int craftedAmount;
    const char* layer;
    glm::vec2 width;
    float light;
    bool placeable = false;
    ToolFunc function;
    float digstrength;
    float placespeed = 10;
    int life;
};

#endif 
