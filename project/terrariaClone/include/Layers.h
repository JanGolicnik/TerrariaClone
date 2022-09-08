#pragma once

#include <core.h>

namespace Layers
{
	struct Layer {
		const char* name;
		GLuint mlayerVA;
		GLuint mlayerVB;
		GLuint mlayerIB;
		BlockVertex* mvertices;
		uint32_t* mindices;
		float mdarkness;
		int mdrawingOffset;
	};

	extern glm::ivec2 mapSize;
	extern glm::ivec2 blocksOnScreen;
	extern glm::ivec2 drawnBlocks;
	extern int nQuads;
	extern int nDrawnIndices;

	extern std::vector<Layer> layers;

	 void setUp(int blocksize, glm::ivec2 fmapSize, glm::ivec2 fscreenRes);

	 void addLayer(const char* name, float darkness, int drawingOffset = 0.0f);

	 Layer* getLayer(const char* l);

	 bool placeBlock(Layer* l, int x, int y, const char* type, float light = 0.0f, int size = 1, bool checkifempty = false, float setlight = 0.0f);
	 bool placeBlock(int x, int y, const char* type, float light = 0.0f, int size = 1, bool checkifempty = false, float setlight = 0.0f);
	 void breakBlock(Layer* l, int x, int y, int size = 1, bool dropitem = false);
	 bool isBlockValid(Layer* l, int x, int y);
	 const char* queryBlock(Layer* l, int x, int y);
	 std::array<BlockVertex, 4> queryQuad(Layer* l, int x, int y);
	 BlockVertex queryVert(Layer* l, int x, int y);

	 void setLight(Layer* l, int x, int y, float light);
	 void addLight(Layer* l, int x, int y, float light);

	 void updateBlock(Layer* l, int x, int y);
	 const char* damageBlock(Layer* l, int x, int y, int size = 1, bool dropitem = true);

};

