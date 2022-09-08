#pragma once
#include <unordered_map>
#include <core.h>
namespace text
{
	struct glyph {
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int xadvance;
	};

	struct textline {
		std::vector <int> ids;
		glm::vec2 pos;
		float scale;
	};
	extern glm::vec2 res;
	extern std::unordered_map<int, glyph> glyphs;
	extern std::unordered_map<std::string, textline> lines;
	extern GLuint texture;

	extern GLuint VA;
	extern GLuint VB;
	extern GLuint IB;

	extern SpriteVertex* vertices;

	extern GLuint shaderID;

	extern const int BSize;

	void load(glm::vec2 res);
	void render();
	void refresh();
	void addLine(std::string name, const char* line, glm::vec2 pos, float scale = 1.0f);
	void updatePos(std::string text, glm::vec2 pos);
	void updateScale(std::string text, float scale);
	void updateText(std::string text, const char* line);

};

