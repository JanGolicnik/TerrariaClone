#pragma once

#include <core.h>

struct glyph {
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
};

namespace text
{
	extern std::unordered_map<int, glyph> glyphs;
    extern GLuint texture;

    extern float lineHeight;
    extern float base;
    extern float scaleW;
    extern float scaleH;

    void setup();
    void clean();

    int widthOfText(const std::string* text, float size = 1.0f);
    int widthOfChar(const std::string* text, int n, float size = 1.0f);
    int heightOfText(const std::string* text, float size = 1.0f);
    int heightOfLine(const std::string* text, int line, float size = 1.0f);
    int numLines(const std::string* text);
    int numCharsInLine(const std::string* text, int line = 1);
    int maxNumChars(const std::string* text);
    std::string cutOffTextAt(const std::string* text, float limit, float size = 1);
};

