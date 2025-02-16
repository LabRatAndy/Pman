//MIT License
//
//LabRatMan Copyright(c) 2025 Andrew Dawson
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files(the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
		Player(const PlayerSpecification& spec_);
		~Player();

		void OnUpdate(float time_step);
		void OnRender();

		void StartGame();
		void LooseALife();
		void EatenGhost();

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
		float m_LostLifeTimer = 0.0f;
	};
}