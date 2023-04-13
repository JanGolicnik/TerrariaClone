#include "text.h"
#include <fstream>
#include <utils.h>

namespace text {

	std::unordered_map<int, glyph> glyphs;
	GLuint texture;

	float lineHeight;
	float base;
	float scaleW;
	float scaleH;

    //GLuint VA;
    //GLuint VB;
    //GLuint IB;
    //GLuint shaderID;
    //SpriteVertex* vertices;
    //const int bsize = 1000;

    void setup()
    {
        //load text from file
        {
            std::ifstream file("assets/fonts/based.fnt", std::ios::in);
            if (file.is_open()) {
                std::string s;
                while (file >> s) {
                    if (s == "common") {
                        char c;
                        int num;
                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        lineHeight = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        base = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        scaleW = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        scaleH = num;
                    }
                    if (s == "char") {
                        glyph tmp;
                        int id;
                        char c;
                        int num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        id = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.x = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.y = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.width = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.height = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.xoffset = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.yoffset = num;

                        file >> c;
                        while (c != '=') {
                            file >> c;
                        }
                        file >> num;
                        tmp.xadvance = num;

                        glyphs.insert(std::pair<int, glyph>(id, tmp));
                    }
                }
                file.close();
            }
        }
        texture = utils::LoadTexture("assets/fonts/based_0.png", 4);
    }
	int widthOfText(const std::string* text, float size)
	{
        int currsize = 0;
        int maxwidth = 0;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\\') {
                if (text->at(i + 1) == 's') {
                    int counter = 2;
                    char c = text->at(i + counter);
                    while (c != '\n' && !(c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9' && c != '.')) {
                        counter++;
                        c = text->at(i + counter);
                    }
                    i += counter;
                }
                if (text->at(i + 1) == 'c') {
                    i += 11;
                }
                if (i >= text->size())continue;
            }
            if (text->at(i) == '\n') {
                if (currsize > maxwidth) maxwidth = currsize;
                currsize = 0;
                continue;
            }
            auto g = &glyphs[int(text->at(i))];
            currsize += g->xadvance;
        }
        if (currsize > maxwidth) maxwidth = currsize;
        return maxwidth * size;
	}
	int widthOfChar(const std::string* text, int n, float size)
	{
        if (text->size() > 0) {
            if (n >= 0 && n < text->size()) {
                if (text->at(n) == '\n') return 0;
                auto g = &glyphs[int(text->at(n))];
                return (g->xoffset + g->xadvance) * size;
            }
        }
	}
	int heightOfText(const std::string* text, float size)
	{
        if (text->size() == 0) {
            return 0;
        }
        bool isline1 = true;
        int height = 0;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\n') {
                isline1 = false;
                height += lineHeight;
                continue;
            }else
            if (isline1) {
                if (glyphs[int(text->at(i))].height > height) {
                    height = glyphs[int(text->at(i))].height;
                }
            }
        }
        return height * size;
	}
	int heightOfLine(const std::string* text, int line, float size)
	{
        int height = 0;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\n') {
                line--;
                if (line == 0) {
                    break;
                }
                height = 0;
            }
            else
            if (glyphs[int(text->at(i))].height > height) {
                height = glyphs[int(text->at(i))].height;
            }
        }
        return height * size;
	}
	int numLines(const std::string* text)
	{
        if (text->size() == 0) {
            return 0;
        }
        int n = 1;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\n') {
                n++;
            }
        }
        return n;
	}
	int numCharsInLine(const std::string* text, int line)
	{
        int n = 0;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\n') {
                line--;
                if (line == 0) {
                    return n;
                }
                n = 0;
                continue;
            }
            n++;
        }
        return 0;
	}
	int maxNumChars(const std::string* text)
	{
        int n = 0;
        int max = 0;
        for (int i = 0; i < text->size(); i++) {
            if (text->at(i) == '\n') {
                if (n > max) max = n;
                n = 0;
                continue;
            }
            n++;
        }
        if (n > max) max = n;
        return max;
	}
    std::string cutOffTextAt(const std::string* text, float limit, float size)
    {
        std::string tmpText = "";
        float width = 0;
        for (int i = 0; i < text->size(); i++) {
            width += widthOfChar(text, i, size);
            if (width > limit) {
                tmpText.push_back('\n');
                width = 0;
            }
            tmpText.push_back(text->at(i));
        }
        return tmpText;
    }
}