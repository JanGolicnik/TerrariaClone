#include "background.h"

#include <utils.h>
#include <globals.h>
#include <layers.h>
#include <player.h>
#include <world.h>

namespace background {
	
	struct bgSprite{
		GLuint tex;
		glm::vec2 res;
		glm::vec4 offset;
		bool repeatY = false;
	};

	typedef std::array<bgSprite, 7> bg;
	std::unordered_map<std::string, bg> bgs;
	float biomeDiff = 0;
	std::string currBg = "forest";
	std::string swapTo = "forest";
	void load()
	{
		clear();
		bg tmp;
		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		glm::vec2 res(0);
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/forest/1.png", 4, false, &res), res, glm::vec4(0, -0.2, 1, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/forest/2.png", 4, false, &res), res, glm::vec4(0, -0.1, 1.5, 1));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/forest/3.png", 4, false, &res), res, glm::vec4(0, -0.2, 2, 1));
		tmp[3] = bgSprite(utils::LoadTexture("assets/backgrounds/forest/4.png", 4, false, &res), res, glm::vec4(0, 0.75, 2.5, 2));
		tmp[4] = bgSprite(utils::LoadTexture("assets/backgrounds/forest/5.png", 4, false, &res), res, glm::vec4(0, 2, 3, 3));
		bgs.insert(std::make_pair("forest", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/snow/1.png", 4, false, &res), res, glm::vec4(0, 0.15, 1, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/snow/2.png", 4, false, &res), res, glm::vec4(0, 0.2, 1.5, 1));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/snow/3.png", 4, false, &res), res, glm::vec4(0, 1.15, 2, 1.5));
		bgs.insert(std::make_pair("snow", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/desert/1.png", 4, false, &res), res, glm::vec4(0, 0.2, 1, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/desert/2.png", 4, false, &res), res, glm::vec4(0.36, 0.3, 1.5, 1));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/desert/3.png", 4, false, &res), res, glm::vec4(0, 0.1, 2, 1));
		tmp[3] = bgSprite(utils::LoadTexture("assets/backgrounds/desert/4.png", 4, false, &res), res, glm::vec4(0.5, 0.8, 3, 1));
		bgs.insert(std::make_pair("desert", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/corruption/1.png", 4, false, &res), res, glm::vec4(0, 0, 1, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/corruption/2.png", 4, false, &res), res, glm::vec4(0.36, 0.1, 1.5, 1));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/corruption/3.png", 4, false, &res), res, glm::vec4(0, 0.3, 1, 1));
		tmp[3] = bgSprite(utils::LoadTexture("assets/backgrounds/corruption/4.png", 4, false, &res), res, glm::vec4(0.6, 0.8, 1.75, 1.5));
		tmp[4] = bgSprite(utils::LoadTexture("assets/backgrounds/corruption/5.png", 4, false, &res), res, glm::vec4(0.66, 1.36, 3, 2));
		bgs.insert(std::make_pair("corruption", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/jungle/1.png", 4, false, &res), res, glm::vec4(0, 0.3, 1, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/jungle/2.png", 4, false, &res), res, glm::vec4(0.36, 1.1, 2, 2));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/jungle/3.png", 4, false, &res), res, glm::vec4(0, 1.5, 3, 2.5));
		bgs.insert(std::make_pair("jungle", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/0.png", 4, false, &res), res, glm::vec4(0,0,0,1));
		bgs.insert(std::make_pair("surface", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/underground/forest/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1));
		bgs.insert(std::make_pair("forestunderground", tmp));
		bgs.insert(std::make_pair("desertunderground", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/underground/corruption/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1), true);
		bgs.insert(std::make_pair("corruptionunderground", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/underground/jungle/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1), true);
		bgs.insert(std::make_pair("jungleunderground", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/caverns/forest/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1), true);
		bgs.insert(std::make_pair("forestcavern", tmp));
		bgs.insert(std::make_pair("desertcavern", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/caverns/corruption/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1), true);
		bgs.insert(std::make_pair("corruptioncavern", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/caverns/jungle/0.png", 4, true, &res), res, glm::vec4(0, 0, 0, 1), true);
		bgs.insert(std::make_pair("junglecavern", tmp));

		tmp.fill(bgSprite(-1, glm::vec2(0), glm::vec4(0)));
		tmp[0] = bgSprite(utils::LoadTexture("assets/backgrounds/underworld/0.png", 4, false, &res), res, glm::vec4(0, 0.25, 0, 1));
		tmp[1] = bgSprite(utils::LoadTexture("assets/backgrounds/underworld/1.png", 4, false, &res), res, glm::vec4(0, 0, 1.5, 1));
		tmp[2] = bgSprite(utils::LoadTexture("assets/backgrounds/underworld/2.png", 4, false, &res), res, glm::vec4(0, 0.1, 2, 1));
		tmp[3] = bgSprite(utils::LoadTexture("assets/backgrounds/underworld/3.png", 4, false, &res), res, glm::vec4(0, 0, 3, 1));
		bgs.insert(std::make_pair("underworld", tmp));
	}

	void render()
	{
		if (currBg == swapTo) {
			biomeDiff = 0;
		}
		if (biomeDiff == 0) {
			swapTo = Layers::currentBiome;
		}
		if (biomeDiff < 0.95) {
			biomeDiff += utils::approach(biomeDiff, 1, 30);
		}
		if (biomeDiff > 0.95) {
			biomeDiff = 0;
			currBg = swapTo;
		}

		std::string curr = Layers::biomes[currBg].bg;
		std::string swap = Layers::biomes[swapTo].bg;


		
		if (Player::pos.y < map::underworldH) {
			curr = "underworld";
			swap = "underworld";
		} else if (Player::pos.y < map::underworldH + (map::surfaceH - map::underworldH) / 2) {
			curr += "cavern";
			swap += "cavern";
		} else if (Player::pos.y < map::surfaceH - map::surfaceScale) {
			curr += "underground";
			swap += "underground";
		}

		renderOne(curr, 1 - biomeDiff);
		renderOne(swap, biomeDiff);
	}

	void renderOne(std::string id, float opacity)
	{
		bg* b = &bgs[id];
		glUseProgram(globals::backgroundShaderID);

		glUniform3f(0, globals::dayclr.r, globals::dayclr.g, globals::dayclr.b);
		glUniform1f(1, opacity);
		glUniform2f(2, globals::resX, globals::resY);
		glUniform1f(3, 0.1);

		for (int i = 0; i < b->size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, b->at(i).tex);
			GLuint texloc = glGetUniformLocation(globals::backgroundShaderID, std::string("uBgs[" + std::to_string(i) + "].tex").c_str());
			GLuint resloc = glGetUniformLocation(globals::backgroundShaderID, std::string("uBgs[" + std::to_string(i) + "].res").c_str());
			GLuint offloc = glGetUniformLocation(globals::backgroundShaderID, std::string("uBgs[" + std::to_string(i) + "].offset").c_str());
			GLuint repeatloc = glGetUniformLocation(globals::backgroundShaderID, std::string("uBgs[" + std::to_string(i) + "].repeatY").c_str());
			glUniform1i(texloc, i);
			glUniform2f(resloc, b->at(i).res.x, b->at(i).res.y);
			glUniform3f(offloc, b->at(i).offset.x + b->at(i).offset.z * globals::bgoffset, b->at(i).offset.y, b->at(i).offset.w);
			glUniform1i(repeatloc, b->at(i).repeatY);
		}

		glBindVertexArray(globals::overlayVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void clear()
	{
		for (auto& i : bgs) {
			for (int j = 0; j < i.second.size(); j++) {
				glDeleteTextures(1, &i.second[j].tex);
			}
		}
		bgs.clear();
	}
}