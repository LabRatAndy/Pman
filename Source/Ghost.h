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
		Vec2<int32_t> InitialPosition = { -1,-1 };
		Vec2<int32_t> ScatterPosition = { -1,-1 };
		Vec2<int32_t> DoorPosition = { -1,-1 };
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
		Vec2<int32_t> GetPosition() const { return m_Position; }
		float GetMoveSpeed() const { return m_Specification.MoveSpeed; }

		void SetPowerPelletActivated();
		void SetEaten();

	private:
		GhostStatus m_Status = GhostStatus::Invalid;
		GhostMode m_Mode = GhostMode::Invalid;
		GhostSpecification m_Specification = GhostSpecification();
		Vec2<int32_t> m_Position = { -1,-1 };
		Vec2<int32_t>m_PixelPosition = { -1,-1 };
		Vec2<int32_t> m_Direction = { 0,0 };
		Vec2<int32_t> m_Target = { -1,-1 };
		float m_FrightenedTimer = 0.0f;
		float m_ModeTimer = 0.0f;
		uint32_t m_AttackWave = 0;
		bool m_CanUseDoor = false;
		int32_t m_TileToMoveToIndex = -1;
		bool m_ModeTimerUp = false;
		bool m_SafeToModeSwitchX = false;
		bool m_SafeToModeSwitchY = false;

		void UpdateTarget();
		void FindPath(const Vec2<int32_t>& tile);
		bool TileIsAbleToMoveTo(const Vec2<int32_t>& target) const;
	};
}