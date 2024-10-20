#pragma once
#include "Sprite.h"
#include "Maths.h"

#include <stack>

namespace Pman
{
	//fwd dec
	class Level;
	enum class GhostType
	{
		Invalid = 0,
		Red,
		Cyan,
		Orange,
		Pink
	};
	enum class GhostStatus
	{
		Invalid = 0,
		NotStarted,
		Running,
		IsBlue,
		EyesOnly
	};
	enum class GhostMode
	{
		Invalid = 0,
		Chase,
		Scatter,
		Eaten
	};

	struct GhostSpecification
	{
		Sprite* MainSprite = nullptr;
		Sprite* BlueSprite = nullptr;
		Sprite* EyesSprite = nullptr;
		GhostType Type = GhostType::Invalid;
		Vec2<float> InitialPosition = { 0.0f,0.0f };
		Vec2<float> ScatterPosition = { 0.0f,0.0f };
		Vec2<float> DoorPosition = { 0.0f,0.0f };
		uint32_t TileSize = 32;
		float MoveSpeed = 1.0f;
		Level* LevelCallback = nullptr;
	};

	class Ghost
	{
	public:
		Ghost(const GhostSpecification& spec);
		~Ghost();

		void OnUpdate(float ts);
		void OnRender();

		void Reset();
		void StartGame();

		GhostStatus GetStatus() const { return m_Status; }
		GhostMode GetMode() const { return m_Mode; }
		Vec2<float> GetPosition() const { return m_Position; }
		float GetMoveSpeed() const { return m_Specification.MoveSpeed; }

		void SetPowerPelletActivated();
		void SetEaten();

	private:
		GhostStatus m_Status = GhostStatus::Invalid;
		GhostMode m_Mode = GhostMode::Invalid;
		GhostSpecification m_Specification = GhostSpecification();
		Vec2<float> m_Position = { 0.0f,0.0f };
		Vec2<int32_t> m_Direction = { 0,0 };
		Vec2<float> m_Target = { 0.0f,0.0f };
		float m_FrightenedTimer = 0.0f;
		float m_ModeTimer = 0.0f;
		uint32_t m_AttackWave = 0;
		bool m_CanUseDoor = false;
		int32_t m_TileToMoveToIndex = -1;
		


		float GetDistanceToTarget(const Vec2<int32_t>& direction, float ts);
		void UpdateTarget();
		Vec2<uint32_t> GetTileCoordinate() const;
		void FindPath(const Vec2<uint32_t>& tile);
	};
}