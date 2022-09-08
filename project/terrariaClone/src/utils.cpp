#include "utils.h"

#include <stb_image/stb_image.h>    
#include <fstream>

std::array<BlockVertex, 4> utils::CreateQuad(float x, float y, glm::vec4 sC, float light, glm::vec2 size) {
    int ss = 8;
    int w = 16000;
    int h = 32;

    double ssw = (double)ss / w;
    double ssh = (double)ss / h;

    BlockVertex v0;
    v0.Position = { x, y, 0.0f };
    v0.SpriteCoords = { sC.r * ssw, sC.g * ssh };
    v0.TexCoords = { 0, 0 };
    v0.light = light;
    v0.breaking = 4.0f;

    BlockVertex v1;
    v1.Position = { x + size.x, y, 0.0f };
    v1.SpriteCoords = { sC.b * ssw, sC.g * ssh };
    v1.TexCoords = { 1, 0 };
    v1.light = light;
    v1.breaking = 4.0f;

    BlockVertex v2;
    v2.Position = { x + size.x, y + size.y, 0.0f };
    v2.SpriteCoords = { sC.b * ssw, sC.a * ssh };
    v2.TexCoords = { 1, 1 };
    v2.light = light;
    v2.breaking = 4.0f;


    BlockVertex v3;
    v3.Position = { x, y + size.y, 0.0f };
    v3.SpriteCoords = { sC.r * ssw, sC.a * ssh };
    v3.TexCoords = { 0, 1 };
    v3.light = light;
    v3.breaking = 4.0f;

    return { v0, v1, v2, v3 };
}

std::array<SpriteVertex, 4> utils::CreateSpriteRect(float x, float y, glm::vec4 sC, float sizeX, float sizeY)
{
    int ss = 8;
    int w = 16000;
    int h = 32;

    double ssw = (double)ss / w;
    double ssh = (double)ss / h;

    SpriteVertex v0;
    v0.Position = { x, y, 0.0f };
    v0.SpriteCoords = { sC.r * ssw, sC.g * ssh };

    SpriteVertex v1;
    v1.Position = { x + sizeX, y, 0.0f };
    v1.SpriteCoords = { sC.b * ssw, sC.g * ssh };

    SpriteVertex v2;
    v2.Position = { x + sizeX, y + sizeY, 0.0f };
    v2.SpriteCoords = { sC.b * ssw, sC.a * ssh };

    SpriteVertex v3;
    v3.Position = { x, y + sizeY, 0.0f };
    v3.SpriteCoords = { sC.r * ssw, sC.a * ssh };

    return { v0, v1, v2, v3 };
}

std::array<SpriteVertex, 4> utils::CreateSpriteRect(float x, float y, glm::vec4 sC, float sizeX, float sizeY, glm::vec2 wh, bool invert)
{
    int w = wh.x;
    int h = wh.y;
    SpriteVertex v0;
    SpriteVertex v2;
    SpriteVertex v1;
    SpriteVertex v3;

    if (invert) {
        v0.Position = { x, y, 0.0f };
        v0.SpriteCoords = { sC.r / w, sC.a / h };

        v1.Position = { x + sizeX, y, 0.0f };
        v1.SpriteCoords = { sC.b / w, sC.a / h };

        v2.Position = { x + sizeX, y + sizeY, 0.0f };
        v2.SpriteCoords = { sC.b / w, sC.g / h };

        v3.Position = { x, y + sizeY, 0.0f };
        v3.SpriteCoords = { sC.r / w, sC.g / h };
    }
    else
    {
        v0.Position = { x, y, 0.0f };
        v0.SpriteCoords = { sC.r / w, sC.g / h };

        v1.Position = { x + sizeX, y, 0.0f };
        v1.SpriteCoords = { sC.b / w, sC.g / h };

        v2.Position = { x + sizeX, y + sizeY, 0.0f };
        v2.SpriteCoords = { sC.b / w, sC.a / h };

        v3.Position = { x, y + sizeY, 0.0f };
        v3.SpriteCoords = { sC.r / w, sC.a / h };
    }

    

    return { v0, v1, v2, v3 };
}

//checked
GLuint utils::LoadTexture(const char* path, int channels, bool repeat) {
    int w, h, bits;

    stbi_set_flip_vertically_on_load(1);
    stbi_uc* pixels = nullptr;
    switch (channels) {
    case 4:
        pixels = stbi_load(path, &w, &h, &bits, STBI_rgb_alpha);
        break;
    case 3:
        pixels = stbi_load(path, &w, &h, &bits, STBI_rgb);
        break;
    case 2:
        pixels = stbi_load(path, &w, &h, &bits, STBI_grey_alpha);
        break;
    case 1:
        pixels = stbi_load(path, &w, &h, &bits, STBI_grey);
        break;
    default:
        std::cout << "incorrect num of channels loading image " << path << "\n";
        break;
    }
    if (pixels == nullptr) {
        std::cout << "error loading image " << path << "\n";
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

//checked
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

void utils::createFrameBuffer(GLuint* framebuffer, GLuint* tex, glm::vec2 res, bool nearest)
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

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "\nframebuffer error\n";
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void utils::cout2(glm::vec2 vec)
{
    std::cout << vec.x << " " << vec.y << "\n";
}

float utils::distance(glm::vec2 v1, glm::vec2 v2) {
    float dist = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
    return dist;
}