#pragma once
#include "Sprite.h"
#include "Maths.h"

namespace Pman
{
	//fwd dec
	class Level;
	struct PlayerSpecification
	{
		Vec2<int32_t> InitialPosition = { -1,-1 };
		float MoveSpeed = 1.0f;
		Sprite* PlayerSprite = nullptr;
		uint32_t PlayerLives = 3;
		uint32_t TileSize = 32;
		uint32_t LevelWidth = 0;
		uint32_t LevelHeight = 0;
		Level* LevelCallback = nullptr;

		constexpr uint32_t GetPixelWidth()
		{
			return LevelWidth * TileSize;
		}
		constexpr uint32_t GetPixelHeight()
		{
			return LevelHeight * TileSize;
		}
	};

	enum class PlayerStatus
	{
		None = 0,
		NotStarted,
		Running,
		Dead
	};

	class Player
	{
	public:
		Player(const PlayerSpecification& spec);
		~Player();

		void OnUpdate(float ts);
		void OnRender();

		void Reset();

		void StartGame();
		void LooseALife() { m_Lives--; }

		Vec2<int32_t> GetPosition() const { return m_Position; }
		uint32_t GetScore() const { return m_Score; }
		uint32_t GetPlayerLives() const { return m_Lives;}
		float GetMoveSpeed() const { return m_Specification.MoveSpeed; }
		Vec2<int32_t> GetDirection() const { return m_Direction; }

	private:
		Vec2<int32_t> m_Position = { -1,-1 };		//tile position
		Vec2<int32_t> m_PixelPosition = { -1,-1 }; //position in pixels used for rendering and movement
		uint32_t m_Score = 0;
		Vec2<int32_t> m_Direction = { 0,0 };
		PlayerStatus m_Status = PlayerStatus::None;
		PlayerSpecification m_Specification = PlayerSpecification();
		uint32_t m_Lives = 3;
	};
}