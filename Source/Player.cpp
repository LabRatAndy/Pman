#include "Player.h"
#include "Application.h"
#include "Renderer.h"
#include "Level.h"
#include "Assert.h"
#include "utills.h"

#include <raylib.h>

#include <iostream>
namespace Pman
{
	
	Player::Player(const PlayerSpecification& spec) : m_Specification(spec), m_Lives(spec.PlayerLives), m_Position(spec.InitialPosition), m_Status(PlayerStatus::NotStarted), m_PixelPosition({ static_cast<int32_t>(spec.InitialPosition.X * spec.TileSize),static_cast<int32_t>(spec.InitialPosition.Y * spec.TileSize) })
	{
		SetTileCoordsFromOriginAndPixelPosition(m_Position, m_PixelPosition, m_Specification.TileSize);
	}
	Player::~Player()
	{

	}
	void Player::OnRender()
	{
		Application::Get().GetRenderer().RenderSprite(m_Specification.PlayerSprite, m_PixelPosition.X, m_PixelPosition.Y, m_Specification.TileSize);
		Application::Get().GetRenderer().RenderScore(m_Score);
		Application::Get().GetRenderer().RenderPlayerLives(m_Lives);
	}
	void Player::OnUpdate(float ts)
	{
		//check if we still have some lives left if not return
		if (m_Lives == 0)
		{
			return;
		}
		if (m_Status == PlayerStatus::Dead)
		{
			if (m_LostLifeTimer <= 0.0f)
			{
				m_Status = PlayerStatus::Running;
			}
			m_LostLifeTimer = m_LostLifeTimer - ts;
		}
		//check if collecting a gem, power pellet or hit a ghost
		if (m_Specification.LevelCallback->CollectGem(m_Position))
		{
			m_Score = m_Score + 10;
		}
		if (m_Specification.LevelCallback->CollectPowerPellet(m_Position))
		{
			m_Score = m_Score + 50;
		}
		m_Specification.LevelCallback->CollideWithGhost(m_PixelPosition);
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
		if (m_Direction.X != 0 || m_Direction.Y != 0)
		{
			float newXposition = m_PixelPosition.X + (m_Direction.X * (m_Specification.MoveSpeed * ts));
			float newYposition = m_PixelPosition.Y + (m_Direction.Y * (m_Specification.MoveSpeed * ts));
			Vec2<int32_t> newpos = { 0,0 };
			if (m_Direction.X == -1)
			{
				newpos.X = static_cast<int32_t>(std::floor(newXposition));
			}
			else
			{
				newpos.X = static_cast<int32_t>(std::ceil(newXposition));
			}
			if (m_Direction.Y == -1)
			{
				newpos.Y = static_cast<int32_t>(std::floor(newYposition));;
			}
			else
			{
				newpos.Y = static_cast<int32_t>(std::ceil(newYposition));
			}
			//check if colliding with a wall, gem, etc. Before applying the movement also add check to see if has been caught by a ghost and then loose a life. Pacman cannot use the door so last parameter has to be false 
			if (!m_Specification.LevelCallback->CollideWithWall(newpos, m_Direction))
			{
				m_PixelPosition = newpos;
			}
		}
		//update m_Position
		SetTileCoordsFromOriginAndPixelPosition(m_Position, m_PixelPosition, m_Specification.TileSize);
	}
	void Player::Reset()
	{
		//TODO
	}
	void Player::StartGame()
	{
		m_Status = PlayerStatus::Running;
	}
	void Player::LooseALife()
	{
		if (m_Status == PlayerStatus::Running)
		{
			m_Lives--;
			m_Status = PlayerStatus::Dead;
			m_LostLifeTimer = 5.0f; //5 seconds to get clear when you loose a live 
		}
	}
}