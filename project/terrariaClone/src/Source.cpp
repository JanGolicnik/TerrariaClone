#include <iostream>

#include <core.h>

#include <Layers.h>
#include <utils.h>

#include <Inventory.h>
#include <DroppedItems.h>
#include <Player.h>
#include <BlockManager.h>
#include <text.h>
#include <animations.h>
#include <world.h>

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

//core
////////////////////////////////////////////////////////////////
GLFWwindow* window;
const int resX = 1024;
const int resY = 800;

void init();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);
void createBlocks();

int frame;

//map stuff
////////////////////////////////////////////////////////////////
const int mapX = 1000;
const int mapY = 300;

const int blocksize = 32;
const int numQuads = mapX * mapY;

void generateMap(int map);
void updateMap();
glm::vec2 mouseBlockCoords();
glm::vec2 camBlockCoords(glm::vec2 offset = glm::vec2(0.0f));
void generateHills(Layers::Layer* layer, bool hasBg, int nhills, int level, int hillspace, int variation, int height, const char* topblock, const char* botblock, const char* bgblock, const char* offblock = "empty", float light = 0.0f, bool genCaves = false,
    std::vector<glm::vec2>* caves = nullptr, bool genOre = false, int orerate = 0, const char* oreblock = 0, int oreY = 0);
void addBlock(const char* name, const char* itemname, const char* layer, glm::vec2 width = glm::vec2(1.0f), float light = 0.0f);
void addRecipe(const char* block, std::vector<recipeItem> items, int craftedAmount);
void addTool(const char* name, ToolFunc function, float strength);

//sahder stuff
////////////////////////////////////////////////////////////////
GLuint blockShaderID;
GLuint spriteShaderID;
GLuint rayCastingShaderID;
GLuint testLightingShaderID;
GLuint displayShaderID;
GLuint backgroundShaderID;
GLuint blurShaderID;

GLuint mainFB;
GLuint mainFBT;

GLuint lightFB;
GLuint lightFBT;

GLuint broken1Tex;
GLuint broken2Tex;
GLuint broken3Tex;

//camera stuffđ
////////////////////////////////////////////////////////////////
glm::mat4 camTrans = glm::mat4(1.0f);
glm::vec2 camMS = glm::vec2(0.0f);
const float camerams = 0.07f;
glm::vec2 camPos = glm::vec2(0.0f);
glm::vec2 camLimits = glm::vec2(-mapX + resX / halfblock, -mapY + resY / halfblock);

bool hasUpdated = true;

void updateCam();

//world constants
////////////////////////////////////////////////////////////////
float grav = 0.04f;

//layers
void renderLayer(Layers::Layer *layer);

//overlays
////////////////////////////////////////////////////////////////
const OverlayVertex overlayVert[4] = {
{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
};
const int32_t overlayEl[6] = {
    0, 1, 3,
    0, 2, 3
};

uint32_t overlayVA;
uint32_t overlayVB;
uint32_t overlayIB;

std::array<GLuint, 3> backgroundTextures;

void drawOverlays();
void calculateLight();

std::vector<Layers::Layer> Layers::layers = {};

float bgoffset = 0.0f;
void drawBG(GLuint tex, float fog, float offset);

//day
const int dayLength = 3600;
int cdayTime = 1000;
glm::vec3 dayclr;
glm::vec3 noonclr;
glm::vec3 eveningclr;
glm::vec3 morningclr;
glm::vec3 nightclr;
float noonW;
float eveningW;
float nightW;
float morningW;
float lnoonW;

int main() {
    init();
    text::load({resX,resY});

    utils::createShader("src/shaders/spriteFrag.frag", "src/shaders/spriteVert.vert", &spriteShaderID);
    utils::createShader("src/shaders/blockFrag.frag", "src/shaders/blockVert.vert", &blockShaderID);
    utils::createShader("src/shaders/rayCastingFrag.frag", "src/shaders/overlayVert.vert", &rayCastingShaderID);
    utils::createShader("src/shaders/testLighting.frag", "src/shaders/overlayVert.vert", &testLightingShaderID);
    utils::createShader("src/shaders/displayFrag.frag", "src/shaders/overlayVert.vert", &displayShaderID);
    utils::createShader("src/shaders/backgroundFrag.frag", "src/shaders/overlayVert.vert", &backgroundShaderID);
    createBlocks();

    BlockM::spriteSheet = utils::LoadTexture("assets/spritesheet.png");

    broken1Tex = utils::LoadTexture("assets/broken1.png");
    broken2Tex = utils::LoadTexture("assets/broken2.png");
    broken3Tex = utils::LoadTexture("assets/broken3.png");

    Layers::setUp(blocksize, glm::ivec2(mapX, mapY), glm::ivec2(resX, resY));

    Layers::addLayer("blocks", 1.0f);
    Layers::addLayer("bg", 0.5f);
    Layers::addLayer("decor", 1.0f, 7);

    Player::create();
    Inventory::create();
    DroppedItems::create();

    glm::vec2 d = glm::vec2(blocksize / (float)resX, blocksize / (float)resY);

    camTrans = glm::scale(glm::mat4(1.0f), glm::vec3(d.x, d.y, 1.0f));

    backgroundTextures[0] = utils::LoadTexture("assets/skorno1.png", 4, true);
    backgroundTextures[1] = utils::LoadTexture("assets/skorno2.png", 4, true);
    backgroundTextures[2] = utils::LoadTexture("assets/skorno3.png", 4, true);

    camPos = glm::vec2(31, 50);

    glBindFramebuffer(GL_FRAMEBUFFER, lightFB);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Layers::Layer* bg = Layers::getLayer("bg");
    Layers::Layer* blocks = Layers::getLayer("blocks");
    Layers::Layer* decor = Layers::getLayer("decor");

    BlockM::lights.push_back(glm::vec3(Player::pos, 1.2f));

    int updateRate = 10;
    int dayRate = 10;
    int maxFps = 60;
    double ctime = 0;
    double ltime = 0;
    double lutime = 0;
    double ldayutime = 0;
    double dutime = 0;
    double daydtime = 0;
    double dtime = 0;
    AnimManager::frame = 0;

    generateMap(2);

    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();

        ctime = glfwGetTime();
        dtime = ctime - ltime;
        dutime = ctime - lutime;
        daydtime = ctime - ldayutime;

        if (daydtime > 1.0f / dayRate) {
            cdayTime++;
            if (cdayTime > dayLength) {
                cdayTime = 0;
            }

            noonclr = glm::vec3(0.9f, 0.9f, 1.0f);
            eveningclr = glm::vec3(0.6f, 0.4f, 0.4f);
            nightclr = glm::vec3(0.15f, 0.15f, 0.2f);
            morningclr = glm::vec3(0.7f, 0.5f, 0.5f);
            lnoonW = noonW;
            noonW = glm::clamp(1 - (abs(cdayTime - dayLength / 4.0f) / (dayLength / 4.0f)), 0.0f, 1.0f);
            eveningW = glm::clamp(1 - (abs(cdayTime - (dayLength / 4.0f * 2)) / (dayLength / 4.0f)), 0.0f, 1.0f);
            nightW = glm::clamp(1 - (abs(cdayTime - (dayLength / 4.0f * 3)) / (dayLength / 4.0f)), 0.0f, 1.0f);
            morningW = glm::clamp(1 - (abs(cdayTime - (dayLength / 4.0f * 0)) / (dayLength / 4.0f)), 0.0f, 1.0f);
            morningW += glm::clamp(1 - (abs(cdayTime - (dayLength / 4.0f * 4)) / (dayLength / 4.0f)), 0.0f, 1.0f);
            dayclr = noonclr * noonW + eveningclr * eveningW + nightclr * nightW + morningclr * morningW;

            ldayutime = ctime;
        }

        if (dutime > 1.0f / updateRate) {
            int startx = abs(camPos.x) - (Layers::blocksOnScreen.x / 2 + 5);
            int starty = abs(camPos.y) - Layers::blocksOnScreen.y / 2;

            for (int x = startx; x < startx + Layers::blocksOnScreen.x + 10; x++) {
                for (int y = starty; y < starty + Layers::blocksOnScreen.y; y++) {
                    Layers::updateBlock(blocks, x, y);
                    Layers::updateBlock(decor, x, y);
                }
            }

            lutime = ctime;
        }

        if (dtime > 1.0f / maxFps) {
            AnimManager::updateAnims();
            Player::update(grav);
            updateCam();
            updateMap();
            DroppedItems::update(grav);
            double ax, ay;
            glfwGetCursorPos(window, &ax, &ay);
            Inventory::refresh(ax/resX, 1-ay/resY);
            
            calculateLight();

            glBindFramebuffer(GL_FRAMEBUFFER, mainFB);
            glClearColor(dayclr.r, dayclr.g, dayclr.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            drawBG(backgroundTextures[2], 0.5f, bgoffset * 0.7f);
            drawBG(backgroundTextures[1], 0.3f, bgoffset * 0.9f);
            drawBG(backgroundTextures[0], 0.1f, bgoffset * 0.9f);
            renderLayer(bg);
            renderLayer(decor);
            renderLayer(blocks);

            Inventory::render();
            DroppedItems::render(camTrans);
            Player::render(spriteShaderID, camTrans);
            text::render();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, lightFB);
            drawOverlays();

            bgoffset = -camPos.x / mapX;

            glfwSwapBuffers(window);

           ltime = ctime;
        }

    }

    glfwDestroyWindow(window);
    glfwTerminate();
    glDeleteBuffers(1, &Player::playerVB);
    glDeleteBuffers(1, &Player::playerVA);
    glDeleteBuffers(1, &Player::playerIB);
    return 0;
}

void init() {
    srand(time(NULL));
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::cout << "failed to init glfw" << std::endl;
    }
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(resX, resY, "simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "failed to create window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "failed to init glad" << std::endl;
    }

    glfwSetKeyCallback(window, (GLFWkeyfun)key_callback);
    glfwSetScrollCallback(window, (GLFWscrollfun)scroll_callback);
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)mousebutton_callback);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //for overlay
    utils::createFrameBuffer(&mainFB, &mainFBT, { resX, resY });
    utils::createFrameBuffer(&lightFB, &lightFBT, { resX, resY });

    glGenVertexArrays(1, &overlayVA);
    glBindVertexArray(overlayVA);

    glGenBuffers(1, &overlayVB);
    glBindBuffer(GL_ARRAY_BUFFER, overlayVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(overlayVert), overlayVert, GL_STATIC_DRAW);

    glGenBuffers(1, &overlayIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlayIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(overlayEl), overlayEl, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), (void*)sizeof(glm::vec3));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void drawBG(GLuint tex, float fog, float offset) {
    glUseProgram(backgroundShaderID);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, tex);
    glUniform1i(5, 0);
    glUniform1i(9, 2);
    glUniform1f(6, fog);
    glUniform3f(7, dayclr.r, dayclr.g, dayclr.b);
    glUniform1f(8, offset);
    glBindVertexArray(overlayVA);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void lightMore(int x, int y, float str, float falloff) {
    if (str <= 0) {
        return;
    }
    Layers::setLight(Layers::getLayer("blocks"), x, y, str);
    lightMore(x-1, y, str - falloff, falloff);
    lightMore(x+1, y, str - falloff, falloff);
    lightMore(x, y-1, str - falloff, falloff);
    lightMore(x, y+1, str - falloff, falloff);
}

void calculateLight() {

    Layers::Layer* blocks = Layers::getLayer("blocks");
    Layers::Layer* bg = Layers::getLayer("bg");
    Layers::Layer* decor = Layers::getLayer("decor");

    int startx = abs(camPos.x) - Layers::blocksOnScreen.x / 2 - 15;
    int starty = abs(camPos.y) - Layers::blocksOnScreen.y / 2 - 15;

    float skyColor = noonW + eveningW / 2.0f + morningW / 2.0f + 0.1f;
    static float prevLight = 0;
    //sky light
    for (int x = startx; x < startx + Layers::blocksOnScreen.x + 35; x++) {
        for (int y = starty; y < starty + Layers::blocksOnScreen.y + 35; y++) {
            const char* b = Layers::queryBlock(bg, x, y);
            if (b == "empty" || b == "glass") {
                Layers::addLight(blocks, x, y, -prevLight);
                Layers::addLight(blocks, x, y, skyColor);
            }
        }
    }

    prevLight = skyColor;

    for (int i = 0; i < BlockM::lights.size(); i++) {
        Layers::setLight(blocks, BlockM::lights[i].x + 1, BlockM::lights[i].y + 1, BlockM::lights[i].z);
    }


    for (int x = startx; x < startx + Layers::blocksOnScreen.x + 35; x++) {
        for (int y = starty; y < starty + Layers::blocksOnScreen.y + 35; y++) {
            float currL = 0.0f;
            currL += Layers::queryVert(blocks, x - 1, y).light;
            currL += Layers::queryVert(blocks, x + 1, y).light;
            currL += Layers::queryVert(blocks, x, y - 1).light;
            currL += Layers::queryVert(blocks, x, y + 1).light;
            Layers::setLight(blocks, x, y, (currL / 4) * 0.99);
            Layers::setLight(bg, x, y, (currL / 4) * 0.99);
            Layers::setLight(decor, x, y, (currL / 4) * 0.99);

        }
    
    }

    BlockM::lights[0] = glm::vec3(Player::pos, 1.2f);


}

void generateHills(Layers::Layer *l, bool hasBg, int nhills, int level, int hillspace, int variation, int height, const char* topblock, const char* botblock, const char* bgblock, const char* offblock, float light, bool genCaves, std::vector<glm::vec2> *caves, bool genOre, int orerate, const char* oreblock, int oreY) {
    
    Layers::Layer* bg = Layers::getLayer("bg");
    
    //generathing hilltops
    glm::vec2 prevH = glm::vec2(-1, level + ((rand() % variation) - variation / 2)); //storing the value of the previous hill
    for (int hill = 0; hill < nhills + 1; hill++) {
        int x = hill * hillspace + (rand() % hillspace);
        int y = level + (prevH.y - level) + ((rand() % variation) - variation / 2); //the last term is because we want it to go from -variation/2 to +variation/2

        //outlining the hills
        glm::vec2 dif = prevH - glm::vec2(x, y);
        for (int i = 0; i < abs(dif.x); i++) {
            //we are looping thru x so x is just x-i
            //for y we add the stepsize(dif.y / abs(dif.x) times the block we're on to its y  :D
            int tx = x - i;
            int ty = y + (dif.y / abs(dif.x) * i);
            Layers::placeBlock(l, tx, ty, topblock, light);
            if (offblock != "empty") {
                if (rand() % 10 == 0) {
                    Layers::placeBlock(l, tx, ty, offblock, light);
                }
            }
            if (genCaves) {
                if (i == abs(dif.x) / 2)
                    if (rand() % 5 == 0) {
                        caves->push_back(glm::vec2(x, y));
                    }
            }


            for (int j = ty - 1; j > 0; j--) {
                Layers::placeBlock(l, tx, j, botblock, light);

                if (genOre)
                    if (rand() % (orerate) == 0 && j < oreY) {
                        Layers::placeBlock(l, tx, j, oreblock, light);
                    }

                if (hasBg)
                    Layers::placeBlock(bg, tx, j, bgblock, 1.0f);

                if (genOre)
                    if (rand() % (orerate) == 0 && j < oreY) {
                        Layers::placeBlock(bg, tx, j, oreblock, 1.0f);
                    }
            }
        }

        prevH.y = y;
        prevH.x = x;
        Layers::placeBlock(l, x, y, topblock);
    }
}

void generateMap(int map) {

    Layers::Layer* blocks = Layers::getLayer("blocks");
    Layers::Layer* decor = Layers::getLayer("decor");
    Layers::Layer* bg = Layers::getLayer("bg");


        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                Layers::placeBlock(blocks, x, y, "empty");
                Layers::placeBlock(bg, x, y, "empty");
                Layers::placeBlock(decor, x, y, "empty");
            }
        }

        if (map == 0) {
            for (int x = 0; x < mapX; x++) {
                for (int y = 0; y < mapY; y++) {
                    if (y <= 20) {
                        Layers::placeBlock(blocks, x, y, "dirt");
                    }
                }
            }
        }
        else if (map == 1) {

        int nhills = mapX / 4; //number of hills
        int level = (mapY / 3) * 2; //minimum hill
        int hillspace = mapX / nhills;// the amount of space each hill gets
        int variation = 5;//how much hill heights can vary
        int height = 10;//how much dirt between hill and stone
        std::vector<glm::vec2> caves;
        generateHills(blocks, true, nhills, level, hillspace, variation, height, "grass2", "dirt", "dirt", "grass", 0.0f, true, &caves);

        ////generating stone
        level = mapY / 3;
        nhills = mapX / 20;
        variation = 5;
        hillspace = mapX / nhills;// the amount of space each hill gets
        int oreSpawnRate = 250;
        int oreSpawnY = mapY / 5;

        generateHills(blocks, true, nhills, level, hillspace, variation, height, "stone", "stone", "stone", "empty", 0.0f, false, nullptr, true, oreSpawnRate, "emerald", oreSpawnY);
        generateHills(bg, false, nhills, level, hillspace, variation, height, "stone", "stone", "empty", "empty", 1.0f, false, nullptr, true, oreSpawnRate, "emerald", oreSpawnY);



        ////generating ores
        ////we've sprinkled some singular ores randomly while generating stone and now we will iterate a few times over them to have them spread
        for (int i = 0; i < 30; i++) {
            for (int x = 0; x < mapX; x++) {
                for (int y = 0; y < mapY; y++) {
                    if (Layers::queryBlock(blocks, x, y) == "emerald") {
                        if (rand() % 10 == 0) {
                            if (rand() % 5 == 0)
                                Layers::placeBlock(blocks, x + 1, y, "emerald");
                            if (rand() % 5 == 0)
                                Layers::placeBlock(blocks, x - 1, y, "emerald");
                            if (rand() % 5 == 0)
                                Layers::placeBlock(blocks, x, y + 1, "emerald");
                            if (rand() % 5 == 0)
                                Layers::placeBlock(blocks, x, y - 1, "emerald");
                        }
                    }
                }
            }
        }


        //caves uwu
        //we pick points in the middle of connections e with hilltops and while generating ston for cave beginnings and then add a few points for each of them and connect them with air
        for (int i = 0; i < caves.size(); i++) {
            int nChildren = rand() % 15 + 10; // length of cave
            int size = rand() % 3 + 3;

            glm::vec3 prevPos = glm::vec3(caves[i], 270.0f);
            for (int c = 0; c < nChildren; c++) {
                int angle = rand() % 90 - 45;
                angle = prevPos.z + angle;
                int length = rand() % 5 + 5;
                int x = prevPos.x + (cos(angle * (PI / 180)) * length);
                int y = prevPos.y + (sin(angle * (PI / 180)) * length);

                glm::vec2 dif = glm::vec2(prevPos.x, prevPos.y) - glm::vec2(x, y);
                for (int seg = 0; seg < length; seg++) {
                    int tx = x + (dif.x / length) * seg;
                    int ty = y + (dif.y / length) * seg;
                    Layers::breakBlock(blocks, tx, ty, size);
                }
                prevPos = glm::vec3(x, y, angle);
            }
        }

        int desertstart = 100;
        int desertend = 100;
        for (int x = desertstart; x < desertstart + desertend; x++) {
            for (int y = Layers::mapSize.y; y >= 0; y--) {
                if (Layers::queryBlock(blocks, x, y) != "empty") {
                    Layers::placeBlock(blocks, x, y, "sand");
                    if (rand() % 10 == 0) {
                        for (int i = 0; i < rand() % 3 + 4; i++) {
                            Layers::placeBlock(x, y + 1 + i, "cactus");
                        }
                    }
                    for (int y2 = y; y2 >= 0; y2--) {
                        Layers::placeBlock(blocks, x, y2, "sand");
                        Layers::placeBlock(x, y2, "sandbg");
                    }
                    break;
                }
            }
        }

        //adding stone to bottom just cuz i think looks alright
        for (int x = 0; x < mapX; x++) {
            Layers::placeBlock(blocks, x, 0, "stone");
        }

        std::vector<glm::vec2> trees;

        for (int x = 0; x < Layers::mapSize.x; x++) {
            for (int y = 0; y < Layers::mapSize.y; y++) {
                if (Layers::queryBlock(blocks, x, y) == "grass2") {
                    if (rand() % 5 == 0)
                        Layers::placeBlock(decor, x, y + 1, "grassdecor", 0.0f);
                    if (rand() % 5 == 0)
                        Layers::placeBlock(decor, x, y + 1, "grassdecor2", 0.0f);
                    if (rand() % 10 == 0)
                        trees.push_back(glm::vec2(x, y + 1));
                }
            }
        }

        for (int i = 0; i < trees.size(); i++) {
            int height = rand() % 5 + 10;
            Layers::placeBlock(decor, trees[i].x, trees[i].y, "trunkbase1", 0.0f);
            Layers::placeBlock(decor, trees[i].x - 1, trees[i].y, "trunkbase2", 0.0f);
            Layers::placeBlock(decor, trees[i].x + 1, trees[i].y, "trunkbase3", 0.0f);
            for (int y = 1; y < height; y++) {
                Layers::placeBlock(decor, trees[i].x, trees[i].y + y, "trunk", 0.0f);
            }
            Layers::placeBlock(decor, trees[i].x - 3, trees[i].y + height, "leaves", 0.0f, 1.0f, false);
            Layers::placeBlock(blocks, trees[i].x, trees[i].y - 1, "dirt");
            Layers::placeBlock(blocks, trees[i].x + 1, trees[i].y - 1, "dirt");
            Layers::placeBlock(blocks, trees[i].x - 1, trees[i].y - 1, "dirt");
        }

    }

        else if (map == 2) {
        int seed = 1134;
            std::vector<float> values = world::generate(0, 1, Layers::mapSize.y, seed, mapX/10 + 1);
            std::vector<float> temp = world::generate(-10, 50, 100, seed, 20);

            std::vector<glm::vec2> trees;
            std::vector<glm::vec2> caves;

            //dirt
            for (int x = 0; x < mapX; x++) {
                float y2 = mapY * world::sampleCosine((float)x / mapX * 10.0f, &values) / 10 + mapY * 0.3;
                float t = world::sampleCosine((float)x / mapX * 19, &temp);
                if(t > 30) {
                    Layers::placeBlock(blocks, x, y2, "sand");
                    for (int y = 0; y < y2 - 1; y++) {
                        Layers::placeBlock(blocks, x, y, "sand");
                        Layers::placeBlock(bg, x, y, "sand");
                    }
                } else if (t > 10) {

                    if (rand() % 10 == 0) {
                        Layers::placeBlock(blocks, x, y2, "grass");
                    }
                    else
                        Layers::placeBlock(blocks, x, y2, "grass2");
                    for (int y = 0; y < y2 - 1; y++) {
                        Layers::placeBlock(blocks, x, y, "dirt");
                        Layers::placeBlock(bg, x, y, "dirt");
                    }
                    if (rand() % 80 == 0) {
                        caves.push_back(glm::vec2(x, y2));
                    }
                }
                else if (t > -10) {
                    Layers::placeBlock(blocks, x, y2, "corruptgrass");
                    for (int y = 0; y < y2 - 1; y++) {
                        Layers::placeBlock(blocks, x, y, "corruptdirt");
                        Layers::placeBlock(bg, x, y, "corruptdirt");
                    }
                    if (rand() % 80 == 0) {
                        caves.push_back(glm::vec2(x, y2));
                    }
                }
            }

            //stone
            for (int x = 0; x < mapX; x++) {
                float y2 = mapY * world::sampleCosine((float)x / mapX * 10.0f, &values) / (rand()%6 + 14) + mapY * 0.22;
                float s = (x + 0);
                for (int y = 0; y < y2; y++) {
                    if(Layers::queryBlock(blocks, x,y) == "dirt")
                    if (rand() % 1800 == 0) {
                        caves.push_back(glm::vec2(x, y2));
                    }
                    Layers::placeBlock(blocks, x, y, "stone");
                    Layers::placeBlock(bg, x, y, "stone");
                }
            }

            //caves
            for (int i = 0; i < caves.size(); i++) {
                int nChildren = rand() % 15 + 10; 
                int size = rand() % 3 + 3;

                glm::vec3 prevPos = glm::vec3(caves[i].x, caves[i].y, 270.0f);
                for (int c = 0; c < nChildren; c++) {
                    int angle = rand() % 90 - 45;
                    angle = prevPos.z + angle;
                    int length = rand() % 5 + 5;
                    int x = prevPos.x + (cos(angle * (PI / 180)) * length);
                    int y = prevPos.y + (sin(angle * (PI / 180)) * length);

                    glm::vec2 dif = glm::vec2(prevPos.x, prevPos.y) - glm::vec2(x, y);
                    for (int seg = 0; seg < length; seg++) {
                        int tx = x + (dif.x / length) * seg;
                        int ty = y + (dif.y / length) * seg;
                        Layers::breakBlock(blocks, tx, ty, size);
                    }
                    prevPos = glm::vec3(x, y, angle);
                }
            }
            //trees
            for (int x = 0; x < Layers::mapSize.x; x++) {
                for (int y = 0; y < Layers::mapSize.y; y++) {
                    if (Layers::queryBlock(blocks, x, y) == "grass2") {
                        int ty = y + 1;
                        if (rand() % 5 == 0)
                            Layers::placeBlock(decor, x, y + 1, "grassdecor", 0.0f);
                        if (rand() % 5 == 0)
                            Layers::placeBlock(decor, x, y + 1, "grassdecor2", 0.0f);
                        if (rand() % 10 == 0) {
                            int height = rand() % 5 + 10;
                            Layers::placeBlock(decor, x, ty, "trunkbase1", 0.0f);
                            Layers::placeBlock(decor, x - 1, ty, "trunkbase2", 0.0f);
                            Layers::placeBlock(decor, x + 1, ty, "trunkbase3", 0.0f);
                            for (int y2 = 1; y2 < height; y2++) {
                                Layers::placeBlock(decor, x, ty + y2, "trunk", 0.0f);
                            }
                            Layers::placeBlock(decor, x - 3, ty + height, "leaves", 0.0f, 1.0f, false);
                            Layers::placeBlock(blocks, x, y, "dirt");
                            Layers::placeBlock(blocks, x + 1, y, "dirt");
                            Layers::placeBlock(blocks, x - 1, y, "dirt");
                        }
                    }else
                    if (Layers::queryBlock(blocks, x, y) == "corruptgrass") {
                        if (rand() % 5 == 0)
                            Layers::placeBlock(decor, x, y + 1, "corruptgrassdecor", 0.0f);
                        if (rand() % 5 == 0)
                            Layers::placeBlock(decor, x, y + 1, "corruptgrassdecor2", 0.0f);
                        if (rand() % 10 == 0) {
                            int ty = y + 1;

                            int height = rand() % 5 + 10;
                            Layers::placeBlock(decor, x, ty, "corrupttrunkbase1", 0.0f);
                            Layers::placeBlock(decor, x - 1, ty, "corrupttrunkbase2", 0.0f);
                            Layers::placeBlock(decor, x + 1, ty, "corrupttrunkbase3", 0.0f);
                            for (int y2 = 1; y2 < height; y2++) {
                                Layers::placeBlock(decor, x, ty + y2, "corrupttrunk", 0.0f);
                            }
                            Layers::placeBlock(decor, x - 3, ty + height, "corruptleaves", 0.0f, 1.0f, false);
                            Layers::placeBlock(blocks, x, y, "corruptdirt");
                            Layers::placeBlock(blocks, x + 1, y, "corruptdirt");
                            Layers::placeBlock(blocks, x - 1, y, "corruptdirt");
                        }
                    }else
                    if (Layers::queryBlock(blocks, x, y) == "sand") {
                        if (rand() % 40 == 0)
                            for (int y2 = 1; y2 < rand() % 4 + 4; y2++) {
                                Layers::placeBlock(x, y+y2, "cactus");
                            }
                    }
                }
            }

            //adding stone to bottom just cuz i think looks alright
            for (int x = 0; x < mapX; x++) {
                Layers::placeBlock(blocks, x, 0, "stone");
            }
        }
}

void addBlock(const char* name, const char* itemname, const char* layer, glm::vec2 width, float light) {
    BlockM::info[name] = {itemname, {},{},{}, 1, layer, width, light, true, NONE, 0 };
}

void addRecipe(const char* block, std::vector<recipeItem> items, int craftedAmount)
{
    BlockM::info[block].recipeFrom = items;
    BlockM::info[block].craftedAmount = craftedAmount;
    for (int i = 0; i < items.size(); i++) {
        BlockM::info[items[i].name].recipeFor.push_back(block);
    }
}

void addTool(const char* name, ToolFunc function, float strength) {
    BlockM::info[name].function = function;
    BlockM::info[name].digstrength = strength;
}

void createBlocks() {
    BlockM::addTex("dirt", { 0,0,1,1 });
    BlockM::addTex("grass", { 2,0,3,1 });
    BlockM::addTex("grass2", { 1,0,2,1 });
    BlockM::addTex("glass", { 5,0,6,1 });
    BlockM::addTex("stone", { 3,0,4,1 });
    BlockM::addTex("emerald", { 4,0,5,1 });
    BlockM::addTex("wood", { 6,0,7,1 });
    BlockM::addTex("woodwall", { 30,0,31,1 });
    BlockM::addTex("player", { 25,0,27,3 });
    BlockM::addTex("playerwalkr1", { 32,0,34,3 });
    BlockM::addTex("playerwalkr2", { 34,0,36,3 });
    BlockM::addTex("playerwalkr3", { 36,0,38,3 });
    BlockM::addTex("playerwalkl1", { 34,0,32,3 });
    BlockM::addTex("playerwalkl2", { 36,0,34,3 });
    BlockM::addTex("playerwalkl3", { 38,0,36,3 });
    BlockM::addTex("playeridle1", { 38,0,40,3 });
    BlockM::addTex("playeridle2", { 40,0,42,3 });
    BlockM::addTex("playeridle3", { 42,0,44,3 });
    BlockM::addTex("playeredit1", { 44,0,46,3 });
    BlockM::addTex("playeredit2", { 46,0,48,3 });
    BlockM::addTex("trunk", { 7,0,8,1 });
    BlockM::addTex("grassdecor", { 11,0,12,1 });
    BlockM::addTex("trunkbase1", { 9,0,10,1 });
    BlockM::addTex("trunkbase3", { 10,0,11,1 });
    BlockM::addTex("trunkbase2", { 8,0,9,1 });
    BlockM::addTex("leaves", { 13,0,17,4 });
    BlockM::addTex("grassdecor2", { 12,0,13,1 });
    BlockM::addTex("invselect", { 23,0,24,1 });
    BlockM::addTex("invleft", { 17, 0, 19, 2 });
    BlockM::addTex("invright", { 21, 0, 23, 2 });
    BlockM::addTex("invmid", { 19, 0, 21, 2 });
    BlockM::addTex("invempty", { 24, 0, 25, 1 });
    BlockM::addTex("table", { 27, 0, 29, 1 });
    BlockM::addTex("stone brick", { 29, 0, 30, 1 });
    BlockM::addTex("torch", { 31, 0, 32, 1 });
    BlockM::addTex("empty", { 0, 2, 0, 2 });
    BlockM::addTex("err", { 0, 3.5, 0.5, 4 });
    BlockM::addTex("ironpickaxe", { 48, 0, 49, 1 });
    BlockM::addTex("stick", { 49, 0, 50, 1 });
    BlockM::addTex("emeraldpickaxe", { 50, 0, 51, 1 });
    BlockM::addTex("sapling", { 51, 0, 52, 1 });
    BlockM::addTex("acorn", { 52, 0, 53, 1 });
    BlockM::addTex("sand", { 53, 2, 54, 3 });
    BlockM::addTex("cactus", { 54, 0, 55, 1 });
    BlockM::addTex("cactustop", { 55, 0, 56, 1 });
    BlockM::addTex("sandbg", { 56, 2, 57, 3 });
    BlockM::addTex("corruptdirt", { 57,0,58,1 });
    BlockM::addTex("corruptgrass", { 58,0,59,1 });
    BlockM::addTex("corruptstone", { 59,0,60,1 });
    BlockM::addTex("corruptwood", { 60,0,61,1 });
    BlockM::addTex("corruptgrassdecor", { 65,0,66,1 });
    BlockM::addTex("corruptgrassdecor2", { 66,0,67,1 });
    BlockM::addTex("corrupttrunk", { 61,0,62,1 });
    BlockM::addTex("corrupttrunkbase1", { 63,0,64,1 });
    BlockM::addTex("corrupttrunkbase3", { 64,0,65,1 });
    BlockM::addTex("corrupttrunkbase2", { 62,0,63,1 });
    BlockM::addTex("corruptleaves", { 67,0,71,4 });

    addBlock("dirt", "dirt", "blocks");
    addBlock("grass", "dirt", "blocks");
    addBlock("grass2", "dirt", "blocks");
    addBlock("glass", "glass", "blocks");
    addBlock("stone", "stone", "blocks");
    addBlock("emerald", "emerald", "blocks");
    addBlock("wood", "wood", "blocks");
    addBlock("woodwall", "woodwall", "bg");
    addBlock("smile", "smile", "blocks");
    addBlock("trunk", "wood", "decor");
    addBlock("grassdecor", "empty", "decor");
    addBlock("trunkbase1", "wood", "decor");
    addBlock("trunkbase2", "wood", "decor");
    addBlock("trunkbase3", "wood", "decor");
    addBlock("leaves", "acorn", "decor", glm::vec2(7));
    addBlock("grassdecor2", "emptyempty", "decor");
    addBlock("torch", "torch", "decor", glm::vec2(1.0f, 1.0f), 2.0f);
    addBlock("table", "table", "decor", glm::vec2(2, 1));
    addBlock("stone brick", "stone brick", "blocks");
    addBlock("sapling", "acorn", "decor");
    addBlock("acorn", "acorn", "decor");
    addBlock("sand", "sand", "blocks");
    addBlock("cactus", "cactus", "decor");
    addBlock("cactustop", "cactus", "decor");
    addBlock("sandbg", "sand", "bg");
    addBlock("corruptdirt", "dirt", "blocks");
    addBlock("corruptgrass", "dirt", "blocks");
    addBlock("corruptstone", "corruptstone", "blocks");
    addBlock("corruptwood", "corruptwood", "blocks");
    addBlock("corruptgrassdecor", "empty", "bg");
    addBlock("corruptgrassdecor2", "empty", "bg");
    addBlock("corrupttrunk", "corruptwood", "bg");
    addBlock("corrupttrunkbase1", "corruptwood", "bg");
    addBlock("corrupttrunkbase2", "corruptwood", "bg");
    addBlock("corrupttrunkbase3", "corruptwood", "bg");
    addBlock("corruptleaves", "corruptwood", "bg", glm::vec2(7));

    addTool("ironpickaxe", PICKAXE, 10);
    addTool("emeraldpickaxe", PICKAXE, 40);

    addRecipe("table", { {"wood", 10 } }, 1);
    addRecipe("torch", { {"wood", 1 } }, 3);
    addRecipe("stone brick", { {"stone", 2 } }, 1);
    addRecipe("woodwall", { {"wood", 1 } }, 4);
    addRecipe("stick", { {"wood", 1 } }, 5);
    addRecipe("emeraldpickaxe", { {"stick", 10 }, {"emerald", 20} }, 1);
    addRecipe("glass", { {"sand", 2 } }, 1);

    AnimManager::createAnim("playerwalkleft", { {0, "playerwalkl1"},{5, "playerwalkl2"}, {10, "playerwalkl3"} }, 15);
    AnimManager::createAnim("playerwalkright", { {0, "playerwalkr1"},{5, "playerwalkr2"}, {10, "playerwalkr3"} }, 15);
    AnimManager::createAnim("playeridle", { {0, "player"},{300, "playeridle1"},{305, "playeridle2"},{315, "playeridle3"} }, 325, true, 2);
    AnimManager::createAnim("playeredit", { {0, "playeredit1"},{5, "playeredit2"} }, 10);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        Player::velocity.x -= Player::ms;
        AnimManager::watchAnim("playerwalkleft", &Player::anim);
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        Player::velocity.x = 0.0f;
        AnimManager::watchAnim("playeridle", &Player::anim);
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        AnimManager::watchAnim("playerwalkright", &Player::anim);
        Player::velocity.x += Player::ms;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        Player::velocity.x = 0.0f;
        AnimManager::watchAnim("playeridle", &Player::anim);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        //camMS.y = -camerams*10;
    }
    if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        camMS.y = 0.0f;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        Player::velocity.y = Player::jumpms;
        AnimManager::watchAnim("playerjump", &Player::anim);
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        Player::velocity.y = Player::ms;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        Player::velocity.y = 0.0f;
    }

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        Player::editLayer = Layers::getLayer("blocks");
    }
    if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
        Player::editLayer = Layers::getLayer("bg");
    }
    if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
        Player::editLayer = Layers::getLayer("decor");
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Inventory::shown = !Inventory::shown;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        glm::vec2 c = mouseBlockCoords();
        BlockM::lights.push_back(glm::vec3(c.x, c.y, 2.0f));
    }

    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        if (Inventory::blocks[Player::invBlock].item != "invempty") {
            glm::vec2 vel = glm::normalize(mouseBlockCoords() - Player::pos);
            DroppedItems::dropItem(Player::center.x, Player::center.y, Inventory::blocks[Player::invBlock].item, false, {vel.x, vel.y});
            Inventory::remove(Player::invBlock, 1);
            Inventory::refreshCrafting();
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        Player::invBlock = 0;
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        Player::invBlock = 1;
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        Player::invBlock = 2;
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        Player::invBlock = 3;
    }    
    if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
        Player::editsize++;
    }    
    if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
        Player::editsize--;
    }

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (!Inventory::shown) {
        Player::invBlock -= yoffset;
    }
    else {
        Player::invBlock -= yoffset;
    }

}

void mousebutton_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Player::digging = true;

        double ax, ay;
        glfwGetCursorPos(window, &ax, &ay);
        glm::vec2 m = mouseBlockCoords();
        Layers::Layer* blocks = Layers::getLayer("blocks");
        float currL = 0.0f;
        currL += Layers::queryVert(blocks, m.x - 1, m.y).light;
        currL += Layers::queryVert(blocks, m.x + 1, m.y).light;
        currL += Layers::queryVert(blocks, m.x, m.y - 1).light;
        currL += Layers::queryVert(blocks, m.x, m.y + 1).light;
        
        ax /= resX;
        ay /= resY;

        Inventory::mouseClicked(ax, 1-ay);
        AnimManager::watchAnim("playeredit", &Player::anim);

    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        Player::digging = false;
        AnimManager::watchAnim("playeridle", &Player::anim);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        Player::placing = true;
        AnimManager::watchAnim("playeredit", &Player::anim);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        Player::placing = false;
        AnimManager::watchAnim("playeridle", &Player::anim);
    }
}

void updateCam() {
    static glm::vec2 lastPos = glm::vec2(0.0f);

    Layers::Layer* blocks = Layers::getLayer("blocks");

    glm::vec2 pC = Player::pos;
    camMS = ((-pC) - camPos) / (1 / camerams);

    camPos += camMS;

    glm::vec2 c = -camBlockCoords();
    if (c.x < (0 + Layers::blocksOnScreen.x/2)) {
        camPos.x = -(Layers:: blocksOnScreen.x / 2);
    }
    if (c.x > Layers::mapSize.x - Layers::blocksOnScreen.x / 2) {
        camPos.x = - Layers::mapSize.x + Layers::blocksOnScreen.x / 2;
    }
    if (c.y < 0 + Layers::blocksOnScreen.y / 2) {
        camPos.y = - Layers::blocksOnScreen.y / 2;
    }
    if (c.y > Layers::mapSize.y - Layers::blocksOnScreen.y / 2) {
        camPos.y = - Layers::mapSize.y + Layers::blocksOnScreen.y / 2;
    }

    camTrans = glm::translate(camTrans, glm::vec3(camPos - lastPos, 0.0f));
    if (lastPos == camPos) {
        hasUpdated = false;
    }
    else {
        hasUpdated = true;
    }
    lastPos = camPos;
}

void renderLayer(Layers::Layer* l) {

    glUseProgram(blockShaderID);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, BlockM::spriteSheet);
    glUniform1i(1, BlockM::spriteSheet);
    
    glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(camTrans));
    
    glUniform1f(2, l->mdarkness);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, broken1Tex);
    glUniform1i(6, 1);

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, broken2Tex);
    glUniform1i(7, 2);

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, broken3Tex);
    glUniform1i(8, 3);

    int offY = 0;

    //not drawing offscreen quads
    glm::vec2 camC = camBlockCoords();
    camC.x = -(camC.x + Layers::blocksOnScreen.x / 2) - l->mdrawingOffset;
    camC.y = -(camC.y + Layers::blocksOnScreen.y / 2);
    if (camC.x < 0) {
        camC.x = 0;
    }

    int index = (int)camC.x * Layers::mapSize.y * 4.0f + (int)camC.y * 4.0f;

    glBindBuffer(GL_ARRAY_BUFFER, l->mlayerVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BlockVertex) * (Layers::drawnBlocks.x + 10) * Layers::mapSize.y * 4, l->mvertices + index);

    glBindVertexArray(l->mlayerVA);
    glDrawElements(GL_TRIANGLES, Layers::nDrawnIndices, GL_UNSIGNED_INT, nullptr);
}

void drawOverlays()
{
    //display
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(displayShaderID);
    glBindVertexArray(overlayVA);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mainFBT);
    glUniform1i(5, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void updateMap() {

    double ctime = glfwGetTime();
    static double lastdig = 0;
    static double lastplace = 0;
    float digspeed = BlockM::info[Inventory::blocks[Player::invBlock].item].digstrength;
    float placespeed = BlockM::info[Inventory::blocks[Player::invBlock].item].placespeed;

    if (!Inventory::shown) {
        glm::vec2 coords = mouseBlockCoords();
        float dist = sqrt(pow(coords.x - Player::pos.x, 2) + pow(coords.y - Player::pos.y, 2));
        if (ctime - lastdig > 1 / digspeed) {
            if (dist < Player::editRange) {
                if (Player::digging) {
                    Layers::damageBlock(Player::editLayer, coords.x, coords.y, Player::editsize);
                    hasUpdated = true;
                }
            }
            lastdig = ctime;
        }
        
        if (ctime - lastplace > 1 / placespeed) {
            if (Player::placing) {
                if (BlockM::info[Inventory::blocks[Player::invBlock].item].placeable) {
                    if (Layers::placeBlock(coords.x, coords.y, Inventory::blocks[Player::invBlock].item, 0.0f, Player::editsize, true, BlockM::info[Inventory::blocks[Player::invBlock].item].light))
                        Inventory::remove(Player::invBlock, 1);
                    hasUpdated = true;
                }
            }
            lastplace = ctime;
        }
    }
}

glm::vec2 camBlockCoords(glm::vec2 offset) {
    float x, y;

    x = (camPos.x) + offset.x;
    y = (camPos.y) + offset.y;

    return glm::vec2(x, y);
}

glm::vec2 mouseBlockCoords() {

    double x, y;
    glfwGetCursorPos(window, &x, &y);

    y = resY - y;

    //mouse position from 0 to 1
    x = x / resX;
    y = y / resY;

    //clamp?
    x = glm::clamp((float)x, 0.0f, 1.0f);
    y = glm::clamp((float)y, 0.0f, 1.0f);

    //find out which block on screen we are
    x = Layers::blocksOnScreen.x * x;
    y = Layers::blocksOnScreen.y * y;

    glm::vec2 camC = camBlockCoords();

    x -= Layers::blocksOnScreen.x / 2 + camC.x;
    y -= Layers::blocksOnScreen.y / 2 + camC.y;


    return glm::vec2(x, y);
}
