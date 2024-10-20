#include "Player.h"
#include "Application.h"
#include "Renderer.h"
#include "Level.h"
#include "Assert.h"

#include <raylib.h>

#include <iostream>
namespace Pman
{
	static Vec2<uint32_t> RoundFloatPostionToUint32(const Vec2<float>& position)
	{
		uint32_t x = std::round(position.X);
		uint32_t y = std::round(position.Y);
		return { x,y };
	}
	Player::Player(const PlayerSpecification& spec) : m_Specification(spec), m_Lives(spec.PlayerLives), m_Position(spec.InitialPosition), m_Status(PlayerStatus::NotStarted)
	{

	}
	Player::~Player()
	{

	}
	void Player::OnRender()
	{
		Application::Get().GetRenderer().RenderSprite(m_Specification.PlayerSprite, m_Position.X, m_Position.Y, m_Specification.TileSize);
	}
	void Player::OnUpdate(float ts)
	{
		//player movement
		if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP) /* && m_Status == PlayerStatus::Running*/)
		{
			m_Direction.Y = -1;
		}
		else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT) /* && m_Status == PlayerStatus::Running*/)
		{
			m_Direction.X = -1;
		}
		else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN) /* && m_Status == PlayerStatus::Running*/)
		{
			m_Direction.Y = 1;
		}
		else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT) /* && m_Status == PlayerStatus::Running*/)
		{
			m_Direction.X = 1;
		}
		else
		{
			m_Direction.X = 0;
			m_Direction.Y = 0;
		}
		if (m_Specification.LevelCallback->CollectGem(m_Position))
		{
			m_Score = m_Score + 10;
		}
		if (m_Specification.LevelCallback->CollectPowerPellet(m_Position))
		{
			m_Score = m_Score + 50;
		}
		if (m_Direction.X != 0 || m_Direction.Y != 0)
		{
			float newXposition = m_Position.X + (m_Direction.X * (m_Specification.MoveSpeed * ts));
			float newYposition = m_Position.Y + (m_Direction.Y * (m_Specification.MoveSpeed * ts));
			//check if colliding with a wall, gem, etc. Before applying the movement also add check to see if has been caught by a ghost and then loose a life. Pacman cannot use the door so last parameter has to be false 
			if (!m_Specification.LevelCallback->CollideWithWall(RoundFloatPostionToUint32({ newXposition,newYposition }), m_Direction, false))
			{
				m_Position.X = newXposition;
				m_Position.Y = newYposition;
			}
		}
	}
	void Player::Reset()
	{
		//TODO
	}
	void Player::StartGame()
	{
		m_Status = PlayerStatus::Running;
	}
}