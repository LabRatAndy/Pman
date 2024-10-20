#pragma once
#include <cinttypes>
namespace Pman
{
	enum class SpriteType
	{
		None = 0,
		RedGhost,
		CyanGhost,
		OrangeGhost,
		PinkGhost,
		BlueGhost,
		PowerPellet,
		Gem,
		Player,
		GhostEyes
	};

	struct Sprite
	{
		int32_t SpriteID = -1;
		SpriteType Type = SpriteType::None;
		uint32_t Width = 0;
		uint32_t Height = 0;
	};
}
