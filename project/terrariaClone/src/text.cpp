#include "text.h"

#include <fstream>
#include <iostream>

#include <utils.h>

namespace text {
	std::unordered_map<int, glyph> glyphs;
	GLuint texture;
	GLuint VA;
	GLuint VB;
	GLuint IB;
	SpriteVertex* vertices;
	GLuint shaderID;
	const int BSize = 1000;
	std::unordered_map<std::string, textline> lines;
	glm::vec2 res;

	void load(glm::vec2 fres)
	{
		std::ifstream file("assets/fonts/mainfont.fnt", std::ios::in);
		if (file.is_open()) {
			std::string s;
			while (file >> s) {
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
		}

		file.close();

		texture = utils::LoadTexture("assets/fonts/mainfont.png", 1);

		utils::createShader("src/shaders/textFrag.frag", "src/shaders/textVert.vert", &shaderID);
		glCreateVertexArrays(1, &VA);
		glBindVertexArray(VA);

		glCreateBuffers(1, &VB);
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteVertex) * BSize * 4, nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, Position));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (const void*)offsetof(SpriteVertex, SpriteCoords));

		uint32_t indices[BSize * 6];

		int offset = 0;
		for (int i = 0; i < BSize * 6; i += 6) {
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &IB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * BSize * 6, indices, GL_STATIC_DRAW);

		vertices = new SpriteVertex[BSize * 4];

		res = fres;
	}

	void render()
	{
		glUseProgram(shaderID);
		glActiveTexture(GL_TEXTURE0 + texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(0, texture);
		glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glBindBuffer(GL_ARRAY_BUFFER, VB);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(SpriteVertex) * BSize * 4, vertices);
		glBindVertexArray(VA);
		glDrawElements(GL_TRIANGLES, BSize * 6, GL_UNSIGNED_INT, nullptr);

	}

	void refresh()
	{
		int c = 0;
		int glyphsadded = 0;
		for (auto it = lines.begin(); it != lines.end(); it++) {
			glm::vec2 cursorpos = it->second.pos;
			for (int i = 0; i < it->second.ids.size(); i++) {
				glyphsadded++;
				glyph g = glyphs[it->second.ids[i]];
				glm::vec4 sc = glm::vec4(g.x, g.y, g.x + g.width, g.y + g.height);
				sc = {
					g.x,
					256 - g.y,
					g.x + g.width,
					256 - (g.y + g.height)
				};
				memcpy(vertices + (glyphsadded) * 4,
					utils::CreateSpriteRect(
						cursorpos.x + (g.xoffset / res.x),
						cursorpos.y,
						sc,
						g.width / res.x * it->second.scale,
						g.height / res.y * it->second.scale,
						{ 256, 256 },
						true
					).data(),
					4 * sizeof(SpriteVertex));
				cursorpos.x += g.xadvance / res.x * it->second.scale;
			}
			c++;
		}
	}

	void addLine(std::string name, const char* line, glm::vec2 pos, float scale)
	{
		if (lines.count(name) < 1) {
			textline tmp;
			for (int i = 0; i < strlen(line); i++) {
				char c = line[i];
				tmp.ids.push_back(line[i]);
			}
			tmp.scale = scale;
			tmp.pos = pos;
			lines.insert(std::pair<std::string, textline>(name, tmp));
			refresh();
			return;
		}
		lines[name].pos = pos;
		lines[name].scale = scale;
		updateText(name, line);
		refresh();
	}

	void updatePos(std::string text, glm::vec2 pos)
	{
		if (lines.count(text) >= 1) {
			lines[text].pos += pos;
		}
	}

	void updateScale(std::string text, float scale)
	{
		if (lines.count(text) >= 1) {
			lines[text].scale += scale;
		}
	}

	void updateText(std::string text, const char* line)
	{
		if (lines.count(text) >= 1) {
			lines[text].ids.clear();
			for (int i = 0; i < strlen(line); i++) {
				char c = line[i];
				if (line[i] == '\n') break;
				lines[text].ids.push_back(line[i]);
			}
		}
	}
}