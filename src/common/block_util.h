#pragma once

#include <string>
using std::string;

typedef unsigned char item_t;

const double BLOCKSIZE = 80.0;

enum
{
	BLOCK_AIR,
	BLOCK_DIRT,
	BLOCK_GRASS,
	BLOCK_STONE,
	BLOCK_BEDROCK
};

const std::string & itemName(item_t id);