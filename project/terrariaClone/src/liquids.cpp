#include "liquids.h"

#include <unordered_map>
#include <layers.h>
#include <camera.h>
#include <textures.h>
#include <utils.h>
#include <world.h>
#include <particles.h>
#include <game.h>
#include <sounds.h>
#include <Window.h>

namespace liquids
{
	//data stored as 0xurddsstl where u is the reset timer, r is direction, d is id, s state, t timer and l level
	std::unordered_map<int, unsigned int> map;
	std::set<int> settledmap;

	std::unordered_map<std::string, int> types;
	
	struct liquidInfo {
		int blockid;
		glm::vec3 light;
	};

	std::unordered_map<int, liquidInfo> info;
	void add(std::string name, unsigned int base, int blockid, glm::vec3 light) {
		static int id = 0;
		base = (base & ~0x00ff0000) | (id<<16);
		types.insert(std::make_pair(name, base));
		info.insert(std::make_pair(id, liquidInfo(blockid, light)));
		id++;
	}
	void place(std::string name, glm::vec2 pos, int level)
	{
		int c = Layers::vecToInt(pos);
		unsigned int data = types[name];
		level = level & 0x0000000fu;
		data += level;
		data = (data & ~0x0f000000) | ((rand() % 2) << 24);
		int max = (data & 0xf0000000)>>28;
		if(max != 0)
		data = (data & ~0x000000f0) | ((rand() % max) << 4);
		map[c] = data;
	}
	void remove(glm::vec2 pos)
	{
		map.erase(Layers::vecToInt(pos));
	}
	void level(glm::vec2 pos)
	{
		int c = Layers::vecToInt(pos);
		if (map.count(c) >= 1) {
			std::cout << (map[c] & 0x0000000f) << "\n";;
		}
	}
	void updateTile(glm::vec2 pos, std::vector<std::pair<int, int>>* toadd, Layer* bs) {

		int c = Layers::vecToInt(pos);
		if (map.count(c) <= 0) return;

		unsigned int data = map[c];
		unsigned int id = (data & 0x00ff00f0u) >> 16;
		unsigned int timer = (data & 0x000000f0u) >> 4;
		unsigned int level = data & 0x0000000fu;
		unsigned int dir = ((data & 0x0f000000u) >> 24);
		unsigned int max = (data & 0xf0000000u) >> 28;
		unsigned int state = (data & 0x0000ff00u) >> 8;

		if (level < 1) {
			map.erase(c);
			return;
		}
		if (level > 8) level = 8;

		if (timer > 0) {
			map[c] = (data & ~0x000000f0u) | ((timer - 1) << 4);
			return;
		}

		map[c] = (data & ~0x000000f0u) | (max << 4);

		state += 9;
		state %= 36;
		map[c] = (map[c] & ~0x0000ff00u) | (state << 8);

		if (id == 1) {
			if (rand() % 1000 == 0) {
				particles::spawnEffect("lavasplash", pos);
			}
		}

		//premakne se dol
		if(c%map::mapY != 0)
		if (Layers::canLiquidGoThru(bs, c - 1)) {
			if (map.count(c - 1) >= 1) {
				int underdata = map[c - 1];
				int underlevel = underdata & 0x0000000fu;
				int underid = (underdata & 0x00ff0000u) >> 16;
				if ((underid == 1 && id == 0) || (underid == 0 && id == 1)) {
					Layers::placeBlock(Layers::intToVec(c - 1), "obsidian");
					sounds::lavapluswater();
					map.erase(c - 1);
					map[c]--;
					return;
				}
				if (underlevel == 8) {

				}
				else if (underlevel + level > 8) {
					level = glm::clamp(8 - underlevel, 0, 8);
					map[c - 1] = (map[c - 1] & ~0x0000000fu) | 0x00000008u;
				}
				else {
					map[c - 1] += level;
					map.erase(c);
					return;
				}
			}
			else {
				toadd->push_back({ c - 1, map[c] });
				map.erase(c);
				return;
			}
		}

		//chekira ceso prazne kocke levo desno
		int offset = map::mapY;
		if (dir == 1) {
			offset *= -1;
		}
		if (Layers::canLiquidGoThru(bs, c + offset)) {
			if (map.count(c + offset) <= 0) {
				unsigned int adddata = (data & ~0x0000000fu) | (unsigned int)ceil(level / 2.0f);
				adddata = (adddata & ~0x000000f0u) | (max << 4);
				toadd->push_back({ c + offset, adddata });
				map[c] = (map[c] & ~0x0000000fu) | (unsigned int)floor(level / 2.0f);
				return;
			}
			else {
				int otherdata = map[c + offset];
				int otherlevel = otherdata & 0x0000000fu;
				int otherid = (otherdata & 0x00ff0000u) >> 16;
				if ((otherid == 1 && id == 0) || (otherid == 0 && id == 1)) {
					Layers::placeBlock(Layers::intToVec(c + offset), "obsidian");
					sounds::lavapluswater();
					map.erase(c + offset);
					map[c]--;
					return;
				}
				if (otherlevel < level) {
					map[c + offset] = (map[c + offset] & ~0x0f000000u) | (dir << 24);
					map[c + offset] = (map[c + offset] & ~0x0000000fu) | (unsigned int)ceil((level + otherlevel) / 2.0f);
					map[c] = (map[c] & ~0x0000000fu) | (unsigned int)floor((level + otherlevel) / 2.0f);
					return;
				}
			}
		}

		if (dir) map[c] = map[c] & ~0x0f000000u;
		else map[c] = (map[c] & ~0x0f000000u) | 0x01000000u;
	}
	void updateOnScreen()
	{
		auto bs = &Layers::blocks;
		std::vector<std::pair<int, int>>toadd;

		glm::vec2 camC = -camera::pos;
		camC -= Layers::blocksOnScreen / glm::vec2(2.0f);

		for (int x = 0; x < Layers::blocksOnScreen.x; x++) {
			for (int y = 0; y < Layers::blocksOnScreen.y; y++) {
				glm::vec2 pos = floor(camC + glm::vec2(x, y));
				updateTile(pos, &toadd, bs);
			}
		}
		for (int i = 0; i < toadd.size(); i++) {
			if (map.count(toadd[i].first) >= 1) {
				int toid = map[toadd[i].first] & 0x00ff0000u >> 16;
				int fromid = toadd[i].second & 0x00ff0000u >> 16;
				if ((toid == 0 && fromid == 1) || (toid == 1 && fromid == 0)) {
					Layers::placeBlock(Layers::intToVec(toadd[i].first), "obsidian");
					sounds::lavapluswater();
					map.erase(toadd[i].first);
					continue;
				}
				map[toadd[i].first] += toadd[i].second & 0x0000000f;
				continue;
			}
			map[toadd[i].first] = toadd[i].second;
		}
		toadd.clear();

		//update sprites
		for (int x = 0; x < Layers::blocksOnScreen.x; x++) {
			for (int y = 0; y < Layers::blocksOnScreen.y; y++) {
				glm::vec2 pos = floor(camC + glm::vec2(x, y));
				int c = Layers::vecToInt(pos);
				if (map.count(c) >= 1) {
					int id = (map[c] & 0x00ff0000u) >> 16;
					int state = (map[c] & 0x0000ff00u) >> 8;
					bool bot = map.count(c - 1) >= 1;
					bool top = map.count(c + 1) >= 1;
					bool right = map.count(c + map::mapY) >= 1;
					bool left = map.count(c - map::mapY) >= 1;
					bot = bot || !Layers::canLiquidGoThru(bs, c - 1);
					top = top || !Layers::canLiquidGoThru(bs, c + 1);
					right = right || !Layers::canLiquidGoThru(bs, c + map::mapY);
					left = left || !Layers::canLiquidGoThru(bs, c - map::mapY);
					state = int(state / 9) * 9;
					if (!bot && !top && !right && !left) state += 0; else
					if (bot && !top && right && left) state += 5; else
					if (bot && top && right && left) state += 4; else
					if (bot && !top && !right && left) state += 2; else
					if (bot && !top && right && !left) state += 8; else 
					if (left) state += 1; else
					if (right) state += 7; else
					if (top && !right && !left) state += 3; else
					if (!top && !right && !left) state += 6;
					state = (state & 0x000000ff) << 8;
					map[c] = (map[c] & ~0x0000ff00) | state;
				}
			}
		}

		int sum = 0;
		for (auto& i : map) {
			sum += i.second & 0x0000000f;
		}
	}
	void renderOnScreen() {

		memset(Layers::drawnVertices, 0, sizeof(BlockVertex) * 4 * Layers::nDrawnBlocks);

		glUseProgram(globals::waterShaderID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures::spriteSheet);
		glUniform1i(1, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, globals::liquidTex);
		glUniform1i(6, 1);

		glUniform1i(7, globals::time);
		glUniform2f(8, Window::res.x, Window::res.y);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, Window::tmpFBT);
		glUniform1i(9, 2);

		glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(camera::trans));

		glm::vec2 camC = -camera::pos;

		camC -= Layers::blocksOnScreen / glm::vec2(2.0f);

		auto blocks = &Layers::blocks;

		int n = 0;
		for (int x = 0; x < Layers::blocksOnScreen.x; x++) {
			for (int y = 0; y < Layers::blocksOnScreen.y; y++) {
				glm::vec2 pos = floor(camC + glm::vec2(x, y));
				int c = Layers::vecToInt(pos);
				if (c < 0 || c > map::mapX * map::mapY - 1) continue;
				if (map.count(c) >= 1) {
					int data = map[c];
					liquidInfo i = info[(data & 0x00ff0000u) >> 16];
					int state = (data & 0x0000ff00u) >> 8;
					float level = (data & 0x0000000fu)/8.0f;

					if (i.light != glm::vec3(0)) {
						Layers::addLight(pos, i.light);
					}

					blocks::PremadeBlock tmp = blocks::blockBuffer[i.blockid][state];
					tmp.v0.Position += glm::vec2(pos.x, pos.y);
					tmp.v1.Position += glm::vec2(pos.x, pos.y);
					tmp.v2.Position += glm::vec2(pos.x, pos.y);
					tmp.v3.Position += glm::vec2(pos.x, pos.y);

					glm::vec3 light = blocks->mblocks[c].light;
					tmp.v0.light += light;
					tmp.v1.light += light;
					tmp.v2.light += light;
					tmp.v3.light += light;

					state %= 9;
					if (state == 2 || state == 4 || state == 5 || state == 8) {
						tmp.v0.breaking = level;
						tmp.v1.breaking = level;
						tmp.v2.breaking = level;
						tmp.v3.breaking = level;
					}
					else {
						tmp.v0.breaking = 1;
						tmp.v1.breaking = 1;
						tmp.v2.breaking = 1;
						tmp.v3.breaking = 1;
					}
					memcpy(Layers::drawnVertices + n * 4, &tmp, 4 * sizeof(BlockVertex));
					n++;
				}
			}
		}
		glBindBuffer(GL_ARRAY_BUFFER, Layers::layersVB);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BlockVertex) * Layers::nDrawnBlocks * 4, Layers::drawnVertices);

		glBindVertexArray(Layers::layersVA);
		glDrawElements(GL_TRIANGLES, Layers::nDrawnIndices, GL_UNSIGNED_INT, nullptr);

	}
	int at(glm::vec2 pos)
	{
		int c = Layers::vecToInt(pos);
		if (map.count(c) <= 0) return 0;
		return map[c];
	}
	int levelAt(glm::vec2 pos)
	{
		int c = Layers::vecToInt(pos);
		if (map.count(c) <= 0) return 0;
		return map[c]&0x0000000fu;
	}
	void save(std::ofstream* file)
	{
		int size = map.size();
		file->write((char*)&size, sizeof(size));
		for (auto& i : map) {
			file->write((char*)&i.first, sizeof(i.first));
			file->write((char*)&i.second, sizeof(i.second));
		}
	}
	void load(std::ifstream* file)
	{
		int size = map.size();
		file->read((char*)&size, sizeof(size));
		for (int i = 0; i < size; i++) {
			int first;
			unsigned int second;
			file->read((char*)&first, sizeof(first));
			file->read((char*)&second, sizeof(second));
			map.insert(std::make_pair(first, second));
		}
	}
	int settle(glm::vec2 pos) {
		int c = Layers::vecToInt(pos);
		return settle(c);
	}
	int settle(uint32_t c, int lastbounce, int lastlastbounce){

		if (map.count(c) <= 0) return 0;
		
		unsigned int data = map[c];
		unsigned int level = data & 0x0000000fu;
		unsigned int dir = ((data & 0x0f000000u) >> 24);
		unsigned int max = (data & 0xf0000000u) >> 28;

		if (c == lastlastbounce || c == lastbounce) {
			settledmap.insert(c);
		}

		if (settledmap.count(c) >= 1) return level;

		if (map.count(c-1) >= 1) {
			unsigned int underlevel = map[c - 1] & 0x0000000fu;
			if (underlevel != 8) {
				underlevel = level + underlevel;
				if (underlevel > 8) {
					level = underlevel%8;
					underlevel = 8;
				}
				else {
					level = 0;
				}
				map[c - 1] = (data & ~0x0000000f) | underlevel;
				settle(c - 1);
				if (level == 0) {
					map.erase(c);
					return 0;
				}
				map[c] = (data & ~0x0000000f) | level;
				data = map[c];
				level = data & 0x0000000fu;
				dir = ((data & 0x0f000000u) >> 24);
			}
		}
		else if (c%map::mapY != 0 && Layers::canLiquidGoThru(&Layers::blocks, c - 1)) {
			map[c - 1] = map[c];
			map.erase(c);
			settle(c - 1);
			return 0;
		}

		int side = map::mapY;
		if (dir == 1) side *= -1;
		if (settle(c + side, c, lastbounce) <= 0 && Layers::canLiquidGoThru(&Layers::blocks, c + side)) {
			map[c + side] = map[c];
			map.erase(c);
			settle(c + side, lastbounce, lastlastbounce);
			return 0;
		}

		side *= -1;
		if (settle(c + side, c, lastbounce) <= 0 && Layers::canLiquidGoThru(&Layers::blocks, c + side)) {
			map[c + side] = (map[c] & ~0x0f000000u) | ((!dir) << 24);
			map.erase(c);
			settle(c + side, c, lastbounce);
			return 0;
		}
		settledmap.insert(c);
		return level;
	}
	void clean()
	{
		map.clear();
	}
	void settleAll() {
		settledmap.clear();
		for (int x = 0; x < map::mapX; x++) {
			for (int y = 0; y < map::mapY; y++) {
				settle({ x,y });
			}
		}
		settledmap.clear();
	}
};
