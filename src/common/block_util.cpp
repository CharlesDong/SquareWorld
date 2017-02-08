#include "block_util.h"

const std::string & itemName(item_t id)
{
	static const std::string names[5] =
	{
		"air",
		"dirt",
		"grass",
		"stone",
		"bedrock"
	};
	return names[id];
}