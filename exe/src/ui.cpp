#include "ui.h"

#include <ECS/ECS.h>

#include <utils.h>
#include <layers.h>
#include <blocks.h>
#include <player.h>
#include <globals.h>
#include <componentSystems.h>
#include <input.h>
#include <game.h>
#include <gameLoop.h>
#include <camera.h>
#include <world.h>
#include <startMenu.h>
#include <text.h>
#include <buffs.h>   

namespace UI {
    std::vector<int> availableRecipes;

    Inventory* inventory;
    Inventory* hotbar;
    Inventory* chest;
    std::vector<int> craftingSlot;
    InventoryItem cursorItem;
    InventoryItem helmetItem;
    InventoryItem breastplateItem;
    InventoryItem greavesItem;
    InventoryItem accessory1Item;
    InventoryItem accessory2Item;
    InventoryItem accessory3Item;
    InventoryItem accessory4Item;
    InventoryItem accessory5Item;
    glm::vec2 inventoryPos = { 100, -100 };
    glm::vec2 chestInvPos = { 200, -600 };

    std::vector<std::shared_ptr<InventoryItem>> PlayerInventory;
    std::vector<std::shared_ptr<InventoryItem>> PlayerHotbar;
    std::vector<std::shared_ptr<InventoryItem>> defaultChest;

    std::set<std::string> craftingStationsInRange;

    int openChest;
    bool capturingInput = false;
    std::string* capturedInput;

    bool canCraft(recipe* rec)
    {
        auto arr = static_cast<ComponentArray<uiC>*>(ECS::getSystem<uiSystem>()->componentArray.get());
        bool canCraft = false;
        for (auto& i : rec->craftingStations) {
            if (craftingStationsInRange.count(i) >= 1) {
                canCraft = true;
                break;
            }
        }

        if (canCraft) {
            //loopamo cez use iteme v receptu
            for (int i = 0; i < rec->items.size(); i++) {
                recipeItem ri = rec->items[i];

                //gremo cez use vidne itemslotte
                for (int c = 0; c < arr->components.size(); c++) {
                    if (arr->components[c].func == ui_ITEMSLOT && arr->components[c].hidden == false) {
                        auto itemp = UI::getStat(&arr->components[c], "item")->itemp;
                        if (itemp->item == ri.item) {
                            //ce majo ta item odstejemo in ce je stevilka po tem kprevermo use itemslotte vecja od nic pomen da jih n idovol seprav ne mormo scraftat
                            ri.num -= itemp->num;
                        }
                    }
                }
                if (ri.num > 0) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    void destroy()
    {
        delete hotbar;
        hotbar = nullptr;
        delete inventory;
        inventory = nullptr;
        delete chest;
        chest = nullptr;
        availableRecipes.clear();
        craftingSlot.clear();
        PlayerInventory.clear();
        PlayerHotbar.clear();
        defaultChest.clear();
        craftingStationsInRange.clear();
        captureInput(nullptr, false);
    }

    void refreshCrafting()
    {
        availableRecipes.clear();
        auto arr = static_cast<ComponentArray<uiC>*>(ECS::getSystem<uiSystem>()->componentArray.get());

        std::set<int> alreadyadded;

        if (inventory->hidden() == false) {
            for (int i = 0; i < inventory->items->size(); i++) {
                auto info = items::getInfo(inventory->items->at(i)->item);
                for (int j = 0; j < info->inRecipes.size(); j++) {
                    if (canCraft(&items::recipes[info->inRecipes[j]])) {
                        if (alreadyadded.count(info->inRecipes[j]) <= 0) {
                            availableRecipes.push_back(info->inRecipes[j]);
                            alreadyadded.insert(info->inRecipes[j]);
                        }
                    }
                }
            }
            for (int i = 0; i < hotbar->items->size(); i++) {
                auto info = items::getInfo(hotbar->items->at(i)->item);
                for (int j = 0; j < info->inRecipes.size(); j++) {
                    if (canCraft(&items::recipes[info->inRecipes[j]])) {
                        if (alreadyadded.count(info->inRecipes[j]) <= 0) {
                            availableRecipes.push_back(info->inRecipes[j]);
                            alreadyadded.insert(info->inRecipes[j]);
                        }
                    }
                }
            }
        }
    }

    void craft(recipe rec) {
        auto arr = static_cast<ComponentArray<uiC>*>(ECS::getSystem<uiSystem>()->componentArray.get());

        for (int i = 0; i < rec.items.size(); i++) {
            for (int n = 0; n < rec.items[i].num; n++) {
                for (int c = 0; c < arr->components.size(); c++) {
                    if (arr->components[c].func == ui_ITEMSLOT && arr->components[c].hidden == false) {
                        auto itemp = UI::getStat(&arr->components[c], "item")->itemp;
                        if (itemp->item == rec.items[i].item) {
                            itemp->num--;
                            break;
                        }
                    }
                }
            }
        }
    }

    void create()
    {
        hotbar = new Inventory({ 100, -100 }, { 10, 1 }, &PlayerHotbar, false, gameLoop::inventoryC);

        inventory = new Inventory({ inventoryPos.x, inventoryPos.y - 110 }, { 10, 4 }, &PlayerInventory, true, gameLoop::inventoryC);
        inventory->toggle(1);

        Inventory::setupInventory(&defaultChest, { 10,4 });
        chest = new Inventory({ chestInvPos, {10,4}, &defaultChest, true, gameLoop::chestInvC, 0.885f});
        openChest = -1;
    }

    void addElement(int entity, UIFunction func, glm::vec2 pos, glm::vec2 size, int parent, std::unordered_map<std::string, uiStat> stats, std::unordered_map<std::string, std::string> textStats, bool hidden, uiAnchor anchor)
    {
        uiC c;
        c.func = func;
        c.pos = pos;
        c.size = size;
        c.hidden = hidden;
        if (parent == -1) {
            parent = uiSystem::body;
        }
        ECS::getComponent<uiC>(parent)->children.push_back(entity);
        c.parent = parent;
        c.onclick = UI_nothing;
        c.onrightclick = UI_nothing;
        c.onhover = UI_nothing;
        c.onrender = UI_nothing;
        c.onnothover = UI_nothing;
        c.anchor = anchor;
        c.textStats = textStats;
        c.stats = stats;

        drawC dc;
        dc.position = std::make_shared<glm::vec2>(glm::vec2(0, 0));
        dc.tex = "empty";
        dc.hidden = hidden;
        dc.parent = parent;
        dc.autolight = false;
        dc.size = size;
        if (stats.count("textSize") >= 1) {
            dc.textScale = stats["textSize"].floatVal;
        }

        //add functions
        switch (func) {
        case ui_TEST:
            c.onrender = testONRENDER;
            break;
        case ui_ITEMSLOT:
            c.onclick = itemslotONCLICK;
            c.onrightclick = itemslotONRIGHTCLICK;
            c.onrender = itemslotONRENDER;
            c.onhover = itemslotONHOVER;
            c.onnothover = itemslotONNOTHOVER;
            break;
        case ui_CURSORITEM:
            dc.textoffset = glm::vec2(0, -1);
            c.onrender = cursoritemONRENDER;
            c.onrightclick = cursoritemONRIGHTCLICK;
            c.anchor = anchorNONE;
            dc.parent = stats["parent"].intVal;
            break;
        case ui_CRAFTABLEITEM:
            dc.color = globals::itemslotColor;
            c.onrender = craftableitemONRENDER;
            c.onclick = craftableitemONCLICK;
            c.onhover = craftableitemONHOVER;
            break;
        case ui_DISPLAY:
            c.onrender = displayONRENDER;
            if (stats.count("autocorrect") >= 1) {
                dc.autocorrect = stats["autocorrect"].boolVal;
            }
            if (stats.count("flipX") >= 1) {
                dc.flipX = stats["flipX"].boolVal;
            }
            if (stats.count("flipY") >= 1) {
                dc.flipY= stats["flipY"].boolVal;
            }
            if (stats.count("color") >= 1) {
                if (stats["color"].vec3p != nullptr) {
                    dc.color = *stats["color"].vec3p;
                }
            }
            break;
        case ui_TEXT:
            c.onrender = textONRENDER;
            c.onhover = textONHOVER;
            c.onnothover= textONNOTHOVER;
            break;
        case ui_TOOLTIP:
            c.onrender = tooltipONRENDER;
            c.anchor = anchorNONE;
            dc.tex = textStats["tex"];
            break;
        case ui_RESOURCEBAR:
            c.onrender = resourcebarONRENDER;
            c.onhover = resourcebarONHOVER;
            break;
        case ui_DRAGFLOAT:
            c.onclick = dragfloatONCLICK;
            c.onrender = dragfloatONRENDER;
            break;
        case ui_TOGGLE:
            c.onclick = toggleONCLICK;
            c.onrender= toggleONRENDER;
            c.size = glm::vec2(stats["width"].floatVal * stats["scale"].floatVal);
            break;
        case ui_BUTTON:
            c.onclick = buttonONCLICK;
            c.onrender = buttonONRENDER;
            dc.tex = textStats["tex"];
            c.propagateClick = false;
            break;
        case ui_DRAGINT:
            c.onclick = dragintONCLICK;
            c.onrender = dragintONRENDER;
            break;
        case ui_CONTAINER:
            c.onrender = containerONRENDER;
            break;
        case ui_FAKESLOT:
            dc.color = globals::itemslotColor;
            c.onrender = fakeslotONRENDER;
            c.onhover = fakeslotONHOVER;
            break;
        case ui_CURSOR:
            c.onrender = cursorONRENDER;
            break;
        case ui_HPBAR:
            c.onrender = hpbarONRENDER;
            dc.parent = globals::topparticleLayer;
            break;
        case ui_PICKUPTEXT:
            c.onrender = pickuptextONRENDER;
            dc.textScale = 0;
            dc.parent = globals::topparticleLayer;
            break;
        case ui_TEXTBOX:
            c.onrender = textboxONRENDER;
            break;
        case ui_BACK:
            c.onrender = backONRENDER;
            if (stats.count("color") >= 1) {
                dc.color = *stats["color"].vec3p;
            }
            break;
        case ui_BUFF:
            c.onrender = buffONRENDER;
            c.onrightclick = buffONRIGHTCLICK;
            c.onhover = buffONHOVER;
            break;
        case ui_BOSSBAR:
            c.onrender = bossbarONRENDER;
            dc.autocorrect = false;
            break;
        case ui_RADIAL:
            c.onrender = radialONRENDER;
            c.size = glm::vec2(7);
            dc.tex = "radial";
            dc.opacity = 0.5;
            dc.parent = globals::topparticleLayer;
            break;
        }

        ECS::addComponent<uiC>(entity, c);
        drawSystem::addComponent(entity, &dc);

        //add children
        uiStat t;
        t.intVal = 0; 
        switch (func) {
        case ui_TEST:
            break;
        case ui_ITEMSLOT:
            {
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 1,1 }, entity, {}, { {"tex", "item"},{"item", "empty"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos, glm::vec2(0), entity, { {"textSize", {.floatVal = globals::fontSize}} }, { {"text", ""} }, hidden, anchor);
        }
            break;
        case ui_CRAFTABLEITEM:
        {
            uiStat centered; centered.boolVal = false;
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 1,1 }, entity, { }, { {"tex", "item"},{"item", "empty"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos, glm::vec2(0), entity, { {"centered", centered}, {"textSize", stats["textSize"]} }, { {"text", "empty"} }, hidden, anchor);
        }   break;
        case ui_TEXT: {
            int ent = ECS::newEntity();
            addElement(ent, ui_EMPTY, c.pos, glm::vec2(0), entity, { {"textSize", stats["textSize"]} }, {}, hidden, anchorNONE);
        }
            break;
        case ui_TOOLTIP:
        {    uiStat centered; centered.boolVal = false;
             addElement(ECS::newEntity(), ui_BACK, c.pos, glm::vec2(0), entity, { {"color", {.vec3p = &globals::tooltipcolor}} }, {}, hidden, anchor);
             addElement(ECS::newEntity(), ui_TEXT, c.pos, glm::vec2(0), entity, { {"centered", centered},{"textSize", stats["textSize"]} }, { {"text", ""} }, hidden, anchor);
        }   break;
        case ui_RESOURCEBAR: {
            glm::vec2 offset = glm::vec2(0);
            int numelements = stats["max"].floatVal / stats["step"].intVal;
            for (int x = 0; x < numelements; x++) {
                if (x % stats["sizex"].intVal == 0) {
                    offset.x = 0;
                    offset.y -= stats["itemsize"].floatVal * 1.1;
                }
                addElement(ECS::newEntity(), ui_DISPLAY, c.pos + offset, glm::vec2(stats["itemsize"].floatVal), entity, {}, { {"tex", textStats["itemtex"]} }, true, anchor);
                offset.x += stats["itemsize"].floatVal * 1.1;
            }
        }   break;
            
        case ui_DRAGFLOAT: {
                std::string text = textStats["label"];
                uiStat centered; centered.boolVal = false;
                uiStat float1; float1.floatVal= 1;
                addElement(ECS::newEntity(), ui_DISPLAY, c.pos, glm::vec2(size.x), entity, {}, { {"tex", "slider"} }, hidden, anchor);
                addElement(ECS::newEntity(), ui_DISPLAY, c.pos, glm::vec2(size.y), entity, {}, { {"tex", "sliderpaddle"} }, hidden, anchor);
                addElement(ECS::newEntity(), ui_TEXT, c.pos - glm::vec2(size.x / 2 + text::widthOfText(&text), 0), { 0,0 }, entity, { {"centered", centered},{"textSize", float1} }, { {"text", textStats["label"]} }, hidden, anchor);
                addElement(ECS::newEntity(), ui_TEXT, c.pos + glm::vec2(size.x/2, 0), {0,0}, entity, { {"centered", centered},{"textSize", float1} }, {{"text", ""} }, hidden, anchor);
            }   break;

        case ui_TOGGLE: {
            int scale = stats["scale"].floatVal;
            int width = stats["width"].floatVal;
            std::string text = textStats["label"];
            uiStat float1; float1.floatVal = 1;
            uiStat centered; centered.boolVal = false;
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, glm::vec2(width * scale), entity, {}, { {"tex", "empty"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos - glm::vec2(width / 2 * scale * 1.2 + text::widthOfText(&text), 0), {0,0}, entity, { {"centered", centered},{"textSize", float1} }, { {"text", textStats["label"]} }, hidden, anchor);
        }   break;
        case ui_BUTTON:
        {uiStat centered; centered.boolVal = true;
        uiStat hoveredScale; hoveredScale.floatVal = stats["textSize"].floatVal * 1.5;
        addElement(ECS::newEntity(), ui_TEXT, c.pos, glm::vec2(0), entity, { {"centered", centered},{"hoveredScale", hoveredScale},{"textSize", stats["textSize"]},{"colorOnHover", {.boolVal = true}} }, { {"text", textStats["text"]} }, hidden, anchor);
        }   break;
        case ui_DRAGINT: {
            std::string text = textStats["label"];
            uiStat centered; centered.boolVal = false;
            uiStat float1; float1.floatVal = 1;
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, glm::vec2(size.x), entity, {}, { {"tex", "slider"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, glm::vec2(size.y), entity, {}, { {"tex", "sliderpaddle"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos - glm::vec2(size.x / 2 + text::widthOfText(&text), 0), { 0,0 }, entity, { {"textSize", float1} }, { {"text", textStats["label"]} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos + glm::vec2(size.x / 2, 0), { 0,0 }, entity, { {"centered", centered},{"textSize", float1} }, { {"text", ""} }, hidden, anchor);
        }   break;
        case ui_FAKESLOT:
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 1,1 }, entity, {}, { {"tex", "item"},{"item", "empty"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos, glm::vec2(0), entity, { {"textSize", stats["textSize"]} }, { {"text", "empty"} }, hidden, anchor);
            break;
        case ui_CURSOR:
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, c.size * glm::vec2(0.9), entity, {}, { {"tex", "cursor"} }, hidden, anchor);
            break;
        case ui_HPBAR:
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 0,0 }, entity, {}, { {"tex", "hpbar2"} }, hidden, anchor);
            break;
        case ui_TEXTBOX:
            addElement(ECS::newEntity(), ui_BACK, c.pos, { 0,0 }, entity, {}, {}, hidden, anchor);
            addElement(ECS::newEntity(), ui_TEXT, c.pos, { 0,0 }, entity, { {"textSize", {.floatVal = 0.8}} }, { {"text", textStats["text"]} }, hidden, anchor);
            break;
        case ui_BACK: {
            glm::vec2 size = glm::vec2(32);
            if (anchor == anchorNONE) size /= globals::blocksizepx;
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, c.size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}} }, { {"tex", "backmiddle"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}} }, { {"tex", "backleft"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}} }, { {"tex", "backtop"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}}, { "flipX", {.boolVal = true} } }, { {"tex", "backleft"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}}, { "flipY", {.boolVal = true} } }, { {"tex", "backtop"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}} }, { {"tex", "backcorner"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}}, { "flipX", {.boolVal = true} } }, { {"tex", "backcorner"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}}, { "flipY", {.boolVal = true} }, { "flipX", {.boolVal = true} } }, { {"tex", "backcorner"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, size, entity, { {"autocorrect", {.boolVal = false}}, {"color",  {.vec3p = stats["color"].vec3p}}, {"flipY", {.boolVal = true} } }, { {"tex", "backcorner"} }, hidden, anchor);

        }   break;
        case ui_CONTAINER:
            addElement(ECS::newEntity(), ui_BACK, c.pos, glm::vec2(0), entity, {}, {}, hidden, anchor);
            break;  
        case ui_BOSSBAR:
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 0,0 }, entity, {}, { {"tex", "bossbar"} }, hidden, anchor);
            addElement(ECS::newEntity(), ui_DISPLAY, c.pos, { 0,0 }, entity, {}, { {"tex", textStats["head"]}}, hidden, anchor);
            break;
        }
    }
    void deleteElement(int entity,bool self)
    {
        auto p = ECS::getComponent<uiC>(entity);
        for (int i = 0; i < p->children.size(); i++) {
            deleteElement(p->children[i], false);
            ECS::queueDeletion(p->children[i]);
        }
        p->children.clear();
        if (self) {
            p->removed = true;
            ECS::queueDeletion(entity);
            auto parent = ECS::getComponent<uiC>(p->parent);
            for (int i = 0; i < parent->children.size(); i++) {
                if (parent->children[i] == entity) {
                    parent->children.erase(parent->children.begin() + i);
                    break;
                }
            }
        }
    }
    uiStat* getStat(uiC* p, std::string name, uiStat def)
    {
        if (p->stats.count(name) <= 0) {
            p->stats[name] = def;
        }
        return &p->stats[name];
    }
    std::string getTStat(uiC* p, std::string name, std::string_view def)
    {
        if (p->textStats.count(name) <= 0) {
            p->textStats[name] = def;
        }
        return p->textStats[name];
    }
    std::string* getTStatRef(uiC* p, std::string name, std::string_view def)
    {
        if (p->textStats.count(name) <= 0) {
            p->textStats[name] = def;
        }
        return &p->textStats[name];
    }
    std::string_view getTStatV(uiC* p, std::string name, std::string_view def)
    {
        if (p->textStats.count(name) <= 0) {
            p->textStats[name] = def;
        }
        return p->textStats[name];
    }
    void setTStat(uiC* p, std::string name, std::string val)
    {
        p->textStats[name] = val;
    }
    int getCraftable(int *i)
    {
        if (availableRecipes.size() == 0) {
            return -1;
        }
        *i = glm::clamp(*i, 0, (int)availableRecipes.size() - 1);
        return availableRecipes[*i];
    }

    void openInventory()
    {
        UI::hideChildren(ECS::getComponent<uiC>(gameLoop::inventoryC), false);
        UI::hideChildren(ECS::getComponent<uiC>(gameLoop::equipmentC), false);
        UI::inventory->toggle(0);
        UI::chest->toggle(0);
        ECS::getComponent<uiC>(gameLoop::crafting)->hidden = UI::inventory->hidden();
    };

    void UI_nothing(uiCfunctionArguments)
    {
    }

    void testONRENDER(uiCfunctionArguments)
    {
    }

    void itemslotONCLICK(uiCfunctionArguments)
    {
        auto item = getStat(p, "item", { .itemp = nullptr })->itemp;
        auto citem = getStat(ECS::getComponent<uiC>(gameLoop::cursoritem), "item", { .itemp = nullptr })->itemp;
        auto citeminfo = items::getInfo(citem->item);
        int itemfam = getStat(p, "limitFamily")->intVal;
        if (itemfam == if_ANY || citeminfo->families.find((itemFamily)itemfam) != citeminfo->families.end() || citeminfo->families.find(if_ANY) != citeminfo->families.end()) {
            if (item != NULL && citem != NULL) {
                if (citem->item == item->item) {
                    int maxStacks = items::info[item->item].maxStacks;
                    //dodamo iz roke dokler ne pridemo do maxa, ce pa smo ze na maxu pa jih sam zamenamo
                    if (maxStacks == item->num) {
                        std::string tmpi = item->item;
                        item->item = citem->item;
                        citem->item = tmpi;
                        int tmpn = item->num;
                        item->num = citem->num;
                        citem->num = tmpn;
                        return;
                    }
                    item->num += citem->num;
                    if (item->num > maxStacks) {
                        citem->num = item->num - maxStacks;
                        item->num = maxStacks;
                    }
                    else {
                        citem->num = 0;
                    }
                }
                else {
                    std::string tmpi = item->item;
                    item->item = citem->item;
                    citem->item = tmpi;
                    int tmpn = item->num;
                    item->num = citem->num;
                    citem->num = tmpn;
                }
            }
        }
    }
    void itemslotONRIGHTCLICK(uiCfunctionArguments)
    {
        auto item = getStat(p, "item", { .itemp = nullptr })->itemp;
        auto citem = getStat(ECS::getComponent<uiC>(gameLoop::cursoritem), "item", { .itemp = nullptr })->itemp;
        if (item != NULL && citem != NULL) {
            if (citem->item == item->item) {
                if (item->num >= 1 && citem->num < items::info[citem->item].maxStacks) {
                    citem->num++;
                    item->num--;
                }
            }
            else if (citem->item == "empty") {
                citem->item = item->item;
                citem->num = 1;
                item->num--;
            }
        }
    }
    void itemslotONRENDER(uiCfunctionArguments)
    {
        auto dc = ECS::getComponent<uiC>(p->children[0]);
        auto tc = ECS::getComponent<uiC>(p->children[1]);
        auto item = getStat(p, "item", { .itemp = nullptr })->itemp;
        if (item != nullptr) {
            if (item->num <= 0) {
                item->item = "empty";
                item->num = 0;
                UI::setTStat(tc, "text", "");
            }
            else {
                if (item->num == 1) {
                    UI::setTStat(tc, "text", "");
                }
                else {
                    UI::setTStat(tc, "text", std::to_string(item->num));
                }
                tc->pos.x = p->pos.x - p->size.x * 0.2;
                tc->pos.y = p->pos.y - p->size.y / 4;
            }

            tc->hidden = p->hidden;

            dc->pos = p->pos;
            dc->size = p->size * glm::vec2(0.6f);
            UI::setTStat(dc, "item", item->item);
            dc->hidden = p->hidden;
        }
        else {
            UI::setTStat(tc, "text", "");
            UI::setTStat(dc, "item", "empty");
        }
        draw->tex = UI::getTStat(p, "tex", "invempty");
        if (UI::getStat(p, "selected", { .boolVal = false })->boolVal == true) {
            draw->color = globals::selecteditemslotColor;
        }
        else {
            draw->color = globals::itemslotColor;
        }
    }

    void craftableitemONCLICK(uiCfunctionArguments)
    {
        int c = getCraftable(&UI::getStat(p, "craftable")->intVal);
        if (c >= 0) {
            auto ci = ECS::getComponent<uiC>(gameLoop::cursoritem);
            auto citemp = UI::getStat(ci, "item")->itemp;
            if (citemp->item == "empty" || citemp->item == items::recipes.at(c).craftedItem) {
                if (canCraft(&items::recipes.at(c))) {
                craft(items::recipes.at(c));
                citemp->item = items::recipes.at(c).craftedItem;
                citemp->num += items::recipes.at(c).craftedAmount;
                }
            }
        }
    }

    void craftableitemONRENDER(uiCfunctionArguments)
    {
        draw->tex = "invempty";

        int craftable = UI::getCraftable(&UI::getStat(p, "craftable")->intVal);
        auto dc = ECS::getComponent<uiC>(p->children[0]);
        auto tc = ECS::getComponent<drawC>(p->children[1]);
        auto tcC = ECS::getComponent<uiC>(p->children[1]);
        if (craftable >= 0) {
            auto recipe = &items::recipes[craftable];
            if (UI::getTStatV(p, "lastItem", "empty") != recipe->craftedItem) {
                UI::setTStat(p, "lastItem", recipe->craftedItem);
                for (int i = 2; i < p->children.size(); i++) {
                    deleteElement(p->children[i]);
                    i--;
                }
                for (int i = 0; i < recipe->items.size(); i++) {
                    uiStat num; num.intVal = recipe->items[i].num;
                    uiStat inheritHidden; inheritHidden.boolVal = true;
                    uiStat textSize; textSize.floatVal = 1;
                    addElement(ECS::newEntity(), ui_FAKESLOT, p->pos + glm::vec2((i + 1) * p->size.x, 0), glm::vec2(p->size.x * 0.9, p->size.y * 0.9), entity,
                        { {"num", num}, {"inheritHidden", inheritHidden}, {"textSize", textSize} },
                        { {"item", recipe->items[i].item} }, p->hidden, p->anchor);
                }
            }
            UI::setTStat(dc, "item", recipe->craftedItem);
            
            dc->pos = p->pos;
            dc->size = p->size * glm::vec2(0.6f);
            dc->hidden = p->hidden;
            tc->text = std::to_string(recipe->craftedAmount);
            tc->textScale = globals::fontSize;
            tcC->hidden = p->hidden;
            tcC->pos.x = p->pos.x - p->size.x * 0.2;
            tcC->pos.y = p->pos.y - p->size.y / 4;
        }
        else {
            UI::setTStat(dc, "item", "empty");
            tc->text = "";
        }
    }

    void cursoritemONRENDER(uiCfunctionArguments)
    {
        auto item = getStat(p, "item")->itemp;

        draw->textScale = globals::fontSize;
        if (item == nullptr) {
            draw->text = *UI::getTStatRef(p, "text", "");
            p->pos = globals::mouseBlockCoordsZoomed(false);
        }
        else {
            if (item->num <= 0) {
                item->item = "empty";
                item->num = 0;
                draw->text = "";
            }
            if (item->item == "empty") {
                draw->text = *UI::getTStatRef(p, "text", "");
            }
            else {
                draw->text = std::to_string(item->num);
            }
            draw->tex = items::getInfo(item->item)->tex.c_str();
            p->pos = globals::mouseBlockCoords(false);
        }
        glm::vec2 offset = p->size;
        offset *= 0.45f;
        if (p->pos.x > Player::pos.x) {
            offset.x *= -1;
        }
        p->pos.x += offset.x;
        p->pos.y -= offset.y;
        *UI::getTStatRef(p, "text", "") = "";
        *draw->position = p->pos;
    }

    void cursoritemONRIGHTCLICK(uiCfunctionArguments)
    {
        auto item = UI::getStat(p, "item", { .itemp = nullptr })->itemp;
        if (item == nullptr) return;

        glm::vec2 vel = glm::normalize(globals::mouseBlockCoords(false) - Player::pos);
        if (isnan(vel.x) && isnan(vel.y)) vel = glm::vec2(0);
        vel *= 0.5;
        if (game::droppedItemSys->dropItem(Player::center, item->item, item->num, false, vel)) {
            item->num = 0;
        }
    }

    void textONRENDER(uiCfunctionArguments)
    {
        auto textC = ECS::getComponent<uiC>(p->children[0]);
        auto text = ECS::getComponent<drawC>(p->children[0]);
        if (p->hidden) {
            text->text = "";
            return;
        }
        text->color = draw->color;
        text->text = UI::getTStat(p, "text");
        if (UI::getStat(p, "colorOnHover", { .boolVal = false })->boolVal == true) {
            text->textScale += utils::approach(text->textScale, UI::getStat(p, "targetScale")->floatVal, 2);
        }
        textC->pos= *draw->position;
        float width = text::widthOfText(&text->text, text->textScale);
        float height = text::heightOfText(&text->text, text->textScale);
        p->size.x = width;
        p->size.y = height;
        if (UI::getStat(p, "centered", {.boolVal=false})->boolVal) {
            textC->pos.x -= (width / (float)globals::blocksizepx) / 2.0f;
            textC->pos.y -= (height / (float)globals::blocksizepx) / 2.0f;
        }
        else {
            draw->position->x += (width / 2) / (float)globals::blocksizepx;
            draw->position->y += (height / 2) / (float)globals::blocksizepx;
        }
        textC->hidden = p->hidden;
    }

    void textONHOVER(uiCfunctionArguments)
    {
        UI::getStat(p, "targetScale")->floatVal = UI::getStat(p, "hoveredScale", { .floatVal = UI::getStat(p, "textSize", {.floatVal = 1 })->floatVal })->floatVal;
        if (UI::getStat(p, "colorOnHover", { .boolVal = false })->boolVal == true) {
            ECS::getComponent<drawC>(p->children[0])->color = globals::textHightlightColor;
        }
    }

    void textONNOTHOVER(uiCfunctionArguments)
    {
        if (UI::getStat(p, "colorOnHover", { .boolVal = false })->boolVal == true) {
            UI::getStat(p, "targetScale")->floatVal = UI::getStat(p, "textSize", { .floatVal = 1 })->floatVal;
            ECS::getComponent<drawC>(p->children[0])->color = globals::textColor;
        }
    }

    void itemslotONHOVER(uiCfunctionArguments)
    {
        auto item = getStat(p, "item")->itemp;
        auto tt = ECS::getComponent<uiC>(gameLoop::tooltip);
        if (item != nullptr) {
            UI::setTStat(tt, "item", item->item);
            UI::getStat(tt, "num", {.intVal = 0})->intVal = item->num;
        }
        auto tc = ECS::getComponent<uiC>(p->children[1]);
        tc->hidden = true;
    }

    void fakeslotONHOVER(uiCfunctionArguments)
    {
        auto tt = ECS::getComponent<uiC>(gameLoop::tooltip);
        UI::setTStat(tt, "item", getTStat(p, "item", "empty"));
        UI::getStat(tt, "num", { .intVal = 0 })->intVal= getStat(p, "num", { .intVal = 0 })->intVal;
        auto tc = ECS::getComponent<uiC>(p->children[1]);
        tc->hidden = true;
    }

    void cursorONRENDER(uiCfunctionArguments)
    {
        auto lastx = UI::getStat(p, "lastx", { .floatVal = p->pos.x });
        auto lasty = UI::getStat(p, "lasty", { .floatVal = p->pos.y });
        auto cursor2 = ECS::getComponent<uiC>(p->children[0]);
        lastx->floatVal = p->pos.x;
        lasty->floatVal = p->pos.y;
        p->pos = globals::mouseBlockCoords(false, glm::vec2(-1, 1), glm::vec2(-1, 1));
        p->pos.x += (p->pos.x - lastx->floatVal)/6;
        p->pos.y += (p->pos.y - lasty->floatVal)/6;
        p->pos.x += p->size.x / 2;
        p->pos.y -= p->size.y / 2;
        cursor2->pos = p->pos;
        *draw->position = p->pos;
        draw->tex = "cursorborder";
        p->hidden = !game::showCursor;
        cursor2->hidden = p->hidden;
    }

    void hpbarONRENDER(uiCfunctionArguments)
    {
        draw->tex = "hpbar1";
        int ent = UI::getStat(p, "entity", { .intVal = -1 })->intVal;
        auto ePhys = ECS::getComponent<physicsC>(ent);
        auto eMob = ECS::getComponent<mobC>(ent);
        if (ent == -1 || ePhys == nullptr || eMob == nullptr || eMob->hpbar == -1) {
            UI::deleteElement(entity);
            return;
        }
        if (eMob->hp == eMob->maxhp || eMob->hp < 0) {
            p->hidden = true;
            return;
        }
        p->hidden = false;
        auto front = ECS::getComponent<uiC>(p->children[0]);
        auto frontDraw = ECS::getComponent<drawC>(p->children[0]);

        p->pos = *ePhys->position;
        p->pos.y -= ePhys->size.y / 2.0f + 0.6;
        p->size = { 2, 2 };
        
        front->pos = p->pos;
        front->size = p->size;
        
        * draw->position = p->pos;
        float ratio = eMob->hp / (float)eMob->maxhp;
        front->size.x *= ratio;
        front->pos.x = p->pos.x - p->size.x / 2 + front->size.x / 2;
        frontDraw->color = draw->color = glm::vec3(1 - ratio, ratio, 0);
        draw->color *= 0.3;
    }

    void pickuptextONRENDER(uiCfunctionArguments)
    {
        p->pos.y += 0.015f;
        draw->textScale += utils::approach(draw->textScale, 1, 5);
        draw->opacity -= 0.008;
        int num = UI::getStat(p, "num", { .intVal = 0 })->intVal;
        auto lastNum = UI::getStat(p, "lastNum", { .intVal = 0 });
        if (lastNum->intVal != num) {
            draw->opacity = 1;
        }
        lastNum->intVal = num;
        if (draw->opacity < 0) {
            UI::deleteElement(entity);
            return;
        }
        std::string text = UI::getTStat(p, "text", "") + " (" + std::to_string(num) + ")";
        draw->text = text;
        draw->position->x -= (text::widthOfText(&draw->text, draw->textScale) / 2.0f) / globals::blocksizepx;
    }

    void textboxONRENDER(uiCfunctionArguments)
    {
        auto backp = ECS::getComponent<uiC>(p->children[0]);
        auto textp = ECS::getComponent<uiC>(p->children[1]);
        std::string text = UI::getTStat(p, "text");
        float textSize = UI::getStat(textp, "textSize", { .floatVal = 0 })->floatVal;
        
        p->size.x = glm::clamp(p->size.x, UI::getStat(p, "minwidth", { .floatVal = 0 })->floatVal, UI::getStat(p, "maxwidth", { .floatVal = 8121739 })->floatVal);
        text = text::cutOffTextAt(&text, p->size.x);
        UI::setTStat(textp, "text", text);

        p->size.y = text::heightOfText(&text, textSize);
        p->size.y = glm::clamp(p->size.y, UI::getStat(p, "minheight", { .floatVal = 0 })->floatVal, UI::getStat(p, "maxheight", { .floatVal = 8121739 })->floatVal);
        
        textp->pos.x = p->pos.x - p->size.x / 2;
        textp->pos.y = p->pos.y + p->size.y / 2 - text::heightOfLine(&text, 1, textSize);

        p->size += UI::getStat(p, "padding", { .floatVal = 0 })->floatVal/2;

        float offset = 0;
        for (int i = 2; i < p->children.size(); i++) {
            auto childP = ECS::getComponent<uiC>(p->children[i]);
            auto childTextp = ECS::getComponent<uiC>(childP->children[0]);
            std::string btntext = UI::getTStat(childTextp, "text");
            float width = text::widthOfText(&btntext, UI::getStat(childTextp, "textSize")->floatVal);
            childP->pos = p->pos - glm::vec2(p->size.x / 2.0f - offset - width/2, p->size.y / 2.0f);
            offset += width * 1.2f;
        }

        p->size += UI::getStat(p, "padding", { .floatVal = 0 })->floatVal/2;

        backp->pos = p->pos;
        backp->size = p->size;
        backp->hidden = p->hidden;
    }

    void backONRENDER(uiCfunctionArguments)
    {
        auto middle = ECS::getComponent<uiC> (p->children[0]);
        middle->hidden = p->hidden;
        middle->size = p->size;
        middle->pos = p->pos;
        
        auto left = ECS::getComponent<uiC>(p->children[1]);
        left->hidden = p->hidden;
        left->pos = p->pos;
        left->pos.x -= p->size.x / 2;
        left->size.y = p->size.y;
        
        auto top = ECS::getComponent<uiC>(p->children[2]);
        top->hidden = p->hidden;
        top->pos = p->pos;
        top->pos.y += p->size.y / 2;
        top->size.x = p->size.x;
        
        auto right = ECS::getComponent<uiC>(p->children[3]);
        right->hidden = p->hidden;
        right->pos = p->pos;
        right->pos.x += p->size.x / 2;
        right->size.y = p->size.y;

        auto bottom = ECS::getComponent<uiC>(p->children[4]);
        bottom->hidden = p->hidden;
        bottom->pos = p->pos;
        bottom->pos.y -= p->size.y / 2;
        bottom->size.x = p->size.x;

        auto lefttop = ECS::getComponent<uiC>(p->children[5]);
        lefttop->hidden = p->hidden;
        lefttop->pos = p->pos;
        lefttop->pos.x -= p->size.x / 2;
        lefttop->pos.y += p->size.y / 2;
        auto righttop = ECS::getComponent<uiC>(p->children[6]);
        righttop->hidden = p->hidden;
        righttop->pos = p->pos;
        righttop->pos.x += p->size.x / 2;
        righttop->pos.y += p->size.y / 2;
        auto rightbot = ECS::getComponent<uiC>(p->children[7]);
        rightbot->hidden = p->hidden;
        rightbot->pos = p->pos;
        rightbot->pos.x += p->size.x / 2;
        rightbot->pos.y -= p->size.y / 2;
        auto leftbot = ECS::getComponent<uiC>(p->children[8]);
        leftbot->hidden = p->hidden;
        leftbot->pos = p->pos;
        leftbot->pos.x -= p->size.x / 2;
        leftbot->pos.y -= p->size.y / 2;
    }

    void buffONRENDER(uiCfunctionArguments)
    {
        auto buff = UI::getStat(p, "buff", { .buffp = nullptr })->buffp;
        if (buff == nullptr) return;
        draw->tex = buffs::buffs[buff->name].tex;
    }

    void buffONRIGHTCLICK(uiCfunctionArguments)
    {
        auto buff = UI::getStat(p, "buff", { .buffp = nullptr })->buffp;
        if (buff == nullptr) return;
        if (!buffs::buffs[buff->name].endable) return;
        buff->name = "nothing";
        buff->countdown = -1;
    }

    void buffONHOVER(uiCfunctionArguments)
    {
        auto buff = UI::getStat(p, "buff", { .buffp = nullptr })->buffp;
        std::string text = buff->name;
        if (buff->countdown > 0) {
            text += "\n" + std::to_string(buff->countdown / 60) + "s";
        }
        std::string description = buffs::buffs[buff->name].description;
        if (description != "") {
            text += "\n" + description;
        }
        if (buff->name != "nothing") {
            UI::setTStat(ECS::getComponent<uiC>(gameLoop::cursoritem), "text", text);
        }
    }

    void bossbarONRENDER(uiCfunctionArguments)
    {
        draw->tex = "bosshp";
        int ent = UI::getStat(p, "entity", { .intVal = -1 })->intVal;
        auto eMob = ECS::getComponent<mobC>(ent);
        if (ent == -1 || eMob == nullptr || eMob->hpbar == -1) {
            UI::deleteElement(entity);
            return;
        }
        auto front = ECS::getComponent<uiC>(p->children[0]);
        auto frontDraw = ECS::getComponent<drawC>(p->children[0]);
        auto head = ECS::getComponent<uiC>(p->children[1]);

        head->pos = front->pos = p->pos = glm::vec2(0, 100);
        front->size = glm::vec2(1000, 1000);
        p->size = glm::vec2(900, 40);

        head->size = glm::vec2(52, 52);
        head->pos.x -= p->size.x / 2 + 17;
        head->pos.y -= 3;


        float ratio = eMob->hp / (float)eMob->maxhp;
        p->pos.x -= p->size.x * (1 - ratio)/2;
        p->size.x *= ratio;


    }

    void radialONRENDER(uiCfunctionArguments)
    {
        *draw->position = globals::mouseBlockCoords();
    }

    void craftableitemONHOVER(uiCfunctionArguments)
    {
        int craftable = UI::getCraftable(&UI::getStat(p, "craftable")->intVal);
        auto tt = ECS::getComponent<uiC>(gameLoop::tooltip);
        if (craftable >= 0) {
            auto recipe = &items::recipes[craftable];
            UI::setTStat(tt, "item", recipe->craftedItem);
            UI::getStat(tt, "num", { .intVal = 0 })->intVal = recipe->craftedAmount;
        }
        else {
            UI::setTStat(tt, "item", "empty");
        }
        auto tc = ECS::getComponent<uiC>(p->children[1]);
        tc->hidden = true;
    }

    void itemslotONNOTHOVER(uiCfunctionArguments) {
    }

    void tooltipONRENDER(uiCfunctionArguments) {
        auto backc = ECS::getComponent<uiC>(p->children[0]);
        auto tc = ECS::getComponent<uiC>(p->children[1]);
        std::string* item = UI::getTStatRef(p, "item", "empty");

        if (*item != "empty") {
            backc->hidden = false;

            std::string text = "\\c"+items::getRarityColorString(items::getInfo(*item)->rarity);
            text += *item;
            int num = UI::getStat(p, "num", { .intVal = 0 })->intVal;
            if (num > 1) {
                text += " (" + std::to_string(num) + ")";
            }
            text += "\n\\c255255255";
            createTooltip(*item, &text);
            UI::setTStat(tc, "text", text);

            float textsize = UI::getStat(tc, "textSize", { .floatVal = 1 })->floatVal;

            backc->size.x = text::widthOfText(&text, textsize);
            backc->size.y = text::lineHeight * text::numLines(&text) * textsize;
            backc->pos = globals::mouseBlockCoords(false);
            if (backc->pos.x > Player::pos.x) {
                backc->pos += glm::vec2(-backc->size.x / globals::blocksizepx, -backc->size.y / globals::blocksizepx / 2);
            }
            backc->pos += glm::vec2(backc->size.x / globals::blocksizepx / 2, -backc->size.y / globals::blocksizepx / 2);

            tc->pos = backc->pos;
            tc->pos.x -= backc->size.x / 2.0f / (float)globals::blocksizepx;
            tc->pos.y += (backc->size.y / 2.0f) / (float)globals::blocksizepx;
            tc->pos.y -= (text::lineHeight * textsize) / (float)globals::blocksizepx;
            tc->hidden = backc->hidden;

            backc->size /= globals::blocksizepx;

            backc->size += UI::getStat(p, "padding")->floatVal;
            *draw->position = backc->pos;
            *item = "empty";
        }
        else {
            backc->hidden = true;
            UI::setTStat(tc, "text", UI::getTStat(p, ""));
            tc->hidden = backc->hidden;
        }
    }

    void displayONRENDER(uiCfunctionArguments)
    {
        if (UI::getTStatV(p, "tex") == "item") {
            draw->tex = items::getInfo(UI::getTStat(p, "item"))->tex;
        }
        else {
            draw->tex = *UI::getTStatRef(p, "tex");
        }
    }

    void resourcebarONRENDER(uiCfunctionArguments)
    {
        float shown = *UI::getStat(p, "value")->floatValp / (float)UI::getStat(p, "step")->intVal;
        float opacity = shown;
        float itemsize = p->stats["itemsize"].floatVal;
        int sizex = p->stats["sizex"].intVal;
        float sizey = ceil(shown / sizex);
        if (shown < sizex) {
            sizex = shown;
        }
        p->size.x = sizex * p->stats["itemsize"].floatVal * 1.1;
        p->size.y = sizey * p->stats["itemsize"].floatVal * 1.1;
        
        if (p->hidden) shown = 0;
        for (int i = 0; i < p->children.size(); i++) {
            ECS::getComponent<uiC>(p->children[i])->hidden = shown < i - 1;
            ECS::getComponent<drawC>(p->children[i])->opacity = glm::clamp(opacity, 0.0f, 1.0f);
            opacity--;
        }

        auto firstchild = ECS::getComponent<uiC>(p->children[0]);
        p->pos = firstchild->pos + glm::vec2(p->size.x / 2, -p->size.y / 2) - glm::vec2(firstchild->size.x/2, -firstchild->size.y/2);
    }

    void resourcebarONHOVER(uiCfunctionArguments)
    {
        std::string text = UI::getTStat(p, "label", "");
        int value = *UI::getStat(p, "value")->floatValp;
        int max = UI::getStat(p, "max")->floatVal;
        text += " (" + std::to_string(value) + "/" + std::to_string(max) + ")";
        UI::setTStat(ECS::getComponent<uiC>(gameLoop::cursoritem), "text", text);
    }

    void dragfloatONCLICK(uiCfunctionArguments)
    {
        auto slider = ECS::getComponent<uiC>(p->children[0]);
        auto paddle = ECS::getComponent<uiC>(p->children[1]);

        float mousex = globals::mouseBlockCoords(false).x;

        glm::vec2 sliderpos = slider->pos / glm::vec2(globals::resX, globals::resY);
        glm::vec2 slidersize = (slider->size / glm::vec2(globals::resX, globals::resY)) / globals::fullScale;

        float max = UI::getStat(p, "max", { .floatVal = 1 })->floatVal;
        float min= UI::getStat(p, "min", { .floatVal = 0 })->floatVal;

        sliderpos *= ((glm::vec2)Layers::trueBsOnScr / glm::vec2(2));
        sliderpos -= camera::pos;
        UI::applyAnchor(&sliderpos, p->anchor);
        float offset = abs((sliderpos.x - slidersize.x / 2) - mousex);
        offset /= slidersize.x;
        float val = min + offset * (max-min);
        if (val > max - UI::getStat(p, "roundAt")->floatVal) {
            val = max;
        }
        if (val <= min + UI::getStat(p, "roundAt")->floatVal) {
            val = min;
        }
        *UI::getStat(p, "ref")->floatValp = val;
    }

    void dragfloatONRENDER(uiCfunctionArguments)
    {
        auto slider = ECS::getComponent<uiC>(p->children[0]);
        auto paddle = ECS::getComponent<uiC>(p->children[1]);
        float max = UI::getStat(p, "max", { .floatVal = 1 })->floatVal;
        float min = UI::getStat(p, "min", { .floatVal = 0 })->floatVal;
        float proc = ( * UI::getStat(p, "ref")->floatValp - min) / (max - min);
        paddle->pos.y = slider->pos.y;
        paddle->pos.x = slider->pos.x - slider->size.x / 2;
        paddle->pos.x += slider->size.x * proc;

        UI::setTStat(ECS::getComponent<uiC>(p->children[3]), "text", std::to_string(*UI::getStat(p, "ref")->floatValp));
    }

    void toggleONRENDER(uiCfunctionArguments)
    {
        if (*UI::getStat(p, "ref")->boolValp) {
            UI::setTStat(ECS::getComponent<uiC>(p->children[0]), "tex", "toggleon");
        }
        else {
            UI::setTStat(ECS::getComponent<uiC>(p->children[0]), "tex", "toggleoff");
        }
    }

    void toggleONCLICK(uiCfunctionArguments)
    {
        *UI::getStat(p, "ref")->boolValp = !*UI::getStat(p, "ref")->boolValp;
    }

    void buttonONCLICK(uiCfunctionArguments)
    {
        (*UI::getStat(p, "func")->funcp)(p);
    }

    void buttonONRENDER(uiCfunctionArguments)
    {
        auto tc = ECS::getComponent<uiC>(p->children[0]);
        std::string text = UI::getTStat(p, "text");
        auto textp = UI::getStat(p, "textp", { .stringp = nullptr })->stringp;
        if (textp != nullptr) text = *textp;
        tc->hidden = p->hidden;
        if (UI::getStat(p, "fitText")->boolVal) {
            float textsize = ECS::getComponent<drawC>(p->children[0])->textScale;
            p->size.x = text::widthOfText(&text, textsize);
            p->size.y = text::lineHeight * text::numLines(&text) * textsize;
            p->size += UI::getStat(p, "padding")->floatVal;
        }
        tc->pos = p->pos;
        UI::setTStat(tc, "text", text);
    }

    void dragintONCLICK(uiCfunctionArguments)
    {
        auto slider = ECS::getComponent<uiC>(p->children[0]);
        auto paddle = ECS::getComponent<uiC>(p->children[1]);

        float mousex = globals::mouseBlockCoords(false).x;

        glm::vec2 sliderpos = slider->pos / glm::vec2(globals::resX, globals::resY);
        glm::vec2 slidersize = (slider->size / glm::vec2(globals::resX, globals::resY)) / globals::fullScale;

        sliderpos *= ((glm::vec2)Layers::trueBsOnScr / glm::vec2(2));
        sliderpos -= camera::pos;
        UI::applyAnchor(&sliderpos, p->anchor);
        float offset = abs((sliderpos.x - slidersize.x / 2) - mousex);
        offset /= slidersize.x;
        float val = offset * UI::getStat(p, "max")->intVal;
        if (val > UI::getStat(p, "max")->intVal - UI::getStat(p, "roundAt")->intVal) {
            val = UI::getStat(p, "max")->intVal;
        }
        if (val <= UI::getStat(p, "roundAt")->intVal) {
            val = 0;
        }
        *UI::getStat(p, "ref")->intValp = val;
    }

    void dragintONRENDER(uiCfunctionArguments)
    {
        auto slider = ECS::getComponent<uiC>(p->children[0]);
        auto paddle = ECS::getComponent<uiC>(p->children[1]);

        float proc = *UI::getStat(p, "ref")->intValp / (float)UI::getStat(p, "max")->intVal;
        paddle->pos.y = slider->pos.y;
        paddle->pos.x = slider->pos.x - slider->size.x / 2;
        paddle->pos.x += slider->size.x * proc;

        UI::setTStat(ECS::getComponent<uiC>(p->children[3]), "text", std::to_string(*UI::getStat(p, "ref")->intValp));
    }

    void containerONRENDER(uiCfunctionArguments)
    {
        auto back = ECS::getComponent<uiC>(p->children[0]);
        //search thru all children to find the point most to the left, right, top and bottom then resize and reposition the container to fit that
        float left = childMostToTheLeft(p, 123141111, false, 1);
        float right = childMostToTheRight(p, -1311131, false, 1);
        float top = childMostToTheTop(p, -1311131, false, 1);
        float bot = childMostToTheBottom(p, 123141111, false, 1);

        back->size.x = abs(right - left);
        back->size.y = abs(top - bot);
        
        back->size.x = glm::clamp(back->size.x, UI::getStat(p, "minwidth", { .floatVal = 0 })->floatVal, UI::getStat(p, "maxwidth", { .floatVal = 8121739 })->floatVal);
        back->size.y = glm::clamp(back->size.y, UI::getStat(p, "minheight", { .floatVal = 0 })->floatVal, UI::getStat(p, "maxheight", { .floatVal = 8121739 })->floatVal);

        if (UI::getStat(p, "move", { .boolVal = true })->boolVal) {
            back->pos.x = left + back->size.x / 2.0f;
            back->pos.y = bot + back->size.y / 2.0f;
        }

        back->size += UI::getStat(p, "padding", {.floatVal = 0})->floatVal;
        
        if (back->anchor != anchorNONE) {
            back->size *= globals::blocksizepx;
        }

        auto color = UI::getStat(p, "colorp", { .vec3p = nullptr })->vec3p;
        if (color != nullptr) {
            draw->color = *color;
        }
    }

    void fakeslotONRENDER(uiCfunctionArguments)
    {
        auto dc = ECS::getComponent<uiC>(p->children[0]);
        auto tc = ECS::getComponent<uiC>(p->children[1]);

        if (UI::getStat(p, "inheritHidden")->boolVal == true) {
            p->hidden = ECS::getComponent<uiC>(p->parent)->hidden;
        }

        UI::setTStat(tc, "text", std::to_string(UI::getStat(p, "num")->intVal));
        tc->pos.x = p->pos.x - p->size.x * 0.2;
        tc->pos.y = p->pos.y - p->size.y / 4;

        tc->hidden = p->hidden;

        dc->pos = p->pos;
        dc->size = p->size * glm::vec2(0.6f);
        UI::setTStat(dc, "item", UI::getTStat(p, "item"));
        dc->hidden = p->hidden;

        draw->tex = UI::getTStat(p, "tex", "invempty");
    }

    void uiCfunc_captureInput(uiC* p)
    {
        UI::captureInput(UI::getStat(p, "ref")->stringp);
    }

    void uiCfunc_startGame(uiC* p)
    {
        gameLoop::newMap = false;
        map::name.clear();
        if (map::load()) {
            game::changeSceneTo(GAME);
        }
    }

    void uiCfunc_createWorld(uiC* p)
    {
        map::tmpname = map::name;
        StartMenu::openMenu(StartMenu::loadingScreen);
        map::worldgenthread = std::thread(map::createWorld);
    }

    void uiCfunc_saveGame(uiC* p)
    {
        map::save();
        Player::save();
    }

    void uiCfunc_toggleSettings(uiC* p)
    {
        gameLoop::toggleSettings();
    }

    void uiCfunc_openSettingsMenu(uiC* p)
    {
        gameLoop::openMenu(UI::getStat(p, "menu")->intVal);
    }

    void uiCfunc_saveAndExitGame(uiC* p)
    {
        if (map::save() && Player::save()) {
            game::changeSceneTo(STARTMENU);
        }
        else {
            std::cout << "\nweird error when saving\n";
        }
    }

    void uiCfunc_exitGame(uiC* p)
    {
        game::swapTo = STARTMENU;
    }

    void uiCfunc_exit(uiC* p)
    {
        glfwSetWindowShouldClose(globals::window, GLFW_TRUE);
    }

    void uiCfunc_loadSpecificWorld(uiC* p)
    {
        int index = UI::getStat(p, "index")->intVal;
        if (StartMenu::availableWorlds.size() > 0) {
            if (index >= 0 && index < StartMenu::availableWorlds.size()) {
                map::tmpname = StartMenu::availableWorlds[index];
                game::changeSceneTo(GAME);
            }
        }
    }

    void uiCfunc_loadPlayer(uiC* p)
    {
        Inventory::setupInventory(&PlayerHotbar, { 11,1 });
        hotbar = new Inventory({ 100, -100 }, { 11, 1 }, &PlayerHotbar, false, gameLoop::inventoryC);

        Inventory::setupInventory(&PlayerInventory, { 11,6 });
        inventory = new Inventory({ inventoryPos.x, inventoryPos.y - 110 }, { 11, 6 }, &PlayerInventory, true, gameLoop::inventoryC);
        inventory->toggle(1);

        Inventory::setupInventory(&defaultChest, { 10,4 });
        chest = new Inventory({ chestInvPos, {10,4}, &defaultChest, true, gameLoop::chestInvC });
        openChest = -1;
        Player::heartcrystals = 0;
        Player::boomerangsout = 0;
        Player::currsummons = 0;
        Player::load();
    }

    void uiCfunc_createPlayer(uiC* p)
    {
        PlayerHotbar.clear();
        PlayerInventory.clear();
        Inventory::setupInventory(&PlayerHotbar, { 10,1 });
        Inventory::setupInventory(&PlayerInventory, { 10,4 });
        
        auto itemit = items::info.begin();
        for (int i = 0; i < PlayerHotbar.size(); i++) {
            if (itemit == items::info.end()) break;
            PlayerHotbar.at(i) = std::make_shared<InventoryItem>( InventoryItem( { itemit->first, 100, i }));
            itemit++;
        }
        for (int i = 0; i < PlayerInventory.size(); i++) {
            if (itemit == items::info.end()) break;
            PlayerInventory[i] = std::make_shared<InventoryItem>(InventoryItem({ itemit->first, 100, i }));
            itemit++;
        }
        Player::heartcrystals = 0;
        Player::boomerangsout = 0;
        Player::currsummons = 0;
        Player::save();
        PlayerHotbar.clear();
        PlayerInventory.clear();
        StartMenu::refreshPlayers();
        StartMenu::openMenu(StartMenu::playerSelectContainer);
    }

    void uiCfunc_loadSpecificPlayer(uiC* p)
    {
        int index = UI::getStat(p, "index")->intVal;
        if (StartMenu::availablePlayers.size() > 0) {
            if (index >= 0 && index < StartMenu::availablePlayers.size()) {
                Player::name = StartMenu::availablePlayers[index];
                Player::load();
                StartMenu::openMenu(p);
                Player::name = StartMenu::availablePlayers[index];
            }
        }
    }

    void uiCfunc_toggleFullscreen(uiC* p)
    {
        game::toggleFullscreen();
    }

    void uiCfunc_nextResolution(uiC* p)
    {
        game::nextResolution();
    }

    void applyAnchor(glm::vec2* pos, uiAnchor anchor)
    {
        switch (anchor) {
        case anchorNONE:
            *pos += globals::mouseBlockCoords(false);
            break;
        case anchorMID:
            break;
        case anchorLEFT:
            pos->x -= Layers::trueBsOnScr.x / 2.0f;
            break;
        case anchorTOPLEFT:
            pos->x -= Layers::trueBsOnScr.x / 2.0f;
            pos->y += Layers::trueBsOnScr.y / 2.0f;
            break;
        case anchorTOP:
            pos->y += Layers::trueBsOnScr.y / 2.0f;
            break;
        case anchorTOPRIGHT:
            pos->x += Layers::trueBsOnScr.x / 2.0f;
            pos->y += Layers::trueBsOnScr.y / 2.0f;
            break;
        case anchorRIGHT:
            pos->x += Layers::trueBsOnScr.x / 2.0f;
            break;
        case anchorBOTRIGHT:
            pos->x += Layers::trueBsOnScr.x / 2.0f;
            pos->y -= Layers::trueBsOnScr.y / 2.0f;
            break;
        case anchorBOT:
            pos->y -= Layers::trueBsOnScr.y / 2.0f;
            break;
        case anchorBOTLEFT:
            pos->x -= Layers::trueBsOnScr.x / 2.0f;
            pos->y -= Layers::trueBsOnScr.y / 2.0f;
            break;
        }
    }
    void hideChildren(uiC* p, int state, bool self)
    {
        if (self) {
            if (state == -1) {
                p->hidden = !p->hidden;
            }
            else {
                p->hidden = state;
            }
        }
        for (int i = 0; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            if (state == -1) {
                c->hidden = !c->hidden;
            }
            else {
                c->hidden = state;
            }
            hideChildren(c, state);
        }
    }
    void setChildrenInteractable(uiC* p, bool state)
    {
        for (int i = 0; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            c->hidden = state;
            setChildrenInteractable(c, state);
        }
    }
    bool addLetter(int key)
    {
        if (capturingInput && capturedInput != nullptr) {
            if (key == GLFW_KEY_BACKSPACE) {
                if (capturedInput->size() >= 1) {
                    capturedInput->pop_back();
                }
            }else if (key == GLFW_KEY_ENTER) {
                *capturedInput += '\n';
                return true;
            }
            else if (key == GLFW_KEY_ESCAPE) {
                captureInput(nullptr, false);
            }
            else if(key >= ' ' && key <= 'Z') {
                if (input::shift) {
                    *capturedInput += toupper((char)key);
                }
                else {
                    *capturedInput += tolower((char)key);
                }
                return true;
            }
        }
        return false;
    }
    void captureInput(std::string* string, bool yes)
    {
        if (yes) {
            capturedInput = string;
            capturingInput = true;
            return;
        }
        capturedInput = nullptr;
        capturingInput = false;
    }
    float childMostToTheLeft(uiC* p, float currMax, bool anchored, int ignore)
    {
        if (anchored) {
            for (int i = ignore; i < p->children.size(); i++) {
                auto c = ECS::getComponent<uiC>(p->children[i]);
                if (!c->hidden) {
                    if (c->pos.x - c->size.x / 2.0f < currMax) {
                        currMax = c->pos.x - c->size.x / 2.0f;
                    }
                }
                currMax = childMostToTheLeft(c, currMax, true);
            }
        }else for (int i = ignore; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            auto cd = ECS::getComponent<drawC>(p->children[i]);
            if (!c->hidden) {
                if (cd->position->x - cd->size.x / 2.0f < currMax) {
                    currMax = cd->position->x - cd->size.x / 2.0f;
                }
            }
            currMax = childMostToTheLeft(c, currMax);
        }


        return currMax;
    }
    float childMostToTheRight(uiC* p, float currMax, bool anchored, int ignore)
    {
        if (anchored) {
            for (int i = ignore; i < p->children.size(); i++) {
                auto c = ECS::getComponent<uiC>(p->children[i]);
                if (!c->hidden) {
                    if (c->pos.x + c->size.x / 2.0f > currMax) {
                        currMax = c->pos.x + c->size.x / 2.0f;
                    }
                }
                currMax = childMostToTheRight(c, currMax, true);
            }
            return currMax;
        }
        for (int i = ignore; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            auto cd = ECS::getComponent<drawC>(p->children[i]);
            if (!c->hidden) {
                if (cd->position->x + cd->size.x / 2.0f > currMax) {
                    currMax = cd->position->x + cd->size.x / 2.0f;
                }
            }
            currMax = childMostToTheRight(c, currMax);
        }
        return currMax;
    }
    float childMostToTheTop(uiC* p, float currMax, bool anchored, int ignore)
    {
        if (anchored) {
            for (int i = ignore; i < p->children.size(); i++) {
                auto c = ECS::getComponent<uiC>(p->children[i]);
                if (!c->hidden) {
                    if (c->pos.y + c->size.y / 2.0f > currMax) {
                        currMax = c->pos.y + c->size.y / 2.0f;
                    }
                }
                currMax = childMostToTheTop(c, currMax, true);
            }
            return currMax;
        }
        for (int i = ignore; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            auto cd = ECS::getComponent<drawC>(p->children[i]);
            if (!c->hidden) {
                if (cd->position->y + cd->size.y / 2.0f > currMax) {
                    currMax = cd->position->y + cd->size.y / 2.0f;
                }
            }
            currMax = childMostToTheTop(c, currMax);
        }
        return currMax;
    }
    float childMostToTheBottom(uiC* p, float currMax, bool anchored, int ignore)
    {
        if (anchored) {
            for (int i = ignore; i < p->children.size(); i++) {
                auto c = ECS::getComponent<uiC>(p->children[i]);
                if (!c->hidden) {
                    if (c->pos.y - c->size.y / 2.0f < currMax) {
                        currMax = c->pos.y - c->size.y / 2.0f;
                    }
                }
                currMax = childMostToTheBottom(c, currMax, true);
            }
            return currMax;
        }
        for (int i = ignore; i < p->children.size(); i++) {
            auto c = ECS::getComponent<uiC>(p->children[i]);
            auto cd = ECS::getComponent<drawC>(p->children[i]);
            if (!c->hidden) {
                if (cd->position->y - cd->size.y / 2.0f < currMax) {
                    currMax = cd->position->y - cd->size.y / 2.0f;
                }
            }
            currMax = childMostToTheBottom(c, currMax);
        }
        return currMax;
    }
    void saveInvItemVector(std::vector<std::shared_ptr<InventoryItem>>* vec, std::ofstream* file)
    {
        int vecSize = vec->size();
        file->write((char*)&vecSize, sizeof(vecSize));

        for (int i = 0; i < vecSize; i++) {
            int itemSize = vec->at(i)->item.length();
            file->write((char*)(&itemSize), sizeof(itemSize));
            file->write(vec->at(i)->item.c_str(), itemSize);
            file->write((char*)(&vec->at(i)->num), sizeof(vec->at(i)->num));
            file->write((char*)(&vec->at(i)->pos), sizeof(vec->at(i)->pos));
        }

    }
    void loadInvItemVector(std::vector<std::shared_ptr<InventoryItem>>* vec, std::ifstream* file)
    {
        int vecSize = vec->size();
        file->read((char*)&vecSize, sizeof(vecSize));

        for (int i = 0; i < vecSize; i++) {
            InventoryItem tmp;
            int itemnameSize;
            file->read((char*)(&itemnameSize), sizeof(itemnameSize));
            char c;
            for (int h = 0; h < itemnameSize; h++) {
                file->get(c);
                tmp.item += c;
            }
            file->read((char*)(&tmp.num), sizeof(tmp.num));
            file->read((char*)(&tmp.pos), sizeof(tmp.pos));
            vec->push_back(std::make_shared<InventoryItem>(tmp));
        }
    }
    void saveInvItem(InventoryItem* item, std::ofstream* file)
    {
        int itemSize = item->item.length();
        file->write((char*)(&itemSize), sizeof(itemSize));
        file->write(item->item.c_str(), itemSize);
        file->write((char*)(&item->num), sizeof(item->num));
        file->write((char*)(&item->pos), sizeof(item->pos));
    }
    void loadInvItem(InventoryItem* item, std::ifstream* file)
    {
        int itemnameSize = item->item.length();
        file->read((char*)(&itemnameSize), sizeof(itemnameSize));
        item->item = "";
        char c;
        for (int i = 0; i < itemnameSize; i++) {
            file->get(c);
            item->item += c;
        }
        file->read((char*)(&item->num), sizeof(item->num));
        file->read((char*)(&item->pos), sizeof(item->pos));
    }
    void createTooltip(std::string item, std::string* text)
    {
        auto info = &items::info[item];
        itemStat idfk = {.valueFloat = -19231.0f };

        if (info->families.count(if_BLOCK) >= 1) {
            *text += "can be placed";
            *text += "\n";
        }

        if (info->inRecipes.size() >= 1) {
            *text += "material";
            *text += "\n";
        }

        float tmpf = items::getStat(item, "damage", idfk).valueFloat;
        if (tmpf != idfk.valueFloat) {
            *text += std::to_string(tmpf) + " damage";
            *text += "\n";
        }
        tmpf = items::getStat(item, "crit", idfk).valueFloat;
        if (tmpf != idfk.valueFloat) {
            *text += std::to_string(tmpf) + " crit";
            *text += "\n";
        }
        tmpf = items::getStat(item, "knockback", idfk).valueFloat;
        if (tmpf != idfk.valueFloat) {
            *text += std::to_string(tmpf) + " knockback";
            *text += "\n";
        }
        idfk.valueInt = idfk.valueFloat;
        int tmpi = items::getStat(item, "defense", idfk).valueInt;
        if (tmpi != idfk.valueInt) {
            *text += std::to_string(tmpi) + " defense";
            *text += "\n";
        }
        std::string tmpstr = items::getStatT(item, "projectile", "empty").data();
        if (tmpstr != "empty") {
            *text += "shoots a " + tmpstr;
        }
    }
}

void Inventory::setupInventory(std::vector<std::shared_ptr<InventoryItem>>* vec, glm::vec2 size)
{
    for (int row = 0; row < size.y; row++) {
        for (int col = 0; col < size.x; col++) {
            int ipos = col + row * size.x;
            InventoryItem tmp;
            tmp.item = "empty";
            tmp.num = 0;
            tmp.pos = ipos;
            vec->push_back(std::make_shared<InventoryItem>(tmp));
        }
    }
}

void Inventory::markAsSelected(int pos)
{
    for (auto i : entities) {
        UI::getStat(ECS::getComponent<uiC>(i), "selected", { .boolVal = false })->boolVal = false;
    }
    UI::getStat(ECS::getComponent<uiC>(entities[pos]), "selected", {.boolVal = false})->boolVal = true;

}

Inventory::Inventory(glm::vec2 pos, glm::vec2 ROWS, std::vector<std::shared_ptr<InventoryItem>>* ITEMS, bool hidden, int container, float scale)
{
    rows = ROWS;
    items = ITEMS;
    float w; // edge width¸
    float h = w = 140 * scale; // size
    float s = h * 0.7f; //vertical spacing
    uiStat textSize; textSize.floatVal = globals::fontSize;
    for (int row = 0; row < rows.y; row++) {
        for (int col = 0; col < rows.x; col++) {
            int ipos = col + row * rows.x;
            uiStat t2;
            t2.itemp = items->at(ipos).get();
            int slot = ECS::newEntity();
            entities.push_back(slot);
            UI::addElement(slot, ui_ITEMSLOT, pos + glm::vec2(s * col, 0), glm::vec2(w / 1.5f, h / 1.5f), container, { {"item", t2}, {"textSize", textSize} }, { }, hidden, anchorTOPLEFT);
            ECS::getComponent<uiC>(slot)->propagateClick = false;
        }
        pos += glm::vec2(0, -s);
    }
    this->size = rows;
}

void Inventory::setTo(std::vector<std::shared_ptr<InventoryItem>>* ITEMS)
{
    items = ITEMS;
    for (int i = 0; i < entities.size(); i++) {
        if (items->size() - i <= 0) return;
        auto p = ECS::getComponent<uiC>(entities[i]);
        UI::getStat(p, "item")->itemp = items->at(i).get();
    }
}

int Inventory::add(std::string item, int num, int spot)
{
    if (spot == -1) {
        for (int i = 0; i < items->size(); i++) {
            if (items->at(i)->item == item) {
                int maxStacks = items::info[items->at(i)->item].maxStacks;
                if (items->at(i)->num == maxStacks) continue;
                items->at(i)->num += num;
                if (items->at(i)->num > maxStacks) {
                    int overflow = items->at(i)->num - maxStacks;
                    items->at(i)->num = maxStacks;
                    return overflow;
                }
                return 0;
            }
        }
        for (int i = 0; i < items->size(); i++) {
            if (items->at(i)->item == "empty") {
                items->at(i)->num = num;
                items->at(i)->item = item;
                int maxStacks = items::info[items->at(i)->item].maxStacks;
                if (items->at(i)->num > maxStacks) {
                    int overflow = items->at(i)->num - maxStacks;
                    items->at(i)->num = maxStacks;
                    return overflow;
                }
                return 0;
            }
        }
    }
    else {
        items->at(spot)->item = item;
        items->at(spot)->num = num;
        return 0;
    }
    return num;
}

bool Inventory::has(std::string item, int num)
{
    int found = 0;
    for (int j = num; j > 0; j--) {
        for (int i = 0; i < items->size(); i++) {
            if (items->at(i)->item == item) {
                found += items->at(i)->num;
                break;
            }
        }
        if (found >= num) {
            return true;
        }
    }
    return false;
}

bool Inventory::remove(std::string item, int num)
{
    for (int j = num; j > 0; j--) {
        for (int i = 0; i < items->size(); i++) {
            if (items->at(i)->item == item) {
                items->at(i)->num--;
                return true;
                break;
            }
        }
    }
    return false;
}

void Inventory::toggle(int toggle)
{
    for (int i = 0; i < entities.size(); i++) {
        auto p = ECS::getComponent<uiC>(entities[i]);
        if (toggle == -1) {
            p->hidden = !p->hidden;
        }
        else {
            p->hidden = toggle;
        }
    }
}

bool Inventory::hidden()
{
    return ECS::getComponent<uiC>(entities[0])->hidden;
}

bool Inventory::contains(std::string item, int num)
{
    for (auto& i : *items) {
        if (i->item == item) {
            num -= i->num;
        }
        if (num <= 0) return true;
    }
    return false;
}

bool Inventory::take(std::string item, int num, bool doublecheck)
{
    if (doublecheck) {
        int found = 0;
        for (int i = 0; i < items->size(); i++) {
            if (items->at(i)->item == item) {
                found += items->at(i)->num;
            }
        }
        if (found < num) {
            return false;
        }
    }
    for (int i = 0; i < items->size(); i++) {
        if (items->at(i)->item == item) {
            if (items->at(i)->num <= num) {
                num -= items->at(i)->num;
                items->at(i)->num = 0;
                items->at(i)->item = "empty";
            }
            else {
                items->at(i)->num -= num;
                return true;
            }
            if (num <= 0) return true;
        }
    }
}

bool Inventory::hasRoomFor(std::string item)
{
    for (int i = 0; i < items->size(); i++) {
        if (items->at(i)->item == item){
            if (items->at(i)->num < items::info[items->at(i)->item].maxStacks) {
                return true;
            }
        }else
        if(items->at(i)->item == "empty") {
            return true;
        }
    }
    return false;
}

InventoryItem* Inventory::at(glm::vec2 pos)
{
    if (pos.x > size.x || pos.y > size.y) {
        return nullptr;
    }
    return items->at(pos.x + pos.y * size.x).get();
}
