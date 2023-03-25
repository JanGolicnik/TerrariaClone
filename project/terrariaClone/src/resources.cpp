#include "resources.h"

#include <iostream>

#include <globals.h>
#include <utils.h>
#include <textures.h>
#include <items.h>
#include <blocks.h>
#include <animations.h>
#include <world.h>
#include <enemyFunctions.h>
#include <background.h>
#include <enemies.h>
#include <ui.h>
#include <layers.h>
#include <particles.h>
#include <player.h>
#include <buffs.h>
#include <liquids.h>
#include <sounds.h>

namespace resources {
    void registerAssets()
    {
        blocks::addBlock("empty", "empty", false, false, "empty", "blocks", false, st_SINGLE, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("dirt", "dirt", true, true, "dirt", "blocks", true, st_BLOCK);
        blocks::addBlock("grass", "grass", true, true, "dirt", "blocks", true, st_BLOCK);
        blocks::addBlock("glass", "glass", true, true, "glass", "blocks", true, st_BLOCK);
        blocks::addBlock("stone", "stone", true, true, "stone", "blocks", true, st_BLOCK);
        blocks::addBlock("sand", "sand", true, true, "sand", "blocks", true, st_BLOCK);
        blocks::addBlock("wood", "wood", true, true, "wood", "blocks", true, st_BLOCK);
        blocks::addBlock("dirtwall", "dirtwall", true, true, "dirtwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("glasswall", "glasswall", true, true, "glasswall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("woodwall", "woodwall", true, true, "woodwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("stonewall", "stonewall", true, true, "stonewall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("trunk", "trunk", false, true, "wood", "blocks", false, st_SINGLE, { 1,1 }, 0, {}, false, false, false, false);
        blocks::addBlock("trunkbase1", "trunkbase1", false, true, "wood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("trunkbase2", "trunkbase2", false, true, "wood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("trunkbase3", "trunkbase3", false, true, "wood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("leaves", "leaves", false, true, "acorn", "blocks", false, st_SINGLE, glm::vec2(1), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("sapling", "sapling", false, true, "acorn", "blocks", false, st_SINGLE, { 1, 2 }, 0, { BConditions::nowall, BConditions::isntreplacablebelow, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("purpletorch", "purpletorch", false, true, "purpletorch", "blocks", false, st_TORCH, glm::vec2(1.0f, 1.0f), 0, { BConditions::isreplacable, BConditions::connected }, false, false, false, false);
        blocks::addBlock("torch", "torch", false, true, "torch", "blocks", false, st_TORCH, glm::vec2(1.0f, 1.0f), 0, { BConditions::isreplacable, BConditions::connected }, false, false, false, false);
        blocks::addBlock("chest", "chest", false, true, "chest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("door", "closeddoor", true, true, "door", "blocks", false, st_SINGLE, glm::vec2(1, 3), 0, { BConditions::solidontop, BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("doorleft", "opendoorleft", false, true, "door", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("doorright", "opendoorright", false, true, "door", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("anvil", "anvil", false, true, "anvil", "blocks", false, st_SINGLE, glm::vec2(2, 1), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("craftingbench", "craftingbench", false, true, "craftingbench", "blocks", false, st_SINGLE, glm::vec2(2, 1), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("table", "table", false, true, "table", "blocks", false, st_SINGLE, glm::vec2(4, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("furnace", "furnace", false, true, "furnace", "blocks", false, st_SINGLE, glm::vec2(3, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("gravestone", "gravestone", false, true, "gravestone", "blocks", false, st_SINGLE, glm::vec2(2, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("chairright", "chairright", false, true, "chair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("chairleft", "chairleft", false, true, "chair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("slimestatue", "slimestatue", false, true, "slimestatue", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ironore", "ironore", true, true, "ironore", "blocks", true, st_BLOCK);
        blocks::addBlock("copperore", "copperore", true, true, "copperore", "blocks", true, st_BLOCK);
        blocks::addBlock("corruptgrass", "corruptgrass", true, true, "dirt", "blocks", true, st_BLOCK);
        blocks::addBlock("junglegrass", "junglegrass", true, true, "mud", "blocks", true, st_BLOCK);
        blocks::addBlock("mud", "mud", true, true, "mud", "blocks", true, st_BLOCK);
        blocks::addBlock("ash", "ash", true, true, "ash", "blocks", true, st_BLOCK);
        blocks::addBlock("snow", "snow", true, true, "snow", "blocks", true, st_BLOCK);
        blocks::addBlock("borealwood", "borealwood", true, true, "borealwood", "blocks", true, st_BLOCK);
        blocks::addBlock("ebonwood", "ebonwood", true, true, "ebonwood", "blocks", true, st_BLOCK);
        blocks::addBlock("mahoganywood", "mahoganywood", true, true, "mahoganywood", "blocks", true, st_BLOCK);
        blocks::addBlock("stonebrick", "stonebrick", true, true, "stonebrick", "blocks", true, st_BLOCK);
        blocks::addBlock("ebonwoodwall", "ebonwoodwall", true, true, "ebonwoodwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("snowwall", "snowwall", true, true, "empty", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("mudwall", "mudwall", true, true, "empty", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("mahoganywoodwall", "mahoganywoodwall", true, true, "mahoganywoodwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("borealwoodwall", "borealwoodwall", true, true, "borealwoodwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("borealdoor", "borealdoor", true, true, "borealdoor", "blocks", false, st_SINGLE, glm::vec2(1, 3), 0, { BConditions::solidontop, BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("borealdoorleft", "borealdoorleft", false, true, "borealdoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("borealdoorright", "borealdoorright", false, true, "borealdoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("borealtable", "borealtable", false, true, "borealtable", "blocks", false, st_SINGLE, glm::vec2(4, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("borealworkbench", "borealworkbench", false, true, "borealworkbench", "blocks", false, st_SINGLE, glm::vec2(2, 1), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganydoor", "mahoganydoor", true, true, "mahoganydoor", "blocks", false, st_SINGLE, glm::vec2(1, 3), 0, { BConditions::solidontop, BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganydoorleft", "mahoganydoorleft", false, true, "mahoganydoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("mahoganydoorright", "mahoganydoorright", false, true, "mahoganydoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("mahoganytable", "mahoganytable", false, true, "mahoganytable", "blocks", false, st_SINGLE, glm::vec2(4, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganyworkbench", "mahoganyworkbench", false, true, "mahoganyworkbench", "blocks", false, st_SINGLE, glm::vec2(2, 1), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ebonwooddoor", "ebonwooddoor", true, true, "ebonwooddoor", "blocks", false, st_SINGLE, glm::vec2(1, 3), 0, { BConditions::solidontop, BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ebonwooddoorleft", "ebonwooddoorleft", false, true, "ebonwooddoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("ebonwooddoorright", "ebonwooddoorright", false, true, "ebonwooddoor", "blocks", false, st_SINGLE, glm::vec2(2, 3), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("ebonwoodtable", "ebonwoodtable", false, true, "ebonwoodtable", "blocks", false, st_SINGLE, glm::vec2(4, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ebonwoodworkbench", "ebonwoodworkbench", false, true, "ebonwoodworkbench", "blocks", false, st_SINGLE, glm::vec2(2, 1), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ebonwoodchairright", "ebonwoodchairright", false, true, "ebonwoodchair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganychairright", "mahoganychairright", false, true, "mahoganychair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("borealchairright", "borealchairright", false, true, "borealchair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("ebonwoodchairleft", "ebonwoodchairleft", false, true, "ebonwoodchair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganychairleft", "mahoganychairleft", false, true, "mahoganychair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("borealchairleft", "borealchairleft", false, true, "borealchair", "blocks", false, st_SINGLE, glm::vec2(1, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("sandstone", "sandstone", true, true, "sandstone", "blocks", true, st_BLOCK);
        blocks::addBlock("hardenedsand", "hardenedsand", true, true, "hardenedsand", "blocks", true, st_BLOCK);
        blocks::addBlock("hardenedsandwall", "hardenedsandwall", true, true, "hardenedsandwall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("sandstonewall", "sandstonewall", true, true, "sandstonewall", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("ebonstonewall", "ebonstonewall", true, true, "empty", "bg", true, st_WALL, { 1, 1 }, 0, { BConditions::isreplacable });
        blocks::addBlock("ebonstone", "ebonstone", true, true, "ebonstone", "blocks", true, st_BLOCK);
        blocks::addBlock("corrupttrunk", "corrupttrunk", false, true, "ebonwood", "blocks", false, st_SINGLE, { 1,1 }, 0, {}, false, false, false, false);
        blocks::addBlock("corrupttrunkbase1", "corrupttrunkbase1", false, true, "ebonwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("corrupttrunkbase2", "corrupttrunkbase2", false, true, "ebonwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("corrupttrunkbase3", "corrupttrunkbase3", false, true, "ebonwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("corruptleaves", "corruptleaves", false, true, "acorn", "blocks", false, st_SINGLE, glm::vec2(1), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("corruptsapling", "corruptsapling", false, true, "acorn", "blocks", false, st_SINGLE, { 1, 2 }, 0, { BConditions::nowall, BConditions::isntreplacablebelow, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("mahoganytrunk", "mahoganytrunk", false, true, "mahoganywood", "blocks", false, st_SINGLE, { 1,1 }, 0, {}, false, false, false, false);
        blocks::addBlock("mahoganytrunkbase1", "mahoganytrunkbase1", false, true, "mahoganywood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("mahoganytrunkbase2", "mahoganytrunkbase2", false, true, "mahoganywood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("mahoganytrunkbase3", "mahoganytrunkbase3", false, true, "mahoganywood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("mahoganyleaves", "mahoganyleaves", false, true, "acorn", "blocks", false, st_SINGLE, glm::vec2(1), 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::addBlock("mahoganysapling", "mahoganysapling", false, true, "acorn", "blocks", false, st_SINGLE, { 1, 2 }, 0, { BConditions::nowall, BConditions::isntreplacablebelow, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("borealtrunk", "borealtrunk", false, true, "borealwood", "blocks", false, st_SINGLE, { 1,1 }, 0, {}, false, false, false, false);
        blocks::addBlock("borealtrunkbase1", "borealtrunkbase1", false, true, "borealwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("borealtrunkbase2", "borealtrunkbase2", false, true, "borealwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("borealtrunkbase3", "borealtrunkbase3", false, true, "borealwood", "blocks", false, st_SINGLE, { 1, 1 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, true, false);
        blocks::addBlock("borealleaves", "borealleaves", false, true, "acorn", "blocks", false, st_SINGLE, glm::vec2(1), 0, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("borealsapling", "borealsapling", false, true, "acorn", "blocks", false, st_SINGLE, { 1, 2 }, 0, { BConditions::nowall, BConditions::isntreplacablebelow, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("normalgrassdecor", "normalgrassdecor", false, false, "empty", "blocks", false, st_GRASS, { 1, 2 }, 0, { BConditions::isempty, BConditions::nowall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("corruptgrassdecor", "corruptgrassdecor", false, false, "empty", "blocks", false, st_GRASS, { 1, 2 }, 0, {BConditions::isempty, BConditions::nowall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("junglegrassdecor", "junglegrassdecor", false, false, "empty", "blocks", false, st_GRASS, { 1, 2 }, 0, { BConditions::isempty, BConditions::nowall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("normalbranchr", "normalbranchr", false, false, "wood", "blocks", false, st_SINGLE, { 2, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("normalbranchl", "normalbranchl", false, false, "wood", "blocks", false, st_SINGLE, { 2, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("corruptbranchr", "corruptbranchr", false, false, "ebonwood", "blocks", false, st_SINGLE, { 2, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("corruptbranchl", "corruptbranchl", false, false, "ebonwood", "blocks", false, st_SINGLE, { 2, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("junglebranchr", "junglebranchr", false, false, "mahoganywood", "blocks", false, st_SINGLE, { 3, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("junglebranchl", "junglebranchl", false, false, "mahoganywood", "blocks", false, st_SINGLE, { 3, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("borealbranchr", "borealbranchr", false, false, "borealwood", "blocks", false, st_SINGLE, { 3, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("borealbranchl", "borealbranchl", false, false, "borealwood", "blocks", false, st_SINGLE, { 3, 2 }, 0, {  }, false, false, false, false);
        blocks::addBlock("icicle", "icicle", false, false, "empty", "blocks", false, st_STALAKTIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("stalaktit", "stalaktit", false, false, "empty", "blocks", false, st_STALAKTIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("corruptstalaktit", "corruptstalaktit", false, false, "empty", "blocks", false, st_STALAKTIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("sandstalaktit", "sandstalaktit", false, false, "empty", "blocks", false, st_STALAKTIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("stalagmit", "stalagmit", false, false, "empty", "blocks", false, st_STALAGMIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("sandstalagmit", "sandstalagmit", false, false, "empty", "blocks", false, st_STALAGMIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("corruptstalagmit", "corruptstalagmit", false, false, "empty", "blocks", false, st_STALAGMIT, { 1, 2 }, 0, { BConditions::haswall, BConditions::isreplacable }, false, false, false, false);
        blocks::setNextNumSprites(6);
        blocks::addBlock("smallrock", "smallrock", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 1 }, 0, { BConditions::isempty,BConditions::isreplacable }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("mediumrock", "mediumrock", false, false, "empty", "blocks", false, st_MULTISPRITE, { 2, 1 }, 0, { BConditions::isempty, BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("woodenplatform", "woodenplatform", false, false, "woodenplatform", "blocks", false, st_PLATFORM, { 1 ,1 }, 1, { BConditions::isreplacable }, false, false, false, false);
        blocks::addBlock("campfire", "campfire", false, true, "campfire", "blocks", false, st_SINGLE, { 3,2 }, 1, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("goldore", "goldore", true, true, "goldore", "blocks", true, st_BLOCK);
        blocks::addBlock("silverore", "silverore", true, true, "silverore", "blocks", true, st_BLOCK);
        blocks::addBlock("hellstone", "hellstone", true, true, "hellstone", "blocks", true, st_BLOCK);
        blocks::addBlock("demoniteore", "demoniteore", true, true, "demoniteore", "blocks", true, st_BLOCK);
        blocks::addBlock("obsidianbrick", "obsidianbrick", true, true, "obsidianbrick", "blocks", true, st_BLOCK);
        blocks::addBlock("obsidian", "obsidian", true, true, "obsidian", "blocks", true, st_BLOCK);
        blocks::addBlock("hellforge", "hellforge", false, true, "hellforge", "blocks", false, st_SINGLE, glm::vec2(3, 2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("cactustop", "cactustop", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusmid", "cactusmid", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusright", "cactusright", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusleft", "cactusleft", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusleftarm", "cactusleftarm", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusarmleftmid", "cactusarmleftmid", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusarmlefttop", "cactusarmlefttop", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusrightarm", "cactusrightarm", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusarmrightmid", "cactusarmrightmid", false, true, "cactus", "blocks", false);
        blocks::addBlock("cactusarmrighttop", "cactusarmrighttop", false, true, "cactus", "blocks", false);
        blocks::addBlock("sunflower", "sunflower", false, true, "sunflower", "blocks", false, st_SINGLE, { 2,4 }, 1, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("heartcrystal", "heartcrystal", false, true, "heartcrystal", "blocks", false, st_SINGLE, { 2,2 }, 1, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("bottle", "placedbottle", false, true, "bottle", "blocks", false, st_SINGLE, { 1,1 }, 1, { BConditions::isreplacable, BConditions::bottle }, false, false, false, false);
        blocks::addBlock("mushroom", "mushroom", false, false, "mushroom", "blocks", false, st_SINGLE, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("daybloom", "daybloom", false, false, "daybloom", "blocks", false, st_SINGLE, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("demonaltar", "demonaltar", false, true, "empty", "blocks", false, st_SINGLE, { 3,3 }, 1, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("shadoworb", "shadoworb", false, true, "empty", "blocks", false, st_SINGLE, { 2,2 }, 1, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("water", "water", false, false, "empty", "blocks", false, st_WATER, { 1,1 }, 1, {}, true, true, true, true);
        blocks::addBlock("lava", "lava", false, false, "empty", "blocks", false, st_WATER, { 1,1 }, 1, {}, true, true, true, true);
        blocks::addBlock("junglespores", "junglespores", false, false, "junglespores", "blocks", false, st_SINGLE, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("normalvines", "normalvines", false, false, "empty", "blocks", false, st_VINES, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("junglevines", "junglevines", false, false, "empty", "blocks", false, st_VINES, { 1,1 }, 1, {}, false, false, false, false);
        blocks::addBlock("pot", "pot", false, true, "empty", "blocks", false, st_POT, { 2, 2 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow, BConditions::haswall }, false, false, false, false);
        blocks::addBlock("icepot", "icepot", false, true, "empty", "blocks", false, st_POT, { 2, 2 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow, BConditions::haswall }, false, false, false, false);
        blocks::addBlock("junglepot", "junglepot", false, true, "empty", "blocks", false, st_POT, { 2, 2 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow, BConditions::haswall }, false, false, false, false);
        blocks::addBlock("hellpot", "hellpot", false, true, "empty", "blocks", false, st_POT, { 2, 2 }, 0, { BConditions::isreplacable, BConditions::isntreplacablebelow, BConditions::haswall }, false, false, false, false);
        blocks::addBlock("frozenchest", "frozenchest", false, true, "frozenchest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("mahoganychest", "mahoganychest", false, true, "mahoganychest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("goldchest", "goldchest", false, true, "goldchest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("shadowchest", "shadowchest", false, true, "shadowchest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::addBlock("waterchest", "waterchest", false, true, "waterchest", "blocks", false, st_SINGLE, glm::vec2(2), 0, { BConditions::isreplacable, BConditions::issolidbelow }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("bigrock", "bigrock", false, false, "empty", "blocks", false, st_MULTISPRITE, { 3, 2 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(18);
        blocks::addBlock("normalflowers", "normalflowers", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 2 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(19);
        blocks::addBlock("jungleflowers", "jungleflowers", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 2 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(13);
        blocks::addBlock("corruptflowers", "corruptflowers", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 2 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(9);
        blocks::addBlock("undergroundskeletondecor", "undergroundskeletondecor", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 1 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(8);
        blocks::addBlock("undergroundtooldecor", "undergroundtooldecor", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 1 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(6);
        blocks::addBlock("smallice", "smallice", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 1 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("mediumice", "mediumice", false, false, "empty", "blocks", false, st_MULTISPRITE, { 2, 1 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("bigice", "bigice", false, false, "empty", "blocks", false, st_MULTISPRITE, { 3, 2 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(6);
        blocks::addBlock("smalldesert", "smalldesert", false, false, "empty", "blocks", false, st_MULTISPRITE, { 1, 1 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("mediumdesert", "mediumdesert", false, false, "empty", "blocks", false, st_MULTISPRITE, { 2, 1 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(3);
        blocks::addBlock("bigdesert", "bigdesert", false, false, "empty", "blocks", false, st_MULTISPRITE, { 3, 2 }, 0, { BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(2);
        blocks::addBlock("bigjungle", "bigjungle", false, false, "empty", "blocks", false, st_MULTISPRITE, { 3, 2 }, 0, { BConditions::isempty,BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);
        blocks::setNextNumSprites(5);
        blocks::addBlock("mediumjungle", "mediumjungle", false, false, "empty", "blocks", false, st_MULTISPRITE, { 2, 2 }, 0, {BConditions::isempty, BConditions::isreplacable, BConditions::isntreplacablebelow }, false, false, false, false);

        globals::waterid = blocks::nameToID["water"];

        blocks::addFunction("door", BFuncs::toggleDoor);
        blocks::addFunction("doorright", BFuncs::toggleDoor);
        blocks::addFunction("doorleft", BFuncs::toggleDoor);
        blocks::addFunction("chest", BFuncs::chestFunction);
        blocks::addFunction("frozenchest", BFuncs::chestFunction);
        blocks::addFunction("shadowchest", BFuncs::chestFunction);
        blocks::addFunction("goldchest", BFuncs::chestFunction);
        blocks::addFunction("mahoganychest", BFuncs::chestFunction);
        blocks::addFunction("waterchest", BFuncs::chestFunction);
        blocks::addFunction("borealdoor", BFuncs::toggleDoor);
        blocks::addFunction("borealdoorright", BFuncs::toggleDoor);
        blocks::addFunction("borealdoorleft", BFuncs::toggleDoor);
        blocks::addFunction("mahoganydoor", BFuncs::toggleDoor);
        blocks::addFunction("mahoganydoorright", BFuncs::toggleDoor);
        blocks::addFunction("mahoganydoorleft", BFuncs::toggleDoor);
        blocks::addFunction("ebonwooddoor", BFuncs::toggleDoor);
        blocks::addFunction("ebonwooddoorright", BFuncs::toggleDoor);
        blocks::addFunction("ebonwooddoorleft", BFuncs::toggleDoor);

        blocks::addOnPlace("chest", BFuncs::chestOnPlace);
        blocks::addOnPlace("frozenchest", BFuncs::chestOnPlace);
        blocks::addOnPlace("shadowchest", BFuncs::chestOnPlace);
        blocks::addOnPlace("goldchest", BFuncs::chestOnPlace);
        blocks::addOnPlace("mahoganychest", BFuncs::chestOnPlace);
        blocks::addOnPlace("waterchest", BFuncs::chestOnPlace);

        blocks::addOnBreak("chest", BFuncs::chestOnBreak);
        blocks::addOnBreak("frozenchest", BFuncs::chestOnBreak);
        blocks::addOnBreak("shadowchest", BFuncs::chestOnBreak);
        blocks::addOnBreak("goldchest", BFuncs::chestOnBreak);
        blocks::addOnBreak("mahoganychest", BFuncs::chestOnBreak);
        blocks::addOnBreak("waterchest", BFuncs::chestOnBreak);
        blocks::addOnBreak("pot", BFuncs::potOnBreak);
        blocks::addOnBreak("hellstone", BFuncs::hellstoneOnBreak);
        blocks::addOnBreak("shadoworb", BFuncs::shadoworbOnBreak);
        blocks::addOnBreak("grass", BFuncs::grasssoundOnBreak);
        blocks::addOnBreak("junglegrass", BFuncs::grasssoundOnBreak);
        blocks::addOnBreak("corruptgrass", BFuncs::grasssoundOnBreak);
        blocks::addOnBreak("vines", BFuncs::grasssoundOnBreak);

        blocks::addOnUpdate("craftingbench", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("borealworkbench", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("mahoganyworkbench", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("ebonwoodworkbench", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("furnace", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("anvil", BFuncs::craftingStationOnUpdate);
        blocks::addOnUpdate("sapling", BFuncs::saplingOnUpdate);
        blocks::addOnUpdate("corruptsapling", BFuncs::saplingOnUpdate);
        blocks::addOnUpdate("mahoganysapling", BFuncs::saplingOnUpdate);
        blocks::addOnUpdate("borealsapling", BFuncs::saplingOnUpdate);
        blocks::addOnUpdate("campfire", BFuncs::buffBlockOnUpdate);
        blocks::addOnUpdate("sunflower", BFuncs::buffBlockOnUpdate);
        blocks::addOnPlace("chairright", BFuncs::chairOnPlace);
        blocks::addOnPlace("borealchairright", BFuncs::chairOnPlace);
        blocks::addOnPlace("ebonwoodchairright", BFuncs::chairOnPlace);
        blocks::addOnPlace("mahoganychairright", BFuncs::chairOnPlace);

        blocks::animate("campfire", { 0, -17, 0, -17 }, 1, 8);
        blocks::animate("heartcrystal", { 0, -16, 0, -16 }, 1, 10);
        blocks::animate("water", { 0, -24, 0, -24 }, 1, 4);
        blocks::animate("lava", { 0, -24, 0, -24 }, 1, 4);

        blocks::addCraftingStation("craftingbench", "craftingbench");
        blocks::addCraftingStation("borealworkbench", "craftingbench");
        blocks::addCraftingStation("mahoganyworkbench", "craftingbench");
        blocks::addCraftingStation("ebonwoodworkbench", "craftingbench");
        blocks::addCraftingStation("anvil", "anvil");
        blocks::addCraftingStation("furnace", "furnace");
        blocks::addCraftingStation("hellforge", "hellforge");
        blocks::addCraftingStation("hellforge", "furnace");
        blocks::addCraftingStation("bottle", "bottle");
        blocks::addCraftingStation("demonaltar", "demonaltar");

        Layers::biomes.insert(std::make_pair<std::string, Biome>("forest", { 100, "forest", {600, 5}, {600, 5}, {600, 5}, {600, 5}, {600, 5} }));
        Layers::biomes.insert(std::make_pair<std::string, Biome>("snow", { 100, "snow", {600, 5}, {600, 5}, {600, 5}, {600, 5}, {600, 5} }));
        Layers::biomes.insert(std::make_pair<std::string, Biome>("desert", { 100, "desert", {600, 5}, {600, 5}, {600, 5}, {600, 5}, {600, 5} }));
        Layers::biomes.insert(std::make_pair<std::string, Biome>("corruption", { 100, "corruption", {390, 6}, {234, 9}, {195, 10}, {156, 11}, {0, 0} }));
        Layers::biomes.insert(std::make_pair<std::string, Biome>("jungle", { 100, "jungle", {240, 7}, {144, 9}, {120, 12}, {96, 13}, {0, 0} }));

        //campfire particles
        {
            particleEmmiterC pec; 
            pec.amount = 1;
            pec.dir = glm::vec2(0, 1);
            pec.disappearAsDie = true;
            pec.killentity = true;
            pec.lifespan = 1;
            pec.ms = 0.02;
            pec.particleLifespan = 120;
            pec.randangle = 15;
            pec.randomizelifespan = 60;
            pec.randomizems = 0.005;
            pec.rate = 20;
            pec.randomizerate = 60;
            pec.opacity = 0.2;
            pec.tex = "particle:smokepart";
            pec.size = glm::vec2(1, 1);
            pec.randomizescale = 0.5;
            pec.weight = 0;
            pec.stoponcollision = true;
            pec.rotation = 0;
            pec.randrotation = 20;
            blocks::addParticle("campfire", pec);
        }

        //hellstone particles
        {
            particleEmmiterC pec;
            pec.amount = 1;
            pec.dir = glm::vec2(0, 1);
            pec.disappearAsDie = true;
            pec.killentity = true;
            pec.lifespan = 1;
            pec.ms = 0.2;
            pec.particleLifespan = 60;
            pec.randangle = 15;
            pec.randomizelifespan = 60;
            pec.randomizems = 0.009;
            pec.rate = 90;
            pec.tex = "particle:torchpart";
            pec.size = glm::vec2(1, 1);
            pec.randomizescale = 0.5;
            pec.weight = 0.6;
            pec.light = glm::vec3(1, 0, 0);
            pec.stoponcollision = false;
            pec.smallerAsDie = true;
            pec.rotation = 0;
            pec.randrotation = 20;
            blocks::addParticle("hellstone", pec);
        }

        //hellstone particles
        {
            particleEmmiterC pec;
            pec.amount = 1;
            pec.dir = glm::vec2(0, 1);
            pec.disappearAsDie = true;
            pec.killentity = true;
            pec.lifespan = 1;
            pec.ms = 0.07;
            pec.particleLifespan = 30;
            pec.randangle = 15;
            pec.randomizelifespan = 10;
            pec.randomizems = 0.009;
            pec.rate = 30;
            pec.tex = "particle:torchpart";
            pec.size = glm::vec2(0.6, 0.6);
            pec.randomizescale = 0.1;
            pec.weight = -0.1;
            pec.light = glm::vec3(1, 0, 0);
            pec.stoponcollision = false;
            pec.smallerAsDie = true;
            pec.rotation = 0;
            pec.randrotation = 20;
            blocks::addParticle("torch", pec);
        }

        blocks::addBiome("grass", "forest");
        blocks::addBiome("snow", "snow");
        blocks::addBiome("sand", "desert");
        blocks::addBiome("corruptgrass", "corruption");
        blocks::addBiome("ebonstone", "corruption");
        blocks::addBiome("junglegrass", "jungle");
        blocks::addBiome("ash", "underworld");

        blocks::addDecor("normalgrassdecor", 0.5, "surface", {}, {"grass"});
        blocks::addDecor("corruptgrassdecor", 0.5, "surface", {}, { "corruptgrass" });
        blocks::addDecor("junglegrassdecor", 0.5, "surface", {}, { "junglegrass" });
        blocks::addDecor("icicle", 0.3, "any", { "snow" });
        blocks::addDecor("stalaktit", 0.2, "underground", { "stone" });
        blocks::addDecor("corruptstalaktit", 0.25, "underground", { "ebonstone" });
        blocks::addDecor("sandstalaktit", 0.25, "underground", { "sandstone" });
        blocks::addDecor("stalagmit", 0.2, "underground", {}, { "stone" });
        blocks::addDecor("corruptstalagmit", 0.2, "underground", {}, { "ebonstone" });
        blocks::addDecor("sandstalagmit", 0.2, "underground", {}, { "sandstone" });
        blocks::addDecor("smallrock", 0.2, "any", {}, { "grass", "stone" });
        blocks::addDecor("mushroom", 0.05, "surface", {}, { "grass" });
        blocks::addDecor("daybloom", 0.05, "surface", {}, { "grass" });
        blocks::addDecor("junglespores", 0.007, "any", {}, { "junglegrass" });
        blocks::addDecor("heartcrystal", 0.001, "underground", {}, { "stone" });
        blocks::addDecor("demonaltar", 0.5, "underground", {}, { "ebonstone" });
        blocks::addDecor("pot", 0.02, "any", {}, { "grass", "stone", "dirt"});
        blocks::addDecor("icepot", 0.02, "any", {}, { "snow" });
        blocks::addDecor("junglepot", 0.01, "any", {}, { "junglegrass" });
        blocks::addDecor("hellpot", 0.05, "any", {}, { "ash", "hellstone"});
        blocks::addDecor("normalflowers", 0.1, "surface", {}, { "grass" });
        blocks::addDecor("jungleflowers", 0.2, "surface", {}, { "junglegrass" });
        blocks::addDecor("corruptflowers", 0.04, "surface", {}, { "corruptgrass" });
        blocks::addDecor("undergroundskeletondecor", 0.04, "underground", {}, { "stone" });
        blocks::addDecor("undergroundtooldecor", 0.04, "underground", {}, { "stone" });
        blocks::addDecor("smallice", 0.15, "any", {}, { "snow" });
        blocks::addDecor("mediumice", 0.1, "any", {}, { "snow" });
        blocks::addDecor("bigice", 0.06, "any", {}, { "snow" });
        blocks::addDecor("smalldesert", 0.15, "underground", {}, { "hardenedsand", "sandstone" });
        blocks::addDecor("mediumdesert", 0.1, "underground", {}, { "hardenedsand", "sandstone" });
        blocks::addDecor("bigdesert", 0.07, "underground", {}, { "hardenedsand", "sandstone" });
        blocks::addDecor("mediumjungle", 0.2, "any", {}, { "junglegrass" });
        blocks::addDecor("bigjungle", 0.1, "any", {}, { "junglegrass"});

        blocks::addDamageParticle("dirt", "particle:dirtpart");
        blocks::addDamageParticle("pot", "particle:dirtpart");
        blocks::addDamageParticle("stone", "particle:stonepart");
        blocks::addDamageParticle("grass", "particle:grasspart");
        blocks::addDamageParticle("normalvines", "particle:grasspart");
        blocks::addDamageParticle("junglevines", "particle:grasspart");
        blocks::addDamageParticle("normalgrassdecor", "particle:leafpart");
        blocks::addDamageParticle("wood", "particle:woodpart");
        blocks::addDamageParticle("ironore", "particle:ironpart");
        blocks::addDamageParticle("copperore", "particle:copperpart");
        blocks::addDamageParticle("corruptgrass", "particle:corruptpart");
        blocks::addDamageParticle("ebonstone", "particle:corruptpart");
        blocks::addDamageParticle("glass", "particle:glasspart");
        blocks::addDamageParticle("sand", "particle:sandpart");
        blocks::addDamageParticle("trunk", "particle:woodpart");
        blocks::addDamageParticle("trunkbase1", "particle:woodpart");
        blocks::addDamageParticle("trunkbase3", "particle:woodpart");
        blocks::addDamageParticle("trunkbase2", "particle:woodpart");
        blocks::addDamageParticle("corrupttrunk", "particle:corruptpart");
        blocks::addDamageParticle("corrupttrunkbase1", "particle:corruptpart");
        blocks::addDamageParticle("corrupttrunkbase3", "particle:corruptpart");
        blocks::addDamageParticle("corrupttrunkbase2", "particle:corruptpart");
        blocks::addDamageParticle("mahoganytrunk", "particle:woodpart");
        blocks::addDamageParticle("mahoganytrunkbase1", "particle:woodpart");
        blocks::addDamageParticle("mahoganytrunkbase3", "particle:woodpart");
        blocks::addDamageParticle("mahoganytrunkbase2", "particle:woodpart");
        blocks::addDamageParticle("borealtrunk", "particle:woodpart");
        blocks::addDamageParticle("borealtrunkbase1", "particle:woodpart");
        blocks::addDamageParticle("borealtrunkbase3", "particle:woodpart");
        blocks::addDamageParticle("borealtrunkbase2", "particle:woodpart");
        blocks::addDamageParticle("demonite", "particle:corruptpart");
        blocks::addDamageParticle("hellstone", "particle:torchpart");
        blocks::addDamageParticle("goldore", "particle:goldpart");
        blocks::addDamageParticle("silverore", "particle:silverpart");
        blocks::addDamageParticle("demonaltar", "particle:corruptpart");

        blocks::addLife("dirt", 50);
        blocks::addLife("sand", 50);
        blocks::addLife("snow", 50);
        blocks::addLife("hardenedsand", 50);
        blocks::addLife("ebonstone", 200);
        blocks::addLife("gravestone", 300);
        blocks::addLife("mushroom", 1);
        blocks::addLife("daybloom", 1);
        blocks::addLife("junglespores", 1);
        blocks::addLife("normalgrassdecor", 1);
        blocks::addLife("junglegrassdecor", 1);
        blocks::addLife("corruptgrassdecor", 1);
        blocks::addLife("normalvines", 1);
        blocks::addLife("junglevines", 1);
        blocks::addLife("pot", 1);

        blocks::emitSkyLight("empty");

        blocks::addMinPickaxe("empty", 65123);
        blocks::addMinPickaxe("ebonstone", 65);
        blocks::addMinPickaxe("demonaltar", 80);
        blocks::addMinPickaxe("ebonstonewall", 80);
        blocks::addMinPickaxe("mushroom", 0);
        blocks::addMinPickaxe("daybloom", 0);
        blocks::addMinPickaxe("junglespores", 0);
        blocks::addMinPickaxe("normalgrassdecor", 0);
        blocks::addMinPickaxe("junglegrassdecor", 0);
        blocks::addMinPickaxe("corruptgrassdecor", 0);
        blocks::addMinPickaxe("normalvines", 0);
        blocks::addMinPickaxe("junglevines", 0);
        blocks::addMinPickaxe("pot", 0);
        blocks::addMinPickaxe("hellstone", 65);

        blocks::modifyCollidable("woodenplatform", true, false, false, false);
        blocks::modifyCollidable("craftingbench", true, false, false, false);
        blocks::modifyCollidable("table", true, false, false, false);
        blocks::modifyCollidable("mahoganytable", true, false, false, false);
        blocks::modifyCollidable("mahoganyworkbench", true, false, false, false);
        blocks::modifyCollidable("borealworkbench", true, false, false, false);
        blocks::modifyCollidable("borealtable", true, false, false, false);
        blocks::modifyCollidable("ebonwoodworkbench", true, false, false, false);
        blocks::modifyCollidable("ebonwoodtable", true, false, false, false);

        blocks::addSkippable("empty");
        blocks::addSkippable("woodenplatform");
        blocks::addSkippable("craftingbench");
        blocks::addSkippable("table");
        blocks::addSkippable("mahoganytable");
        blocks::addSkippable("mahoganyworkbench");
        blocks::addSkippable("borealworkbench");
        blocks::addSkippable("borealtable");
        blocks::addSkippable("ebonwoodworkbench");
        blocks::addSkippable("ebonwoodtable");

        blocks::addDamagableWith("trunk", if_AXE);
        blocks::addDamagableWith("trunkbase1", if_AXE);
        blocks::addDamagableWith("trunkbase2", if_AXE);
        blocks::addDamagableWith("trunkbase3", if_AXE);
        blocks::addDamagableWith("normalbranchl", if_AXE);
        blocks::addDamagableWith("normalbranchr", if_AXE);
        blocks::addDamagableWith("corrupttrunk", if_AXE);
        blocks::addDamagableWith("corrupttrunkbase1", if_AXE);
        blocks::addDamagableWith("corrupttrunkbase2", if_AXE);
        blocks::addDamagableWith("corrupttrunkbase3", if_AXE);
        blocks::addDamagableWith("corruptbranchl", if_AXE);
        blocks::addDamagableWith("corruptbranchr", if_AXE);
        blocks::addDamagableWith("borealtrunk", if_AXE);
        blocks::addDamagableWith("borealtrunkbase1", if_AXE);
        blocks::addDamagableWith("borealtrunkbase2", if_AXE);
        blocks::addDamagableWith("borealtrunkbase3", if_AXE);
        blocks::addDamagableWith("borealbranchl", if_AXE);
        blocks::addDamagableWith("borealbranchr", if_AXE);
        blocks::addDamagableWith("mahoganytrunk", if_AXE);
        blocks::addDamagableWith("mahoganytrunkbase1", if_AXE);
        blocks::addDamagableWith("mahoganytrunkbase2", if_AXE);
        blocks::addDamagableWith("mahoganytrunkbase3", if_AXE);
        blocks::addDamagableWith("mahoganybranchl", if_AXE);
        blocks::addDamagableWith("mahoganybranchr", if_AXE);
        blocks::addDamagableWith("demonaltar", if_HAMMER);
        blocks::addDamagableWith("dirtwall", if_HAMMER);
        blocks::addDamagableWith("hardenedsandwall", if_HAMMER);
        blocks::addDamagableWith("sandstonewall", if_HAMMER);
        blocks::addDamagableWith("ebonstonewall", if_HAMMER);
        blocks::addDamagableWith("glasswall", if_HAMMER);
        blocks::addDamagableWith("woodwall", if_HAMMER);
        blocks::addDamagableWith("shadoworb", if_HAMMER);

        blocks::makeUnbreakableBelow("chest");
        blocks::makeUnbreakableBelow("frozenchest");
        blocks::makeUnbreakableBelow("shadowchest");
        blocks::makeUnbreakableBelow("goldchest");
        blocks::makeUnbreakableBelow("mahoganychest");
        blocks::makeUnbreakableBelow("waterchest");
        blocks::makeUnbreakableBelow("trunkbase1");
        blocks::makeUnbreakableBelow("trunkbase2");
        blocks::makeUnbreakableBelow("trunkbase3");
        blocks::makeUnbreakableBelow("corrupttrunkbase1");
        blocks::makeUnbreakableBelow("corrupttrunkbase2");
        blocks::makeUnbreakableBelow("corrupttrunkbase3");
        blocks::makeUnbreakableBelow("borealtrunkbase1");
        blocks::makeUnbreakableBelow("borealtrunkbase2");
        blocks::makeUnbreakableBelow("borealtrunkbase3");
        blocks::makeUnbreakableBelow("mahoganytrunkbase1");
        blocks::makeUnbreakableBelow("mahoganytrunkbase2");
        blocks::makeUnbreakableBelow("mahoganytrunkbase3");
        blocks::makeUnbreakableBelow("demonaltar");

        blocks::addBuff("campfire", "cozyfire");
        blocks::addBuff("sunflower", "happy");
            
        liquids::add("water", 0x00000000, blocks::nameToID["water"]);
        liquids::add("lava", 0x30000000, blocks::nameToID["lava"], glm::vec3(0.25, 0.1,0));

        items::info.clear();
        items::addItem("empty", "", "empty", {}, 0, { if_ANY });
        items::addItem("dirt", "Dirt Block", "dirtitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("glass", "Glass Block", "glassitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("stone", "Stone Block", "stoneitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("wood", "Wood", "wooditem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("sand", "Sand Block", "sanditem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("dirtwall", "Dirt Wall", "dirtwallitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("glasswall", "Glass Wall", "glasswallitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("stonewall", "Stone Wall", "stonewallitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("woodwall", "Wood Wall", "woodwallitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("grass", "Grass Block", "grass", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("torch", "Torch", "torchitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("purpletorch", "Purple Torch", "purpletorch", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("acorn", "Acorn", "acorn", { itemFuncs::placeSapling }, globals::blockPlaceSpeed, { if_MISC }, {}, { itemConditions::hasAmmo });
        items::addItem("woodenbow", "Wooden Bow", "woodenbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("arrow", "Arrow", "arrow", {}, 0, { if_ARROW, if_PROJECTILE }, {});
        items::addItem("bomb", "Bomb", "bomb", { itemFuncs::throwSelf }, 20, { if_THROWABLE }, {}, { itemConditions::hasAmmo });
        items::addItem("minishark", "Minishark", "minishark", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 4, { if_GUN, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("waterspell", "Water Bolt", "waterspell", { itemFuncs::shootArrow }, 1, { if_SPELL, if_RANGED }, {}, { itemConditions::nothing }, ia_USE);
        items::addItem("gel", "Gel", "gel", {}, globals::blockPlaceSpeed, { if_MISC }, {});
        items::addItem("bullet", "Bullet", "bullet", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("waterbolt", "waterbolt", "waterbolt", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("aquabolt", "aquabolt", "aquabolt", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("chest", "Chest", "chestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("door", "Wood Door", "dooritem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("table", "Wood Table", "tableitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("anvil", "Iron Anvil", "anvilitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("furnace", "Furnace", "furnaceitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("craftingbench", "Work Bench", "craftingbenchitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("gravestone", "Gravestone", "gravestoneitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("slimestatue", "Slime Statue", "slimestatue", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("chair", "Wood Chair", "chairitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ironbar", "Iron Bar", "ironbar", {}, globals::blockPlaceSpeed, { if_BAR }, {}, {});
        items::addItem("ironore", "Iron Ore", "ironoreitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE, if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("copperore", "Copper Ore", "ironoreitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("stonebrick", "Stone Brick", "stonebrickitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mud", "Mud", "muditem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("snow", "Snow", "snowitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealwood", "Boreal Wood", "borealwooditem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwood", "Ebonwood", "ebonwooditem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganywood", "Mahogany Wood", "mahoganywoooditem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganywoodwall", "Mahogany Wood Wall", "mahoganywoodwallitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwoodwall", "Ebonwood Wall", "ebonwoodwallitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealwoodwall", "Boreal Wood Wall", "borealwoodwallitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealdoor", "Boreal Door", "borealdooritem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealtable", "Boreal Table", "borealtableitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealworkbench", "Boreal Work Bench", "borealworkbenchitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganydoor", "Mahogany Door", "mahoganydooritem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganytable", "Mahogany Table", "mahoganytableitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganyworkbench", "Mahogany Work Bench", "mahoganyworkbenchitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwooddoor", "Ebonwood Door", "ebonwooddooritem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwoodtable", "Ebonwood Table", "ebonwoodtableitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwoodworkbench", "Ebonwood Work Bench", "ebonwoodworkbenchitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_ORE,if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonwoodchair", "Ebonwood Chair", "ebonwoodchairitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganychair", "Mahogany Chair", "mahoganychairitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("borealchair", "Boreal Chair", "borealchairitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ash", "Ash", "ashitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("sandstone", "Sandstone Block", "sandstoneitem", { itemFuncs::place },  globals::blockPlaceSpeed, {if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("sandstonewall", "Sandstone Wall", "sandstonewallitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("hardenedsand", "Hardened Sand", "hardenedsanditem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("hardenedsandwall", "Hardened Sand Wall", "hardenedsandwallitem", { itemFuncs::place },  globals::blockPlaceSpeed, {if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("ebonstone", "Ebonstone", "ebonstoneitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("woodenbreastplate", "Wood Breatplate", "woodenbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE}, {}, { itemConditions::hasAmmo });
        items::addItem("woodenhelmet", "Wood Helmet", "woodenhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("woodengreaves", "Wood greaves", "woodengreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("fallenstar", "Fallen Star", "fallenstar", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("coppercoin", "Copper Coin", "coppercoin", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("silvercoin", "Silver Coin", "silvercoin", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("goldcoin", "Gold Coin", "goldcoin", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("platinumcoin", "Platinum Coin", "platinumcoin", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("woodenplatform", "Wooden Platform", "woodenplatform", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("campfire", "Campfire", "campfireitem", { itemFuncs::place },  globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("bandofregeneration", "Band of Regeneration", "regenband", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {});
        items::addItem("suseye", "Sus eye", "suseye", {itemFuncs::summonEye, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, {if_MISC }, {}, {});
        items::addItem("magicmirror", "Magic Mirror", "magicmirror", {itemFuncs::teleportToSpawn}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("cloudinabottle", "Cloud in a Bottle", "bottlecloud", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {});
        items::addItem("hermesboots", "Hermes Boots", "hermesboots", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {});
        items::addItem("bandofstarpower", "Band of Starpower", "manaband", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {});
        items::addItem("lens", "Lens", "lens", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("shackle", "Shackle", "shackle", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, { });
        items::addItem("enchantedboomerang", "Enchanted Boomerang", "magicboomerang", {itemFuncs::shootBoomerang}, 30, { if_WEAPON, if_MEELE }, {}, { itemConditions::boomerang }, ia_NONE);
        items::addItem("vilethorn", "Vilethorn", "vilethorn", { itemFuncs::vilethorn ,itemFuncs::takeMana }, 30, { if_WEAPON, if_MAGIC }, {}, { itemConditions::hasMana });
        items::addItem("terrablade", "Terra Blade", "terrablade", { itemFuncs::terraBlade }, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("starfury", "Starfury", "starfury", {itemFuncs::starfuryStar}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("magicmissile", "Magic missile", "magicmissile", { itemFuncs::shootArrow, itemFuncs::takeMana }, 30, { if_WEAPON, if_MAGIC }, {}, { itemConditions::hasMana });
        items::addItem("aquascepter", "Aqua Scepter", "aquascepter", { itemFuncs::shootArrow, itemFuncs::takeMana }, 30, { if_WEAPON, if_MAGIC }, {}, { itemConditions::hasMana }, ia_USE);
        items::addItem("opticstaff", "Optic staff", "opticstaff", { itemFuncs::opticStaff, itemFuncs::takeMana }, 30, { if_WEAPON, if_SUMMON }, {}, {itemConditions::hasSummonSpace, itemConditions::hasMana});
        items::addItem("frozenboomerang", "Frozen Boomerang", "frozenboomerang", { itemFuncs::shootBoomerang }, 30, { if_WEAPON, if_MEELE }, {}, { itemConditions::boomerang }, ia_NONE);
        items::addItem("woodenboomerang", "Wooden Boomerang", "woodenboomerang", { itemFuncs::shootBoomerang }, 30, { if_WEAPON, if_MEELE }, {}, { itemConditions::boomerang }, ia_NONE);
        items::addItem("lightdisc", "Light Disc", "lightdisc", { itemFuncs::shootBoomerang }, 30, { if_WEAPON, if_MEELE }, {}, { itemConditions::lightdisc }, ia_NONE);
        items::addItem("iceblade", "Ice Blade", "iceblade", { itemFuncs::shootArrow }, 30, { if_WEAPON, if_MEELE });
        items::addItem("icebladeprojectile", "icebladeprojectile", "icebladeprojectile", {}, 30, { if_PROJECTILE });
        items::addItem("lightsbane", "Lights Bane", "lightsbane", {}, 30, { if_WEAPON, if_MEELE });
        items::addItem("waraxeofthenight", "War Axe of the Night", "waraxeofthenight", { itemFuncs::dig }, 40, { if_WEAPON, if_AXE, if_MEELE }, {});
        items::addItem("demonbow", "Demon Bow", "demonbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("nightmarepickaxe", "Nightmare Pickaxe", "nightmarepickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("cactuspickaxe", "Cactus Pickaxe", "cactuspickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("cactussword", "Cactus Sword", "cactussword", {}, 30, { if_WEAPON, if_MEELE });
        items::addItem("spacegun", "Space Gun", "spacegun", { itemFuncs::shootArrow }, 30, { if_WEAPON, if_MAGIC }, {}, { itemConditions::hasMana }, ia_USE);
        items::addItem("spacegunprojectile", "spacegunprojectile", "spacegunprojectile", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("enchantedsword", "Enchanted Sword", "enchantedsword", {itemFuncs::shootArrow}, globals::blockPlaceSpeed, { if_WEAPON, if_MEELE });
        items::addItem("enchantedswordprojectile", "enchantedswordprojectile", "enchantedswordprojectile", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("snowballcannon", "Snowball Cannon", "snowballcannon", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_WEAPON, if_MEELE }, {}, { itemConditions::hasAmmo }, ia_USE);
        items::addItem("snowball", "Snowball", "snowball", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE }, {}, { itemConditions::hasAmmo }, ia_NONE);
        items::addItem("beegun", "Bee gun", "beegun", { itemFuncs::beegun, itemFuncs::takeMana }, globals::blockPlaceSpeed, { if_WEAPON, if_MAGIC }, {}, { itemConditions::hasMana }, ia_USE);
        items::addItem("bee", "bee", "bee", {}, globals::blockPlaceSpeed, { if_BULLET, if_PROJECTILE });
        items::addItem("hellstone", "Hellstone", "hellstoneitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("demoniteore", "Demonite Ore", "demoniteoreitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("silverore", "Silver Ore", "silveroreitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("goldore", "Gold Ore", "goldoreitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("obsidianbrick", "Obsidian Brick", "obsidianbrickitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("silverbar", "Silver Bar", "silverbar", {}, globals::blockPlaceSpeed, { if_BAR }, {}, {});
        items::addItem("goldbar", "Gold Bar", "goldbar", {}, globals::blockPlaceSpeed, { if_BAR }, {}, {});
        items::addItem("demonitebar", "Demonite Bar", "demonitebar", {}, globals::blockPlaceSpeed, { if_BAR }, {}, {});
        items::addItem("hellstonebar", "Hellstone Bar", "hellstonebar", {}, globals::blockPlaceSpeed, { if_BAR }, {}, {});
        items::addItem("ankletofthewind", "Anklet of the Wind", "ankletofthewind", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, { });
        items::addItem("feralclaws", "Feral Claws", "feralclaws", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {  });
        items::addItem("aglet", "Aglet", "aglet", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {  });
        items::addItem("obsidian", "Obsidian", "obsidian", {}, globals::blockPlaceSpeed, { if_ACCESORY }, {}, {  });
        items::addItem("hellforge", "Hellforge", "hellforge", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("coppersword", "Copper Sword", "coppersword", {}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("ironsword", "Iron Sword", "ironsword", {}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("silversword", "Silver Sword", "silversword", {}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("goldsword", "Gold Sword", "goldsword", {}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("copperaxe", "Copper Axe", "copperaxe", { itemFuncs::dig }, 40, { if_WEAPON, if_AXE, if_MEELE }, {});
        items::addItem("ironaxe", "Iron Axe", "ironaxe", { itemFuncs::dig }, 40, { if_WEAPON, if_AXE, if_MEELE }, {});
        items::addItem("silveraxe", "Silver Axe", "silveraxe", { itemFuncs::dig }, 40, { if_WEAPON, if_AXE, if_MEELE }, {});
        items::addItem("goldaxe", "Gold Axe", "goldaxe", { itemFuncs::dig }, 40, { if_WEAPON, if_AXE, if_MEELE }, {});
        items::addItem("copperhammer", "Copper Hammer", "copperhammer", { itemFuncs::dig }, 40, { if_WEAPON, if_HAMMER, if_MEELE }, {});
        items::addItem("ironhammer", "Iron Hammer", "ironhammer", { itemFuncs::dig }, 40, { if_WEAPON, if_HAMMER, if_MEELE }, {});
        items::addItem("silverhammer", "Silver Hammer", "silverhammer", { itemFuncs::dig }, 40, { if_WEAPON, if_HAMMER, if_MEELE }, {});
        items::addItem("goldhammer", "Gold Hammer", "goldhammer", { itemFuncs::dig }, 40, { if_WEAPON, if_HAMMER, if_MEELE }, {});
        items::addItem("copperpickaxe", "Copper Pickaxe", "copperpickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("ironpickaxe", "Iron Pickaxe", "ironpickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("silverpickaxe", "Silver Pickaxe", "silverpickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("goldpickaxe", "Gold Pickaxe", "goldpickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("copperbow", "Copper Bow", "copperbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("ironbow", "Iron Bow", "ironbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("silverbow", "Silver Bow", "silverbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("goldbow", "Gold Bow", "goldbow", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("moltenfury", "Molten Fury", "moltenfury", { itemFuncs::shootArrow, itemFuncs::removeAmmo }, 20, { if_BOW, if_RANGED }, {}, { itemConditions::hasAmmo }, ia_USE, 2);
        items::addItem("moltenhamaxe", "Molten Hamaxe", "moltenhamaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_HAMMER, if_AXE, if_MEELE });
        items::addItem("moltenpickaxe", "Molten Pickaxe", "moltenpickaxe", { itemFuncs::dig }, 15, { if_WEAPON, if_PICKAXE, if_MEELE });
        items::addItem("volcano", "Volcano", "volcano", {}, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("impstaff", "Imp Staff", "impstaff", { itemFuncs::impStaff, itemFuncs::takeMana }, 30, { if_WEAPON, if_SUMMON }, {}, { itemConditions::hasSummonSpace, itemConditions::hasMana });
        items::addItem("cactus", "Cactus", "cactus", { }, 30, { if_BLOCK });
        items::addItem("sunflower", "Sunflower", "sunfloweritem", { itemFuncs::place, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("heartcrystal", "Heart Crystal", "heartcrystal", { itemFuncs::increaseMaxHealth, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::heartCrystal });
        items::addItem("bottle", "Bottle", "bottle", { itemFuncs::place, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BLOCK }, {}, {});
        items::addItem("healthpotion", "Health Potion", "healthpot", { itemFuncs::heal, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::heal });
        items::addItem("mushroom", "Mushroom", "mushroomitem", { itemFuncs::heal, itemFuncs::removeAmmo }, globals::blockPlaceSpeed, { if_BLOCK }, {}, {itemConditions::heal});
        items::addItem("ironbreastplate", "Iron Breastplate", "ironbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("ironhelmet", "Iron Helmet", "ironhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("irongreaves", "Iron Greaves", "irongreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("silverbreastplate", "Silver Breastplate", "silverbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("silverhelmet", "Silver Helmet", "silverhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("silvergreaves", "Silver Greaves", "silvergreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("goldbreastplate", "Gold Breastplate", "goldbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("goldhelmet", "Gold Helmet", "goldhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("goldgreaves", "Gold Greaves", "goldgreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("copperbreastplate", "Copper Breastplate", "copperbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("copperhelmet", "Copper Helmet", "copperhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("coppergreaves", "Copper Greaves", "coppergreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("mininghelmet", "Mining Helmet", "mininghelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("moltenbreastplate", "Molten Breastplate", "moltenbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("moltenhelmet", "Molten Helmet", "moltenhelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("moltengreaves", "Molten Greaves", "moltengreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("cactusbreastplate", "Cactus Breastplate", "cactusbreastplate", {}, globals::blockPlaceSpeed, { if_BREASTPLATE }, {}, { itemConditions::hasAmmo });
        items::addItem("cactushelmet", "Cactus Helmet", "cactushelmet", {}, globals::blockPlaceSpeed, { if_HELMET }, {}, { });
        items::addItem("cactusgreaves", "Cactus Greaves", "cactusgreaves", {}, globals::blockPlaceSpeed, { if_GREAVES }, {}, {});
        items::addItem("shadowscales", "Shadow Scales", "shadowscales", {}, globals::blockPlaceSpeed, { if_MISC }, {}, {});
        items::addItem("bladeofgrass", "Blade of Grass", "bladeofgrass", { itemFuncs::bladeOfGrass }, 30, { if_WEAPON, if_MEELE }, {});
        items::addItem("stinger", "Stinger", "stinger", { }, 30, { if_MISC });
        items::addItem("junglespores", "Jungle Spores", "junglesporesitem", { }, 30, { if_MISC });
        items::addItem("vines", "Vines", "vinesitem", { }, 30, { if_MISC });
        items::addItem("daybloom", "Daybloom", "daybloom", { }, 30, { if_MISC });
        items::addItem("voodoodoll", "Voodo Doll", "voodoodollitem", { }, 30, { if_MISC });
        items::addItem("frozenchest", "Frozen Chest", "frozenchestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("goldchest", "Gold Chest", "goldchestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("shadowchest", "Shadow Chest", "shadowchestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("mahoganychest", "Mahogany Chest", "mahoganychestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("waterchest", "Water Chest", "waterchestitem", { itemFuncs::place }, globals::blockPlaceSpeed, { if_BLOCK }, {}, { itemConditions::hasAmmo });
        items::addItem("manacrystal", "Mana Crystal", "manacrystal", { itemFuncs::removeAmmo, itemFuncs::manaCrystal}, globals::blockPlaceSpeed, {if_BLOCK}, {}, {itemConditions::manaCrystal});

        items::addSet("none", itemFuncs::nonearmorbonus, "");
        items::addSet("ironarmor", itemFuncs::ironarmorbonus, "Set bonus: 2 defense");
        items::addSet("silverarmor", itemFuncs::silverarmorbonus, "Set bonus: 3 defense");
        items::addSet("goldarmor", itemFuncs::goldarmorbonus, "Set bonus: 3 defense");
        items::addSet("copperarmor", itemFuncs::copperarmorbonus, "Set bonus: 2 defense");
        items::addSet("moltenarmor", itemFuncs::moltenarmorbonus, "Set bonus: 10% meele damage");
        items::addSet("cactusarmor", itemFuncs::cactusarmorbonus, "Set bonus: Attackers take damage from the cactus spikes");

        items::addToSet("ironbreastplate", "ironarmor");
        items::addToSet("ironhelmet", "ironarmor");
        items::addToSet("irongreaves", "ironarmor");
        items::addToSet("silverbreastplate", "silverarmor");
        items::addToSet("silverhelmet", "silverarmor");
        items::addToSet("silvergreaves", "silverarmor");
        items::addToSet("goldbreastplate", "goldarmor");
        items::addToSet("goldhelmet", "goldarmor");
        items::addToSet("goldgreaves", "goldarmor");
        items::addToSet("copperbreastplate", "copperrarmor");
        items::addToSet("copperhelmet", "copperarmor");
        items::addToSet("coppergreaves", "copperarmor");
        items::addToSet("moltenbreastplate", "moltenarmor");
        items::addToSet("moltenhelmet", "moltenarmor");
        items::addToSet("moltengreaves", "moltenarmor");
        items::addToSet("cactusbreastplate", "cactusarmor");
        items::addToSet("cactushelmet", "cactusarmor");
        items::addToSet("cactusgreaves", "cactusarmor");

        items::addDroppedAnim("coppercoin", "coppercoin");
        items::addDroppedAnim("goldcoin", "goldcoin");
        items::addDroppedAnim("platinumcoin", "platinumcoin");
        items::addDroppedAnim("silvercoin", "silvercoin");

        items::addItemToNaturalChest("surface", "dirt", 0.5, 10, 5);
        items::addItemToNaturalChest("surface", "torch", 0.2, 15, 5);
        items::addItemToNaturalChest("surface", "healthpotion", 0.2, 1, 0);
        items::addItemToNaturalChest("surface", "aglet", 0.05, 1, 0);
        items::addItemToNaturalChest("surface", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("surface", "magicmirror", 0.05, 1, 0);
        items::addItemToNaturalChest("surface", "bandofregeneration", 0.05, 1, 0);
        items::addItemToNaturalChest("surface", "shackle", 0.15, 1, 0);
        items::addItemToNaturalChest("surface", "mushroom", 0.3, 4, 1);
        items::addItemToNaturalChest("surface", "bottle", 0.3, 2, 1);
        items::addItemToNaturalChest("surface", "sunflower", 0.1, 1, 0);
        items::addItemToNaturalChest("surface", "daybloom", 0.1, 4, 2);
        items::addItemToNaturalChest("surface", "bomb", 0.14, 3, 2);
        items::addItemToNaturalChest("surface", "arrow", 0.14, 10, 3);
        items::addItemToNaturalChest("underground", "torch", 0.4, 15, 5);
        items::addItemToNaturalChest("underground", "healthpotion", 0.1, 3, 2);
        items::addItemToNaturalChest("underground", "aglet", 0.15, 1, 0);
        items::addItemToNaturalChest("underground", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("underground", "magicmirror", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "bandofregeneration", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "bandofstarpower", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "shackle", 0.15, 1, 0);
        items::addItemToNaturalChest("underground", "bottle", 0.2, 3, 2);
        items::addItemToNaturalChest("underground", "copperore", 0.1, 10, 4);
        items::addItemToNaturalChest("underground", "ironore", 0.1, 6, 4);
        items::addItemToNaturalChest("underground", "ironbar", 0.1, 3, 1);
        items::addItemToNaturalChest("underground", "copperbar", 0.1, 3, 2);
        items::addItemToNaturalChest("underground", "enchantedsword", 0.01, 1, 0);
        items::addItemToNaturalChest("underground", "minishark", 0.01, 1, 0);
        items::addItemToNaturalChest("underground", "magicmissile", 0.01, 1, 0);
        items::addItemToNaturalChest("underground", "boomerang", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "bomb", 0.14, 5, 2);
        items::addItemToNaturalChest("underground", "arrow", 0.2, 40, 15);
        items::addItemToNaturalChest("underground", "bullet", 0.1, 40, 15);
        items::addItemToNaturalChest("underground", "hermesboots", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "cloudinabottle", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "slimestatue", 0.1, 1, 0);
        items::addItemToNaturalChest("underground", "mininghelmet", 0.05, 1, 0);
        items::addItemToNaturalChest("underground", "suseye", 0.05, 1, 0);
        items::addItemToNaturalChest("underground", "enchantedboomerang", 0.03, 1, 0);
        items::addItemToNaturalChest("underground", "starfury", 0.01, 1, 0);
        items::addItemToNaturalChest("hell", "torch", 0.4, 15, 5);
        items::addItemToNaturalChest("hell", "healthpotion", 0.1, 3, 2);
        items::addItemToNaturalChest("hell", "aglet", 0.15, 1, 0);
        items::addItemToNaturalChest("hell", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("hell", "magicmirror", 0.1, 1, 0);
        items::addItemToNaturalChest("hell", "bandofregeneration", 0.1, 1, 0);
        items::addItemToNaturalChest("hell", "shackle", 0.15, 1, 0);
        items::addItemToNaturalChest("hell", "bottle", 0.2, 3, 2);
        items::addItemToNaturalChest("hell", "goldore", 0.1, 10, 4);
        items::addItemToNaturalChest("hell", "silverore", 0.1, 6, 4);
        items::addItemToNaturalChest("hell", "goldbar", 0.1, 3, 1);
        items::addItemToNaturalChest("hell", "silverbar", 0.1, 3, 2);
        items::addItemToNaturalChest("hell", "enchantedsword", 0.01, 1, 0);
        items::addItemToNaturalChest("hell", "minishark", 0.01, 1, 0);
        items::addItemToNaturalChest("hell", "magicmissile", 0.01, 1, 0);
        items::addItemToNaturalChest("hell", "bomb", 0.14, 5, 2);
        items::addItemToNaturalChest("hell", "flamingarrow", 0.2, 40, 15);
        items::addItemToNaturalChest("hell", "demonicarrow", 0.2, 20, 5);
        items::addItemToNaturalChest("hell", "lightdisc", 0.2, 40, 15);
        items::addItemToNaturalChest("hell", "bullet", 0.1, 40, 15);
        items::addItemToNaturalChest("hell", "hellforge", 0.1, 1, 0);
        items::addItemToNaturalChest("hell", "hermesboots", 0.1, 1, 0);
        items::addItemToNaturalChest("hell", "bandofstarpower", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "torch", 0.4, 15, 5);
        items::addItemToNaturalChest("frozen", "healthpotion", 0.1, 3, 2);
        items::addItemToNaturalChest("frozen", "aglet", 0.15, 1, 0);
        items::addItemToNaturalChest("frozen", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("frozen", "magicmirror", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "bandofregeneration", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "bandofstarpower", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "shackle", 0.15, 1, 0);
        items::addItemToNaturalChest("frozen", "bottle", 0.2, 3, 2);
        items::addItemToNaturalChest("frozen", "copperore", 0.1, 10, 4);
        items::addItemToNaturalChest("frozen", "ironore", 0.1, 6, 4);
        items::addItemToNaturalChest("frozen", "ironbar", 0.1, 3, 1);
        items::addItemToNaturalChest("frozen", "copperbar", 0.1, 3, 2);
        items::addItemToNaturalChest("frozen", "iceblade", 0.03, 1, 0);
        items::addItemToNaturalChest("frozen", "snowballcannon", 0.01, 1, 0);
        items::addItemToNaturalChest("frozen", "bomb", 0.14, 5, 2);
        items::addItemToNaturalChest("frozen", "arrow", 0.2, 40, 15);
        items::addItemToNaturalChest("frozen", "snowball", 0.2, 20, 5);
        items::addItemToNaturalChest("frozen", "bullet", 0.1, 40, 15);
        items::addItemToNaturalChest("frozen", "hermesboots", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "cloudinabottle", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "slimestatue", 0.1, 1, 0);
        items::addItemToNaturalChest("frozen", "mininghelmet", 0.05, 1, 0);
        items::addItemToNaturalChest("frozen", "frozenboomerang", 0.03, 1, 0);
        items::addItemToNaturalChest("jungle", "torch", 0.4, 15, 5);
        items::addItemToNaturalChest("jungle", "healthpotion", 0.1, 3, 2);
        items::addItemToNaturalChest("jungle", "aglet", 0.15, 1, 0);
        items::addItemToNaturalChest("jungle", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("jungle", "magicmirror", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "bandofregeneration", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "bandofstarpower", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "ankletofthewind", 0.15, 1, 0);
        items::addItemToNaturalChest("jungle", "bottle", 0.2, 3, 2);
        items::addItemToNaturalChest("jungle", "copperore", 0.1, 10, 4);
        items::addItemToNaturalChest("jungle", "ironore", 0.1, 6, 4);
        items::addItemToNaturalChest("jungle", "ironbar", 0.1, 3, 1);
        items::addItemToNaturalChest("jungle", "junglespores", 0.1, 3, 2);
        items::addItemToNaturalChest("jungle", "feralclaws", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "stinger", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "bomb", 0.14, 5, 2);
        items::addItemToNaturalChest("jungle", "arrow", 0.2, 40, 15);
        items::addItemToNaturalChest("jungle", "vines", 0.1, 5, 2);
        items::addItemToNaturalChest("jungle", "bullet", 0.1, 40, 15);
        items::addItemToNaturalChest("jungle", "hermesboots", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "cloudinabottle", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "slimestatue", 0.1, 1, 0);
        items::addItemToNaturalChest("jungle", "mininghelmet", 0.05, 1, 0);
        items::addItemToNaturalChest("jungle", "beegun", 0.05, 1, 0);
        items::addItemToNaturalChest("water", "torch", 0.4, 15, 5);
        items::addItemToNaturalChest("water", "healthpotion", 0.1, 3, 2);
        items::addItemToNaturalChest("water", "aglet", 0.15, 1, 0);
        items::addItemToNaturalChest("water", "coppercoin", 0.2, 70, 10);
        items::addItemToNaturalChest("water", "magicmirror", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "bandofregeneration", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "bandofstarpower", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "shackle", 0.15, 1, 0);
        items::addItemToNaturalChest("water", "bottle", 0.2, 3, 2);
        items::addItemToNaturalChest("water", "copperore", 0.1, 10, 4);
        items::addItemToNaturalChest("water", "ironore", 0.1, 6, 4);
        items::addItemToNaturalChest("water", "ironbar", 0.1, 3, 1);
        items::addItemToNaturalChest("water", "copperbar", 0.1, 3, 2);
        items::addItemToNaturalChest("water", "enchantedsword", 0.01, 1, 0);
        items::addItemToNaturalChest("water", "aquastaff", 0.2, 1, 0);
        items::addItemToNaturalChest("water", "boomerang", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "bomb", 0.14, 5, 2);
        items::addItemToNaturalChest("water", "arrow", 0.2, 40, 15);
        items::addItemToNaturalChest("water", "bullet", 0.1, 40, 15);
        items::addItemToNaturalChest("water", "hermesboots", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "cloudinabottle", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "slimestatue", 0.1, 1, 0);
        items::addItemToNaturalChest("water", "mininghelmet", 0.05, 1, 0);
        items::addItemToNaturalChest("water", "suseye", 0.05, 1, 0);
        items::addItemToNaturalChest("water", "enchantedboomerang", 0.03, 1, 0);

        items::addOnPickup("coppercoin", itemFuncs::coppercoinonpickup);
        items::addOnPickup("silvercoin", itemFuncs::silvercoinonpickup);
        items::addOnPickup("goldcoin", itemFuncs::goldcoinonpickup);

        items::addLight("torch", glm::vec3(2.4, 1, 0.3));
        items::addLight("purpletorch", glm::vec3(2.3, 0.2, 4.0));

        items::addSizeMod("terrablade", 2.4);
        items::addSizeMod("aquascepter", 2);
        items::addSizeMod("starfury", 1.8);
        items::addSizeMod("iceblade", 1.8);
        items::addSizeMod("nightmarepickaxe", 1.8);
        items::addSizeMod("demonbow", 1.8);
        items::addSizeMod("waraxeofthenight", 1.8);
        items::addSizeMod("lightsbane", 1.8);
        items::addSizeMod("cactussword", 2.1);
        items::addSizeMod("cactuspickaxe", 1.8);
        items::addSizeMod("spacegun", 1.8);
        items::addSizeMod("enchantedsword", 1.8);
        items::addSizeMod("snowballcannon", 1.8);
        items::addSizeMod("vilethorn", 1.8);
        items::addSizeMod("beegun", 1.8);
        items::addSizeMod("copperpickaxe", 1.8);
        items::addSizeMod("copperaxe", 1.8);
        items::addSizeMod("coppersword", 1.8);
        items::addSizeMod("copperhammer", 1.8);
        items::addSizeMod("ironpickaxe", 1.8);
        items::addSizeMod("ironpickaxeaxe", 1.8);
        items::addSizeMod("ironpickaxesword", 1.8);
        items::addSizeMod("ironpickaxehammer", 1.8);
        items::addSizeMod("silverpickaxe", 1.8);
        items::addSizeMod("silveraxe", 1.8);
        items::addSizeMod("silversword", 1.8);
        items::addSizeMod("silverhammer", 1.8);
        items::addSizeMod("goldpickaxe", 1.8);
        items::addSizeMod("goldaxe", 1.8);
        items::addSizeMod("goldsword", 1.8);
        items::addSizeMod("goldhammer", 1.8);
        items::addSizeMod("woodenbow", 1.8);
        items::addSizeMod("ironbow", 1.8);
        items::addSizeMod("silverbow", 1.8);
        items::addSizeMod("goldbow", 1.8);
        items::addSizeMod("moltenfury", 1.8);
        items::addSizeMod("moltenhamaxe", 1.8);
        items::addSizeMod("moltenpickaxe", 1.8);
        items::addSizeMod("volcano", 2.2);
        items::addSizeMod("bladeofgrass", 2.2);

        items::disableAutouse("magicmissile");
        items::disableAutouse("starfury");
        items::disableAutouse("ironsword");
        items::disableAutouse("lightsbane");
        items::disableAutouse("woodenbow");
        items::disableAutouse("demonbow");
        items::disableAutouse("woodenboomerang");
        items::disableAutouse("enchantedboomerang");
        items::disableAutouse("volcano");
        items::disableAutouse("bladeofgrass");
        items::disableAutouse("ironsword");
        items::disableAutouse("coppersword");
        items::disableAutouse("goldsword");
        items::disableAutouse("platinumsword");

        items::addSound("minishark", &sounds::gun);
        items::addSound("aquastaff", &sounds::aquastaff);
        items::addSound("woodenbow", &sounds::bow);
        items::addSound("ironbow", &sounds::bow);
        items::addSound("goldbow", &sounds::bow);
        items::addSound("copperbow", &sounds::bow);
        items::addSound("silverbow", &sounds::bow);
        items::addSound("demonbow", &sounds::bow);
        items::addSound("moltenfury", &sounds::bow);
        items::addSound("mushroom", &sounds::eat);
        items::addSound("healthpotion", &sounds::drink);
        items::addSound("magicmirror", &sounds::magicmirror);
        items::addSound("spacegun", &sounds::spacegun);
        items::addSound("star", &sounds::star);
        items::addSound("manastar", &sounds::star);
        items::addSound("heartcrystal", &sounds::star);

        //volcanoeffect
        {
            particleEmmiterC pec;
            pec.amount = 2;
            pec.dir = glm::vec2(1, 0);
            pec.killentity = false;
            pec.lifespan = -1;
            pec.tex = "particle:torchpart";
            pec.light = glm::vec3(0.2, 0, 0);
            pec.ms = 0.01;
            pec.particleLifespan = 40;
            pec.randomizelifespan = 20;
            pec.radius = 1.5;
            pec.randangle = 180;
            pec.randomizems = 0.0008;
            pec.randomizescale = 0.1;
            pec.randrotation = 4;
            pec.rate = 1;
            pec.randomizerate = 0.5;
            pec.stopAsDie = true;
            pec.stoponcollision = false;
            pec.weight = 0;
            pec.size = glm::vec2(0.8, 0.8);
            pec.smallerAsDie = true;
            items::addEmmiter("volcano", pec);
        }

        items::addOnhit("volcano", collisionFs::volcanoDamage);
        items::addOnhit("bladeofgrass", collisionFs::bladeofgrassDamage);
        items::addOnhit("moltenpickaxe", collisionFs::moltenPickaxeDamage);
        items::addOnhit("moltenhamaxe", collisionFs::moltenHamaxeDamage);

        buffs::addBuff("nothing", "empty", buffs::nothing, true);
        buffs::addBuff("onfire", "onfirebuff", buffs::onfirebuff, false, "Slowly losing life");
        buffs::addBuff("twins", "twinsbuff", buffs::nothing, true, "The twins will fight for you ");
        buffs::addBuff("imp", "impbuff", buffs::nothing, true, "The imp will fight for you ");
        buffs::addBuff("cozyfire", "cozyfirebuff", buffs::cozyfirebuff, true, "Life regen is slightly increased");
        buffs::addBuff("happy", "happybuff", buffs::happybuff, true, "Movement speed increased and monster spawns reduced ");
        buffs::addBuff("potionsickness", "potionsicknessbuff", buffs::nothing, false, "Cannot consume anymore healing items");
        buffs::addBuff("poisoned", "poisonedbuff", buffs::poisoned, false, "Slowly losing life");
        buffs::addBuff("chilled", "chilledbuff", buffs::chilled, false, "Your movement speed has been reduced");
   

    }
    void resources::loadAssets(bool shaders, bool textures, bool items)
    {
        std::fstream file("assets/data.txt", std::ios::in);
        std::string s;
        while (file) {
            std::getline(file, s);
            if (s == "items") {
                loadItems(&file);
            }
            else if (s == "lights") {
                loadLights(&file);
            }
        }
        file.close();

        if (shaders) {
            glDeleteProgram(globals::spriteShaderID);
            glDeleteProgram(globals::blockShaderID);
            glDeleteProgram(globals::rayCastingShaderID);
            glDeleteProgram(globals::testLightingShaderID);
            glDeleteProgram(globals::displayShaderID);
            glDeleteProgram(globals::backgroundShaderID);
            glDeleteProgram(globals::lightShaderID);
            glDeleteProgram(globals::zoomShaderID);
            glDeleteProgram(globals::bloomShaderID);

            utils::createShader("src/shaders/spriteFrag.frag", "src/shaders/spriteVert.vert", &globals::spriteShaderID);
            utils::createShader("src/shaders/blockFrag.frag", "src/shaders/blockVert.vert", &globals::blockShaderID);
            utils::createShader("src/shaders/rayCastingFrag.frag", "src/shaders/overlayVert.vert", &globals::rayCastingShaderID);
            utils::createShader("src/shaders/testLighting.frag", "src/shaders/overlayVert.vert", &globals::testLightingShaderID);
            utils::createShader("src/shaders/displayFrag.frag", "src/shaders/overlayVert.vert", &globals::displayShaderID);
            utils::createShader("src/shaders/backgroundFrag.frag", "src/shaders/overlayVert.vert", &globals::backgroundShaderID);
            utils::createShader("src/shaders/lightFrag.frag", "src/shaders/blockVert.vert", &globals::lightShaderID);
            utils::createShader("src/shaders/zoomFrag.frag", "src/shaders/overlayVert.vert", &globals::zoomShaderID);
            utils::createShader("src/shaders/bloomFrag.frag", "src/shaders/overlayVert.vert", &globals::bloomShaderID);
            utils::createShader("src/shaders/waterFrag.frag", "src/shaders/waterVert.vert", &globals::waterShaderID);
        }

        if (textures) {
            glDeleteTextures(1, &textures::spriteSheet);
            glDeleteTextures(1, &globals::broken1Tex);
            glDeleteTextures(1, &globals::broken2Tex);
            glDeleteTextures(1, &globals::broken3Tex);
            background::load();

            textures::spriteSheet = utils::LoadTexture("assets/spritesheet2.png");
            textures::playerSheet = utils::LoadTexture("assets/playersheet.png");

            globals::broken1Tex = utils::LoadTexture("assets/broken1.png");
            globals::broken2Tex = utils::LoadTexture("assets/broken2.png");
            globals::broken3Tex = utils::LoadTexture("assets/broken3.png");
            
            globals::liquidTex = utils::LoadTexture("assets/liquid.png", 4, true);
        }

        if (items) {
            createBlocks();
        }
        createProjectiles();
        createEnemies();
        createEffects();
    }

    void createBlocks() {
        //animations
        {
            animations::clear();
            animations::createAnim("playerwalkright", textures::nametocoords["player"]->coords, { {0, {16, 0, 16, 0}, ft_OFFSET1},{5, {32, 0, 32, 0}, ft_OFFSET1}, {10, {48, 0, 48, 0}, ft_OFFSET1} }, 15);
            animations::createAnim("playerwalkleft", textures::nametocoords["player"]->coords, { {0, {2, 0, 0, 0}, ft_OFFSET16},{5, {3, 0, 1, 0}, ft_OFFSET16}, {10, {4, 0, 2, 0}, ft_OFFSET16} }, 15);
            animations::createAnim("playeridle", textures::nametocoords["player"]->coords, { {300, {4, 0, 4, 0}, ft_OFFSET16}, {305, {5, 0, 5, 0}, ft_OFFSET16},{315, {6, 0, 6, 0}, ft_OFFSET16 } }, 325, true, 1);
            animations::createAnim("playeredit", textures::nametocoords["player"]->coords, { {0, {7,0,7,0}, ft_OFFSET16},{5, {8,0,8,0}, ft_OFFSET16} }, 10);

            animations::createAnim("zombiewalk", textures::nametocoords["zombie"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET8}, {5, {0, -3, 0, -3}, ft_OFFSET8}, {10, {0,-6, 0, -6}, ft_OFFSET8} }, 15);

            animations::createAnim("slimejump", textures::nametocoords["greenslime"]->coords, { {0, {0, -13, 0, -11} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 8);
            animations::createAnim("iceslimejump", textures::nametocoords["iceslime"]->coords, { {0, {0, -13, 0, -11} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 8);
            animations::createAnim("lavaslimejump", textures::nametocoords["lavaslime"]->coords, { {0, {0, -13, 0, -11} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 8);

            std::vector<animations::keyframe> tmpframes;
            for (int i = 0; i < 7; i++) {
                tmpframes.push_back({ i * 6, {0, -i, 0, -i}, ft_OFFSET16 });
            }
            animations::createAnim("bunnyhop", textures::nametocoords["bunny"]->coords, { tmpframes }, 7 * 6);
            animations::createAnim("bunny", textures::nametocoords["bunny"]->coords, { {0, {0, 0, 0, 0} , ft_OFFSET1 } }, 1);
            tmpframes.clear();
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * 16, {0, -i * 12, 0, -i * 12}, ft_OFFSET1 });
            }
            animations::createAnim("fallenstar", textures::nametocoords["fallenstar"]->coords, { tmpframes }, 8 * 16);

            int speed = 8;
            int pixels = 7;
            tmpframes.clear();
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * speed, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1});
            }
            animations::createAnim("coppercoin", textures::nametocoords["coppercoindrop"]->coords, { tmpframes }, 8 * speed);

            pixels = 9;
            tmpframes.clear();
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * speed, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("platinumcoin", textures::nametocoords["platinumcoindrop"]->coords, { tmpframes }, 8 * speed);

            pixels = 9;
            tmpframes.clear();
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * speed, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("goldcoin", textures::nametocoords["goldcoindrop"]->coords, { tmpframes }, 8 * speed);

            pixels = 8;
            tmpframes.clear();
            speed = 5;
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * speed, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("silvercoin", textures::nametocoords["silvercoindrop"]->coords, { tmpframes }, 8 * speed);
            animations::createAnim("bee", textures::nametocoords["bee"]->coords, { {2, {0, -6, 0, -6}, ft_OFFSET1},{4, {0, -12, 0, -12}, ft_OFFSET1}, {6, {0, -18, 0,-18}, ft_OFFSET1} }, 8);
        
            pixels = 22;
            tmpframes.clear();
            for (int i = 0; i < 8; i++) {
                tmpframes.push_back({ i * speed, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("imp", textures::nametocoords["imp"]->coords, { tmpframes }, 8 * speed);

            pixels = 4;
            tmpframes.clear();
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 2, {0, -i * pixels - 8, 0, -i * pixels - 8}, ft_OFFSET1 });
            }
            animations::createAnim("mousewalk", textures::nametocoords["mouse"]->coords, { tmpframes }, 9);
            animations::createAnim("mouse", textures::nametocoords["mouse"]->coords, { {2, {0, -4, 0, -4}, ft_OFFSET1} }, 4);

            animations::createAnim("colorableslimejump", textures::nametocoords["colorableslime"]->coords, { {0, {0, -17, 0, -16} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 6);
            animations::createAnim("spikediceslimejump", textures::nametocoords["spikediceslime"]->coords, { {0, {0, -16, 0, -15} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 6);
            animations::createAnim("spikedjungleslimejump", textures::nametocoords["spikedjungleslime"]->coords, { {0, {0, -16, 0, -15} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 6);
            
            animations::createAnim("eaterofsouls", textures::nametocoords["eaterofsouls"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET1 },{5, {0, -41, 0, -39} , ft_OFFSET1 } }, 10, true);
        
            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 4, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("cavebatfly", textures::nametocoords["cavebat"]->coords, { tmpframes }, 17);
            animations::createAnim("cavebathang", textures::nametocoords["cavebat"]->coords, { {0, {0, -35, 0, -35}, ft_OFFSET1 } }, 1);
        
            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 4, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("junglebatfly", textures::nametocoords["junglebat"]->coords, { tmpframes }, 17);
            animations::createAnim("junglebathang", textures::nametocoords["junglebat"]->coords, { {0, {0, -35, 0, -35}, ft_OFFSET1 } }, 1);


            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 4, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("hellbatfly", textures::nametocoords["hellbat"]->coords, { tmpframes }, 17);
            animations::createAnim("hellbathang", textures::nametocoords["hellbat"]->coords, { {0, {0, -35, 0, -35}, ft_OFFSET1 } }, 1);

            tmpframes.clear();
            pixels = 29;
            for (int i = 0; i < 5; i++) {
                tmpframes.push_back({ i * 6, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("demon", textures::nametocoords["demon"]->coords, { tmpframes }, 31);
        
            tmpframes.clear();
            pixels = 24;
            for (int i = 1; i <= 5; i++) {
                tmpframes.push_back({ i * 6, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("vulturefly", textures::nametocoords["vulture"]->coords, { tmpframes }, 5 * 6);

            tmpframes.clear();
            pixels = 19;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 6, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("hornetfly", textures::nametocoords["hornet"]->coords, { tmpframes }, 3 * 6);

            tmpframes.clear();
            pixels = 19;
            for (int i = 0; i < 6; i++) {
                tmpframes.push_back({ i * 5, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("antlionchargerwalk", textures::nametocoords["antlioncharger"]->coords, { tmpframes }, 6 * 5);
            animations::createAnim("darkcastercast", textures::nametocoords["darkcastercast"]->coords, { { 0, {0,0,0,0}, ft_OFFSET1 } }, 30, false);
            
            tmpframes.clear();
            pixels = 83;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 9, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("eyeofcthulu1", textures::nametocoords["eyeofcthulu1"]->coords, { tmpframes }, 3 * 9);

            tmpframes.clear();
            pixels = 83;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 9, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("eyeofcthulu2", textures::nametocoords["eyeofcthulu2"]->coords, { tmpframes }, 3 * 9);

            tmpframes.clear();
            pixels = 10;
            for (int i = 0; i < 7; i++) {
                tmpframes.push_back({ i * 2, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("bladeofgrassprojectile", textures::nametocoords["bladeofgrassprojectile"]->coords, { tmpframes }, 2 * 8);

            tmpframes.clear();
            pixels = 19;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels }, ft_OFFSET1 });
            }
            animations::createAnim("penguin1walk", textures::nametocoords["pengu1"]->coords, { tmpframes }, 3 * 4);
            animations::createAnim("penguin1", textures::nametocoords["pengu1"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET1} }, 1);

            tmpframes.clear();
            pixels = 19;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("penguin2walk", textures::nametocoords["pengu2"]->coords, { tmpframes }, 3 * 4);
            animations::createAnim("penguin2", textures::nametocoords["pengu2"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET1} }, 1);

            tmpframes.clear();
            pixels = 20;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("penguin3walk", textures::nametocoords["pengu3"]->coords, { tmpframes }, 3 * 4);
            animations::createAnim("penguin3", textures::nametocoords["pengu3"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET1} }, 1);

            tmpframes.clear();
            pixels = 19;
            for (int i = 0; i < 3; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("penguin4walk", textures::nametocoords["pengu4"]->coords, { tmpframes }, 3 * 4);
            animations::createAnim("penguin4", textures::nametocoords["pengu4"]->coords, { {0, {0, 0, 0, 0}, ft_OFFSET1} }, 1);

            animations::createAnim("grasshopperjump", textures::nametocoords["grasshopper"]->coords, { {0, {0, -5, 0, -5} , ft_OFFSET1 }, {4, {0, 0, 0, 0} , ft_OFFSET1 } }, 8);

            animations::createAnim("fireflylit", textures::nametocoords["fireflylit"]->coords, { {0, {0, 0, 0, 0} , ft_OFFSET1 }, {2, {0, -4, 0, -4} , ft_OFFSET1 } }, 4);
            animations::createAnim("firefly", textures::nametocoords["firefly"]->coords, { {0, {0, 0, 0, 0} , ft_OFFSET1 }, {2, {0, -4, 0, -4} , ft_OFFSET1 } }, 4);

            tmpframes.clear();
            pixels = 24;
            for (int i = 0; i < 11; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("guidewalk", textures::nametocoords["guidewalk"]->coords, { tmpframes }, 3 * 11);

            tmpframes.clear();
            pixels = 24;
            for (int i = 0; i < 11; i++) {
                tmpframes.push_back({ i * 3, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("merchantwalk", textures::nametocoords["merchantwalk"]->coords, { tmpframes }, 3 * 11);

            tmpframes.clear();
            pixels = 11;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 2, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("scorpionwalk", textures::nametocoords["scorpion"]->coords, { tmpframes }, 4 * 2);

            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 6; i++) {
                tmpframes.push_back({ i * 8, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("frogidle", textures::nametocoords["frog"]->coords, { tmpframes }, 8 * 6);

            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 7; i++) {
                tmpframes.push_back({ i * 4, {0, -45 -i * pixels, 0, -45 -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("frogwalk", textures::nametocoords["frog"]->coords, { tmpframes }, 7 * 4);

            tmpframes.clear();
            pixels = 4;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 6, {0, - i * pixels, 0, - i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("grubbywalk", textures::nametocoords["grubby"]->coords, { tmpframes }, 6 * 4);

            tmpframes.clear();
            pixels = 8;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 6, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("snailwalk", textures::nametocoords["snail"]->coords, { tmpframes }, 6 * 4);

            animations::createAnim("snailstuck", textures::nametocoords["snail"]->coords, { {0, {0, 0, 0, 0} , ft_OFFSET1 }, {6, {0, -32, 0, -32} , ft_OFFSET1 }, {12, {0, -40, 0, -40} , ft_OFFSET1 } }, 18, true, 1);

            tmpframes.clear();
            pixels = 8;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 6, {0, -i * pixels, 0, -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("snailwalk", textures::nametocoords["snail"]->coords, { tmpframes }, 6 * 4);

            tmpframes.clear();
            pixels = 9;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 3, {0, -9 -i * pixels, 0, -9 -i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("birdfly", textures::nametocoords["bird"]->coords, { tmpframes }, 3 * 4);

            tmpframes.clear();
            pixels = 11;
            for (int i = 0; i < 4; i++) {
                tmpframes.push_back({ i * 6, {0, - i * pixels, 0, - i * pixels}, ft_OFFSET1 });
            }
            animations::createAnim("goldfishswim", textures::nametocoords["goldfish"]->coords, { tmpframes }, 6 * 4);

            animations::createAnim("goldfishonground", textures::nametocoords["goldfish"]->coords, { {0, {0, -45, 0, -48} , ft_OFFSET1 }, {4, {0, -53, 0, -56} , ft_OFFSET1 } }, 8);

            animations::createAnim("jellyfishboost", textures::nametocoords["jellyfish"]->coords, { {0, {0, -0, 0, -0} , ft_OFFSET1 }, {3, {0, -15, 0, -15}, ft_OFFSET1 },
                                                                                                    {6, {0, -30, 0, -30}, ft_OFFSET1 }, {40, {0, -45, 0, -45}, ft_OFFSET1 } },44, false);


            items::addArmorSprites("skin", textures::nametocoords["plrhands"]->coords, glm::vec4(0));
            items::addArmorSprites("empty", textures::nametocoords["plrclothes"]->coords, textures::nametocoords["plremptylegswalk"]->coords);
            items::addArmorSprites("gold", textures::nametocoords["plrgoldarmor"]->coords, textures::nametocoords["plrgoldarmorlegswalk"]->coords);
            items::addArmorSprites("silver", textures::nametocoords["plrsilverarmor"]->coords, textures::nametocoords["plrsilverarmorlegswalk"]->coords);
            items::addArmorSprites("iron", textures::nametocoords["plrironarmor"]->coords, textures::nametocoords["plrironarmorlegswalk"]->coords);
            items::addArmorSprites("copper", textures::nametocoords["plrcopperarmor"]->coords, textures::nametocoords["plrcopperarmorlegswalk"]->coords);
            items::addArmorSprites("cactus", textures::nametocoords["plrcactusarmor"]->coords, textures::nametocoords["plrcactusarmorlegswalk"]->coords);
            items::addArmorSprites("molten", textures::nametocoords["plrmoltenarmor"]->coords, textures::nametocoords["plrmoltenarmorlegswalk"]->coords);
            items::addArmorSprites("shoes", textures::nametocoords["plrshoes"]->coords, textures::nametocoords["plrshoeswalk"]->coords);

        }

        //rules
        {
        blocks::addRule("grass", { {br_IS, bs_STAT, glm::vec2(1,0), "updates", "blocks"},
                                    {br_IS, bs_STAT, glm::vec2(-1,0), "updates", "blocks"},
                                    {br_IS, bs_STAT, glm::vec2(0,1), "updates", "blocks"},
                                    {br_IS, bs_STAT, glm::vec2(0,-1), "updates", "blocks"}
            }, BRules::replaceWith, "dirt", "blocks");

        blocks::addRule("trunk", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "trunk", "blocks"},
                                    {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "trunkbase1", "blocks"},
            }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("trunkbase1", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("trunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("trunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(1,0), "trunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("trunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("trunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(-1,0), "trunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("leaves", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "trunk", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("normalbranchr", { { br_ISNT, bs_BLOCK, glm::vec2(2, 0), "trunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("normalbranchl", { { br_ISNT, bs_BLOCK, glm::vec2(-1, 0), "trunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("normalgrassdecor",  { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "normalgrassdecor", "blocks"},
                                            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "grass", "blocks"}
                                        }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("junglegrassdecor", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "junglegrassdecor", "blocks"},
                                {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "junglegrass", "blocks"}
            }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("corruptgrassdecor", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corruptgrassdecor", "blocks"},
                                {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corruptgrass", "blocks"}
            }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("normalflowers", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "normalgrassdecor", "blocks"},
                                            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "grass", "blocks"}
            }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("jungleflowers", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "jungleflowers", "blocks"},
                                    {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "junglegrass", "blocks"}
            }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("corruptflowers", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corruptflowers", "blocks"},
                                    {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corruptgrass", "blocks"}
            }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("torch", { {br_ISNT, bs_STAT, glm::vec2(1,0), "updates", "blocks"},
                                    {br_ISNT, bs_STAT, glm::vec2(-1,0), "updates", "blocks"},
                                    {br_ISNT, bs_STAT, glm::vec2(0,1), "updates", "blocks"},
                                    {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"},
                                    {br_ISNT, bs_STAT, glm::vec2(0,0), "updates", "bg"}
                                    }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("purpletorch", { {br_ISNT, bs_STAT, glm::vec2(1,0), "updates", "blocks"},
                                            {br_ISNT, bs_STAT, glm::vec2(-1,0), "updates", "blocks"},
                                            {br_ISNT, bs_STAT, glm::vec2(0,1), "updates", "blocks"},
                                            {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"},
                                            {br_ISNT, bs_STAT, glm::vec2(0,0), "updates", "bg"}
                                            }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sand", { {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"} }, BRules::breakSelf, "empty", "blocks", false);
        blocks::addRule("sand", { {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"} }, BRules::spawnSand);
        blocks::addRule("corrupttrunk", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corrupttrunk", "blocks"},
                        {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corrupttrunkbase1", "blocks"},
            }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corrupttrunkbase1", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corrupttrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corrupttrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(1,0), "corrupttrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corrupttrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "dirt", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corrupttrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(-1,0), "corrupttrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptleaves", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corrupttrunk", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptbranchr", { { br_ISNT, bs_BLOCK, glm::vec2(2, 0), "corrupttrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptbranchl", { { br_ISNT, bs_BLOCK, glm::vec2(-1, 0), "corrupttrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunk", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "borealtrunk", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "borealtrunkbase1", "blocks"},
            }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunkbase1", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "snow", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "snow", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(1,0), "borealtrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "snow", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealtrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(-1,0), "borealtrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealleaves", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "borealtrunk", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealbranchr", { { br_ISNT, bs_BLOCK, glm::vec2(3, 0), "borealtrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealbranchl", { { br_ISNT, bs_BLOCK, glm::vec2(-1, 0), "borealtrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunk", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mahoganytrunk", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mahoganytrunkbase1", "blocks"},
            }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunkbase1", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mud", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mud", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunkbase2", { {br_ISNT, bs_BLOCK, glm::vec2(1,0), "mahoganytrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mud", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganytrunkbase3", { {br_ISNT, bs_BLOCK, glm::vec2(-1,0), "mahoganytrunkbase1", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganyleaves", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "mahoganytrunk", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("junglebranchr", { { br_ISNT, bs_BLOCK, glm::vec2(3, 0), "mahoganytrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("junglebranchl", { { br_ISNT, bs_BLOCK, glm::vec2(-1, 0), "mahoganytrunk", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("icicle", { { br_ISNT, bs_BLOCK, glm::vec2(0, 2), "snow", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("stalaktit", { { br_ISNT, bs_BLOCK, glm::vec2(0, 2), "stone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptstalaktit", { { br_ISNT, bs_BLOCK, glm::vec2(0, 2), "ebonstone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sandstalaktit", { { br_ISNT, bs_BLOCK, glm::vec2(0, 2), "sandstone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("stalagmit", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptstalagmit", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "ebonstone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sandstalagmit", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "sandstone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("smallrock", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "grass", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mediumrock", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "grass", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" },
    { br_ISNT, bs_BLOCK, glm::vec2(2, -1), "grass", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "stone", "blocks" }, }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("bigrock", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "grass", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" },
            { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "grass", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(1, -1), "stone", "blocks" }, 
            { br_ISNT, bs_BLOCK, glm::vec2(2, -1), "grass", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "stone", "blocks" },}, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("smallice", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "snow", "blocks" }}, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mediumice", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "snow", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "snow", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("bigice", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "snow", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "snow", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "snow", "blocks" }, }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mediumjungle", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "sjunglegrassnow", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "snow", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("bigjungle", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "junglegrass", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "snow", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "snow", "blocks" }, }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("smalldesert", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "sandstone", "blocks" }, { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "hardenedsand", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mediumdesert", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "sandstone", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(0, -1), "hardenedsand", "blocks" },
    { br_ISNT, bs_BLOCK, glm::vec2(2, -1), "sandstone", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "hardenedsand", "blocks" }, }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("bigdesert", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "sandstone", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(0, -1), "hardenedsand", "blocks" },
            { br_ISNT, bs_BLOCK, glm::vec2(1, -1), "sandstone", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(1, -1), "hardenedsand", "blocks" },
            { br_ISNT, bs_BLOCK, glm::vec2(2, -1), "sandstone", "blocks" },{ br_ISNT, bs_BLOCK, glm::vec2(2, -1), "hardenedsand", "blocks" }, }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("pot", { { br_ISNT, bs_STAT, glm::vec2(0, -1), "updates", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("pot", { { br_ISNT, bs_STAT, glm::vec2(1, -1), "updates", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sapling", {  {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "grass", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("mahoganysapling", {    {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "junglegrass", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("corruptsapling", {  {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "corruptgrass", "blocks"}   }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("borealsapling", {  {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "snow", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("undergroundskeletondecor", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("undergroundtooldecor", { { br_ISNT, bs_BLOCK, glm::vec2(0, -1), "stone", "blocks" } }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "sand", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusleft", "blocks"}, 
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusright", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusleft", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "sand", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusleft", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusright", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusright", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "sand", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusleft", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusright", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactustop", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusleft", "blocks"}, 
            {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusright", "blocks"} }, BRules::replaceWith, "cactustop", "blocks", true);
        blocks::addRule("cactustop", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusleft", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusright", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusleftarm", { {br_ISNT, bs_BLOCK, glm::vec2(1,0), "cactusleft", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusrightarm", { {br_ISNT, bs_BLOCK, glm::vec2(-1,0), "cactusright", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusarmleftmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusarmleftmid", "blocks"},
                                            {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusleftarm", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusarmrightmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusarmrightmid", "blocks"},
                                                {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusrightarm", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusarmleftmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusarmleftmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusarmlefttop", "blocks"} }, BRules::replaceWith, "cactusarmlefttop", "blocks", true);
        blocks::addRule("cactusarmrightmid", { {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusarmrightmid", "blocks"},
            {br_ISNT, bs_BLOCK, glm::vec2(0,1), "cactusarmrighttop", "blocks"} }, BRules::replaceWith, "cactusarmrightop", "blocks", true);
        blocks::addRule("cactusarmlefttop", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusarmleftmid", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("cactusarmrighttop", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "cactusarmrightmid", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sunflower", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "grass", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("sunflower", { {br_ISNT, bs_BLOCK, glm::vec2(0,-1), "grass", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("heartcrystal", { {br_ISNT, bs_STAT, glm::vec2(1,-1), "updates", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("heartcrystal", { {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("bottle", { {br_ISNT, bs_STAT, glm::vec2(0,-1), "updates", "blocks"} }, BRules::breakSelf, "empty", "blocks", true);
        blocks::addRule("normalvines", { {br_ISNT, bs_BLOCK, glm::vec2(0,1), "normalvines", "blocks"},
                                    {br_ISNT, bs_BLOCK, glm::vec2(0,1), "grass", "blocks"},
            }, BRules::breakSelfAndUpdateBelow, "empty", "blocks", true);
        blocks::addRule("junglevines", { {br_ISNT, bs_BLOCK, glm::vec2(0,1), "junglevines", "blocks"},
                                    {br_ISNT, bs_BLOCK, glm::vec2(0,1), "junglegrass", "blocks"},
            }, BRules::breakSelfAndUpdateBelow, "empty", "blocks", true);
        }

        //recipes
        {
            items::recipes.clear();
            items::addRecipe({ {{"wood", 1}}, 4, "torch", { "none" } });
            items::addRecipe({ {{"sand", 5}}, 1, "glass", { "furnace" } });
            items::addRecipe({ {{"wood", 1}}, 4, "woodwall", { "none" } });
            items::addRecipe({ {{"woodwall", 4}}, 1, "wood", { "none" } });
            items::addRecipe({ {{"wood", 8}, {"ironbar", 2}}, 1, "chest", { "craftingbench" } });
            items::addRecipe({ {{"ironore", 3}}, 1, "ironbar", { "furnace" } });
            items::addRecipe({ {{"stone", 50}}, 1, "slimestatue", { "craftingbench" } });
            items::addRecipe({ {{"wood", 6}}, 1, "door", { "craftingbench" } });
            items::addRecipe({ {{"wood", 4},{"stone", 20},{"torch", 3}}, 1, "furnace", { "craftingbench" } });
            items::addRecipe({ {{"wood", 8}}, 1, "table", { "craftingbench" } });
            items::addRecipe({ {{"wood", 4}}, 1, "chair", { "craftingbench" } });
            items::addRecipe({ {{"ironbar", 5}}, 1, "anvil", { "craftingbench" } });
            items::addRecipe({ {{"wood", 10}}, 1, "craftingbench", { "none" } });
            items::addRecipe({ {{"borealwood", 8}}, 1, "borealtable", { "craftingbench" } });
            items::addRecipe({ {{"borealwood", 6}}, 1, "borealdoor", { "craftingbench" } });
            items::addRecipe({ {{"borealwood", 10}}, 1, "borealworkbench", { "none" } });
            items::addRecipe({ {{"mahoganywood", 8}}, 1, "mahoganytable", { "craftingbench" } });
            items::addRecipe({ {{"mahoganywood", 6}}, 1, "mahoganydoor", { "craftingbench" } });
            items::addRecipe({ {{"mahoganywood", 10}}, 1, "mahoganyworkbench", { "none" } });
            items::addRecipe({ {{"ebonwoodwood", 8}}, 1, "ebonwoodtable", { "craftingbench" } });
            items::addRecipe({ {{"ebonwoodwood", 6}}, 1, "ebonwooddoor", { "craftingbench" } });
            items::addRecipe({ {{"ebonwoodwood", 10}}, 1, "ebonwoodworkbench", { "none" } });
            items::addRecipe({ {{"wood", 1}}, 2, "woodenplatform", { "none" } });
            items::addRecipe({ {{"wood", 10}, {"torch", 5 }}, 1, "campfire", {"none"} });
            items::addRecipe({ {{"hellstone", 3}, {"obsidian", 1}}, 1, "hellstonebar", {"hellforge"}});
            items::addRecipe({ {{"demoniteore", 3}}, 1, "demonitebar", {"furnace"} });
            items::addRecipe({ {{"goldore", 4}}, 1, "goldbar", {"furnace"} });
            items::addRecipe({ {{"silverore", 4}}, 1, "silverbar", {"furnace"} });
            items::addRecipe({ {{"hellstonebar", 17}}, 1, "impstaff", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 20}}, 1, "volcano", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 20}}, 1, "moltenpickaxe", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 15}}, 1, "moltenhamaxe", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 15}}, 1, "moltenfury", {"anvil"} });
            items::addRecipe({ {{"goldbar", 7}}, 1, "goldbow", {"anvil"} });
            items::addRecipe({ {{"silverbar", 7}}, 1, "silverbow", {"anvil"} });
            items::addRecipe({ {{"ironbar", 7}}, 1, "ironbow", {"anvil"} });
            items::addRecipe({ {{"copperbar", 7}}, 1, "copperbow", {"anvil"} });
            items::addRecipe({ {{"goldbar", 10}, {"wood", 4}}, 1, "goldpickaxe", {"anvil"} });
            items::addRecipe({ {{"silverbar", 10}, {"wood", 4}}, 1, "silverpickaxe", {"anvil"} });
            items::addRecipe({ {{"ironbar", 10}, {"wood", 4}}, 1, "ironpickaxe", {"anvil"} });
            items::addRecipe({ {{"copperbar", 10}, {"wood", 4}}, 1, "copperpickaxe", {"anvil"} });
            items::addRecipe({ {{"goldbar", 10}, {"wood", 3}}, 1, "goldhammer", {"anvil"} });
            items::addRecipe({ {{"silverbar", 10}, {"wood", 3}}, 1, "silverhammer", {"anvil"} });
            items::addRecipe({ {{"copperbar", 10}, {"wood", 3}}, 1, "copperhammer", {"anvil"} });
            items::addRecipe({ {{"ironbar", 10}, {"wood", 3}}, 1, "ironhammer", {"anvil"} });
            items::addRecipe({ {{"goldbar", 9}, {"wood", 3}}, 1, "goldaxe", {"anvil"} });
            items::addRecipe({ {{"silverbar", 9}, {"wood", 3}}, 1, "silveraxe", {"anvil"} });
            items::addRecipe({ {{"ironbar", 9}, {"wood", 3}}, 1, "ironaxe", {"anvil"} });
            items::addRecipe({ {{"copperbar", 9}, {"wood", 3}}, 1, "copperaxe", {"anvil"} });
            items::addRecipe({ {{"goldbar", 8}}, 1, "goldsword", {"anvil"} });
            items::addRecipe({ {{"silverbar", 8}}, 1, "silversword", {"anvil"} });
            items::addRecipe({ {{"ironbar", 8}}, 1, "ironsword", {"anvil"} });
            items::addRecipe({ {{"copperbar", 8}}, 1, "coppersword", {"anvil"} });
            items::addRecipe({ {{"snow", 1}}, 15, "snowball", {"none"} });
            items::addRecipe({ {{"cactus", 15}}, 1, "cactuspickaxe", {"workbench"} });
            items::addRecipe({ {{"cactus", 10}}, 1, "cactussword", {"workbench"} });
            items::addRecipe({ {{"demonitebar", 8}}, 1, "demonbow", {"anvil"} });
            items::addRecipe({ {{"demonitebar", 10}}, 1, "waraxeofthenight", {"anvil"} });
            items::addRecipe({ {{"demonitebar", 8}}, 1, "lightsbane", {"anvil"} });
            items::addRecipe({ {{"woodenboomerang", 1}, {"fallenstar", 1}}, 1, "enchantedboomerang", {"anvil"} });
            items::addRecipe({ {{"woodenboomerang", 1}, {"snow", 50}}, 1, "frozenboomerang", {"anvil"} });
            items::addRecipe({ {{"glass", 1}}, 2, "bottle", {"furnace"} });
            items::addRecipe({ {{"bottle", 1}, {"mushroom", 5}}, 1, "healthpotion", {"bottle"} });
            items::addRecipe({ {{"copperbar", 20}}, 1, "copperbreastplate", {"anvil"} });
            items::addRecipe({ {{"copperbar", 16}}, 1, "coppergreaves", {"anvil"} });
            items::addRecipe({ {{"copperbar", 12}}, 1, "copperhelmet", {"anvil"} });
            items::addRecipe({ {{"silverbar", 15}}, 1, "silverhelmet", {"anvil"} });
            items::addRecipe({ {{"silverbar", 20}}, 1, "silvergreaves", {"anvil"} });
            items::addRecipe({ {{"silverbar", 25}}, 1, "silverbreastplate", {"anvil"} });
            items::addRecipe({ {{"goldbar", 30}}, 1, "goldbreastplate", {"anvil"} });
            items::addRecipe({ {{"goldbar", 25}}, 1, "goldgreaves", {"anvil"} });
            items::addRecipe({ {{"goldbar", 20}}, 1, "goldhelmet", {"anvil"} });
            items::addRecipe({ {{"ironbar", 15}}, 1, "ironhelmet", {"anvil"} });
            items::addRecipe({ {{"ironbar", 25}}, 1, "ironbreastplate", {"anvil"} });
            items::addRecipe({ {{"ironbar", 20}}, 1, "irongreaves", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 10}}, 1, "moltenhelmet", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 20}}, 1, "moltenbreastplate", {"anvil"} });
            items::addRecipe({ {{"hellstonebar", 15}}, 1, "moltengreaves", {"anvil"} });
            items::addRecipe({ {{"junglespores", 15},{"vines", 3},{"stinger", 12}}, 1, "bladeofgrass", {"anvil"} });
            items::addRecipe({ {{"iceblade", 1},{"lightsbane", 1},{"bladeofgrass", 1},{"volcano", 1}}, 1, "terrablade", {"anvil"} });
            items::addRecipe({ {{"star", 5}}, 1, "manacrystal", {"none"} });
        }
    }

    void createProjectiles()
    {
        projectileBase base;
        projectileBase tmp;
        //arrow
        {
            base = tmp;
            base.dc.tex = "arrow";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1.8, 0.4 };
            base.pc.weight = 0.6;
            base.mc.damage = 4;
            base.mc.hitboxradius = 1;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.ac.ai = ai_ARROW;
            base.mc.destroydecor = true;
            base.ac.onupdate = mobFunctions::arrowUpdate;
            items::projectiles.insert(std::make_pair("arrow", base));
        }
        //bullet
        {
            base = tmp;
            base.dc.tex = "flyingbullet";
            base.dc.size = { 0.7,0.7 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.2,0.2 };
            base.pc.weight = 0.3;
            base.mc.damage = 3;
            base.mc.hitboxradius = 0.2;
            base.mc.hp = 1;
            base.mc.knockback = 0.2;
            base.mc.onDeath = mobFunctions::ondie_bullet;
            base.ac.ai = ai_ARROW;
            base.mc.destroydecor = true;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            items::projectiles.insert(std::make_pair("bullet", base));
        }
        //waterbolt
        {
            base = tmp;
            base.dc.tex = "empty";
            base.dc.size = { 0,0 };
            base.dc.hasmat = false;
            base.dc.hidden = true;
            base.pc.stoponcollision = false;
            base.pc.bounce = true;
            base.pc.bounciness = 1;
            base.pc.size = { 0.45,0.45 };
            base.pc.weight = 0;
            base.mc.damage = 40;
            base.mc.hp = 11230;
            base.mc.knockback = 0.2;
            base.mc.light = { 0.15 ,0.15, 0.6 };
            base.mc.hitboxradius = 0.2;
            base.mc.killin = 300;
            base.mc.onDeath = mobFunctions::ondie_waterbolt;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::waterboltUpdate;
            base.pec.tex = "waterbolt";
            base.pec.amount = 1;
            base.pec.rate = 1;
            base.pec.dir = { 0,0 };
            base.pec.ms = 0.07;
            base.pec.randomizems = 0.03;
            base.pec.randangle = 10;
            base.pec.randomizeamount = 1;
            base.pec.randomizescale = 0.1;
            base.pec.size = { 0.8,0.8 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 20;
            base.pec.randomizelifespan = 10;
            base.pec.rotation = 3;
            base.pec.randrotation = 1;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.pec.disappearAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("waterbolt", base));
        }
        //aqua scepter
        {
            base = tmp;
            base.dc.tex = "empty";
            base.dc.size = { 0,0 };
            base.dc.hasmat = false;
            base.dc.hidden = true;
            base.pc.stoponcollision = true;
            base.pc.size = { 0.1,0.1 };
            base.pc.weight = 0.7;
            base.pc.friction = false;
            base.mc.damage = 40;
            base.mc.hp = 11230;
            base.mc.knockback = 0.92;
            base.mc.light = { 0.5 ,0.56, 1.2 };
            base.mc.killin = 300;
            base.mc.hitboxradius = 0.2;
            base.ac.ai = ai_ARROW;
            base.mc.onDeath = mobFunctions::ondie_aquabolt;
            base.ac.onupdate = mobFunctions::aquaboltUpdate;
            base.pec.tex = "aquabolt";
            base.pec.amount = 2;
            base.pec.rate = 0.5;
            base.pec.weight = 0.2;
            base.pec.dir = { 0,0 };
            base.pec.ms = 0.25;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 5;
            base.pec.randomizeamount = 1;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.35,0.35 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 20;
            base.pec.randomizelifespan = 10;
            base.pec.rotation = 0;
            base.pec.randrotation = 4;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.pec.smallerAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("aquabolt", base));
        }
        //magic missile
        {
            base = tmp;
            base.dc.tex = "magicprojectile";
            base.dc.size = { 2.2,2.2 };
            base.dc.hasmat = false;
            base.dc.hidden = false;
            base.pc.stoponcollision = true;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0.2;
            base.pc.friction = false;
            base.mc.damage = 40;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.light = { 2.2 ,2.2, 3 };
            base.mc.hitboxradius = 0.5;
            base.mc.killin = -1;
            base.ac.ai = ai_ARROW;
            base.pec.tex = "particle:magicpart";
            base.pec.amount = 1;
            base.pec.rate = 0.7;
            base.pec.randomizerate = 0.2;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.15;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 360;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.55,0.55 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.pec.stopAsDie = true;
            base.mc.onDeath = mobFunctions::ondie_magicprojectile;
            base.ac.onupdate = mobFunctions::magicprojectileUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("magicprojectile", base));
        }
        //starfurystar
        {
            base = tmp;
            base.dc.tex = "starfurystar";
            base.dc.size = { 1.2,1.2 };
            base.dc.hasmat = false;
            base.dc.hidden = false;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0.2;
            base.pc.stoponcollision = false;
            base.pc.friction = false;
            base.mc.damage = 50;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.light = { 2 ,1, 1 };
            base.mc.hitboxradius = 1;
            base.mc.killin = -1;
            base.ac.ai = ai_ARROW;
            base.pec.tex = "particle:star1part";
            base.pec.amount = 1;
            base.pec.rate = 0.5;
            base.pec.randomizerate = 0.1;
            base.pec.weight = 0.1;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.25;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 40;
            base.pec.randomizescale = 0.5;
            base.pec.size = { 0.6,0.6};
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 70;
            base.pec.randomizelifespan = 60;
            base.pec.randrotation = 8;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.pec.stopAsDie = true;
            base.mc.onDeath = mobFunctions::ondie_starfurystar;
            base.ac.onupdate = mobFunctions::starfurystarUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("starfurystar", base));
        }
        //enchantedboomerang
        {
            base = tmp;
            base.dc.tex = "magicboomerang";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.mat = glm::mat4(1);
            base.dc.hidden = false;
            base.pc.size = { 1.3,1.3 };
            base.pc.weight = 0.2;
            base.pc.stoponcollision = false;
            base.pc.friction = false;
            base.mc.damage = 40;
            base.mc.hp = 121321;
            base.mc.knockback = 0.1;
            base.mc.light = { .5 ,.5, 3};
            base.mc.hitboxradius = 1.5;
            base.mc.killin = -1;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.actions = { {10, mobFunctions::boomerangChangeDir, 1} };
            base.ac.onupdate = mobFunctions::boomerangOnUpdate;
            base.pec.tex = "particle:star1part";
            base.pec.amount = 1;
            base.pec.rate = 1.1;
            base.pec.randomizerate = 0.05;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.15;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 360;
            base.pec.randomizescale = 0.5;
            base.pec.size = { 0.3,0.3 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 70;
            base.pec.randomizelifespan = 60;
            base.pec.randrotation = 8;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("enchantedboomerang", base));
        }
        //wooden boomerang
        {
            base = tmp;
            base.dc.tex = "woodenboomerang";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.mat = glm::mat4(1);
            base.dc.hidden = false;
            base.pc.size = { 1.3,1.3 };
            base.pc.weight = 0.2;
            base.pc.stoponcollision = false;
            base.pc.friction = false;
            base.mc.damage = 40;
            base.mc.hitboxradius = 1.1;
            base.mc.hp = 121321;
            base.mc.knockback = 0.1;
            base.mc.killin = -1;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.actions = { {6, mobFunctions::boomerangChangeDir, 1} };
            base.ac.onupdate = mobFunctions::boomerangOnUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("woodenboomerang", base));
        }
        //frozenboomerang
        {
            base = tmp;
            base.dc.tex = "frozenboomerang";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.mat = glm::mat4(1);
            base.dc.hidden = false;
            base.pc.size = { 1.3,1.3 };
            base.pc.weight = 0.2;
            base.pc.stoponcollision = false;
            base.pc.friction = false;
            base.mc.damage = 40;
            base.mc.hp = 121321;
            base.mc.knockback = 0.1;
            base.mc.light = { 1 ,1, 1 };
            base.mc.hitboxradius = 1.6;
            base.mc.killin = -1;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.actions = { {11, mobFunctions::boomerangChangeDir, 1} };
            base.ac.onupdate = mobFunctions::boomerangOnUpdate;
            base.pec.tex = "particle:cloudpart";
            base.pec.amount = 1;
            base.pec.rate = 0.9;
            base.pec.randomizerate = 0.05;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.1;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 360;
            base.pec.randomizescale = 0.5;
            base.pec.size = { 0.8,0.8 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 70;
            base.pec.randomizelifespan = 60;
            base.pec.randrotation = 8;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("frozenboomerang", base));
        }
        //lightdisc
        {
            base = tmp;
            base.dc.tex = "lightdisc";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.mat = glm::mat4(1);
            base.dc.hidden = false;
            base.pc.size = { 1.3,1.3 };
            base.pc.weight = 0;
            base.pc.stoponcollision = false;
            base.pc.bounce = true;
            base.pc.bounciness = 1;
            base.pc.friction = false;
            base.mc.hitboxradius = 1.6;
            base.mc.damage = 40;
            base.mc.hp = 121321;
            base.mc.knockback = 0.1;
            base.mc.light = { 1 ,1, .5 };
            base.mc.killin = -1;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.actions = { {16, mobFunctions::boomerangChangeDir, 1} };
            base.ac.onupdate = mobFunctions::lightdiscOnUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("lightdisc", base));
        }
        //icebladeprojectile
        {
            base = tmp;
            base.dc.tex = "empty";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = true;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 0.5;
            base.mc.damage = 40;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.killin = -1;
            base.ac.ai = ai_ARROW;
            base.pec.tex = "particle:icepart";
            base.pec.amount = 1;
            base.pec.rate = 0.7;
            base.pec.randomizerate = 0.2;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.15;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 15;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.65,0.65 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.05, 0.05, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.stopAsDie = true;
            base.mc.onDeath = mobFunctions::ondie_icebladeprojectile;
            base.ac.onupdate = mobFunctions::icebladeprojectileOnUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("icebladeprojectile", base));
        }
        //terrabladeswing
        {
            base = tmp;
            base.dc.tex = "terrabladeprojectile1";
            base.dc.size = { 16, 16 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = true;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 16;
            base.mc.damage = 85;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.killin = 18;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::terraBladedamage));
            base.ac.ai = ai_ARROW;
            base.pec.tex = "particle:terrapart";
            base.pec.amount = 1;
            base.pec.rate = 4;
            base.pec.randomizerate = 0.2;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.01;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 360;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.65,0.65 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.05, 0.05, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.radius = 6;
            base.pec.randradius = 2;
            base.pec.stopAsDie = true;
            base.ac.onupdate = mobFunctions::terrabladeswingOnUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("terrabladeswing", base));
        }
        //terrabladeprojectile
        {
            base = tmp;
            base.dc.tex = "terrabladeprojectile1";
            base.dc.size = { 0, 0 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 12;
            base.mc.damage = 85;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.killin = 100;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::terraBladedamage));
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::terrabladeprojectileOnUpdate;
            base.pec.tex = "particle:terrapart";
            base.pec.amount = 1;
            base.pec.rate = 4;
            base.pec.randomizerate = 0.2;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.04;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 20;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.65,0.65 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.05, 0.2, 0.05);
            base.pec.smallerAsDie = true;
            base.pec.radius = 6;
            base.pec.randradius = 2;
            base.pec.stopAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("terrabladeprojectile", base));
        }
        //spacegun
        {
            base = tmp;
            base.dc.tex = "spacegunprojectile";
            base.dc.size = { 2.5,1.5 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.1,0.1 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 0.1;
            base.mc.light = glm::vec3(0.3, 1.4, 0.3);
            base.mc.damage = 40;
            base.mc.hp = 2;
            base.mc.knockback = 0.1;
            base.mc.killin = 100;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("spacegunprojectile", base));
        }
        //enchantedsword
        {
            base = tmp;
            base.dc.tex = "enchantedswordprojectile";
            base.dc.size = { 2.2, 2.2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.1,0.1 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 2;
            base.mc.light = glm::vec3(0.6, 0.6, 1.3);
            base.mc.damage = 40;
            base.mc.hp = 1;
            base.mc.knockback = 0.1;
            base.mc.killin = 100;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::enchantedswordUpdate;
            base.pec.tex = "particle:star1part";
            base.pec.amount = 1;
            base.pec.rate = 0.2;
            base.pec.randomizerate = 0.1;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.04;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 20;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.75,0.75 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.05, 0.05, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.stopAsDie = true;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("enchantedswordprojectile", base));
        }
        //snowball
        {
            base = tmp;
            base.dc.tex = "snowball";
            base.dc.size = { 0.7,0.7 };
            base.dc.hasmat = false;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.2,0.2 };
            base.pc.weight = 0.3;
            base.mc.damage = 3;
            base.mc.hitboxradius = 0.2;
            base.mc.hp = 1;
            base.mc.knockback = 0.2;
            base.mc.onDeath = mobFunctions::ondie_snowball;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::aquaboltUpdate;
            base.pec.tex = "particle:cloudpart";
            base.pec.amount = 1;
            base.pec.rate = 2;
            base.pec.randomizerate = 1;
            base.pec.weight = 0.1;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.03;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 180;
            base.pec.randomizescale = 0.2;
            base.pec.size = { .55,.55 };;
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 50;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.05, 0.05, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.stopAsDie = true;
            base.pec.speedmultiplier = 1;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("snowball", base));
        }
        //vilethorn
        {
            base = tmp;
            base.dc.tex = "vilethornbody";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.dc.opacity = 0;
            base.pc.size = { 0.2,0.2 };
            base.pc.isstatic = true;
            base.pc.dontcollide = true;
            base.pc.stoponcollision = false;
            base.mc.damage = 10;
            base.mc.hitboxradius = 1;
            base.mc.hp = 1;
            base.mc.knockback = 0.2;
            base.mc.killin = 40;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::vilethornUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("vilethorn", base));
        }
        //bee
        {
            base = tmp;
            base.dc.tex = "bee";
            base.dc.size = { 0.8,0.8 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.size = { 0.2,0.2 };
            base.pc.stoponcollision = false;
            base.pc.bounce = true;
            base.pc.bounciness = 1;
            base.pc.weight = 0;
            base.mc.damage = 7;
            base.mc.onDeath = mobFunctions::ondie_bee;
            base.mc.hitboxradius = 0.2;
            base.mc.hp = 2;
            base.mc.knockback = 0.2;
            base.mc.killin = 300;
            base.ac.stats["target"].intVal = -1;
            base.ac.stats["counter"].intVal = 0;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::beeUpdate;
            items::projectiles.insert(std::make_pair("bee", base));
        }
        //mini retina laser
        {
            base = tmp;
            base.dc.tex = "flyingbullet";
            base.dc.size = { 1.5,1.5 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 0.1,0.1 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 0.1;
            base.mc.light = glm::vec3(1.3, 0.4, 0.4);
            base.mc.damage = 40;
            base.mc.hp = 2;
            base.mc.knockback = 0.1;
            base.mc.killin = 100;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::damageBoth1));
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("miniretinalaser", base));
        }
        //imp
        {
            base = tmp;
            base.dc.tex = "empty";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = true;
            base.pc.size = { 0.3,0.3 };
            base.pc.weight = 0;
            base.pc.friction = false;
            base.mc.hitboxradius = 0.5;
            base.mc.damage = 17;
            base.mc.hp = 123;
            base.mc.knockback = 0.1;
            base.mc.killin = -1;
            base.ac.ai = ai_ARROW;
            base.pec.tex = "particle:torchpart";
            base.pec.amount = 1;
            base.pec.rate = 0.7;
            base.pec.randomizerate = 0.2;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.15;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 15;
            base.pec.randomizescale = 0.2;
            base.pec.size = { 0.65,0.65 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 30;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.radius = 0.3;
            base.pec.light = glm::vec3(0.05, 0.05, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.stopAsDie = true;
            base.mc.onDeath = mobFunctions::ondie_impprojectile;
            base.ac.onupdate = mobFunctions::icebladeprojectileOnUpdate;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::volcanoDamage));
            base.mc.destroydecor = true;
            items::projectiles.insert(std::make_pair("impfireball", base));
        }
        //spikediceslime
        {
            base = tmp;
            base.dc.tex = "spikediceslimeprojectile";
            base.dc.size = { 1.2,1.2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1, 0.4 };
            base.pc.weight = 0.6;
            base.mc.damage = 4;
            base.mc.hitboxradius = 1;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            base.mc.onPlayerCollision = collisionFs::damagePlayerIce;
            base.pec.tex = "particle:cloudpart";
            base.pec.amount = 1;
            base.pec.rate = 2.5;
            base.pec.randomizerate = 0.05;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.1;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 360;
            base.pec.randomizescale = 0.5;
            base.pec.size = { 0.6,0.6 };
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 50;
            base.pec.randomizelifespan = 30;
            base.pec.randrotation = 8;
            base.pec.stoponcollision = false;
            base.pec.smallerAsDie = true;
            items::projectiles.insert(std::make_pair("spikediceslimeprojectile", base));
        }
        //spikedjungleslime
        {
            base = tmp;
            base.dc.tex = "spikedjungleslimeprojectile";
            base.dc.size = { 1.2,1.2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1, 0.4 };
            base.pc.weight = 0.6;
            base.mc.damage = 4;
            base.mc.hitboxradius = 1;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            base.mc.onPlayerCollision = collisionFs::damagePlayerPoison;
            items::projectiles.insert(std::make_pair("spikedjungleslimeprojectile", base));
        }
        //demonscythe
        {
            base = tmp;
            base.dc.tex = "demonscythe";
            base.dc.size = { 2,2 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1.5, 1.5 };
            base.pc.weight = 0;
            base.mc.damage = 42;
            base.mc.hitboxradius = 2;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.ac.ai = ai_ARROW;
            base.ac.onupdate = mobFunctions::demonscytheOnUpdate;
            base.mc.onPlayerCollision = collisionFs::damagePlayer;
            base.pec.tex = "particle:purplepart";
            base.pec.amount = 1;
            base.pec.rate = 1.2;
            base.pec.randomizerate = 1;
            base.pec.weight = 0;
            base.pec.dir = { -1,0 };
            base.pec.ms = 0.03;
            base.pec.randomizems = 0.05;
            base.pec.randangle = 20;
            base.pec.randomizescale = 0.2;
            base.pec.size = { .35,.35 };;
            base.pec.lifespan = -1;
            base.pec.particleLifespan = 40;
            base.pec.randomizelifespan = 10;
            base.pec.randrotation = 5;
            base.pec.stoponcollision = false;
            base.pec.light = glm::vec3(0.1, 0.0, 0.1);
            base.pec.smallerAsDie = true;
            base.pec.disappearAsDie = true;
            base.pec.radius = 2;
            items::projectiles.insert(std::make_pair("demonscythe", base));
        }
        //hornetsting
        {
            base = tmp;
            base.dc.tex = "hornetsting";
            base.dc.size = { 1,1 };
            base.dc.hasmat = true;
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1, 1 };
            base.pc.weight = 0;
            base.mc.damage = 26;
            base.mc.hitboxradius = 2;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.mc.light = glm::vec3(0,0,0.7);
            base.ac.ai = ai_ARROW;
            base.mc.onPlayerCollision = collisionFs::damagePlayerPoison;
            base.ac.onupdate = mobFunctions::bulletUpdate;
            items::projectiles.insert(std::make_pair("hornetsting", base));
        }
        //dark caster projectile
        {
            base = tmp;
            base.dc.tex = "casterbolt";
            base.dc.size = { 0.9,0.9};
            base.dc.hidden = false;
            base.pc.stoponcollision = false;
            base.pc.size = { 1, 1 };
            base.pc.weight = 0;
            base.mc.damage = 26;
            base.mc.hitboxradius = 2;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.ac.ai = ai_ARROW;
            base.mc.onPlayerCollision = collisionFs::damagePlayer;
            items::projectiles.insert(std::make_pair("darkcasterprojectile", base));
        }
        //blade of grass
        {
            base = tmp;
            base.dc.tex = "bladeofgrassprojectile";
            base.dc.size = { 2,2 };
            base.dc.hidden = false;
            base.dc.hasmat = true;
            base.pc.stoponcollision = false;
            base.pc.size = { 2, 2 };
            base.pc.weight = 0;
            base.mc.damage = 26;
            base.mc.hitboxradius = 2;
            base.mc.hp = 1;
            base.mc.knockback = 0;
            base.mc.killin = 180;
            base.ac.ai = ai_ARROW;
            base.mc.onDeath = mobFunctions::ondie_bladeofgrassprojectile;
            base.ac.onupdate = mobFunctions::bladeOfGrassOnUpdate;
            base.mc.onCollision.insert(std::make_pair(mf_ENEMY, collisionFs::bladeofgrassDamage));
            items::projectiles.insert(std::make_pair("bladeofgrassprojectile", base));
        }
    }

    void createEnemies()
    {
        enemyBase base;
        base.dC.mat = glm::mat4(1.0f);
        base.dC.autolight = true;
        base.dC.tex = "empty";
        base.dC.hidden = false;
        base.dC.size = glm::vec2(1.0f);
        base.dC.parent = globals::mobLayer;
        base.dC.anim = -1;
        base.pC.vel = { 0,0 };
        base.pC.size = glm::vec2(1.0f);
        base.aC.onupdate = mobFunctions::nothing;
        base.mC.hp = 0;
        base.mC.damage = 0;
        base.mC.iframes = 0;
        base.mC.kbResist = 1;
        base.hpbar = false;
        base.spawnFunc = enemies::defaultSpawnFunc;
        base.pec.lifespan = 0;
        base.dC.spriteCoords = glm::vec4(-1);
        base.mC.candespawn = true;

        //green slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 2, 1.4 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 2,2 };
            slime.dC.tex = "greenslime";
            slime.mC.displayName = "Green Slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions.push_back({ 180, mobFunctions::slimeWindup, -1 });
            slime.aC.stats["jumpanim"].charp = "slimejump";
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayer;
            slime.mC.damage = 6;
            slime.mC.kbResist = -0.2;
            slime.mC.hp = 14;
            slime.mC.maxhp = 14;
            slime.drops = { {"gel", {2, 1, 1}}, {"coppercoin", {3, 1, 1}} };
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("greenslime", slime));
        }
        //ice slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 2, 1.4 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 2,2 };
            slime.dC.tex = "iceslime";
            slime.mC.displayName = "ice Slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions.push_back({ 180, mobFunctions::slimeWindup, -1 });
            slime.aC.stats["jumpanim"].charp = "iceslimejump";
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayerIce;
            slime.mC.kbResist = 0;
            slime.mC.damage = 12;
            slime.mC.defense = 2;
            slime.mC.hp = slime.mC.maxhp = 35;
            slime.drops = { {"gel", {2, 1, 1}}, {"coppercoin", {3, 1, 1}} };
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("iceslime", slime));
        }
        //lava slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 2, 1.4 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 2,2 };
            slime.dC.tex = "lavaslime";
            slime.mC.displayName = "lava Slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions.push_back({ 180, mobFunctions::slimeWindup, -1 });
            slime.aC.stats["jumpanim"].charp = "lavaslimejump";
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayerBurn;
            slime.mC.kbResist = 0;
            slime.mC.damage = 12;
            slime.mC.defense = 2;
            slime.mC.hp = slime.mC.maxhp = 35;
            slime.drops = { {"gel", {2, 1, 1}}, {"coppercoin", {3, 1, 1}} };
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("lavaslime", slime));
        }
        //random slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 4, 2.8 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 4, 4 };
            slime.dC.tex = "colorableslime";
            slime.mC.displayName = "Slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions.push_back({ 140, mobFunctions::slimeWindup, -1 });
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.aC.stats["jumpanim"].charp = "colorableslimejump";
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayer;
            slime.mC.damage = 12;
            slime.drops = { {"gel", {3, 1, 0.8}}, {"coppercoin", {22, 5, 1}} };
            slime.mC.hp = 35;
            slime.mC.kbResist = 0;
            slime.mC.maxhp = 35;
            slime.mC.defense = 2;
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.randomsize = 0.6;
            slime.randomcolor = 0.6;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("colorableslime", slime));
        }
        //spiked ice slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 3, 2.1 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 3, 3 };
            slime.dC.tex = "spikediceslime";
            slime.mC.displayName = "Spiked ice slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions = { { 140, mobFunctions::slimeWindup, -1 }, { 90, mobFunctions::slimeFireProjectile, -1 } };
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.aC.stats["jumpanim"].charp = "spikediceslime";
            slime.aC.stats["projectile"].charp = "spikediceslimeprojectile";
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayer;
            slime.mC.damage = 12;
            slime.drops = { {"gel", {3, 1, 0.8}}, {"coppercoin", {22, 5, 1}} };
            slime.mC.kbResist = 0;
            slime.mC.hp = 35;
            slime.mC.maxhp = 35;
            slime.mC.defense = 2;
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("spikediceslime", slime));
        }
        //spiked jungle slime
        {
            enemyBase slime = base;
            slime.aC.ai = ai_GREENSLIME;
            slime.pC.size = { 3, 2.1 };
            slime.pC.weight = 1;
            slime.pC.friction = false;
            slime.dC.size = { 3, 3 };
            slime.dC.tex = "spikedjungleslime";
            slime.mC.displayName = "Spiked jungle slime";
            slime.mC.hitboxradius = 1.2;
            slime.dC.mat = glm::mat4(1.0f);
            slime.dC.mat = glm::mat4(1.0f);
            slime.aC.actions = { { 140, mobFunctions::slimeWindup, -1 }, { 90, mobFunctions::slimeFireProjectile, -1 } };
            slime.aC.onupdate = mobFunctions::slimeOnUpdate;
            slime.aC.stats["jumpanim"].charp = "spikedjungleslime";
            slime.aC.stats["projectile"].charp = "spikedjungleslimeprojectile";
            slime.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            slime.mC.onPlayerCollision = collisionFs::damagePlayer;
            slime.mC.damage = 12;
            slime.drops = { {"gel", {3, 1, 0.8}}, {"coppercoin", {22, 5, 1}} };
            slime.mC.kbResist = 0;
            slime.mC.hp = 35;
            slime.mC.maxhp = 35;
            slime.mC.defense = 2;
            slime.mC.families = { mf_ENEMY };
            slime.hpbar = true;
            slime.mC.damageSound = &sounds::slimehit;
            enemies::enemies.insert(std::make_pair("spikedjungleslime", slime));
        }
        //sand
        {
            enemyBase sand = base;
            sand.aC.ai = ai_SAND;
            sand.pC.size = { 0.3,1.05 };
            sand.dC.tex = "sand";
            sand.aC.onupdate = mobFunctions::sandOnUpdate;
            sand.mC.hp = sand.mC.maxhp = 1931;
            sand.slots = 0;
            enemies::enemies.insert(std::make_pair("sand", sand));
        }
        //demoneye
        {
            enemyBase demoneye = base;
            demoneye.aC.ai = ai_DEMONEYE;
            demoneye.dC.size = glm::vec2(1.7);
            demoneye.pC.size = glm::vec2(1.4);
            demoneye.pC.friction = false;
            demoneye.dC.tex = "demoneye";
            demoneye.aC.onupdate = mobFunctions::demoneyeOnUpdate;
            demoneye.pC.bounce = true;
            demoneye.pC.bounciness = .9;
            demoneye.pC.stoponcollision = false;
            demoneye.pC.weight = 0;
            demoneye.mC.hitboxradius = 1.2;
            demoneye.mC.hp = demoneye.mC.maxhp = 60;
            demoneye.mC.families = { mf_ENEMY };
            demoneye.mC.kbResist = 0.2;
            demoneye.mC.knockback = 0.5;
            demoneye.mC.damage = 18;
            demoneye.mC.displayName = "Demon eye";
            demoneye.hpbar = true;
            demoneye.spawnFunc = enemies::flyingSpawnFunc;
            demoneye.drops = { {"coppercoin", {60, 10, 1}} };
            demoneye.mC.gore = "demoneye";
            enemies::enemies.insert(std::make_pair("demoneye", demoneye));
        }
        //servantofcthulu
        {
            enemyBase demoneye = base;
            demoneye.aC.ai = ai_DEMONEYE;
            demoneye.dC.size = glm::vec2(1.5);
            demoneye.pC.size = glm::vec2(1);
            demoneye.pC.friction = false;
            demoneye.dC.tex = "servantofcthulu";
            demoneye.aC.onupdate = mobFunctions::demoneyeOnUpdate;
            demoneye.pC.stoponcollision = false;
            demoneye.pC.weight = 0;
            demoneye.mC.hitboxradius = 0.9;
            demoneye.mC.hp = demoneye.mC.maxhp = 8;
            demoneye.mC.families = { mf_ENEMY };
            demoneye.mC.kbResist = 0;
            demoneye.mC.knockback = 0.5;
            demoneye.mC.damage = 12;
            demoneye.mC.displayName = "Servant of Cthulu";
            demoneye.hpbar = true;
            demoneye.mC.gore = "servantofcthulu";
            demoneye.slots = 0;
            enemies::enemies.insert(std::make_pair("demoneye", demoneye));
        }
        //zombie
        {
            enemyBase zombie = base;
            zombie.aC.ai = ai_ZOMBIE;
            zombie.pC.size = glm::vec2(2.125, 3);
            zombie.pC.friction = false;
            zombie.pC.weight = 1.1;
            zombie.dC.size = glm::vec2(3);
            zombie.dC.tex = "zombie";
            zombie.aC.onupdate = mobFunctions::zombieOnUpdate;
            zombie.aC.stats["walkanim"].charp = "zombiewalk";
            zombie.mC.hitboxradius = 1.3;
            zombie.mC.hp = zombie.mC.maxhp = 150;
            zombie.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            zombie.mC.families = { mf_ENEMY };
            zombie.mC.damage = 10;
            zombie.mC.knockback = .7;
            zombie.mC.kbResist= .2;
            zombie.mC.displayName = "Zombie";
            zombie.hpbar = true;
            zombie.mC.onPlayerCollision = collisionFs::damagePlayer;
            zombie.drops = { {"shackle", {1, 0, 0.02}}, {"coppercoin", {60, 10, 1}} };
            zombie.mC.deathSound = &sounds::zombiedie;
            zombie.mC.gore = "zombie";
            enemies::enemies.insert(std::make_pair("zombie", zombie));
        }
        //bunny
        {
            enemyBase bunny = base;
            bunny.aC.ai = ai_BUNNY;
            bunny.pC.size = glm::vec2(2);
            bunny.dC.size = glm::vec2(2);
            bunny.dC.tex = "bunny";
            bunny.aC.actions.push_back({ 1, mobFunctions::bunnyUpdate, -1 });
            bunny.aC.stats["standinganim"].charp = "bunny";
            bunny.aC.stats["walkinganim"].charp = "bunnyhop";
            bunny.aC.stats["stridetime"].floatVal = 42;
            bunny.pC.weight = 0.8;
            bunny.pC.friction = false;
            bunny.mC.hitboxradius = 0.9;
            bunny.mC.hp = bunny.mC.maxhp = 3;
            bunny.mC.families = { mf_CRITTER };
            bunny.mC.displayName = "Bunyy omg uwuwuwu";
            bunny.hpbar = true;
            bunny.mC.gore = "bunny";
            bunny.slots = 0;
            enemies::enemies.insert(std::make_pair("bunny", bunny));
        }
        //star
        {
            enemyBase star = base;
            star.aC.ai = ai_FALLENSTAR;
            star.pC.vel = { 0.5, 0.1 };
            star.pC.friction = false;
            star.pC.size = glm::vec2(2);
            star.dC.size = glm::vec2(2);
            star.dC.tex = "fallenstar";
            star.aC.onupdate = mobFunctions::starOnUpdate;
            star.pC.weight = 0.8;
            star.dC.hasmat = true;
            star.mC.hitboxradius = 0.9;
            star.mC.hp = star.mC.maxhp = 1231;
            star.mC.light = glm::vec3(0.60, 0.60, 0);
            star.pec.amount = 2;
            star.pec.countdown = 0;
            star.pec.dir = glm::vec2(-1, 0);
            star.pec.randangle = 360;
            star.pec.lifespan = -1;
            star.pec.particleLifespan = 40;
            star.pec.size = glm::vec2(1, 1);
            star.pec.tex = "particle:star2part";
            star.pec.smallerAsDie = true;
            star.pec.randrotation = 15;
            star.pec.ms = 0.04;
            star.pec.rate = 6;
            star.spawnFunc = enemies::fallenStarSpawnFunc;
            star.slots = 0;
            enemies::enemies.insert(std::make_pair("star", star));
        }
        //bomb
        {
            enemyBase bomb = base;
            bomb.aC.ai = ai_BOMB;
            bomb.pC.size = glm::vec2(1.3);
            bomb.pC.friction = false;
            bomb.dC.size = glm::vec2(2);
            bomb.dC.tex = "bomb";
            bomb.aC.onupdate = mobFunctions::bombOnUpdate;
            bomb.aC.actions.push_back({ 60, mobFunctions::bombExplode, 1 });
            bomb.pC.weight = 1.2;
            bomb.dC.hasmat = true;
            bomb.mC.hp = bomb.mC.maxhp = 1231;
            bomb.mC.onCollision = { {mf_ENEMY, collisionFs::damage}, {mf_CRITTER, collisionFs::damage} };
            bomb.mC.hitboxradius = 0;
            bomb.mC.damage = 100;
            bomb.childrenBases.push_back("bombparticle");
            enemies::enemies.insert(std::make_pair("bomb", bomb));
        }
        //bombparticle
        {
            enemyBase bombpart = base;
            bombpart.aC.ai = ai_BOMB;
            bombpart.pC.size = glm::vec2(0);
            bombpart.dC.size = glm::vec2(0);
            bombpart.mC.hp = bombpart.mC.maxhp = 1231;
            bombpart.pec.amount = 5;
            bombpart.pec.dir = glm::vec2(-1, 0);
            bombpart.pec.randangle = 360;
            bombpart.pec.disappearAsDie = true;
            bombpart.pec.lifespan = -1;
            bombpart.pec.ms = 0.08;
            bombpart.pec.particleLifespan = 20;
            bombpart.pec.size = glm::vec2(0.3);
            bombpart.pec.tex = "particle:torchpart";
            bombpart.pec.weight = 0;
            bombpart.pec.stoponcollision = false;
            bombpart.pec.rate = 1;
            enemies::enemies.insert(std::make_pair("bombparticle", bombpart));
        }
        //guide
        {
            enemyBase guide = base;
            guide.aC.ai = ai_GUIDE;
            guide.pC.size = glm::vec2(2,3);
            guide.dC.size = glm::vec2(3);
            guide.dC.tex = "guide";
            guide.aC.stats["walkanim"].charp = "guidewalk";
            guide.aC.stats["npcmenu"].charp = "guide";
            guide.aC.stats["npcname"].charp = "guide";
            guide.mC.hp = guide.mC.maxhp = 250;
            guide.hpbar = true;
            guide.aC.onupdate = mobFunctions::guideOnUpdate;
            guide.aC.onclick = mobFunctions::guideOnClick;
            guide.mC.onDeath = mobFunctions::ondie_npc;
            guide.pC.weight = 1;
            guide.pC.friction = false;
            guide.mC.hitboxradius = 1.3;
            guide.mC.displayName = "Jackson";
            guide.mC.despawntimer = -1;
            guide.mC.candespawn = false;
            guide.drops = { {"arrow",{10, 10, 1}} };
            enemies::enemies.insert(std::make_pair("guide", guide));
            enemies::addNPCDialogue("guide", "Hello $NAME$");
            enemies::addNPCDialogue("guide", "Hi $NAME$");
            enemies::addNPCDialogue("guide", "Its quite nice here");
            enemies::addNPCButton("guide", "Help", UI::uiCfunc_guideHelp);
            enemies::addNPCButton("guide", "Crafting", UI::uiCfunc_openGuideslot);
        }
        //merchant
        {
            enemyBase merchant = base;
            merchant.aC.ai = ai_GUIDE;
            merchant.pC.size = glm::vec2(1.5, 3);
            merchant.dC.size = glm::vec2(2.8);
            merchant.dC.tex = "merchant";
            merchant.mC.hp = merchant.mC.maxhp = 250;
            merchant.hpbar = true;
            merchant.aC.stats["walkanim"].charp = "merchantwalk";
            merchant.aC.stats["npcmenu"].charp = "merchant";
            merchant.aC.stats["npcname"].charp = "merchant";
            merchant.aC.onupdate = mobFunctions::guideOnUpdate;
            merchant.aC.onclick = mobFunctions::guideOnClick;
            merchant.mC.onDeath = mobFunctions::ondie_npc;
            merchant.pC.weight = 1;
            merchant.pC.friction = false;
            merchant.mC.hitboxradius = 1.3;
            merchant.mC.displayName = "Jackson";
            merchant.mC.despawntimer = -1;
            merchant.mC.candespawn = false;
            merchant.drops = { {"arrow",{10, 10, 1}} };
            enemies::enemies.insert(std::make_pair("merchant", merchant));
            enemies::addNPCDialogue("merchant", "i am merchant $NAME$");
            enemies::addNPCButton("merchant", "Shop", UI::uiCfunc_openMerchantShop);
        }
        //twinssummon1
        {
            enemyBase twinssummon1 = base;
            twinssummon1.aC.ai = ai_TWINSSUMMON;
            twinssummon1.dC.size = glm::vec2(2.5);
            twinssummon1.pC.size = glm::vec2(1.4);
            twinssummon1.pC.friction = false;
            twinssummon1.dC.tex = "twinssummon1";
            twinssummon1.dC.hasmat = true;
            twinssummon1.aC.onupdate = mobFunctions::twinssummon1OnUpdate;
            twinssummon1.pC.bounce = true;
            twinssummon1.pC.bounciness = .9;
            twinssummon1.pC.stoponcollision = false;
            twinssummon1.pC.weight = 0;
            twinssummon1.mC.hitboxradius = 1.2;
            twinssummon1.mC.damage = 23;
            twinssummon1.mC.hp = twinssummon1.mC.maxhp = 1;
            twinssummon1.mC.onCollision = { {mf_ENEMY, collisionFs::damage} };
            twinssummon1.mC.families = { mf_SUMMON };
            enemies::enemies.insert(std::make_pair("twinssummon1", twinssummon1));
        }
        //twinssummon2
        {
            enemyBase twinssummon2 = base;
            twinssummon2.aC.ai = ai_TWINSSUMMON;
            twinssummon2.dC.size = glm::vec2(2.5);
            twinssummon2.pC.size = glm::vec2(1.4);
            twinssummon2.pC.friction = false;
            twinssummon2.dC.tex = "twinssummon2";
            twinssummon2.dC.hasmat = true;
            twinssummon2.aC.onupdate = mobFunctions::twinssummon2OnUpdate;
            twinssummon2.pC.bounce = true;
            twinssummon2.pC.bounciness = .9;
            twinssummon2.pC.stoponcollision = false;
            twinssummon2.pC.weight = 0;
            twinssummon2.mC.hitboxradius = 1.2;
            twinssummon2.mC.damage = 20;
            twinssummon2.mC.hp = twinssummon2.mC.maxhp = 1;
            twinssummon2.mC.onCollision = { {mf_ENEMY, collisionFs::damage} };
            twinssummon2.mC.families = { mf_SUMMON };
            enemies::enemies.insert(std::make_pair("twinssummon2", twinssummon2));
        }
        //imp
        {
            enemyBase imp = base;
            imp.aC.ai = ai_IMP;
            imp.dC.size = glm::vec2(3.5);
            imp.pC.size = glm::vec2(1.4);
            imp.pC.friction = false;
            imp.dC.tex = "imp";
            imp.dC.hasmat = true;
            imp.aC.onupdate = mobFunctions::impOnUpdate;
            imp.pC.stoponcollision = false;
            imp.pC.weight = 0;
            imp.mC.hp = imp.mC.maxhp = 1;
            imp.mC.families = { mf_SUMMON };
            imp.pec.tex = "particle:torchpart";
            imp.pec.amount = 1;
            imp.pec.rate = 1.7;
            imp.pec.randomizerate = 0.2;
            imp.pec.weight = 0;
            imp.pec.dir = { -1,0 };
            imp.pec.ms = 0.15;
            imp.pec.randomizems = 0.05;
            imp.pec.randangle = 15;
            imp.pec.randomizescale = 0.2;
            imp.pec.size = { 0.65,0.65 };
            imp.pec.lifespan = -1;
            imp.pec.particleLifespan = 30;
            imp.pec.randomizelifespan = 10;
            imp.pec.randrotation = 5;
            imp.pec.radius = 0.5;
            imp.pec.stoponcollision = false;
            imp.pec.light = glm::vec3(0.05, 0.05, 0.1);
            imp.pec.smallerAsDie = true;
            imp.pec.stopAsDie = true;
            enemies::enemies.insert(std::make_pair("imp", imp));
        }
        //damage text
        {
            enemyBase damagetext = base;
            damagetext.aC.ai = ai_IMP;
            damagetext.dC.textScale = globals::fontSize * 1.2;
            damagetext.pC.friction = false;
            damagetext.dC.tex = "empty";
            damagetext.dC.color = glm::vec3(1, 0.5, 0);
            damagetext.dC.hasmat = true;
            damagetext.aC.onupdate = mobFunctions::damageTextOnUpdate;
            damagetext.pC.stoponcollision = false;
            damagetext.pC.weight = 0;
            damagetext.mC.hp = damagetext.mC.maxhp = 1;
            damagetext.slots = 0;
            enemies::enemies.insert(std::make_pair("damagetext", damagetext));
        }
        //mouse
        {
            enemyBase mouse = base;
            mouse.aC.ai = ai_BUNNY;
            mouse.pC.size = glm::vec2(0.9);
            mouse.dC.size = glm::vec2(1);
            mouse.dC.tex = "mouse";
            mouse.aC.actions.push_back({ 1, mobFunctions::bunnyUpdate, -1 });
            mouse.aC.stats["ms"].floatVal = 2.2;
            mouse.aC.stats["standinganim"].charp = "mouse";
            mouse.aC.stats["walkinganim"].charp = "mousewalk";
            mouse.aC.stats["stridetime"].floatVal = 8;
            mouse.pC.weight = 0.8;
            mouse.pC.friction = false;
            mouse.mC.hitboxradius = 1.2;
            mouse.mC.hp = mouse.mC.maxhp = 5;
            mouse.mC.families = { mf_CRITTER };
            mouse.mC.displayName = "mouserrrrrrrr";
            mouse.hpbar = true;
            mouse.slots = 0;
            enemies::enemies.insert(std::make_pair("mouse", mouse));
        }
        //eaterofsouls
        {
            enemyBase eaterofsouls = base;
            eaterofsouls.aC.ai = ai_DEMONEYE;
            eaterofsouls.dC.size = glm::vec2(5);
            eaterofsouls.pC.size = glm::vec2(4);
            eaterofsouls.pC.friction = false;
            eaterofsouls.dC.tex = "eaterofsouls";
            eaterofsouls.aC.onupdate = mobFunctions::eaterOfSoulsOnUpdate;
            eaterofsouls.pC.bounce = true;
            eaterofsouls.pC.bounciness = .9;
            eaterofsouls.pC.stoponcollision = false;
            eaterofsouls.pC.weight = 0;
            eaterofsouls.mC.hitboxradius = 1.2;
            eaterofsouls.mC.knockback = 0.5;
            eaterofsouls.mC.kbResist = 0.5;
            eaterofsouls.mC.hp = eaterofsouls.mC.maxhp = 100;
            eaterofsouls.mC.families = { mf_ENEMY };
            eaterofsouls.mC.displayName = "Eater of Souls";
            eaterofsouls.mC.onPlayerCollision = collisionFs::damagePlayer;
            eaterofsouls.hpbar = true;
            eaterofsouls.randomsize = 0.8;
            eaterofsouls.spawnFunc = enemies::flyingSpawnFunc;
            eaterofsouls.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            eaterofsouls.mC.gore = "eaterofsouls";
            enemies::enemies.insert(std::make_pair("eaterofsouls", eaterofsouls));
        }
        //cave bat
        {
            enemyBase cavebat = base;
            cavebat.aC.ai = ai_DEMONEYE;
            cavebat.dC.size = glm::vec2(1.5);
            cavebat.pC.size = glm::vec2(1);
            cavebat.pC.friction = false;
            cavebat.dC.tex = "cavebat";
            cavebat.dC.hasmat = true;
            cavebat.aC.onupdate = mobFunctions::batOnUpdate;
            cavebat.pC.bounce = true;
            cavebat.pC.bounciness = .9;
            cavebat.pC.stoponcollision = false;
            cavebat.pC.weight = 0;
            cavebat.mC.hitboxradius = 1;
            cavebat.mC.hp = cavebat.mC.maxhp = 16;
            cavebat.mC.damage = 13;
            cavebat.mC.knockback = 0.2;
            cavebat.mC.kbResist = 0.2;
            cavebat.mC.defense = 2;
            cavebat.mC.families = { mf_ENEMY };
            cavebat.mC.displayName = "Cave Bat";
            cavebat.aC.stats["flyingtex"].charp = "cavebatfly";
            cavebat.aC.stats["hangingtex"].charp = "cavebathang";
            cavebat.drops = { {"coppercoin", {85, 10, 1}} };
            cavebat.mC.onPlayerCollision = collisionFs::damagePlayer;
            cavebat.hpbar = true;
            cavebat.spawnFunc = enemies::flyingSpawnFunc;
            cavebat.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            cavebat.mC.deathSound = &sounds::batdie;
            enemies::enemies.insert(std::make_pair("cavebat", cavebat));
        }
        //jungle bat
        {
            enemyBase cavebat = base;
            cavebat.aC.ai = ai_DEMONEYE;
            cavebat.dC.size = glm::vec2(1.5);
            cavebat.pC.size = glm::vec2(1);
            cavebat.pC.friction = false;
            cavebat.dC.tex = "junglebat";
            cavebat.dC.hasmat = true;
            cavebat.aC.onupdate = mobFunctions::batOnUpdate;
            cavebat.pC.bounce = true;
            cavebat.pC.bounciness = .9;
            cavebat.pC.stoponcollision = false;
            cavebat.pC.weight = 0;
            cavebat.mC.hitboxradius = 1;
            cavebat.mC.hp = cavebat.mC.maxhp = 36;
            cavebat.mC.damage = 23;
            cavebat.mC.knockback = 0.2;
            cavebat.mC.kbResist = 0.2;
            cavebat.mC.defense = 5;
            cavebat.mC.families = { mf_ENEMY };
            cavebat.mC.displayName = "jungle Bat";
            cavebat.aC.stats["flyingtex"].charp = "junglebatfly";
            cavebat.aC.stats["hangingtex"].charp = "junglebathang";
            cavebat.drops = { {"coppercoin", {85, 10, 1}} };
            cavebat.mC.onPlayerCollision = collisionFs::damagePlayer;
            cavebat.hpbar = true;
            cavebat.spawnFunc = enemies::flyingSpawnFunc;
            cavebat.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            cavebat.mC.deathSound = &sounds::batdie;
            enemies::enemies.insert(std::make_pair("junglebat", cavebat));
        }
        //hell bat
        {
            enemyBase cavebat = base;
            cavebat.aC.ai = ai_DEMONEYE;
            cavebat.dC.size = glm::vec2(1.5);
            cavebat.pC.size = glm::vec2(1);
            cavebat.pC.friction = false;
            cavebat.dC.tex = "hellbat";
            cavebat.dC.hasmat = true;
            cavebat.aC.onupdate = mobFunctions::batOnUpdate;
            cavebat.pC.bounce = true;
            cavebat.pC.bounciness = .9;
            cavebat.pC.stoponcollision = false;
            cavebat.pC.weight = 0;
            cavebat.mC.hitboxradius = 1;
            cavebat.mC.hp = cavebat.mC.maxhp = 46;
            cavebat.mC.damage = 33;
            cavebat.mC.knockback = 0.2;
            cavebat.mC.kbResist = 0.2;
            cavebat.mC.defense = 8;
            cavebat.mC.families = { mf_ENEMY };
            cavebat.mC.displayName = "Hell Bat";
            cavebat.aC.stats["flyingtex"].charp = "hellbatfly";
            cavebat.aC.stats["hangingtex"].charp = "hellbathang";
            cavebat.drops = { {"coppercoin", {85, 10, 1}} };
            cavebat.mC.onPlayerCollision = collisionFs::damagePlayer;
            cavebat.hpbar = true;
            cavebat.spawnFunc = enemies::flyingSpawnFunc;
            cavebat.drops = {{"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            cavebat.mC.deathSound = &sounds::batdie;
            enemies::enemies.insert(std::make_pair("hellbat", cavebat));
        }
        //demon
        {
            enemyBase demon = base;
            demon.aC.ai = ai_DEMONEYE;
            demon.dC.size = glm::vec2(5);
            demon.pC.size = glm::vec2(4);
            demon.pC.friction = false;
            demon.dC.tex = "demon";
            demon.dC.hasmat = true;
            demon.aC.onupdate = mobFunctions::batOnUpdate;
            demon.aC.actions.push_back({ 180, mobFunctions::demonShoot, -1 });
            demon.pC.bounce = true;
            demon.pC.bounciness = .9;
            demon.pC.stoponcollision = false;
            demon.pC.weight = 0;
            demon.mC.hitboxradius = 1;
            demon.mC.hp = demon.mC.maxhp = 120;
            demon.mC.knockback = 0.2;
            demon.mC.kbResist = 0.2;
            demon.mC.damage = 33;
            demon.mC.defense = 8;
            demon.mC.families = { mf_ENEMY };
            demon.mC.displayName = "Demon";
            demon.aC.stats["flyingtex"].charp = "demon";
            demon.drops = { {"coppercoin", {85, 10, 1}} };
            demon.mC.onPlayerCollision = collisionFs::damagePlayer;
            demon.hpbar = true;
            demon.spawnFunc = enemies::flyingSpawnFunc;
            demon.drops = { {"voodoodoll",{1, 0, 0.1}},{"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            demon.mC.gore = "demon";
            enemies::enemies.insert(std::make_pair("demon", demon));
        }
        //vulture
        {
            enemyBase vulture = base;
            vulture.aC.ai = ai_DEMONEYE;
            vulture.dC.size = glm::vec2(5);
            vulture.pC.size = glm::vec2(4);
            vulture.pC.friction = false;
            vulture.dC.tex = "vulture";
            vulture.dC.hasmat = true;
            vulture.aC.onupdate = mobFunctions::vultureOnUpdate;
            vulture.pC.bounce = true;
            vulture.pC.bounciness = .999f;
            vulture.pC.stoponcollision = false;
            vulture.pC.weight = 0;
            vulture.mC.hitboxradius = 1;
            vulture.mC.hp = vulture.mC.maxhp = 40;
            vulture.mC.knockback = 0.2;
            vulture.mC.kbResist = 0.5;
            vulture.mC.damage = 14;
            vulture.mC.defense = 4;
            vulture.mC.families = { mf_ENEMY };
            vulture.mC.displayName = "vulture";
            vulture.drops = { {"coppercoin", {85, 10, 1}} };
            vulture.mC.onPlayerCollision = collisionFs::damagePlayer;
            vulture.hpbar = true;
            vulture.spawnFunc = enemies::vultureSpawnFunc;
            vulture.drops = {{"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            vulture.mC.gore = "vulture";
            enemies::enemies.insert(std::make_pair("vulture", vulture));
        }
        //hornet
        {
            enemyBase hornet = base;
            hornet.aC.ai = ai_DEMONEYE;
            hornet.dC.size = glm::vec2(3);
            hornet.pC.size = glm::vec2(2.2);
            hornet.pC.friction = false;
            hornet.dC.tex = "hornet";
            hornet.dC.hasmat = true;
            hornet.aC.onupdate = mobFunctions::batOnUpdate;
            hornet.aC.actions.push_back({ 180, mobFunctions::hornetShoot, -1 });
            hornet.pC.bounce = true;
            hornet.pC.bounciness = .9;
            hornet.pC.stoponcollision = false;
            hornet.pC.weight = 0;
            hornet.mC.hitboxradius = 1;
            hornet.mC.hp = hornet.mC.maxhp = 48;
            hornet.mC.knockback = 0.5;
            hornet.mC.kbResist = 0.5;
            hornet.mC.damage = 26;
            hornet.mC.defense = 12;
            hornet.mC.families = { mf_ENEMY };
            hornet.mC.displayName = "hornet";
            hornet.aC.stats["flyingtex"].charp = "hornetfly";
            hornet.aC.stats["followrange"].floatVal = 20;
            hornet.drops = { {"silvercoin", {2, 1, 1}} };
            hornet.mC.onPlayerCollision = collisionFs::damagePlayerPoison;
            hornet.hpbar = true;
            hornet.drops = { {"stinger", {1, 0, 0.3}}, {"vines", {2, 1, 0.2}}, {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            enemies::enemies.insert(std::make_pair("hornet", hornet));
        }
        //antlioncharger
        {
            enemyBase antlioncharger = base;
            antlioncharger.aC.ai = ai_ZOMBIE;
            antlioncharger.pC.size = glm::vec2(2, 2.32);
            antlioncharger.pC.friction = false;
            antlioncharger.pC.weight = 1.1;
            antlioncharger.dC.size = glm::vec2(5);
            antlioncharger.dC.tex = "antlioncharger";
            antlioncharger.aC.onupdate = mobFunctions::zombieOnUpdate;
            antlioncharger.mC.hitboxradius = 1.3;
            antlioncharger.mC.hp = antlioncharger.mC.maxhp = 80;
            antlioncharger.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            antlioncharger.mC.families = { mf_ENEMY };
            antlioncharger.mC.damage = 25;
            antlioncharger.mC.defense = 10;
            antlioncharger.mC.kbResist = 0.6;
            antlioncharger.mC.knockback = .5;
            antlioncharger.aC.stats["walkanim"].charp = "antlionchargerwalk";
            antlioncharger.aC.stats["ms"].floatVal = 0.3;
            antlioncharger.mC.displayName = "Antlion Charger";
            antlioncharger.hpbar = true;
            antlioncharger.mC.onPlayerCollision = collisionFs::damagePlayer;
            antlioncharger.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            antlioncharger.mC.gore = "antlion";
            enemies::enemies.insert(std::make_pair("antlioncharger", antlioncharger));
        }
        //darkcaster
        {
            enemyBase darkcaster = base;
            darkcaster.aC.ai = ai_ZOMBIE;
            darkcaster.pC.size = glm::vec2(2.8);
            darkcaster.pC.friction = false;
            darkcaster.pC.weight = 1.1;
            darkcaster.aC.actions.push_back({ 180, mobFunctions::casterTeleport, -1 });
            darkcaster.aC.actions.push_back({ 140, mobFunctions::casterShoot, 1 });
            darkcaster.dC.size = glm::vec2(2.8);
            darkcaster.dC.tex = "darkcaster";
            darkcaster.mC.hitboxradius = 1.3;
            darkcaster.mC.hp = darkcaster.mC.maxhp = 50;
            darkcaster.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            darkcaster.mC.families = { mf_ENEMY };
            darkcaster.mC.damage = 20;
            darkcaster.mC.defense = 2;
            darkcaster.mC.knockback = .7;
            darkcaster.mC.kbResist = .4;
            darkcaster.mC.displayName = "Dark Caster";
            darkcaster.aC.stats["projectile"].charp = "darkcasterprojectile";
            darkcaster.hpbar = true;
            darkcaster.mC.onPlayerCollision = collisionFs::damagePlayer;
            darkcaster.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}}, {"waterspell", {1, 0, 0.5}} };
            darkcaster.mC.damageSound = &sounds::skeletonhit;
            darkcaster.mC.gore = "skeleton";
            enemies::enemies.insert(std::make_pair("darkcaster", darkcaster));
        }
        //devourerhead
        {
            enemyBase devourer = base;
            devourer.aC.ai = ai_ZOMBIE;
            devourer.pC.size = glm::vec2(1.3);
            devourer.pC.friction = false;
            devourer.pC.stoponcollision = false;
            devourer.pC.weight = 0;
            devourer.dC.size = glm::vec2(1.3);
            devourer.dC.tex = "devourerhead";
            devourer.aC.onupdate = mobFunctions::wormHeadOnUpdate;
            devourer.aC.actions = { {1, mobFunctions::wormCreateBody, 1} };
            devourer.mC.hitboxradius = 1.3;
            devourer.mC.hp = devourer.mC.maxhp = 50;
            devourer.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            devourer.mC.families = { mf_ENEMY };
            devourer.mC.damage = 20;
            devourer.mC.defense = 2;
            devourer.mC.knockback = .7;
            devourer.mC.kbResist = 1;
            devourer.mC.displayName = "Devourer";
            devourer.hpbar = true;
            devourer.mC.onPlayerCollision = collisionFs::damagePlayer;
            devourer.spawnFunc = enemies::wormSpawnFunc;
            devourer.drops = { {"silvercoin",{2, 1, 0.75}}, {"coppercoin",{60, 20, 1}} };
            devourer.mC.gore = "devourerhead";
            enemies::enemies.insert(std::make_pair("devourerhead", devourer));
        }
        //devourerbody
        {
            enemyBase devourer = base;
            devourer.aC.ai = ai_ZOMBIE;
            devourer.pC.size = glm::vec2(1.3);
            devourer.pC.friction = false;
            devourer.pC.weight = 0;
            devourer.pC.stoponcollision = false;
            devourer.dC.size = glm::vec2(1.3);
            devourer.dC.tex = "devourerbody";
            devourer.aC.onupdate = mobFunctions::wormBodyOnUpdate;
            devourer.aC.stats["length"].intVal = 8;
            devourer.aC.stats["body"].charp = "devourerbody";
            devourer.aC.stats["tail"].charp = "devourertail";
            devourer.mC.hitboxradius = 1.3;
            devourer.mC.hp = devourer.mC.maxhp = 50;
            devourer.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            devourer.mC.families = { mf_ENEMY };
            devourer.mC.damage = 20;
            devourer.mC.defense = 2;
            devourer.mC.kbResist = 1;
            devourer.mC.knockback = .7;
            devourer.mC.displayName = "Devourer";
            devourer.hpbar = true;
            devourer.mC.onPlayerCollision = collisionFs::damagePlayer;
            devourer.mC.gore = "devourerbody";
            devourer.slots = 0;
            enemies::enemies.insert(std::make_pair("devourerbody", devourer));
        }
        //devourertail
        {
            enemyBase devourer = base;
            devourer.aC.ai = ai_ZOMBIE;
            devourer.pC.size = glm::vec2(1.3);
            devourer.pC.friction = false;
            devourer.pC.weight = 0;
            devourer.pC.stoponcollision = false;
            devourer.dC.size = glm::vec2(1.3);
            devourer.dC.tex = "devourertail";
            devourer.aC.onupdate = mobFunctions::wormBodyOnUpdate;
            devourer.mC.hitboxradius = 1.3;
            devourer.mC.hp = devourer.mC.maxhp = 50;
            devourer.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            devourer.mC.families = { mf_ENEMY };
            devourer.mC.damage = 20;
            devourer.mC.defense = 2;
            devourer.mC.kbResist = 1;
            devourer.mC.knockback = .7;
            devourer.mC.displayName = "Devourer";
            devourer.hpbar = true;
            devourer.mC.onPlayerCollision = collisionFs::damagePlayer;
            devourer.mC.gore = "devourertail";
            devourer.slots = 0;
            enemies::enemies.insert(std::make_pair("devourertail", devourer));
        }
        //eyeofcthulu
        {
            enemyBase eyeofcthulu = base;
            eyeofcthulu.aC.ai = ai_ZOMBIE;
            eyeofcthulu.pC.size = glm::vec2(11);
            eyeofcthulu.pC.friction = false;
            eyeofcthulu.pC.weight = 0;
            eyeofcthulu.pC.stoponcollision = false;
            eyeofcthulu.dC.size = glm::vec2(11);
            eyeofcthulu.dC.tex = "eyeofcthulu1";
            eyeofcthulu.aC.onupdate = mobFunctions::eyeofcthuluOnUpdate;
            eyeofcthulu.mC.hitboxradius = 11;
            eyeofcthulu.mC.hp = eyeofcthulu.mC.maxhp = 2800;
            eyeofcthulu.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            eyeofcthulu.mC.families = { mf_ENEMY };
            eyeofcthulu.mC.damage = 15;
            eyeofcthulu.mC.defense = 12;
            eyeofcthulu.mC.kbResist = 1;
            eyeofcthulu.mC.knockback = .7;
            eyeofcthulu.mC.displayName = "eyeofcthulu";
            eyeofcthulu.hpbar = true;
            eyeofcthulu.mC.onPlayerCollision = collisionFs::damagePlayer;
            eyeofcthulu.pec.tex = "particle:bloodpart";
            eyeofcthulu.pec.amount = 1;
            eyeofcthulu.pec.rate = 20;
            eyeofcthulu.pec.randomizerate = 0.05;
            eyeofcthulu.pec.weight = 0.4;
            eyeofcthulu.pec.dir = { 0,-1 };
            eyeofcthulu.pec.ms = 0.05;
            eyeofcthulu.pec.randomizems = 0.02;
            eyeofcthulu.pec.randangle = 20;
            eyeofcthulu.pec.randomizescale = 0.5;
            eyeofcthulu.pec.size = { 0.5,0.5 };
            eyeofcthulu.pec.lifespan = -1;
            eyeofcthulu.pec.particleLifespan = 520;
            eyeofcthulu.pec.randomizelifespan = 40;
            eyeofcthulu.pec.randrotation = 8;
            eyeofcthulu.pec.stoponcollision = true;
            eyeofcthulu.pec.smallerAsDie = true;
            eyeofcthulu.pec.disappearAsDie = true;
            eyeofcthulu.pec.radius = 11/7.0f;
            eyeofcthulu.pec.randradius = 11/4.0f;
            eyeofcthulu.drops = { {"demoniteore",{60, 30, 1}}, {"healthpotion", {10, 5, 1}}, {"opticstaff",{1, 0, 0.5}}, {"goldcoin",{1, 0, 0.5}}, {"silvercoin",{60, 20, 1}} };
            eyeofcthulu.bossbar = "bosshead0";
            eyeofcthulu.hpbar = false;
            eyeofcthulu.mC.gore = "eyeofcthulu";
            eyeofcthulu.prespawntext = "You feel an evil presence watching you..";
            eyeofcthulu.spawntext = "The Eye of Cthulu has awoken!";
            eyeofcthulu.prespawntimer = 60 * 30;
            eyeofcthulu.slots = 10;
            enemies::enemies.insert(std::make_pair("eyeofcthulu", eyeofcthulu));
        }
        //eaterofworldshead
        {
            enemyBase eaterofworlds = base;
            eaterofworlds.aC.ai = ai_ZOMBIE;
            eaterofworlds.pC.size = glm::vec2(5);
            eaterofworlds.pC.friction = false;
            eaterofworlds.pC.stoponcollision = false;
            eaterofworlds.pC.weight = 0;
            eaterofworlds.dC.size = glm::vec2(5);
            eaterofworlds.dC.tex = "eaterofworldshead";
            eaterofworlds.aC.onupdate = mobFunctions::eaterofworldsHeadOnUpdate;
            eaterofworlds.aC.actions = { {1, mobFunctions::wormCreateBody, 1} };
            eaterofworlds.aC.stats["length"].intVal = 67;
            eaterofworlds.aC.stats["redirectdmgtohead"].boolVal = false;
            eaterofworlds.aC.stats["body"].charp = "eaterofworldsbody";
            eaterofworlds.aC.stats["tail"].charp = "eaterofworldstail";
            eaterofworlds.mC.hitboxradius = 1.3;
            eaterofworlds.mC.hp = eaterofworlds.mC.maxhp = 50;
            eaterofworlds.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            eaterofworlds.mC.families = { mf_ENEMY };
            eaterofworlds.mC.damage = 20;
            eaterofworlds.mC.defense = 2;
            eaterofworlds.mC.knockback = .7;
            eaterofworlds.mC.kbResist = 1;
            eaterofworlds.mC.displayName = "Eater of worlds";
            eaterofworlds.mC.onDeath = mobFunctions::ondie_eaterofworlds;
            eaterofworlds.hpbar = true;
            eaterofworlds.mC.onPlayerCollision = collisionFs::damagePlayer;
            eaterofworlds.drops = { {"demoniteore",{5, 2, 1}}, {"healthpotion", {1, 0, 0.1}}, {"shadowscales", {2, 1, 0.5}}, {"silvercoin",{4, 2, 1}} };
            eaterofworlds.spawnFunc = enemies::wormSpawnFunc;
            eaterofworlds.mC.gore = "eaterofworldshead";
            eaterofworlds.slots = 10;
            enemies::enemies.insert(std::make_pair("eaterofworldshead", eaterofworlds));
        }
        //eaterofworldsbody
        {
            enemyBase eaterofworlds = base;
            eaterofworlds.aC.ai = ai_ZOMBIE;
            eaterofworlds.pC.size = glm::vec2(4);
            eaterofworlds.pC.friction = false;
            eaterofworlds.pC.weight = 0;
            eaterofworlds.pC.stoponcollision = false;
            eaterofworlds.dC.size = glm::vec2(4);
            eaterofworlds.dC.tex = "eaterofworldsbody";
            eaterofworlds.aC.onupdate = mobFunctions::eaterofworldsbodyOnUpdate;
            eaterofworlds.mC.hitboxradius = 1.3;
            eaterofworlds.mC.hp = eaterofworlds.mC.maxhp = 50;
            eaterofworlds.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            eaterofworlds.mC.families = { mf_ENEMY };
            eaterofworlds.mC.damage = 20;
            eaterofworlds.mC.defense = 2;
            eaterofworlds.mC.kbResist = 1;
            eaterofworlds.mC.knockback = .7;
            eaterofworlds.mC.displayName = "Eater of worlds";
            eaterofworlds.mC.onDeath = mobFunctions::ondie_eaterofworlds;
            eaterofworlds.hpbar = true;
            eaterofworlds.drops = { {"demoniteore",{5, 2, 1}}, {"healthpotion", {1, 0, 0.1}}, {"shadowscales", {2, 1, 0.5}}, {"silvercoin",{4, 2, 1}} };
            eaterofworlds.mC.onPlayerCollision = collisionFs::damagePlayer;
            eaterofworlds.mC.gore = "eaterofworldsbody";
            eaterofworlds.slots = 0;
            enemies::enemies.insert(std::make_pair("eaterofworldsbody", eaterofworlds));
        }
        //eaterofsoulstail
        {
            enemyBase eaterofworlds = base;
            eaterofworlds.aC.ai = ai_ZOMBIE;
            eaterofworlds.pC.size = glm::vec2(5);
            eaterofworlds.pC.friction = false;
            eaterofworlds.pC.weight = 0;
            eaterofworlds.pC.stoponcollision = false;
            eaterofworlds.dC.size = glm::vec2(5);
            eaterofworlds.dC.tex = "eaterofworldstail";
            eaterofworlds.aC.onupdate = mobFunctions::eaterofworldsbodyOnUpdate;
            eaterofworlds.mC.hitboxradius = 4;
            eaterofworlds.mC.hp = eaterofworlds.mC.maxhp = 50;
            eaterofworlds.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            eaterofworlds.mC.families = { mf_ENEMY };
            eaterofworlds.mC.damage = 20;
            eaterofworlds.mC.defense = 2;
            eaterofworlds.mC.kbResist = 1;
            eaterofworlds.mC.knockback = .7;
            eaterofworlds.mC.displayName = "Eater of worlds";
            eaterofworlds.mC.onDeath = mobFunctions::ondie_eaterofworlds;
            eaterofworlds.hpbar = true;
            eaterofworlds.drops = { {"demoniteore",{5, 2, 1}}, {"healthpotion", {1, 0, 0.1}}, {"shadowscales", {2, 1, 0.5}}, {"silvercoin",{4, 2, 1}} };
            eaterofworlds.mC.onPlayerCollision = collisionFs::damagePlayer;
            eaterofworlds.mC.gore = "eaterofworldstail";
            eaterofworlds.slots = 0;
            enemies::enemies.insert(std::make_pair("eaterofworldstail", eaterofworlds));
        }
        //cloud
        {
            enemyBase cloud = base;
            cloud.aC.ai = ai_ZOMBIE;
            cloud.pC.friction = false;
            cloud.pC.weight = 0;
            cloud.dC.opacity = 0;
            cloud.pC.stoponcollision = false;
            cloud.spawnFunc = enemies::anywhereSpawnFunc;
            cloud.aC.onupdate = mobFunctions::cloudOnUpdate;
            cloud.mC.hp = cloud.mC.maxhp = 1;
            cloud.resizeToSprite = true;
            cloud.behindBlocks = true;
            cloud.possibleSprites = { "cloud1", "cloud2", "cloud3", "cloud4", "cloud5", "cloud6", "cloud7", "cloud8","cloud9","cloud10","treecloud"};
            cloud.slots = 0;
            enemies::enemies.insert(std::make_pair("cloud", cloud));
        }
        //penguin1
        {
            enemyBase penguin1 = base;
            penguin1.aC.ai = ai_BUNNY;
            penguin1.pC.size = glm::vec2(1.3, 3.1);
            penguin1.dC.size = glm::vec2(3.1);
            penguin1.dC.tex = "pengu1";
            penguin1.aC.onupdate = mobFunctions::bunnyUpdate;
            penguin1.aC.stats["ms"].floatVal = 0.6;
            penguin1.aC.stats["standinganim"].charp = "pengu1";
            penguin1.aC.stats["walkinganim"].charp = "penguin1walk";
            penguin1.aC.stats["stridetime"].floatVal = 9;
            penguin1.pC.weight = 0.8;
            penguin1.pC.friction = false;
            penguin1.mC.hitboxradius = 1.2;
            penguin1.mC.hp = penguin1.mC.maxhp = 5;
            penguin1.mC.families = { mf_CRITTER };
            penguin1.mC.displayName = "penguin <3";
            penguin1.hpbar = true;
            penguin1.slots = 0;
            enemies::enemies.insert(std::make_pair("penguin1", penguin1));
        }
        //penguin2
        {
            enemyBase penguin1 = base;
            penguin1.aC.ai = ai_BUNNY;
            penguin1.pC.size = glm::vec2(1.3, 3.1);
            penguin1.dC.size = glm::vec2(3.1);
            penguin1.dC.tex = "pengu2";
            penguin1.aC.onupdate = mobFunctions::bunnyUpdate;
            penguin1.aC.stats["ms"].floatVal = 0.6;
            penguin1.aC.stats["standinganim"].charp = "pengu2";
            penguin1.aC.stats["walkinganim"].charp = "penguin2walk";
            penguin1.aC.stats["stridetime"].floatVal = 9;
            penguin1.pC.weight = 0.8;
            penguin1.pC.friction = false;
            penguin1.mC.hitboxradius = 1.2;
            penguin1.mC.hp = penguin1.mC.maxhp = 5;
            penguin1.mC.families = { mf_CRITTER };
            penguin1.mC.displayName = "penguin";
            penguin1.hpbar = true;
            penguin1.slots = 0;
            enemies::enemies.insert(std::make_pair("penguin2", penguin1));
        }
        //penguin3
        {
            enemyBase penguin1 = base;
            penguin1.aC.ai = ai_BUNNY;
            penguin1.pC.size = glm::vec2(1.3, 3.1);
            penguin1.dC.size = glm::vec2(3.1);
            penguin1.dC.tex = "pengu1";
            penguin1.aC.onupdate = mobFunctions::bunnyUpdate;
            penguin1.aC.stats["ms"].floatVal = 0.6;
            penguin1.aC.stats["standinganim"].charp = "pengu3";
            penguin1.aC.stats["walkinganim"].charp = "penguin3walk";
            penguin1.aC.stats["stridetime"].floatVal = 9;
            penguin1.pC.weight = 0.8;
            penguin1.pC.friction = false;
            penguin1.mC.hitboxradius = 1.2;
            penguin1.mC.hp = penguin1.mC.maxhp = 5;
            penguin1.mC.families = { mf_CRITTER };
            penguin1.mC.displayName = "penguin!";
            penguin1.hpbar = true;
            penguin1.slots = 0;
            enemies::enemies.insert(std::make_pair("penguin3", penguin1));
        }
        //penguin4
        {
            enemyBase penguin1 = base;
            penguin1.aC.ai = ai_BUNNY;
            penguin1.pC.size = glm::vec2(1.3, 3.1);
            penguin1.dC.size = glm::vec2(3.1);
            penguin1.dC.tex = "pengu4";
            penguin1.aC.onupdate = mobFunctions::bunnyUpdate;
            penguin1.aC.stats["ms"].floatVal = 0.6;
            penguin1.aC.stats["standinganim"].charp = "pengu4";
            penguin1.aC.stats["walkinganim"].charp = "penguin4walk";
            penguin1.aC.stats["stridetime"].floatVal = 9;
            penguin1.pC.weight = 0.8;
            penguin1.pC.friction = false;
            penguin1.mC.hitboxradius = 1.2;
            penguin1.mC.hp = penguin1.mC.maxhp = 5;
            penguin1.mC.families = { mf_CRITTER };
            penguin1.mC.displayName = "penguin!!";
            penguin1.hpbar = true;
            penguin1.slots = 0;
            enemies::enemies.insert(std::make_pair("penguin4", penguin1));
        }
        //grasshopper
        {
            enemyBase grasshopper = base;
            grasshopper.pC.size = { 0.2, 0.2 };
            grasshopper.pC.weight = 1;
            grasshopper.pC.friction = false;
            grasshopper.dC.size = { 0.7,0.7 };
            grasshopper.dC.tex = "grasshopper";
            grasshopper.mC.displayName = "Grasshopper";
            grasshopper.mC.hitboxradius = 0.4;
            grasshopper.dC.mat = glm::mat4(1.0f);
            grasshopper.dC.mat = glm::mat4(1.0f);
            grasshopper.aC.actions.push_back({ 180, mobFunctions::slimeWindup, -1 });
            grasshopper.aC.stats["jumpanim"].charp = "grasshopperjump";
            grasshopper.aC.stats["flip"].boolVal = true;
            grasshopper.aC.onupdate = mobFunctions::slimeOnUpdate;
            grasshopper.mC.maxhp = grasshopper.mC.hp = 2;
            grasshopper.mC.families = { mf_CRITTER };
            grasshopper.hpbar = true;
            grasshopper.slots = 0;
            enemies::enemies.insert(std::make_pair("grasshopper", grasshopper));
        }
        //firefly
        {
            enemyBase firefly = base;
            firefly.pC.size = { 0.2, 0.2 };
            firefly.pC.weight = 0;
            firefly.pC.vel = glm::vec2(-1, 0);
            firefly.pC.friction = false;
            firefly.dC.size = { 0.7,0.7 };
            firefly.dC.tex = "firefly";
            firefly.mC.displayName = "Firefly";
            firefly.mC.hitboxradius = 1.2;
            firefly.dC.mat = glm::mat4(1.0f);
            firefly.aC.onupdate = mobFunctions::fireflyOnUpdate;
            firefly.mC.maxhp = firefly.mC.hp = 2;
            firefly.mC.families = { mf_CRITTER };
            firefly.hpbar = true;
            firefly.slots = 0;
            enemies::enemies.insert(std::make_pair("firefly", firefly));
        }
        //tombstone
        {
            enemyBase tombstone = base;
            tombstone.pC.size = { 2, 2 };
            tombstone.pC.weight = 2;
            tombstone.pC.vel = glm::vec2(0, 0.7);
            tombstone.pC.friction = false;
            tombstone.dC.size = { 2, 2 };
            tombstone.dC.tex = "tombstone";
            tombstone.dC.mat = glm::mat4(1.0f);
            tombstone.aC.onupdate = mobFunctions::tombstoneOnUpdate;
            tombstone.mC.maxhp = tombstone.mC.hp = 1;
            tombstone.randomvelangle = 25;
            tombstone.slots = 0;
            enemies::enemies.insert(std::make_pair("tombstone", tombstone));
        }
        //scorpion
        {
            enemyBase scorpion = base;
            scorpion.aC.ai = ai_BUNNY;
            scorpion.pC.size = glm::vec2(1.75, 1.375);
            scorpion.dC.size = glm::vec2(1.75);
            scorpion.dC.tex = "scorpion";
            scorpion.aC.onupdate = mobFunctions::bunnyUpdate;
            scorpion.aC.stats["ms"].floatVal = 0.7;
            scorpion.aC.stats["walkinganim"].charp = "scorpionwalk";
            scorpion.aC.stats["stridetime"].floatVal = 8;
            scorpion.pC.weight = 0.8;
            scorpion.pC.friction = false;
            scorpion.mC.hitboxradius = 0.4;
            scorpion.mC.hp = scorpion.mC.maxhp = 5;
            scorpion.mC.families = { mf_CRITTER };
            scorpion.mC.displayName = "Scawwy scowwpion";
            scorpion.hpbar = true;
            scorpion.slots = 0;
            enemies::enemies.insert(std::make_pair("scorpion", scorpion));
        }
        //frog
        {
            enemyBase frog = base;
            frog.aC.ai = ai_BUNNY;
            frog.pC.size = glm::vec2(1.5, 1.125);
            frog.dC.size = glm::vec2(1.5);
            frog.dC.tex = "frog";
            frog.aC.onupdate = mobFunctions::bunnyUpdate;
            frog.aC.stats["ms"].floatVal = 0.55;
            frog.aC.stats["standinganim"].charp = "frogidle";
            frog.aC.stats["walkinganim"].charp = "frogwalk";
            frog.aC.stats["stridetime"].floatVal = 28;
            frog.pC.weight = 0.8;
            frog.pC.friction = false;
            frog.mC.hitboxradius = 0.4;
            frog.mC.hp = frog.mC.maxhp = 5;
            frog.mC.families = { mf_CRITTER };
            frog.mC.displayName = "Froggster!";
            frog.hpbar = true;
            frog.slots = 0;
            enemies::enemies.insert(std::make_pair("frog", frog));
        }
        //grubby
        {
            enemyBase grubby = base;
            grubby.aC.ai = ai_BUNNY;
            grubby.pC.size = glm::vec2(1, .5);
            grubby.dC.size = glm::vec2(1);
            grubby.dC.tex = "grubby";
            grubby.aC.onupdate = mobFunctions::bunnyUpdate;
            grubby.aC.stats["ms"].floatVal = 0.2;
            grubby.aC.stats["walkinganim"].charp = "grubbywalk";
            grubby.aC.stats["stridetime"].floatVal = 24;
            grubby.pC.weight = 0.8;
            grubby.pC.friction = false;
            grubby.mC.hitboxradius = 0.3;
            grubby.mC.hp = grubby.mC.maxhp = 5;
            grubby.mC.families = { mf_CRITTER };
            grubby.mC.displayName = "Grubby";
            grubby.hpbar = true;
            grubby.slots = 0;
            enemies::enemies.insert(std::make_pair("grubby", grubby));
        }

        //snail
        {
            enemyBase snail = base;
            snail.aC.ai = ai_BUNNY;
            snail.pC.size = glm::vec2(1.625, 1);
            snail.dC.size = glm::vec2(1.625);
            snail.dC.tex = "snail";
            snail.aC.onupdate = mobFunctions::snailUpdate;
            snail.aC.stats["ms"].floatVal = 0.02;
            snail.aC.stats["walkinganim"].charp = "snailwalk";
            snail.aC.stats["stuckanim"].charp = "snailstuck";
            snail.pC.weight = 0.8;
            snail.pC.friction = false;
            snail.mC.hitboxradius = 0.3;
            snail.mC.hp = snail.mC.maxhp = 5;
            snail.mC.families = { mf_CRITTER };
            snail.mC.displayName = "take me to snurch ill worship like a ...snail in the shrine of your light ill tell you my snins so you can snarpen your snife";
            snail.hpbar = true;
            snail.slots = 0;
            enemies::enemies.insert(std::make_pair("snail", snail));
        }
        //bird
        {
            enemyBase bird = base;
            bird.aC.ai = ai_BUNNY;
            bird.pC.size = glm::vec2(1.875, 1.125);
            bird.dC.size = glm::vec2(1.875);
            bird.dC.tex = "bird";
            bird.aC.onupdate = mobFunctions::birdUpdate;
            bird.pC.weight = 0.8;
            bird.pC.friction = false;
            bird.mC.hitboxradius = 0.3;
            bird.mC.hp = bird.mC.maxhp = 5;
            bird.mC.families = { mf_CRITTER };
            bird.mC.displayName = "birb";
            bird.hpbar = true;
            bird.slots = 0;
            enemies::enemies.insert(std::make_pair("bird", bird));
        }
        //fish
        {
            enemyBase goldfish = base;
            goldfish.aC.ai = ai_BUNNY;
            goldfish.pC.size = glm::vec2(1.625, 1.375);
            goldfish.dC.size = glm::vec2(1.625);
            goldfish.dC.tex = "goldfish";
            goldfish.dC.hasmat = true;
            goldfish.aC.onupdate = mobFunctions::goldfishUpdate;
            goldfish.pC.weight = 0.8;
            goldfish.pC.friction = false;
            goldfish.pC.stoponcollision = false;
            goldfish.pC.bounce = true;
            goldfish.pC.bounciness = 0.5;
            goldfish.mC.hitboxradius = 0.3;
            goldfish.mC.hp = goldfish.mC.maxhp = 5;
            goldfish.mC.families = { mf_CRITTER };
            goldfish.mC.displayName = "Goldfish";
            goldfish.spawnFunc = enemies::liquidSpawnFunc;
            goldfish.hpbar = true;
            goldfish.slots = 0;
            enemies::enemies.insert(std::make_pair("goldfish", goldfish));
        }
        //fish
        {
            enemyBase jellyfish = base;
            jellyfish.aC.ai = ai_BUNNY;
            jellyfish.pC.size = glm::vec2(1.625, 1.875);
            jellyfish.dC.size = glm::vec2(1.875);
            jellyfish.dC.tex = "jellyfish";
            jellyfish.dC.hasmat = true;
            jellyfish.aC.onupdate = mobFunctions::jellyfishUpdate;
            jellyfish.pC.weight = 0.8;
            jellyfish.pC.friction = false;
            jellyfish.pC.stoponcollision = false;
            jellyfish.pC.bounce = true;
            jellyfish.pC.bounciness = 0.2;
            jellyfish.mC.hitboxradius = 0.3;
            jellyfish.mC.light = glm::vec3(0.3);
            jellyfish.mC.hp = jellyfish.mC.maxhp = 5;
            jellyfish.mC.families = { mf_ENEMY };
            jellyfish.mC.displayName = "Blue Jellyfish";
            jellyfish.mC.hitboxradius = 1.3;
            jellyfish.mC.hp = jellyfish.mC.maxhp = 68;
            jellyfish.mC.onCollision = { {mf_CRITTER, collisionFs::damage} };
            jellyfish.mC.families = { mf_ENEMY };
            jellyfish.mC.damage = 50;
            jellyfish.mC.defense = 4;
            jellyfish.mC.knockback = .7;
            jellyfish.mC.kbResist = .1;
            jellyfish.hpbar = true;
            jellyfish.mC.onPlayerCollision = collisionFs::damagePlayer;
            jellyfish.drops = { {"silvercoin",{4, 2, 1}} };
            jellyfish.spawnFunc = enemies::liquidSpawnFunc;
            jellyfish.mC.gore = "bluejellyfish";
            jellyfish.hpbar = true;
            jellyfish.slots = 1;
            enemies::enemies.insert(std::make_pair("jellyfish", jellyfish));
        }
        enemies::addEnemyToDay("grasshopper", "forest");
        enemies::addEnemyToDay("penguin1", "snow");
        enemies::addEnemyToDay("penguin2", "snow");
        enemies::addEnemyToDay("penguin3", "snow");
        enemies::addEnemyToDay("penguin4", "snow");
        enemies::addEnemyToDay("cloud", "forest");
        enemies::addEnemyToDay("cloud", "jungle");
        enemies::addEnemyToDay("cloud", "snow");
        enemies::addEnemyToDay("cloud", "corruption");
        enemies::addEnemyToDay("bunny", "forest");
        enemies::addEnemyToDay("greenslime", "forest");
        enemies::addEnemyToDay("iceslime", "snow");
        enemies::addEnemyToUnderground("iceslime", "snow");
        enemies::addEnemyToCavern("iceslime", "snow");
        enemies::addEnemyToUnderworld("lavaslime", "forest");
        enemies::addEnemyToDay("spikedjungleslime", "jungle");
        enemies::addEnemyToUnderground("spikediceslime", "snow");
        enemies::addEnemyToCavern("spikediceslime", "snow");
        enemies::addEnemyToUnderground("colorableslime", "forest");
        enemies::addEnemyToNight("zombie", "forest");
        enemies::addEnemyToNight("zombie", "snow");
        enemies::addEnemyToNight("zombie", "desert");
        enemies::addEnemyToNight("demoneye", "forest");
        enemies::addEnemyToNight("demoneye", "snow");
        enemies::addEnemyToNight("demoneye", "desert");
        enemies::addEnemyToDay("eaterofsouls", "corruption");
        enemies::addEnemyToNight("eaterofsouls", "corruption");
        enemies::addEnemyToNight("star", "forest");
        enemies::addEnemyToNight("star", "corruption");
        enemies::addEnemyToNight("star", "jungle");
        enemies::addEnemyToNight("star", "snow");
        enemies::addEnemyToUnderground("mouse", "forest");
        enemies::addEnemyToUnderground("darkcaster", "forest");
        enemies::addEnemyToUnderground("cavebat", "forest");
        enemies::addEnemyToUnderworld("demon", "forest");
        enemies::addEnemyToUnderworld("hellbat", "forest");
        enemies::addEnemyToUnderground("hornet", "jungle");
        enemies::addEnemyToDay("vulture", "desert");
        enemies::addEnemyToUnderground("antlioncharger", "desert");
        enemies::addEnemyToDay("antlioncharger", "desert");
        enemies::addEnemyToNight("eyeofcthulu", "forest");
        enemies::addEnemyToCavern("firefly", "forest");


        std::vector<std::string> names = {
            "micky",
            "jacky",
            "guidemaster123",
            "MIAIJ JAKISON",
            "HE HE"
        };
        map::npcs["guide"] = map::npc(rand() % names.size(), names, "guide", true, true);
        names = {
            "shoppster",
            "amazon.co",
            "Merchant"
            "ISELLSTUFF"
        };
        map::npcs["merchant"] = map::npc(rand() % names.size(), names, "merchant");
    }

    void createEffects()
    {
        particles::particleBase base;
        base.offset = glm::vec2(0);
        base.peC.amount = 1;
        base.peC.dir = glm::vec2(-1, 0);
        base.peC.disappearAsDie = false;
        base.peC.killentity = true;
        base.peC.lifespan = 1;
        base.peC.ms = 0;
        base.peC.rate = 1;
        base.peC.tex = "dirt";
        base.peC.size = glm::vec2(1);
        base.peC.stoponcollision = false;

        //cloud in a bottle
        {
            particles::effectBase effect;
            particles::particleBase cloud = base;
            cloud.peC.tex = "particle:cloudpart2";
            cloud.peC.amount = 7;
            cloud.peC.randangle = 360;
            cloud.peC.ms = 0.045;
            cloud.peC.randomizems = 0.035;
            cloud.peC.particleLifespan = 80;
            cloud.peC.randomizelifespan = 20;
            cloud.peC.randrotation = 5;
            cloud.peC.disappearAsDie = true;
            cloud.peC.size = glm::vec2(0.7);
            cloud.peC.stopAsDie = true;
            effect.push_back(cloud);
            cloud.offset = glm::vec2(-2, 0);
            effect.push_back(cloud);
            cloud.offset = glm::vec2(2, 0);
            effect.push_back(cloud);

            cloud = base;
            cloud.peC.tex = "particle:cloudpart";
            cloud.peC.particleLifespan = 60;
            cloud.peC.disappearAsDie = true;
            cloud.peC.size = glm::vec2(2.3);
            cloud.peC.randangle = 360;
            cloud.peC.ms = 0.01;
            cloud.peC.randomizems = 0.005;
            cloud.peC.randrotation = 10;
            cloud.offset = glm::vec2(-2, 0);
            effect.push_back(cloud);
            cloud.offset = glm::vec2(0, 0);
            effect.push_back(cloud);
            cloud.offset = glm::vec2(2, 0);
            effect.push_back(cloud);

            particles::addEffect("cloudinabottle", effect);
        }

        //magic mirror
        {
            particles::effectBase effect;
            particles::particleBase cloud = base;
            cloud.peC.tex = "particle:magicpart";
            cloud.peC.amount = 4;
            cloud.peC.rate = 1;
            cloud.peC.lifespan = 25;
            cloud.peC.randangle = 360;
            cloud.peC.ms = 0.065;
            cloud.peC.randomizems = 0.035;
            cloud.peC.particleLifespan = 100;
            cloud.peC.randomizelifespan = 20;
            cloud.peC.randrotation = 5;
            cloud.peC.disappearAsDie = true;
            cloud.peC.size = glm::vec2(0.8);
            cloud.peC.randomizescale = 0.3;
            cloud.peC.stopAsDie = true;
            effect.push_back(cloud);
            particles::addEffect("magicmirror", effect);
        }

        //test
        {
            particles::effectBase effect;
            particles::particleBase cloud = base;
            cloud.peC.tex = "particle:dustpart";
            cloud.peC.amount = 1;
            cloud.peC.rate = 0.01;
            cloud.peC.lifespan = 60;
            cloud.peC.randangle = 90;
            cloud.peC.dir = glm::vec2(0, 1);
            cloud.peC.ms = 1;
            cloud.peC.particleLifespan = 60;
            cloud.peC.size = glm::vec2(1);
            effect.push_back(cloud);
            particles::addEffect("test", effect);
        }

        //terra blade
        {
            particles::effectBase effect;
            particles::particleBase terrablade = base;
            terrablade.peC.tex = "particle:terrapart";
            terrablade.peC.amount = 4;
            terrablade.peC.rate = 1;
            terrablade.peC.lifespan = 1;
            terrablade.peC.randangle = 180;
            terrablade.peC.dir = glm::vec2(0, 1);
            terrablade.peC.radius = 0.5;
            terrablade.peC.randradius = 0.5;
            terrablade.peC.ms = 0.01;
            terrablade.peC.particleLifespan = 30;
            terrablade.peC.randomizelifespan = 10;
            terrablade.peC.smallerAsDie = true;
            terrablade.peC.disappearAsDie = true;
            terrablade.peC.size = glm::vec2(0.8);
            effect.push_back(terrablade);
            particles::addEffect("terrabladedamage", effect);
        }

        //onfire
        {
            particles::effectBase effect;
            particles::particleBase onfire = base;
            onfire.peC.tex = "particle:torchpart";
            onfire.peC.amount = 3;
            onfire.peC.rate = 1;
            onfire.peC.lifespan = 2;
            onfire.peC.randangle = 7;
            onfire.peC.ms = 0.045;
            onfire.peC.randomizems = 0.015;
            onfire.peC.particleLifespan = 60;
            onfire.peC.randomizelifespan = 20;
            onfire.peC.randrotation = 5;
            onfire.peC.dir = glm::vec2(0, 1);
            onfire.peC.size = glm::vec2(1);
            onfire.peC.randomizescale = 0.3;
            onfire.peC.smallerAsDie = true;
            onfire.peC.radius = 0.75;
            onfire.peC.randradius = 0.5;
            onfire.peC.opacity = 0.5;
            onfire.peC.opacityovertime = 1.0f / 45;
            onfire.peC.parent = globals::topparticleLayer;
            effect.push_back(onfire);
            particles::addEffect("onfire", effect);
        }

        //bomb explosion
        {
            particles::effectBase effect;
            particles::particleBase bomb = base;
            bomb.peC.amount = 15;
            bomb.peC.dir = glm::vec2(-1, 0);
            bomb.peC.disappearAsDie = true;
            bomb.peC.killentity = true;
            bomb.peC.lifespan = 2;
            bomb.peC.ms = 0.02;
            bomb.peC.randomizems = 0.18;
            bomb.peC.size = glm::vec2(1.5);
            bomb.peC.randomizescale = 0.5;
            bomb.peC.particleLifespan = 40;
            bomb.peC.randangle = 360;
            bomb.peC.randomizeamount = 5;
            bomb.peC.randomizelifespan = 20;
            bomb.peC.randrotation = 5;
            bomb.peC.rotation = 10;
            bomb.peC.rate = 1;
            bomb.peC.stoponcollision = false;
            bomb.peC.tex = "particle:smokepart";
            bomb.peC.weight = 0;
            effect.push_back(bomb);
            particles::addEffect("bombexplosion", effect);
        }

        //eyeofcthuludash
        {
            particles::effectBase effect;
            particles::particleBase eyeofcthuludash = base;
            eyeofcthuludash.peC.amount = 1;
            eyeofcthuludash.peC.dir = glm::vec2(-1, 0);
            eyeofcthuludash.peC.disappearAsDie = true;
            eyeofcthuludash.peC.killentity = true;
            eyeofcthuludash.peC.lifespan = 1;
            eyeofcthuludash.peC.ms = 0;
            eyeofcthuludash.peC.size = glm::vec2(11);
            eyeofcthuludash.peC.particleLifespan = 9;
            eyeofcthuludash.peC.rate = 1;
            eyeofcthuludash.peC.stoponcollision = false;
            eyeofcthuludash.peC.tex = "eyeofcthulu2";
            eyeofcthuludash.peC.weight = 0;
            eyeofcthuludash.peC.color = glm::vec3(0.5, 0.5, 0.5);
            eyeofcthuludash.peC.opacity = 0.3;
            eyeofcthuludash.peC.matchparticledir = true;
            effect.push_back(eyeofcthuludash);
            particles::addEffect("eyeofcthuludash", effect);
        }

        //watersplash
        {
            particles::effectBase effect;
            particles::particleBase watersplash = base;
            watersplash.peC.amount = 4;
            watersplash.peC.dir = glm::vec2(0, 1);
            watersplash.peC.disappearAsDie = true;
            watersplash.peC.killentity = true;
            watersplash.peC.lifespan = 1;
            watersplash.peC.ms = 0.3;
            watersplash.peC.size = glm::vec2(0.5);
            watersplash.peC.randangle = 30;
            watersplash.peC.randomizems = 0.3;
            watersplash.peC.randomizescale = 0.3;
            watersplash.peC.particleLifespan = 60;
            watersplash.peC.rate = 1;
            watersplash.peC.stoponcollision = false;
            watersplash.peC.tex = "particle:waterpart";
            watersplash.peC.weight = 1;
            watersplash.peC.randrotation = 20;
            effect.push_back(watersplash);
            particles::addEffect("watersplash", effect);
        }

        //lavasplash
        {
            particles::effectBase effect;
            particles::particleBase lavasplash = base;
            lavasplash.peC.amount = 4;
            lavasplash.peC.dir = glm::vec2(0, 1);
            lavasplash.peC.disappearAsDie = true;
            lavasplash.peC.smallerAsDie = true;
            lavasplash.peC.killentity = true;
            lavasplash.peC.lifespan = 1;
            lavasplash.peC.ms = 0.3;
            lavasplash.peC.size = glm::vec2(0.5);
            lavasplash.peC.randangle = 30;
            lavasplash.peC.randomizems = 0.3;
            lavasplash.peC.randomizescale = 0.3;
            lavasplash.peC.particleLifespan = 60;
            lavasplash.peC.rate = 1;
            lavasplash.peC.stoponcollision = false;
            lavasplash.peC.tex = "particle:lavapart";
            lavasplash.peC.weight = 1.5;
            lavasplash.peC.randrotation = 20;
            effect.push_back(lavasplash);
            particles::addEffect("lavasplash", effect);
        }

    }
    void registerTexture(const char* name)
    {   
        int id = textures::texidcounter;
        Sprite tmp = { glm::vec4(0), glm::vec2(0) };
        auto p = std::make_shared<Sprite>(tmp);
        textures::nametocoords.insert(std::make_pair(name, p));
        textures::idtocoords.insert(std::make_pair(id, p));
        textures::nameToID.insert(std::make_pair(name, id));
        textures::idtoname.insert(std::make_pair(id, name));
        textures::texidcounter++;
    }

    void addTexture(const char* name, glm::vec4 c, glm::vec2 size)
    {
        int id = 0;
        c.g = globals::spritesheetWH.y - c.g;
        c.a = globals::spritesheetWH.y - c.a;
        c.r /= globals::spritesheetWH.x;
        c.b /= globals::spritesheetWH.x;
        c.g /= globals::spritesheetWH.y;
        c.a /= globals::spritesheetWH.y;

        Sprite tmp = { c, size };
        std::shared_ptr<Sprite> ssprite = std::make_shared<Sprite>(tmp);
        textures::nametocoords[name] = ssprite;
        textures::idtocoords[id] = ssprite;
        textures::idtoname[id] = name;
        textures::nameToID[name] = id;
        id++;
    }

    void loadTextures()
    {
        std::fstream file("assets/data.txt", std::ios::in);
        std::string s;
        while (file) {
            std::getline(file, s);
            if (s == "texCoords") {
                std::string rstr;
                char ch = ' ';
                int n = 0;
                float f = 0;

                std::string id;
                glm::vec4 c = glm::vec4(0);
                glm::vec2 size = glm::vec2(0);

                file >> rstr;
                if (rstr != "[") {
                    return;
                }

                while (file >> id) {
                    if (id == "]") {
                        break;
                    }

                    file >> ch;
                    while (ch != '{') {
                        file >> ch;
                    }
                    file >> n;
                    c.r = n;

                    file >> ch;
                    while (ch != ',') {
                        file >> ch;
                    }
                    file >> n;
                    c.g = n;

                    file >> ch;
                    while (ch != ',') {
                        file >> ch;
                    }
                    file >> n;
                    c.b = n;

                    file >> ch;
                    while (ch != ',') {
                        file >> ch;
                    }
                    file >> n;
                    c.a = n;

                    file >> ch;
                    while (ch != '{') {
                        file >> ch;
                    }
                    file >> f;
                    size.x = f;
                    file >> ch;

                    while (ch != ',') {
                        file >> ch;
                    }
                    file >> f;
                    size.y = f;

                    file >> ch;
                    while (ch != '}') {
                        file >> ch;
                    }

                    addTexture(id.c_str(), c, size);
                }
            }
        }
        file.close();
        
        glm::vec4 spritesheet = glm::vec4(globals::spritesheetWH.x, globals::spritesheetWH.y, globals::spritesheetWH.x, globals::spritesheetWH.y);
        glm::vec4 coords = textures::nametocoords["playerhair"]->coords;
        for (int i = 0; i < 6; i++) {
            textures::playerhair.push_back(coords);
            coords += glm::vec4(20, 0, 20, 0) / spritesheet;
        }
    }

    void loadItems(std::fstream* file)
    {
        std::string rstr;
        char ch = ' ';
        int n = 0;

        std::string id;
        int usespeed;
        int maxstacks;
        std::string texture;
        std::string stattype;
        std::string statname;
        bool valuebool;
        float valuefloat;
        int valueint;
        glm::vec2 valuev2;
        glm::vec3 valuev3;
        std::string valuet;
        std::string str;

        glm::vec4 c = glm::vec4(0);

        *file >> rstr;
        if (rstr != "[") {
            return;
        }

        while (*file >> id) {
            if (id == "]") {
                break;
            }

            *file >> str;
            if (str != "{") {
                break;
            }

            *file >> str;
            usespeed = std::stoi(str);
            *file >> texture;
            *file >> str;
            maxstacks = std::stoi(str);

            items::getInfo(id)->useSpeed = usespeed;
            items::getInfo(id)->tex = texture;
            items::getInfo(id)->maxStacks = maxstacks;

            while (file->good()) {
                *file >> stattype;
                if (stattype == "}") break;
                *file >> statname;
                if (stattype == "f") {
                    *file >> valuefloat;
                    items::addStatF(id.c_str(), statname.c_str(), valuefloat);
                }else
                if (stattype == "i") {
                    *file >> valueint;
                    if (statname == "rarity") {
                        items::addRarity(id.c_str(), (Rarity)valueint);
                        continue;
                    }
                    items::addStatI(id.c_str(), statname.c_str(), valueint);
                }else
                if (stattype == "b") {
                    *file >> valuebool;
                    items::addStatB(id.c_str(), statname.c_str(), valuebool);
                }else
                if (stattype == "v2") {
                    *file >> valuefloat;
                    valuev2.x = valuefloat;
                    *file >> valuefloat;
                    valuev2.y = valuefloat;
                    items::addStatV2(id.c_str(), statname.c_str(), valuev2);
                }else
                if (stattype == "v3") {
                    *file >> valuefloat;
                    valuev3.x = valuefloat;
                    *file >> valuefloat;
                    valuev3.y = valuefloat;
                    *file >> valuefloat;
                    valuev3.z = valuefloat;
                    items::addStatV3(id.c_str(), statname.c_str(), valuev3);
                }else
                if (stattype == "t") {
                    if (statname == "desc") {
                        std::getline(*file, valuet);
                        items::getInfo(id)->description = valuet.substr(1);
                    }
                    else {
                        *file >> valuet;
                        items::addStatT(id.c_str(), statname.c_str(), valuet);
                    }
                }
            }
        }
    }
    void loadLights(std::fstream* file)
    {
        char ch = ' ';
        float n = 0;

        std::string id;
        glm::vec3 color = glm::vec4(0);

        *file >> id;
        if (id != "[") {
            return;
        }

        while (*file >> id) {
            if (id == "]") {
                break;
            }

            *file >> ch;
            while (ch != '{') {
                *file >> ch;
            }
            *file >> n;
            color.r = n;

            *file >> ch;
            while (ch != ',') {
                *file >> ch;
            }
            *file >> n;
            color.g = n;

            *file >> ch;
            while (ch != ',') {
                *file >> ch;
            }
            *file >> n;
            color.b = n;

            *file >> ch;
            while (ch != '}') {
                *file >> ch;
            }

            blocks::nameToInfo[id].light = color;
            blocks::idToInfo[blocks::nameToID[id]].light = color;
        }
    }
}
    

