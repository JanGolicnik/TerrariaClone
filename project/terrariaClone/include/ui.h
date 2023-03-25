#pragma once

#include <core.h>

#include <componentsystems.h>
#include <items.h>

#include <fstream>

#define uiCfunctionArguments uiC* p, drawC* draw, int entity, ComponentArray<uiC>* arr

class Inventory {
public:
    std::vector<std::shared_ptr<InventoryItem>>* items;
    glm::vec2 size;
    std::vector<int> entities;
    glm::vec2 rows;

    static void setupInventory(std::vector<std::shared_ptr<InventoryItem>>* vec, glm::vec2 size);
    void markAsSelected(int pos);
    Inventory(glm::vec2 pos, glm::vec2 rows, std::vector<std::shared_ptr<InventoryItem>>* items, bool hidden, int container = -1, float scale = 1, bool shop = false);
    void setTo(std::vector<std::shared_ptr<InventoryItem>>* items);
    int add(std::string item, int num = 1, int spot = -1);
    bool has(std::string item, int num = 1);
    bool remove(std::string item, int num = 1);
    void toggle(int toggle = -1);
    bool hidden();
    bool contains(std::string item, int num);
    bool take(std::string item, int num, bool takeless = false);
    bool hasRoomFor(std::string item);
    InventoryItem* at(glm::vec2 pos);
};

namespace UI
{
    extern std::vector<int> availableRecipes;

    extern Inventory* inventory;
    extern Inventory* hotbar;
    extern Inventory* chest;
    extern Inventory* npc;
    extern std::vector<int> craftingSlot;

    extern InventoryItem cursorItem;
    extern InventoryItem guideItem;
    extern InventoryItem helmetItem;
    extern InventoryItem breastplateItem;
    extern InventoryItem greavesItem;
    extern InventoryItem accessory1Item;
    extern InventoryItem accessory2Item;
    extern InventoryItem accessory3Item;
    extern InventoryItem accessory4Item;
    extern InventoryItem accessory5Item;

    extern std::vector<std::shared_ptr<InventoryItem>> PlayerInventory;
    extern std::vector<std::shared_ptr<InventoryItem>> PlayerHotbar;
    extern std::vector<std::shared_ptr<InventoryItem>> defaultChest;
    extern std::vector<std::shared_ptr<InventoryItem>> merchantInventory;

    extern glm::vec2 chestInvPos;
    extern glm::vec2 inventoryPos;

    extern std::set<std::string> craftingStationsInRange;

    extern int openChest;

    extern bool capturingInput;
    extern std::string* capturedInput;


    void destroy();

    void create();
    void addElement(int entity, UIFunction func, glm::vec2 pos = glm::vec2(0), glm::vec2 size = glm::vec2(1.0f, 1.0f), int parent = -1, std::unordered_map<std::string, uiStat> stats = {}, std::unordered_map<std::string, std::string> textStats = {}, bool hidden = false, uiAnchor anchor = anchorMID);
    void deleteElement(int entity, bool self = true);

    uiStat* getStat(uiC* p, std::string name, uiStat def = { .floatVal = 0 });
    std::string getTStat(uiC* p, std::string name, std::string_view def = "");
    std::string* getTStatRef(uiC* p, std::string name, std::string_view def = "");
    std::string_view getTStatV(uiC* p, std::string name, std::string_view def = "");
    void setTStat(uiC* p, std::string name, std::string val);

    void refreshCrafting();
    bool canCraft(recipe* rec);
    void craft(recipe rec);
    int getCraftable(int* i);

    void openInventory();

    void UI_nothing(uiCfunctionArguments);
    void testONRENDER(uiCfunctionArguments);
    void itemslotONCLICK(uiCfunctionArguments);
    void itemslotONRIGHTCLICK(uiCfunctionArguments);
    void itemslotONRENDER(uiCfunctionArguments);
    void itemslotONHOVER(uiCfunctionArguments);
    void itemslotONCLICK(uiCfunctionArguments);
    void itemslotONHOLD(uiCfunctionArguments);
    void shopitemslotONHOLD(uiCfunctionArguments);
    void shopitemslotONRENDER(uiCfunctionArguments);
    void shopitemslotONHOVER(uiCfunctionArguments);
    void guideslotONRENDER(uiCfunctionArguments);
    void craftableitemONCLICK(uiCfunctionArguments);
    void craftableitemONRENDER(uiCfunctionArguments);
    void craftableitemONHOVER(uiCfunctionArguments);
    void cursoritemONRENDER(uiCfunctionArguments);
    void cursoritemONRIGHTCLICK(uiCfunctionArguments);
    void textONRENDER(uiCfunctionArguments);
    void textONHOVER(uiCfunctionArguments);
    void textONNOTHOVER(uiCfunctionArguments);
    void tooltipONRENDER(uiCfunctionArguments);
    void displayONRENDER(uiCfunctionArguments);
    void resourcebarONRENDER(uiCfunctionArguments);
    void resourcebarONHOVER(uiCfunctionArguments);
    void dragfloatONRENDER(uiCfunctionArguments);
    void dragfloatONHOLD(uiCfunctionArguments);
    void toggleONRENDER(uiCfunctionArguments);
    void toggleONCLICK(uiCfunctionArguments);
    void buttonONCLICK(uiCfunctionArguments);
    void buttonONRENDER(uiCfunctionArguments);
    void dragintONHOLD(uiCfunctionArguments);
    void dragintONRENDER(uiCfunctionArguments);
    void containerONRENDER(uiCfunctionArguments);
    void fakeslotONRENDER(uiCfunctionArguments);
    void fakeslotONHOVER(uiCfunctionArguments);
    void cursorONRENDER(uiCfunctionArguments);
    void hpbarONRENDER(uiCfunctionArguments);
    void pickuptextONRENDER(uiCfunctionArguments);
    void textboxONRENDER(uiCfunctionArguments);
    void backONRENDER(uiCfunctionArguments);
    void backONHOVER(uiCfunctionArguments);
    void buffONRENDER(uiCfunctionArguments);
    void buffONRIGHTCLICK(uiCfunctionArguments);
    void buffONHOVER(uiCfunctionArguments);
    void bossbarONRENDER(uiCfunctionArguments);
    void radialONRENDER(uiCfunctionArguments);
    void logoONRENDER(uiCfunctionArguments);
    void keybindONRENDER(uiCfunctionArguments);
    void keybindONCLICK(uiCfunctionArguments);
    void cursoritemONLEFTCLICK(uiCfunctionArguments);
    void cursoritemONHOLD(uiCfunctionArguments);
    void cursoritemONRIGHTCLICK(uiCfunctionArguments);
    void soundONENTER(uiCfunctionArguments);
    void toastONRENDER(uiCfunctionArguments);

    void uiCfunc_captureInput(uiC* p);
    void uiCfunc_startGame(uiC* p);
    void uiCfunc_createWorld(uiC* p);
    void uiCfunc_saveGame(uiC* p);
    void uiCfunc_toggleSettings(uiC* p);
    void uiCfunc_openSettingsMenu(uiC* p);
    void uiCfunc_saveAndExitGame(uiC* p);
    void uiCfunc_exitGame(uiC* p);
    void uiCfunc_exit(uiC* p);
    void uiCfunc_loadSpecificWorld(uiC* p);
    void uiCfunc_loadPlayer(uiC* p);
    void uiCfunc_createPlayer(uiC* p);
    void uiCfunc_loadSpecificPlayer(uiC* p);
    void uiCfunc_toggleFullscreen(uiC* p);
    void uiCfunc_nextResolution(uiC* p);
    void uiCfunc_resetKeybinds(uiC* p);
    void uiCfunc_openGuideslot(uiC* p);
    void uiCfunc_guideHelp(uiC* p);
    void uiCfunc_openMerchantShop(uiC* p);

    void applyAnchor(glm::vec2* pos, uiAnchor anchor);
    void hideChildren(uiC* p, int state, bool self = false);
    void setChildrenInteractable(uiC* p, bool state);
    bool addLetter(int key);
    void captureInput(std::string* string, bool yes = true, int amount = -1, int maxlength = -1);

    float childMostToTheLeft(uiC* p, float currMax, bool anchored = false, int ignore = 0);
    float childMostToTheRight(uiC* p, float currMax, bool anchored = false, int ignore = 0);
    float childMostToTheTop(uiC* p, float currMax, bool anchored = false, int ignore = 0);
    float childMostToTheBottom(uiC* p, float currMax, bool anchored = false, int ignore = 0);

    void saveInvItemVector(std::vector<std::shared_ptr<InventoryItem>>* vec, std::ofstream* file);
    void loadInvItemVector(std::vector<std::shared_ptr<InventoryItem>>* vec, std::ifstream* file);
    void saveInvItem(InventoryItem* vec, std::ofstream* file);
    void loadInvItem(InventoryItem* vec, std::ifstream* file);

    void createTooltip(std::string item, std::string* text);

    namespace Elements {

        void setnextdrawparent(int parent);
        int test(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int empty(int parent, glm::vec2 pos = glm::vec2(0), uiAnchor anchor = anchorNONE, float textSize = 0);
        int itemslot(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor, InventoryItem* itemp, float textsize, itemFamily limitfamily = if_ANY);
        int shopitemslot(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor, InventoryItem* itemp, float textsize, itemFamily limitfamily = if_ANY);
        int guideslot(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor, InventoryItem* itemp, float textsize, itemFamily limitfamily = if_ANY);
        int text(int parent, glm::vec2 pos, bool hidden, uiAnchor anchor, std::string text = "", float textSize = globals::fontSize, bool centered = false, bool colorOnHover = false, float hoveredScale = 1, float opacity = 1);
        int text(int parent, glm::vec2 pos, bool hidden, uiAnchor anchor, std::string *text = nullptr, float textSize = globals::fontSize, bool centered = false, bool colorOnHover = false, float hoveredScale = 1, float opacity = 1);
        int toast(int parent, glm::vec2 pos, bool hidden, uiAnchor anchor, std::string text, float textSize = globals::fontSize);
        int cursoritem(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor, InventoryItem* itemp);
        int cursortext(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int craftingslot(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int display(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor, std::string tex = "empty", bool autocorrect = true, bool flipX = false, bool flipY = false, glm::vec3* color = nullptr, bool autolight = false, float* huep = nullptr);
        int tooltip(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int resourcebar(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int dragfloat(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int toggle(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int button(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int dragint(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int container(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int fakeslot(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int cursor(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int hpbar(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int pickuptext(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int textbox(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int back(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int buff(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int bossbar(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int radial(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int logo(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
        int keybind(int parent, glm::vec2 pos, glm::vec2 size, bool hidden, uiAnchor anchor);
    }

};

