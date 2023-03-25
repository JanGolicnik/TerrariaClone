#include "utils.h"

#include <fstream>
#include <blocks.h>

#include <ECS/ECS.h>
#include <componentsystems.h>

#include <textures.h>

std::array<BlockVertex, 4> utils::CreateQuad(float x, float y, glm::vec4 sC, glm::vec3 light, glm::vec2 size)
{

    BlockVertex v0;
    v0.Position = { x - size.x / 2, y - size.y / 2};
    v0.SpriteCoords = { sC.r, sC.g };
    v0.TexCoords = { 0, 0 };
    v0.light = light;

    BlockVertex v1;
    v1.Position = { x + size.x / 2, y - size.y / 2};
    v1.SpriteCoords = { sC.b, sC.g };
    v1.TexCoords = { 1, 0 };
    v1.light = light;

    BlockVertex v2;
    v2.Position = { x + size.x / 2, y + size.y / 2};
    v2.SpriteCoords = { sC.b, sC.a };
    v2.TexCoords = { 1, 1 };
    v2.light = light;

    BlockVertex v3;
    v3.Position = { x - size.x / 2, y + size.x / 2};
    v3.SpriteCoords = { sC.r, sC.a };
    v3.TexCoords = { 0, 1 };
    v3.light = light;

    return { v0, v1, v2, v3 };
}

std::array<SpriteVertex, 4> utils::CreateSpriteRect(glm::vec3 pos, std::string tex, glm::vec2 size, glm::vec4 color, bool flipX, bool flipY, bool autocorrect)
{
    glm::vec4 sC;
    if (textures::nametocoords.count(tex) >= 1) {
        sC = textures::nametocoords.at(tex)->coords;
    }
    else {
        sC = textures::nametocoords.at("dirt")->coords;
    }

    SpriteVertex v0;
    SpriteVertex v2;
    SpriteVertex v1;
    SpriteVertex v3;

    glm::vec2 siz;
    if (autocorrect) {
        siz.x = (sC.b - sC.r);
        siz.y = (sC.a - sC.g);
        if (siz.x > siz.y) {
            siz.y /= siz.x;
            siz.x /= siz.x;
        }
        else {
            siz.x /= siz.y;
            siz.y /= siz.y;
        }
        siz *= size;
    }
    else
    {
        siz = size;
    }
    siz /= glm::vec2(2);
    
    v0.Position = { pos.x - siz.x, pos.y - siz.y, pos.z };
    v1.Position = { pos.x + siz.x, pos.y - siz.y, pos.z };
    v2.Position = { pos.x + siz.x, pos.y + siz.y, pos.z };
    v3.Position = { pos.x - siz.x, pos.y + siz.y, pos.z };
    v0.spritesheet = 0;
    v1.spritesheet = 0;
    v2.spritesheet = 0;
    v3.spritesheet = 0;
    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;
    v0.SpriteCoords = { sC.r, sC.g };
    v1.SpriteCoords = { sC.b, sC.g };
    v2.SpriteCoords = { sC.b, sC.a };
    v3.SpriteCoords = { sC.r, sC.a };

    if (flipX) {
        glm::vec2 tmp = v0.SpriteCoords;
        v0.SpriteCoords = v1.SpriteCoords;
        v1.SpriteCoords = tmp;
        tmp = v2.SpriteCoords;
        v2.SpriteCoords = v3.SpriteCoords;
        v3.SpriteCoords = tmp;
    }
    if (flipY)
    {
        glm::vec2 tmp = v0.SpriteCoords;
        v0.SpriteCoords = v3.SpriteCoords;
        v3.SpriteCoords = tmp;
        tmp = v1.SpriteCoords;
        v1.SpriteCoords = v2.SpriteCoords;
        v2.SpriteCoords = tmp;
    }


    return { v0, v1, v2, v3 };
}

std::array<SpriteVertex, 4> utils::CreateSpriteRect(glm::vec3 pos, glm::vec4 sC, glm::vec2 size, glm::vec4 color, bool flipX, bool flipY, bool autocorrect)
{
    SpriteVertex v0;
    SpriteVertex v2;
    SpriteVertex v1;
    SpriteVertex v3;

    glm::vec2 siz;
    if (autocorrect) {
        siz.x = (sC.b - sC.r);
        siz.y = (sC.a - sC.g);
        if (siz.x > siz.y) {
            siz.y /= siz.x;
            siz.x /= siz.x;
        }
        else {
            siz.x /= siz.y;
            siz.y /= siz.y;
        }
        siz *= size;
    }
    else
    {
        siz = size;
    }
    siz /= glm::vec2(2);

    v0.Position = { pos.x - siz.x, pos.y - siz.y, pos.z };
    v1.Position = { pos.x + siz.x, pos.y - siz.y, pos.z };
    v2.Position = { pos.x + siz.x, pos.y + siz.y, pos.z };
    v3.Position = { pos.x - siz.x, pos.y + siz.y, pos.z };
    v0.spritesheet = 0;
    v1.spritesheet = 0;
    v2.spritesheet = 0;
    v3.spritesheet = 0;
    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;
    v0.SpriteCoords = { sC.r, sC.g };
    v1.SpriteCoords = { sC.b, sC.g };
    v2.SpriteCoords = { sC.b, sC.a };
    v3.SpriteCoords = { sC.r, sC.a };

    if (flipX) {
        glm::vec2 tmp = v0.SpriteCoords;
        v0.SpriteCoords = v1.SpriteCoords;
        v1.SpriteCoords = tmp;
        tmp = v2.SpriteCoords;
        v2.SpriteCoords = v3.SpriteCoords;
        v3.SpriteCoords = tmp;
    }
    if (flipY)
    {
        glm::vec2 tmp = v0.SpriteCoords;
        v0.SpriteCoords = v3.SpriteCoords;
        v3.SpriteCoords = tmp;
        tmp = v1.SpriteCoords;
        v1.SpriteCoords = v2.SpriteCoords;
        v2.SpriteCoords = tmp;
    }

    return { v0, v1, v2, v3 };
}

std::array<SpriteVertex, 4> utils::CreateFontRect(glm::vec3 pos, glm::vec4 sC, glm::vec2 size, glm::vec4 color, bool flipX, bool flipY)
{
    SpriteVertex v0;
    SpriteVertex v2;
    SpriteVertex v1;
    SpriteVertex v3;

    glm::vec2 siz = size;

    v0.Position = { pos.x, pos.y, pos.z };
    v1.Position = { pos.x + siz.x, pos.y, pos.z };
    v2.Position = { pos.x + siz.x, pos.y + siz.y, pos.z };
    v3.Position = { pos.x, pos.y + siz.y, pos.z };

    v0.color = color;
    v1.color = color;
    v2.color = color;
    v3.color = color;
    v0.spritesheet= 1;
    v1.spritesheet = 1;
    v2.spritesheet = 1;
    v3.spritesheet = 1;
    v0.SpriteCoords = { sC.r, sC.g };
    v1.SpriteCoords = { sC.b, sC.g };
    v2.SpriteCoords = { sC.b, sC.a };
    v3.SpriteCoords = { sC.r, sC.a };

    if (flipX) {
    glm::vec2 tmp = v0.SpriteCoords;
    v0.SpriteCoords = v1.SpriteCoords;
    v1.SpriteCoords = tmp;
    tmp = v2.SpriteCoords;
    v2.SpriteCoords = v3.SpriteCoords;
    v3.SpriteCoords = tmp;
    }
    if (flipY)
    {
    glm::vec2 tmp = v0.SpriteCoords;
    v0.SpriteCoords = v3.SpriteCoords;
    v3.SpriteCoords = tmp;
    tmp = v1.SpriteCoords;
    v1.SpriteCoords = v2.SpriteCoords;
    v2.SpriteCoords = tmp;
    }

    return { v0, v1, v2, v3 };
}


GLuint utils::LoadTexture(const char* path, int channels, bool repeat, glm::vec2 *size) {
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* pixels = nullptr;
    if(channels < 0 ||channels > 4) std::cout << "incorrect num of channels loading image " << path << "\n";
    pixels = stbi_load(path, &w, &h, &bits, channels);
    if (pixels == nullptr) {
        std::cout << "error loading image " << path << "\n";
        return -1;
    }
    if (size != nullptr) {
        size->x = w;
        size->y = h;
    }
    GLuint textureID;
    glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    if (repeat) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    switch (channels) {
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        break;
    case 2:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16, w, h, 0, GL_RG, GL_UNSIGNED_BYTE, pixels);
        break;
    case 1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R16, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
        break;
    default:
        std::cout << "incorrect num of channels loading image " << path << "\n";
        break;
    }

    stbi_image_free(pixels);

    return textureID;
}

GLFWimage utils::LoadImage(const char* path, int channels, bool repeat) {
    GLFWimage img;
    stbi_set_flip_vertically_on_load(1);
    if (channels < 1 && channels > 4) std::cout << "incorrect num channels on " << path << "\n";
    img.pixels = stbi_load(path, &img.width, &img.height, &channels, channels);
    if (img.pixels == nullptr) {
        std::cout << "error loading image " << path << "\n";
    }
    return img;
}

void utils::freeImage(stbi_uc* pixels) {
    stbi_image_free(pixels);
}


void utils::createShader(const char* fragPath, const char* vertPath, GLuint* id) {
    const char* fragmentPath = fragPath;
    const char* vertexPath = vertPath;

    std::string l;

    std::string fragstr;

    std::ifstream fragmentFile(fragmentPath);
    if (fragmentFile.is_open()) {
        while (std::getline(fragmentFile, l)) {
            fragstr += l;
            fragstr += "\n";
        }
    }
    else {
        
       std::cout << "error reading fragment shader " << fragmentPath << "\n";
    }

    const char* fragmentShaderSource = fragstr.c_str();

    std::string vertstr;

    std::ifstream vertexFile(vertexPath);
    if (vertexFile.is_open()) {
        while (std::getline(vertexFile, l)) {
            vertstr += l;
            vertstr += "\n";
        }
    }
    else {
        std::cout << "error reading vertex shader " << vertexPath << "\n";
    }

    const char* vertexShaderSource = vertstr.c_str();

    uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
        std::cout << "Error in vertex shader: " << vertexPath << " " << infolog << std::endl;
    }

    uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
        std::cout << "Error in fragment shader: " << fragmentPath << " " << infolog << std::endl;
    }

    *id = glCreateProgram();
    glAttachShader(*id, vertexShader);
    glAttachShader(*id, fragmentShader);
    glLinkProgram(*id);

    glGetProgramiv(*id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*id, 512, NULL, infolog);
        std::cout << "Error in shader program: " << fragmentPath << " " << vertexPath << " " << infolog << std::endl;
    }

    glDetachShader(*id, fragmentShader);
    glDetachShader(*id, vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

void utils::createFrameBuffer(GLuint* framebuffer, GLuint* tex, glm::vec2 res, bool nearest, bool depth)
{
    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D, *tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, res.x, res.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    if (nearest) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }
    else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "\nframebuffer error\n";
    }

    if (depth) {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, res.x, res.y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void utils::createFrameBufferMultisampled (GLuint* framebuffer, GLuint* tex, glm::vec2 res, int samples)
{
    glGenFramebuffers(1, framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, *framebuffer);

    glGenTextures(1, tex);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, *tex);

    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGBA32F, res.x, res.y, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, *tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "\nframebuffer error\n";
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void utils::cout2(glm::vec2 vec)
{
    std::cout << vec.x << " " << vec.y << "\n";
}

double utils::approach(float val1, float val2, float step)
{
    double dif = val2 - val1;
    if (step == 0) {
        return 0;
    }
    return dif / (double)step;
}

glm::vec2 utils::approach(glm::vec2 val1, glm::vec2 val2, float step)
{
    return glm::vec2(approach(val1.x, val2.x, step), approach(val1.y, val2.y, step));
}

glm::vec3 utils::approach(glm::vec3 val1, glm::vec3 val2, float step)
{
    return glm::vec3(approach(val1.x, val2.x, step), approach(val1.y, val2.y, step), approach(val1.z, val2.z, step));
}

void utils::updateSprite(SpriteVertex* verts, glm::mat4 *mat, int nverts, glm::vec2 offset)
{
    float ofx = (verts + 1)->Position.x;
    float ofy = (verts + 2)->Position.y;
    glm::vec3 nf = (*verts).Position + (glm::vec3(ofx-(*verts).Position.x, ofy - (*verts).Position.y, 0.0f)/2.0f) + glm::vec3(offset,0.0f);
    for (int i = 0; i < nverts; i++) {
        (verts + i)->Position -= nf;
        (verts + i)->Position = glm::vec3(*mat * glm::vec4((verts + i)->Position, 1.0f));
        (verts + i)->Position += nf;
    }
}

void utils::rotateSprite(SpriteVertex* verts, float rotation, glm::vec2 rotpoint, int nverts, bool add) {
    auto rotmat = glm::rotate(glm::mat4(1.0f), (float)(rotation * (PI / 180.0f)), glm::vec3(0, 0, 1));

    glm::vec3 nf = (*verts).Position + glm::vec3(rotpoint, 0.0f);
    for (int i = 0; i < nverts; i++) {
        (verts + i)->Position -= nf;
        (verts + i)->Position = glm::vec3(glm::vec4((verts + i)->Position, 1.0f) * rotmat);
        (verts + i)->Position += nf;
    }
}

void utils::moveSprite(SpriteVertex* verts, int nverts, glm::vec2 offset, bool abs) {
    glm::vec2 nf;
    if (abs) {
        nf = offset;
    }
    else {
        nf = glm::vec2((*verts).Position) + offset;
    }
    for (int i = 0; i < nverts; i++) {
        (verts + i)->Position -= (*verts).Position;
        (verts + i)->Position += glm::vec3(nf,0);
    }
}

float utils::angleOfVector(glm::vec2 vec)
{
    glm::vec2 normed = glm::normalize(vec);
    return atan2(normed.y, normed.x) / PI * 180.0f + 180;
}

//float utils::distPointLine(glm::vec2 p, glm::vec2 ls, glm::vec2 le)
//{
//    float num = abs((le.x - ls.x) * (ls.y - p.y) - (ls.x - p.x) * (le.y - ls.y));
//    float den = sqrt((le.x - ls.x) * (le.x - ls.x) + (le.y - ls.y) * (le.y - ls.y));
//    float dist = num / den;
//    return dist;
//}

float utils::distPointLine(glm::vec2 p, glm::vec2 ls, glm::vec2 le) {
    float A = p.x - ls.x;
    float B = p.y - ls.y;
    float C = le.x - ls.x;
    float D = le.y - ls.y;

    float dot = A * C + B * D;
    float len_sq = C * C + D * D;
    float param = -1;

    if (len_sq != 0) {
        param = dot / len_sq;
    }

    float xx, yy;
    if (param < 0) {
        xx = ls.x;
        yy = ls.y;
    }
    else if (param > 1) {
        xx = le.x;
        yy = le.y;
    }
    else {
        xx = ls.x + param * C;
        yy = ls.y + param * D;
    }

    float dx = p.x - xx;
    float dy = p.y - yy;
    return sqrt(dx * dx + dy * dy);



}

glm::vec3 utils::hsvToRgb(glm::vec3 hsv)
{
    hsv.x = glm::clamp(hsv.x, 0.0f, 359.99f);
    hsv.x /= 60;
    int hSec = hsv.x;
    float hRem = hsv.x - hSec;
    float c = hsv.y * hsv.z;
    float x = c * (1 - abs(hRem - 1));
    
    glm::vec3 rgb(0);
    
    switch (hSec) {
    case 0:
        rgb.r = c;
        rgb.g = x;
        break;
    case 1:
        rgb.g = c;
        rgb.r = x;
        break;
    case 2:
        rgb.g = c;
        rgb.b = x;
        break;
    case 3:
        rgb.b = c;
        rgb.g = x;
        break;
    case 4:
        rgb.b = c;
        rgb.r = x;
        break;
    case 5:
        rgb.r = c;
        rgb.b = x;
        break;
    }

    rgb += hsv.z - c;

    return rgb;
}

glm::vec2 utils::rotateVecByAngle(glm::vec2 vec, float angle)
{
    float x = vec.x;
    float y = vec.y;

    double a = angle * PI / 180;

    vec.x = cos(a) * x - sin(a) * y;
    vec.y = sin(a) * x + cos(a) * y;

    return vec;
}

void utils::rotateVecByAngle(glm::vec2* vec, float angle)
{
    float x = vec->x;
    float y = vec->y;

    double a = angle * PI / 180;

    vec->x = cos(a) * x - sin(a) * y;
    vec->y = sin(a) * x + cos(a) * y;
}

void utils::rotateVecByAngleRad(glm::vec2* vec, float angle)
{
    float x = vec->x;
    float y = vec->y;

    vec->x = cos(angle) * x - sin(angle) * y;
    vec->y = sin(angle) * x + cos(angle) * y;
}
