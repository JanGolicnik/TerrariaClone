#pragma once

#include <core.h>

namespace Inventory
{
    struct InvItem {
        const char* item;
        int num;
    };
    extern GLuint VA;
    extern GLuint VB;
    extern GLuint IB;

    extern const int rowLength;
    extern const int numRows;
    extern bool shown;
    extern int shownRows;
    extern const int recipeSize;
    extern const int invBSize;

    extern GLuint midTex;
    extern GLuint leftTex;
    extern GLuint rightTex;
    extern GLuint selectTex;
    extern GLuint slotTex;

    extern GLuint shaderID;
    
    extern glm::vec2 hoverpos;

    extern float size;
    extern float invY;
    extern float startOffset;
    extern float invX;
    extern float spacing;
    extern float il; // left pos
    extern float im; // mid pos
    extern float ir; // right edge pos
    extern float ew; // edge widths
    extern float mw; // mid width
    extern float ih; // inv height
    extern float selSize;
    extern float marginBot;
    extern float marginLeft;
    
    extern float craftingY;
    extern float craftingX;
    extern float craftingW;

    extern std::vector<const char*> craftableItems;

    extern glm::mat4 trans;
    extern SpriteVertex* vertices;
    extern std::array<InvItem, 9 * 5 + 1> blocks;

    void render();
    void refresh(double ax, double ay);
    void create();
    bool add(const char* block, int num = 1);
    bool addToSpot(const char* block, glm::vec2 pos, int num = 1);
    void remove(int index, int num = 1);
    int mouseClicked(float x, float y);
    void refreshCrafting(const char* item);
    void refreshCrafting();
    bool canCraft(const char* block);
    void craft(const char* block);
    void updateText(glm::vec2 mouse);


};

