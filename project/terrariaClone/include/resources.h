#pragma once

#include <core.h>
#include <fstream>

namespace resources
{
    void registerAssets();
    void loadAssets(bool shaders = true, bool textures = true, bool items = true);

    void createBlocks();
    void createProjectiles();
    void createEnemies();
    void createEffects();

    void registerTexture(const char* name);
    void addTexture(const char* name, glm::vec4 coords, glm::vec2 size);
    void loadTextures();

    void loadItems(std::fstream* file);

    void loadLights(std::fstream* file);
};

