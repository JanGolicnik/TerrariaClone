#include "background.h"

#include <utils.h>
#include <globals.h>
#include <layers.h>

namespace background {
	std::unordered_map<int, bg> bgs;
	std::unordered_map<std::string, int> bgNames;
	float biomeDiff = 0;
	std::string currBg = "forest";
	std::string swapTo = "forest";
	void load()
	{
		clear();
		int id = 0;
		bg tmp;
		tmp.res = glm::vec2(1500, 600);
		tmp.front = utils::LoadTexture("assets/skorno1.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/skorno2.png", 4, true);
		tmp.back = utils::LoadTexture("assets/skorno3.png", 4, true);
		tmp.mid = utils::LoadTexture("assets/empty.png", 4, true);
		tmp.back = utils::LoadTexture("assets/empty.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("forest", id));
		id++;

		tmp.front = utils::LoadTexture("assets/corruptskorno1.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/corruptskorno2.png", 4, true);
		tmp.back = utils::LoadTexture("assets/corruptskorno3.png", 4, true);
		tmp.mid = utils::LoadTexture("assets/empty.png", 4, true);
		tmp.back = utils::LoadTexture("assets/empty.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("corruption", id));
		id++;

		tmp.front = utils::LoadTexture("assets/pesekskorno1.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/pesekskorno2.png", 4, true);
		tmp.back = utils::LoadTexture("assets/pesekskorno3.png", 4, true);
		tmp.mid = utils::LoadTexture("assets/empty.png", 4, true);
		tmp.back = utils::LoadTexture("assets/empty.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("desert", id));
		id++;

		tmp.front = utils::LoadTexture("assets/prokletskorno1.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/prokletskorno2.png", 4, true);
		tmp.back = utils::LoadTexture("assets/prokletskorno3.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("underworld", id));
		id++;

		tmp.front = utils::LoadTexture("assets/kamenoskorno1.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/kamenoskorno2.png", 4, true);
		tmp.back = utils::LoadTexture("assets/kamenoskorno3.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("underground", id));
		id++;

		tmp.front = utils::LoadTexture("assets/space.png", 4, true, &tmp.res);
		tmp.mid = utils::LoadTexture("assets/empty.png", 4, true);
		tmp.back = utils::LoadTexture("assets/empty.png", 4, true);
		bgs.insert(std::make_pair(id, tmp));
		bgNames.insert(std::make_pair("space", id));
		id++;
	}

	void render()
	{
		if (background::currBg == background::swapTo) {
			background::biomeDiff = 0;
		}
		if (background::biomeDiff == 0) {
			background::swapTo = Layers::currentBiome;
		}
		if (background::biomeDiff < 0.95) {
			background::biomeDiff += utils::approach(background::biomeDiff, 1, 30);
		}
		if (background::biomeDiff > 0.95) {
			background::biomeDiff = 0;
			background::currBg = background::swapTo;
		}
		background::renderTwo(background::bgNames[Layers::biomes[background::currBg].bg], background::bgNames[Layers::biomes[background::swapTo].bg], background::biomeDiff);
	}

	void renderOne(int id)
	{
		auto bg = &bgs[id];
		glUseProgram(globals::backgroundShaderID);
		glUniform1i(2, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, bg->front);
		glUniform1i(3, 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, bg->mid);
		glUniform1i(4, 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, bg->back);
		glUniform2f(5, bg->res.x, bg->res.y);

		glUniform1i(6, 3);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(7, 4);
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(8, 5);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform2f(9, 0, 0);

		glUniform3f(10, globals::dayclr.r, globals::dayclr.g, globals::dayclr.b);
		glUniform1f(11, globals::bgoffset);
		glUniform1f(12, 0);
		glBindVertexArray(globals::overlayVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void renderTwo(int id1, int id2, float mixfactor)
	{
		auto bg1 = &bgs[id1];
		auto bg2 = &bgs[id2];
		glUseProgram(globals::backgroundShaderID);
		glUniform1i(2, 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, bg1->front);
		glUniform1i(3, 1);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, bg1->mid);
		glUniform1i(4, 2);
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, bg1->back);
		glUniform2f(5, bg1->res.x, bg1->res.y);

		glUniform1i(6, 3);
		glActiveTexture(GL_TEXTURE0 + 3);
		glBindTexture(GL_TEXTURE_2D, bg2->front);
		glUniform1i(7, 4);
		glActiveTexture(GL_TEXTURE0 + 4);
		glBindTexture(GL_TEXTURE_2D, bg2->mid);
		glUniform1i(8, 5);
		glActiveTexture(GL_TEXTURE0 + 5);
		glBindTexture(GL_TEXTURE_2D, bg2->back);
		glUniform2f(9, bg2->res.x, bg2->res.y);

		glUniform3f(10, globals::dayclr.r, globals::dayclr.g, globals::dayclr.b);
		glUniform1f(11, globals::bgoffset);
		glUniform1f(12, mixfactor);
		glBindVertexArray(globals::overlayVA);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void clear()
	{
		for (auto& i : bgs) {
			glDeleteTextures(1, &i.second.front);
			glDeleteTextures(1, &i.second.mid);
			glDeleteTextures(1, &i.second.back);
		}
		bgs.clear();
	}
}