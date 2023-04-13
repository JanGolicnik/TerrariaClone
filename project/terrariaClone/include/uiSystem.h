#pragma once

#include <core.h>
#include <drawSystem.h>

struct ActiveBuff;

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
