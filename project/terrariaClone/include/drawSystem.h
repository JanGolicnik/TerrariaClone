#pragma once

#include <ECS/ECS.h>
#include <core.h>
#include <globals.h>

struct drawC {
    std::shared_ptr<glm::vec2> position;
    std::string tex = "empty";
    glm::mat4 mat = glm::mat4(1.0f);
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
    glm::vec3 color = glm::vec3(1, 1, 1);
    float opacity = 1;
    glm::vec4 spriteCoords = glm::vec4(-1, -1, -1, -1);
    bool autocorrect = true;
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