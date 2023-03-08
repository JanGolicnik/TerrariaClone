#include "world.h"

#include <thread>

#include <layers.h>
#include <noise.h>
#include <blocks.h>
#include <startMenu.h>
#include <items.h>

namespace map {
    std::string name;
    std::string tmpname;

    int seed = 1;

    int mapX = 1000;
    int mapY = 1000;

    glm::vec2 PlayerSpawn = glm::vec2(0,0);

    float surfaceH = mapY * 0.8;
    float surfaceScale = 40;
    extern int surfaceCaveRate = 5000;
    float undergroundH = mapY * 0.7;
    int undergroundDirtRate = 50;
    int cavernHoleRate = 50;
    int cavernDirtRate = 100;
    int undergroundCaveRate = 3000;
    int corruptionHoleRate = 15;
    float holeNum = 10;
    float ironRate = 260;
    float copperRate = 160;
    float silverRate = 360;
    float goldRate = 460;
    float treeRate = 17;
    float underworldH = mapY / 8;

    int worldEvilx1;
    int worldEvilx2;

    int moonphase;

    std::unordered_map<std::string, std::string> blockToWall;

    std::mutex worldGenProgress_mutex;
    std::string worldGenProgress;
    std::thread worldgenthread;

    int guideEnt = -1;
    std::string guideName;
    std::vector<std::string> guidenames;

    int demonaltarsbroken = 0;

    std::array<std::string, 8> moonphases = {
        "moon1",
        "moon2",
        "moon3",
        "moon4",
        "moon5",
        "moon6",
        "moon7",
        "moon8"
    };

    void clear()
    {
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg"); 
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                Layers::fastPlaceBlock({ x, y }, "empty", blocks);
                Layers::fastPlaceBlock({ x, y }, "empty", bg);
            }
        }
    }

    void growBlock(glm::vec2 pos, std::string block, int depth, int chance, std::set<std::string>ignore)
    {
        auto blocks = Layers::getLayer("blocks");
        if (depth == 0) {
            return;
        }
        if (rand() % chance == 0) {
            if (ignore.count(*Layers::queryBlockName(blocks, pos)) >= 1) return;

            Layers::fastPlaceBlock(pos + glm::vec2(0, 0), block);
            Layers::fastPlaceBlock(pos + glm::vec2(0, 1), block);
            Layers::fastPlaceBlock(pos + glm::vec2(0, -1), block);
            Layers::fastPlaceBlock(pos + glm::vec2(1, 0), block);
            Layers::fastPlaceBlock(pos + glm::vec2(-1, 0), block);
            growBlock(pos + glm::vec2(0, 1), block, depth - 1, chance);
            growBlock(pos + glm::vec2(0, -1), block, depth - 1, chance);
            growBlock(pos + glm::vec2(1, 0), block, depth - 1, chance);
            growBlock(pos + glm::vec2(-1, 0), block, depth - 1, chance);
        }
    }

    void generateTest()
	{
        PlayerSpawn = glm::vec2(mapX / 2, mapY / 2);
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");

        clear();

        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                if (y <= 20) {
                    if (rand() % 4 == 0) {
                        Layers::placeBlock({ x, y }, "stone");
                    }else
                    Layers::placeBlock({ x, y }, "dirt");
                }
                if (y <= 23) {
                    Layers::placeBlock({ x, y }, "dirtwall");
                }
            }
        }

        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                Layers::autoSprite(blocks, { x,y });
            }
        }

        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                Layers::setLight(blocks, { x, y }, glm::vec3(0));
                Layers::setLight(bg, { x, y }, glm::vec3(0));
            }
        }
	}

	void generategame()
	{
        surfaceH = mapY * 0.8;
        undergroundH = mapY * 0.7;
        underworldH = mapY / 8;

        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");
        clear();
        
        std::vector<float> height = noise::generate(0, 1, surfaceScale, seed, mapX / 30);
        std::vector<glm::vec2> caves;

        //najprej nafilamo do surfaca v ravni liniji z stonom polpa z noisom nardimo hribe

        modifyWorldProgress("adding base");
        for (int x = 0; x < mapX; x++) {
            for (int y = underworldH; y < surfaceH - surfaceScale; y++) {
                Layers::fastPlaceBlock({ x,y }, "stone");
                Layers::fastPlaceBlock({ x,y }, "stonewall");
            }
        }

        for (int x = 0; x < mapX; x++) {
            float y2 = surfaceScale * noise::sampleCosine((float)x / mapX, &height) / 2;
            bool placewall = false;
            for (int y = surfaceScale; y > -surfaceScale - rand() % 4 - 1; y--) {
                if (y < y2) {
                    Layers::fastPlaceBlock({ x,y + surfaceH}, "dirt");
                    if(placewall)Layers::fastPlaceBlock({ x,y + surfaceH }, "dirtwall");
                    placewall = true;
                }
            }
        }

        modifyWorldProgress("adding patches");
        for (int x = 0; x < mapX; x++) {
            for (int y = undergroundH; y < surfaceH - surfaceScale; y++) {
                if (rand() % (int)undergroundDirtRate == 0) {
                    growBlock({ x,y }, "dirt", 6, 4);
                }
            }
        }

        for (int x = 0; x < mapX; x++) {
            for (int y = underworldH; y < surfaceH - surfaceScale; y++) {
                if (rand() % (int)cavernHoleRate == 0) {
                    growBlock({ x,y }, "empty", 6, 4);
                }
                else if (rand() % (int)cavernDirtRate == 0) {
                    growBlock({ x,y }, "dirt", 6, 4);
                }
            }
        }

        int junglesnowdesert = rand() % 3;
        bool left = rand() % 2;

        modifyWorldProgress("doing a little trolling");
        if (rand() % 2) {
            makeCorruption(left);
        }
        else {
            makeCorruption(left);
            //makeCrimson(left);
        }

        modifyWorldProgress("adding caves");
        for (int x = 0; x < mapX; x++) {
            for (int y = underworldH; y < surfaceH + surfaceScale; y++) {
                int rate = surfaceCaveRate;
                if (x < worldEvilx2 && x > worldEvilx1) break;
                if (y < undergroundH) rate = undergroundCaveRate;
                if (rand() % rate == 0) {
                    caves.push_back({ x,y });
                }
            }
        }

        for (int i = 0; i < caves.size(); i++) {
            int nChildren = rand() % 15 + 15;
            int size = rand() % 4 + 7;

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
                    Layers::breakBlock(blocks, { tx, ty }, size);
                }
                prevPos = glm::vec3(x, y, angle);
            }
        }

        switch (junglesnowdesert) {
        case 2:
            makeJungle(left);
            break;
        case 1:
            makeSnow(left);
            break;
        default:
            makeDesert(left);
        }

        modifyWorldProgress("growing ores");
        for (int x = 0; x < mapX; x++) {
            for (int y = underworldH; y < surfaceH; y++) {
                if (*Layers::queryBlockName(blocks, { x,y }) == "stone") {
                    if (rand() % (int)ironRate == 0) {
                        growBlock({ x,y }, "ironore", 6, 4);
                    }
                    else if (rand() % (int)copperRate == 0) {
                        growBlock({ x,y }, "copperore", 6, 4);
                    }
                    else if (rand() % (int)silverRate == 0) {
                        growBlock({ x,y }, "goldore", 6, 4);
                    }
                    else if (rand() % (int)goldRate == 0) {
                        growBlock({ x,y }, "silverore", 6, 4);
                    }
                }
            }
        }

        modifyWorldProgress("creating hell :D");
        makeHell();


        modifyWorldProgress("growing grass");
        for (int x = 0; x < mapX; x++) {
            for (int y = underworldH; y < mapY; y++) {
                if (*Layers::queryBlockName(blocks, { x,y }) == "dirt" && mapY > surfaceH - surfaceScale + rand() % 10) {
                    if (*Layers::queryBlockName(blocks, { x,y + 1 }) == "empty" ||
                        *Layers::queryBlockName(blocks, { x,y }) == "empty" ||
                        *Layers::queryBlockName(blocks, { x + 1,y }) == "empty" ||
                        *Layers::queryBlockName(blocks, { x - 1,y }) == "empty") {
                        if (x > worldEvilx1 && x < worldEvilx2) {
                            Layers::fastPlaceBlock({ x,y }, "corruptgrass");
                        }
                        else {
                            Layers::fastPlaceBlock({ x,y }, "grass");
                        }
                    }
                }
                else
                    if (*Layers::queryBlockName(blocks, { x,y }) == "mud") {
                        if (*Layers::queryBlockName(blocks, { x,y + 1 }) == "empty" ||
                            *Layers::queryBlockName(blocks, { x,y }) == "empty" ||
                            *Layers::queryBlockName(blocks, { x + 1,y }) == "empty" ||
                            *Layers::queryBlockName(blocks, { x - 1,y }) == "empty") {

                            Layers::fastPlaceBlock({ x,y }, "junglegrass");
                        }
                    }
            }
        }

        modifyWorldProgress("growing tress");
        for (int x = 0; x < mapX; x++) {
            for (int y = surfaceH; y < surfaceH + surfaceScale; y++) {
                if (*Layers::queryBlockName(blocks, { x,y }) == "grass" && *Layers::queryBlockName(blocks, { x,y + 1 }) == "empty") {
                    if (rand() % (int)treeRate == 0) {
                        BRules::growNormalTree(nullptr, {x,y + 1});
                    }
                }
                if (*Layers::queryBlockName(blocks, { x,y }) == "corruptgrass" && *Layers::queryBlockName(blocks, { x,y + 1 }) == "empty") {
                    if (rand() % (int)treeRate == 0) {
                        BRules::growCorruptTree(nullptr, { x,y + 1 });
                    }
                }
                if (*Layers::queryBlockName(blocks, { x,y }) == "snow" && *Layers::queryBlockName(blocks, { x,y + 1 }) == "empty") {
                    if (rand() % (int)treeRate == 0) {
                        BRules::growWinterTree(nullptr, { x,y + 1 });
                    }
                }
                if (*Layers::queryBlockName(blocks, { x,y }) == "junglegrass" && *Layers::queryBlockName(blocks, { x,y + 1 }) == "empty") {
                    if (rand() % (int)treeRate == 0) {
                        BRules::growJungleTree(nullptr, { x,y + 1 });
                    }
                }
            }
        }

        modifyWorldProgress("adding realism :0");
        decorate();

        modifyWorldProgress("setting spawn");
        for (int y = mapY; y > 0; y--) {
            if (*Layers::queryBlockName(blocks, { mapX / 2, y }) != "empty") {
                PlayerSpawn = { mapX / 2, y + 2 };
                break;
            }
        }

        modifyWorldProgress("generating structures");
        genStructures();

        modifyWorldProgress("beautifying");
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                Layers::autoSprite(blocks, { x,y });
                Layers::autoSprite(bg, { x,y });
                Layers::setLight(blocks, { x,y }, glm::vec3(0));
                Layers::setLight(bg, { x,y }, glm::vec3(0));
            }
        }
        modifyWorldProgress("THE ONE PIECE IS REAL");
        placeChests();
        placePots();

        guideName = guidenames[rand() % guidenames.size()];

        modifyWorldProgress("finished");
	}

    void createWorld()
    {
        Layers::setUp();
        globals::cdayTime = 450;
        demonaltarsbroken = 0;
        moonphase = 0;
        generategame();
        save();
        Layers::clean();
    }

    bool save()
    {
        std::string filename = "worlds/" + tmpname + ".bak";
        remove(filename.c_str());
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening file for saving\n"; return false; }

        file.write((char*)&map::PlayerSpawn, sizeof(glm::vec2));
        file.write((char*)&map::moonphase, sizeof(map::moonphase));
        file.write((char*)&globals::cdayTime, sizeof(globals::cdayTime));
        file.write((char*)&demonaltarsbroken, sizeof(demonaltarsbroken));

        file.write((char*)&map::mapX, sizeof(int));
        file.write((char*)&map::mapY, sizeof(int));

        int guideNameSize = guideName.size();
        file.write((char*)&guideNameSize, sizeof(guideNameSize));
        if (guideName != "") {
            file.write(guideName.c_str(), guideNameSize);
        }

        Layers::childParent->save(&file);

        Layers::saveChests(&file);

        for (auto& layer : Layers::layers) {
            for (int i = 0; i < map::mapX * map::mapY; i++) {
                file.write((char*)&layer.mblocks[i], sizeof(Block));
            }
        }
        file.close();
        return true;
    }
    bool load()
    {
        std::string filename = "worlds/" + tmpname + ".bak";
        std::ifstream file(filename, std::ios::out | std::ios::binary);
        if (!file) { std::cout << "error opening file for laoding\n"; return false; }

        file.read((char*)&map::PlayerSpawn, sizeof(glm::vec2));
        file.read((char*)&map::moonphase, sizeof(map::moonphase));
        file.read((char*)&globals::cdayTime, sizeof(globals::cdayTime));
        file.read((char*)&demonaltarsbroken, sizeof(demonaltarsbroken));

        file.read((char*)&map::mapX, sizeof(int));
        file.read((char*)&map::mapY, sizeof(int));

        Layers::setUp();

        int guideNameSize;
        file.read((char*)(&guideNameSize), sizeof(guideNameSize));
        if (guideNameSize > 0) {
            std::string GUIDENAME;
            char c;
            for (int i = 0; i < guideNameSize; i++) {
                file.get(c);
                GUIDENAME += c;
            }
            guideName = GUIDENAME;
        }
       
        Layers::childParent->load(&file);

        Layers::loadChests(&file);

        for (auto& layer : Layers::layers) {
            for (int i = 0; i < map::mapX * map::mapY; i++) {
                file.read((char*)&layer.mblocks[i], sizeof(Block));
            }
        }
        file.close();
        return true;
    }
    void makeDesert(bool left)
    {
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");
        modifyWorldProgress("placing desert");
        std::vector<glm::vec2> holes;
        int X1 = mapX * 0.25 - rand() % (mapX / 20);
        if (!left) {
            X1 += mapX * 0.4;
        }
        int X2 = X1 + mapX * 0.3 - rand() % (mapX / 20);

        glm::vec2 r = glm::vec2( abs(X2 - X1) / 2.0f, (abs(X2 - X1) / 2.0f) * 1.3);
        glm::vec2 mid = glm::vec2(X1 + r.x, surfaceH + surfaceScale * 0.7 - r.y);

        for (int y = surfaceH + surfaceScale; y > 0; y--) {
            for (int x = X1; x < X2 ; x++) {
                if (((x - mid.x) * (x - mid.x) * r.y * r.y + (y - mid.y) * (y - mid.y) * r.x * r.x) <= (r.x * r.x * r.y * r.y)) {
                    Layers::breakBlock(bg, { x,y });
                    Layers::fastPlaceBlock({ x,y }, "sand");
               }
            }
        }

        for (int y = surfaceH + surfaceScale; y > 0; y--) {
            for (int x = X1; x < X2; x++) {
                if (*Layers::queryBlockName(blocks, { x,y }) != "sand") continue;
                if (Layers::queryBlockInfo(blocks, { x,y + 1 })->notReplacable) continue;
                if (rand() % 30 != 0) continue;
                
                placeCactus({ x,y + 1 });
            }
        }

        mid.y -= mapY * 0.03;
        for (int y = surfaceH + surfaceScale; y > 0; y--) {
            for (int x = X1; x < X2; x++) {
                float dist = ((x - mid.x) * (x - mid.x) * r.y * r.y + (y - mid.y) * (y - mid.y) * r.x * r.x);
                if (dist <= (r.x * r.x * r.y * r.y)) {
                    Layers::breakBlock(bg,{x,y});
                    Layers::fastPlaceBlock({ x,y }, "hardenedsandwall");
                    if (rand() % 5 == 0) {
                        Layers::fastPlaceBlock({ x,y }, "sand");
                    }
                    else {
                        Layers::fastPlaceBlock({ x,y }, "hardenedsand");
                    }
                    if(dist < (r.x * r.x * r.y * r.y) * 0.9)
                    if (rand() % 10 == 0) {
                        holes.push_back({ x,y });
                    }
                }
            }
        }

        for (auto& hole : holes) {
            growBlock(hole, "sandstone", 12, 3, { "dirt" });
            growBlock(hole, "sandstonewall", 14, 3, { });
            growBlock(hole, "empty", 12, 3, {"hardenedsand"});
        }

        int width = r.x * 0.2f;
        int y = 0;
        while (width > 0) {
            for (int x = -width / 2; x < width / 2; x++) {
                Layers::breakBlock(blocks, { int(mid.x + x), int(surfaceH + surfaceScale - y )});
            }
            if (rand() % 4 == 0) width--;
            y++;
        }


    }
    void makeSnow(bool left)
    {
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");
        modifyWorldProgress("placing snow");
        int snowX1 = (2 * mapX) / 7 - rand() % (mapX / 20);
        if (!left) {
            snowX1 += (3 * mapX) / 7;
        }
        int snowX2 = snowX1 + mapX / 5 - rand() % (mapX / 20);
        int offset = 0;
        for (int y = surfaceH + surfaceScale; y > mapY / 2; y--) {
            for (int x = snowX1 - offset; x < snowX2 + offset; x++) {
                auto b = Layers::queryBlockName(blocks, { x,y });
                if (*b == "dirt" || *b == "stone") {
                    Layers::fastPlaceBlock({ x,y }, "snow");
                }
                b = Layers::queryBlockName(bg, { x,y });
                if (*b == "dirtwall" || *b == "stonewall") {
                    Layers::fastPlaceBlock({ x,y }, "snowwall");
                }
            }
            offset += rand() % 3;
        }

    }
    void makeJungle(bool left)
    {
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");
        //dzunglo si zberemo eno petino surfaca k bo jungla in nardimo tko kvadrat dol do polovice sveta pol pa postavmo par krogov kso dost velki nekje tm okol pod zemlo
        modifyWorldProgress("placing mud");
        int jungleX1 = (2 * mapX) / 7 - rand() % (mapX / 20);
        if (!left) {
            jungleX1 += (3 * mapX) / 7;
        }
        int jungleX2 = jungleX1 + mapX / 5 - rand() % (mapX / 20);
        int buffer = 30;
        for (int x = jungleX1 - buffer; x < jungleX2 + buffer; x++) {
            for (int y = mapY / 2; y < mapY; y++) {
                if ((x - jungleX1) < 0) {
                    if (rand() % (x - jungleX1) != 0) {
                        continue;
                    }
                }
                if ((x - jungleX2) > 0) {
                    if (rand() % (x - jungleX2) != 0) {
                        continue;
                    }
                }
                auto b = Layers::queryBlockName(blocks, { x,y });
                if (*b == "dirt" || *b == "stone") {
                    Layers::fastPlaceBlock({ x,y }, "mud");
                }
                b = Layers::queryBlockName(bg, { x,y });
                if (*b == "dirtwall" || *b == "stonewall") {
                    Layers::fastPlaceBlock({ x,y }, "mudwall");
                }
            }
        }

        for (int i = 0; i < 5; i++) {
            int x2 = rand() % (jungleX2 - jungleX1) + jungleX1;
            int y2 = rand() % (int)(mapY / 2 - surfaceH) + mapY / 2;
            int width = rand() % 50 + 50;
            for (int x = x2 - width; x < x2 + width; x++) {
                for (int y = y2 - width; y < y2 + width; y++) {
                    if (glm::distance(glm::vec2(x, y), glm::vec2(x2, y2)) < width) {
                        auto b = Layers::queryBlockName(blocks, { x,y });
                        if (*b == "dirt" || *b == "stone") {
                            Layers::fastPlaceBlock({ x,y }, "mud");
                        }
                        b = Layers::queryBlockName(bg, { x,y });
                        if (*b == "dirtwall" || *b == "stonewall") {
                            Layers::fastPlaceBlock({ x,y }, "mudwall");
                        }
                    }
                }
            }
        }
    }
    void makeCorruption(bool left)
    {
        Layer* blocks = Layers::getLayer("blocks");
        Layer* bg = Layers::getLayer("bg");

        std::vector<glm::vec2>caves;

        worldEvilx1 = (5 * mapX) / 7 - rand() % (mapX / 20);
        if (!left) {
            worldEvilx1 -= (2 * mapX) / 7;
        }
        worldEvilx2 = worldEvilx1 + mapX / 5 - rand() % (mapX / 20);

        //vodoravno postavmo
        int y = surfaceH - 35 + rand() % 10;
        for (int x = worldEvilx1; x < worldEvilx2; x++) {
            int y2 = y + rand() % 6 - 3;
            Layers::fastPlaceBlock({ x,y2 }, "ebonstone", nullptr, rand() % 8 + 6);
            Layers::fastPlaceBlock({ x,y2 }, "ebonstonewall", nullptr, rand() % 8 + 6);
        }


        //une navzdol
        for (int x = worldEvilx1; x < worldEvilx2; x++) {
            for (int y = surfaceH + surfaceScale; y > surfaceH - surfaceScale; y--) {
                if (Layers::queryBlock(blocks, { x,y })->id != globals::emptyid) {
                    if (rand() % corruptionHoleRate == 0) {
                        caves.push_back({ x,y });
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < caves.size(); i++) {
            int nChildren = rand() % 15 + 15;
            float sizemod = (rand() % 5 + 10 )/ 10.0f;
            int size = rand() % 4 + 7;
            int length = rand() % 40 + 50;
            int x = caves[i].x;
            int maxy = caves[i].y - length;
            for (int y = caves[i].y-4; y > maxy; y--) {
                size = (rand() % 3 + 7) * sizemod;
                Layers::placeBlock({ x, y }, "ebonstone", { 0, 0, 0 }, size * 1.5);
                Layers::placeBlock({ x, y }, "ebonstonewall", { 0, 0, 0 }, size*1.5);
            }
            x = caves[i].x;

            for (int y = caves[i].y + 10; y > caves[i].y - length; y--) {
                size = (rand() % 4 + 3) * sizemod;
                if (y == caves[i].y + 10) {
                    size += 6;
                }
                Layers::breakBlock(blocks, { x,y }, size);
            }
        }

        //vodoravno nardimo lukno
        for (int x = worldEvilx1 + 4; x < worldEvilx2 - 4; x++) {
            int y2 = y + rand() % 6 - 3;
            Layers::breakBlock(blocks, { x,y2 }, rand() % 4 + 5);
        }

    }
    void makeCrimson(bool left)
    {
    }
    void modifyWorldProgress(std::string progress)
    {
        std::lock_guard<std::mutex> lock(worldGenProgress_mutex);
        worldGenProgress = progress;
    }
    void decorate()
    {
        modifyWorldProgress("decorating");
        auto bl = Layers::getLayer("blocks");
        auto bg = Layers::getLayer("bg");
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                auto b = Layers::queryBlockName(bl, { x,y });
                for (auto& d : blocks::decorations) {
                    if (rand() % (int)ceil(1 / d.chance) == 0) {
                        if (d.onbot.count(*b) >= 1) {
                            Layers::placeBlock({ x, y + 1 }, d.block, { 0,0,0 }, 1, &blocks::nameToInfo[d.block].conditions);
                        }
                        if (d.ontop.count(*b) >= 1) {
                            Layers::placeBlock({ x, y - d.size.y }, d.block, { 0,0,0 }, 1, &blocks::nameToInfo[d.block].conditions);
                        }
                        if (d.onright.count(*b) >= 1) {
                            Layers::placeBlock({ x - d.size.x, y }, d.block, { 0,0,0 }, 1, &blocks::nameToInfo[d.block].conditions);
                        }
                        if (d.onleft.count(*b) >= 1) {
                            Layers::placeBlock({ x + 1, y }, d.block, { 0,0,0 }, 1, &blocks::nameToInfo[d.block].conditions);
                        }
                    }
                }
            }
        }
    }
    void placeChests()
    {
        auto bs = Layers::getLayer("blocks");
        auto bg = Layers::getLayer("bg");
        std::vector<std::function<bool(BlockConditionArgs)>> conditions = blocks::nameToInfo["chest"].conditions;
        conditions.push_back(BConditions::isntreplacablebelow);
        conditions.push_back(BConditions::haswall);
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                auto block = Layers::queryBlockName(bs, { x,y });
                if (rand() % 60 == 0) {
                    if (*block == "stone" && y < undergroundH) {
                        if (Layers::placeBlock(bs, { x,y + 1 }, "chest", { 0,0,0 }, 1, &conditions)) {
                            fillChest({ x,y+1 }, "underground");
                        }
                    }else
                    if (*block == "grass" || *block == "dirt") {
                        if (Layers::placeBlock(bs, { x,y + 1 }, "chest", { 0,0,0 }, 1, &conditions)) {
                            fillChest({ x,y+1}, "surface");
                        }
                    }
                }
                
            }
        }
    }
    void placePots()
    {
        auto bs = Layers::getLayer("blocks");
        std::vector<std::function<bool(BlockConditionArgs)>> conditions = blocks::nameToInfo["chest"].conditions;
        conditions.push_back(BConditions::haswall);
        conditions.push_back(BConditions::isntreplacablebelow);
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < mapY; y++) {
                auto info = Layers::queryBlockInfo(bs, { x,y });
                if (!info->collidableTop) continue;
                if (rand() % 20 == 0) {
                    Layers::placeBlock(bs, { x,y + 1 }, "pot", { 0,0,0 }, 1, &conditions);
                }

            }
        }
    }
    void fillChest(glm::vec2 pos, std::string chestName)
    {
        int c = Layers::vecToInt(pos);
        if (Layers::chests.count(c) >= 1) {
            int item = 0;
            int maxitems = 5 + rand()% 4;
            for (auto& i : items::naturalChests[chestName].items) {
                if (Layers::chests[c].items.size() <= item) return;
                if (item == maxitems) return;
                if (rand() % int((1.0f / i.second.chance)) == 0) {
                    Layers::chests[c].items[item]->item = i.first;
                    Layers::chests[c].items[item]->num = i.second.num + (rand()%1000 - 500) / 500.0f;
                    item++;
                }
            }
        }
    }
    void genStructures()
    {
        auto bs = Layers::getLayer("blocks");
        for(int x = 0; x < mapX; x++){
            for (int y = 0; y < mapY; y++) {
                if (y < undergroundH && y > underworldH) {
                    if (Layers::queryBlock(bs, { x,y })->id == globals::emptyid) {
                        if (rand() % 10000 == 0) {
                            addUndergroundHouse({ x,y });
                        }
                    }
                }
            }
        }
    }
    void addUndergroundHouse(glm::vec2 pos)
    {
        auto bs = Layers::getLayer("blocks");
        int numRooms = rand() % 3 + 1;
        int base = pos.y;
        for (int i = 0; i < numRooms; i++) {
            int height = rand()%5 + 6;
            int width = rand()% 5 + 2 + height;
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    glm::vec2 pos2 = glm::vec2(pos.x + x, y + base);
                    Layers::breakBlock(bs, pos2);
                    if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                        Layers::fastPlaceBlock(pos2, "wood");
                    }
                    else {
                        Layers::fastPlaceBlock(pos2, "woodwall");
                        if (y == height / 2) {
                            if (rand() % 12 == 0) {
                                Layers::fastPlaceBlock(pos2, "torch");
                            }
                        }
                    }
                }
            }
            for (int x = 0; x < width; x++) {
                glm::vec2 pos2 = pos + glm::vec2(x, 1);
                if (rand() % 25 == 0) {
                    if (Layers::placeBlock(pos2, "chest", { 0,0,0 }, 1, &blocks::nameToInfo["chest"].conditions)) {
                        fillChest(pos2, "undergroundhouse");
                    }
                }
                else if (rand() % 25 == 0) {
                    Layers::placeBlock(pos2, "anvil", { 0,0,0 }, 1, &blocks::nameToInfo["anvil"].conditions);
                }
                else if (rand() % 25 == 0) {
                    Layers::placeBlock(pos2, "craftingbench", { 0,0,0 }, 1, &blocks::nameToInfo["workbench"].conditions);
                }
                else if (rand() % 25 == 0) {
                    Layers::placeBlock(pos2, "table", { 0,0,0 }, 1, &blocks::nameToInfo["table"].conditions);
                }
            }
            if (rand() % 5 != 0) {
                if (rand() % 2 == 0) {
                    Layers::breakBlock(bs, pos + glm::vec2(width - 1, 1));
                    Layers::breakBlock(bs, pos + glm::vec2(width - 1, 2));
                    Layers::breakBlock(bs, pos + glm::vec2(width - 1, 3));
                    Layers::placeBlock(pos + glm::vec2(width - 1, 1), "door");
                }
                else {
                    Layers::breakBlock(bs, pos + glm::vec2(0, 1));
                    Layers::breakBlock(bs, pos + glm::vec2(0, 2));
                    Layers::breakBlock(bs, pos + glm::vec2(0, 3));
                    Layers::placeBlock(pos + glm::vec2(0, 1), "door");
                }
            }
            base += height;
        }
    }
    void placeCactus(glm::vec2 pos)
    {
        int height = rand() % 4 + 3;
        bool hasleftarm = false;
        bool hasrightarm = false;
        for (int i = 0; i < height; i++) {
            if (!hasleftarm) {
                if (rand() % 4 == 0) {
                    int armheight = rand() % 3 + 1;
                    Layers::placeBlock(pos + glm::vec2(0, i), "cactusleft");
                    Layers::placeBlock(pos + glm::vec2(-1, i), "cactusleftarm");
                    for (int j = 1; j < armheight; j++) {
                        Layers::placeBlock(pos + glm::vec2(-1, i+j), "cactusarmleftmid");
                    }
                    Layers::placeBlock(pos + glm::vec2(-1, i + armheight), "cactusarmlefttop");
                    hasleftarm = true;
                    continue;
                }
            }
            if (!hasrightarm) {
                if (rand() % 4 == 0) {
                    int armheight = rand() % 3 + 1;
                    Layers::placeBlock(pos + glm::vec2(0, i), "cactusright");
                    Layers::placeBlock(pos + glm::vec2(1, i), "cactusrightarm");
                    for (int j = 1; j < armheight; j++) {
                        Layers::placeBlock(pos + glm::vec2(1, i + j), "cactusarmrightmid");
                    }
                    Layers::placeBlock(pos + glm::vec2(1, i + armheight), "cactusarmrighttop");
                    hasrightarm = true;
                    continue;
                }
            }
            Layers::placeBlock(pos + glm::vec2(0, i), "cactusmid");
        }
        Layers::placeBlock(pos + glm::vec2(0, height), "cactustop");
    }
    void placeRandomCactus()
    {
        auto bs = Layers::getLayer("blocks");
        for (int y = undergroundH; y < surfaceH - surfaceScale; y++) {
            for (int x = 0; x < mapX; x++) {
                if (*Layers::queryBlockName(bs, { x,y }) != "sand") continue;
                if (rand() % 200 == 0) {
                    if (Layers::queryBlockInfo(bs, { x,y + 1 })->notReplacable) continue;
                    placeCactus({ x,y + 1 });
                    return;
                }
            }
        }
    }

    void spawnDebugSetup(glm::vec2 pos)
    {
        auto itemit = items::info.begin();

        int counter = 0;

        Layers::placeBlock(pos, "chest");
        Layers::placeBlock(pos + glm::vec2(0, -1), "wood");
        Layers::placeBlock(pos + glm::vec2(1, -1), "wood");
        Chest* currchest = &Layers::chests[Layers::vecToInt(pos)];

        while (itemit != items::info.end()) {
            currchest->items[counter % 40]->item = itemit->first;
            currchest->items[counter % 40]->num = itemit->second.maxStacks;
            if ((counter+1) % 40 == 0) {
                pos.x += 2;
                Layers::placeBlock(pos, "chest");
                Layers::placeBlock(pos + glm::vec2(0, -1), "wood");
                Layers::placeBlock(pos + glm::vec2(1, -1), "wood");
                currchest = &Layers::chests[Layers::vecToInt(pos)];
            }
            counter++;
            itemit++;
        }
    }
    void makeHell()
    {
        auto bs = Layers::getLayer("blocks");

        //celo nafilamo
        for (int x = 0; x < mapX; x++) {
            for (int y = 40; y < underworldH; y++) {
                Layers::fastPlaceBlock({ x,y }, "ash");
            }
        }

        //hilli navrh
        std::vector<float> height = noise::generate(0, 1, 40, seed, mapX / 30);
        for (int x = 0; x < mapX; x++) {
            float y2 = 40 * noise::sampleCosine((float)x / mapX, &height) / 2;
            for (int y = 40; y > -40; y--) {
                if (y < y2) {
                    Layers::fastPlaceBlock({ x,y + underworldH}, "ash");
                }
            }
        }

        //spice zgori
        height = noise::generate(0, 1, 40, seed, mapX / 5);
        for (int x = 0; x < mapX; x++) {
            float y2 = 40 * noise::sampleCosine((float)x / mapX, &height) / 2;
            for (int y = 40; y > -40; y--) {
                if (y < y2) {
                    Layers::breakBlock(bs, { x,y + underworldH* 0.6});
                }
            }
        }

        //spice spodi
        height = noise::generate(0, 1, 40, seed, mapX / 15);
        for (int x = 0; x < mapX; x++) {
            float y2 = 90 * noise::sampleCosine((float)x / mapX, &height) / 2;
            for (int y = 90; y > 0; y--) {
                if (y < y2) {
                    Layers::fastPlaceBlock({ x,y }, "ash");
                }
            }
        }

        //nardimo lukne
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < underworldH; y++) {
                if (*Layers::queryBlockName(bs, { x,y }) == "ash") {
                    if (rand() % 30 == 0) {
                        map::growBlock({ x,y }, "empty", 10, 3);
                    }
                }
            }
        }

        //damo hellstone
        for (int x = 0; x < mapX; x++) {
            for (int y = 0; y < 90; y++) {
                if (*Layers::queryBlockName(bs, { x,y }) == "ash") {
                    if (rand() % 30 == 0) {
                        map::growBlock({ x,y }, "hellstone", 10, 4);
                    }
                }
            }
        }
    
    }
}