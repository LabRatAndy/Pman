#include "Ghost.h"
#include "Level.h"
#include "Application.h"
#include "Assert.h"
#include "Renderer.h"
#include "utills.h"



#include <queue>

namespace Pman
{
	
	Ghost::Ghost(const GhostSpecification& spec) : m_Status(GhostStatus::NotStarted), m_Mode(GhostMode::Scatter), m_Specification(spec), m_Position(spec.InitialPosition), m_PixelPosition({ static_cast<int32_t>(spec.InitialPosition.X * spec.TileSize),static_cast<int32_t>(spec.InitialPosition.Y * spec.TileSize) })
	{
	}
	Ghost::~Ghost()
	{
	}
	void Ghost::OnUpdate(float ts)
	{
		TRACE("New frame on update\n");
		if (m_Status == GhostStatus::NotStarted) //the game is not running!
			return;
		if (m_Status == GhostStatus::IsBlue)
		{
			if (m_FrightenedTimer <= 0.0f)
			{
				// ghost back to normal
				m_Status = GhostStatus::Running;
			}
			m_FrightenedTimer -= ts;
		}
		UpdateTarget();
		ASSERT((m_Target.X <= m_Specification.LevelCallback->GetLevelWidthInTiles()), "Error invalid target width");
		ASSERT((m_Target.Y <= m_Specification.LevelCallback->GetLevelHeightInTiles()), "Error invalid target height")
		//work out direction to take
		{
			if (ts < 0.001)
			{
				ts = 0.0166668f;
			}
			FindPath(m_Position);
			const auto& tile = m_Specification.LevelCallback->GetTile(m_TileToMoveToIndex);
			TRACE("Current position is: {},{}", (int32_t)m_Position.X, (int32_t)m_Position.Y);
			TRACE("Tile to move to is: {},{}", tile.GetTileXPosition(), tile.GetTileYPosition());
			if (m_Position.X > tile.GetTileXPosition() && m_Direction.X != 1)
			{
				//need togo left
				m_Direction.X = -1;
				m_Direction.Y = 0;
			}
			else if (m_Position.X < tile.GetTileXPosition() && m_Direction.X != -1)
			{
				//need togo right
				m_Direction.X = 1;
				m_Direction.Y = 0;
			}
			else if (m_Position.Y > tile.GetTileYPosition() && m_Direction.Y != 1)
			{
				//need togo up
				m_Direction.X = 0;
				m_Direction.Y = -1;
			}
			else if (m_Position.Y < tile.GetTileYPosition() && m_Direction.Y != -1)
			{
				//need togo down
				m_Direction.X = 0;
				m_Direction.Y = 1;
			}
			else
			{
				//need to reverse the direction of the ghost 
				WARN("Fell thorugh to allow reverse section, please check carefully that this is correct");
				if (m_Position.X > tile.GetTileXPosition())
				{
					//need togo left
					m_Direction.X = -1;
					m_Direction.Y = 0;
				}
				else if (m_Position.X < tile.GetTileXPosition())
				{
					//need togo right
					m_Direction.X = 1;
					m_Direction.Y = 0;
				}
				else if (m_Position.Y > tile.GetTileYPosition())
				{
					//need togo up
					m_Direction.X = 0;
					m_Direction.Y = -1;
				}
				else if (m_Position.Y < tile.GetTileYPosition())
				{
					//need togo down
					m_Direction.X = 0;
					m_Direction.Y = 1;
				}
			}
			if (m_Position == m_Target)
			{
				//stop moveing as we are all ready at target
				m_Direction.X = 0;
				m_Direction.Y = 0;
			}
		}
		{
			//now move the ghost 
			TRACE("Movespeed is: {} Timestep is: {}", (float)m_Specification.MoveSpeed, (float)ts);
			TRACE("Initial Pixel Position: {},{}", (int32_t)m_PixelPosition.X, (int32_t)m_PixelPosition.Y);
			m_PixelPosition.X = m_PixelPosition.X + static_cast<int32_t>(m_Direction.X * (m_Specification.MoveSpeed * ts));
			m_PixelPosition.Y = m_PixelPosition.Y + static_cast<int32_t>(m_Direction.Y * (m_Specification.MoveSpeed * ts));
			TRACE("New pixel position: {},{}", (int32_t)m_PixelPosition.X, (int32_t)m_PixelPosition.Y);
			//check for going though the tunnel from 1 size to the other
			if (m_PixelPosition.X <= 0)
			{
				m_PixelPosition.X = (static_cast<int32_t>(m_Specification.MoveSpeed * ts) - m_Specification.LevelCallback->GetAbsoluteWidth());
			}
			else if (m_PixelPosition.X >= m_Specification.LevelCallback->GetAbsoluteWidth())
			{
				m_PixelPosition.X = 0 + static_cast<int32_t>(m_Specification.MoveSpeed * ts);
			}
			//update position in tiles
	
			if (m_PixelPosition.X % m_Specification.TileSize == 0)
			{
				m_Position.X = static_cast<int32_t>(std::floor(m_PixelPosition.X / m_Specification.TileSize));
				TRACE("Safe to mode switch!");
				m_SafeToModeSwitchX = true;
			}
			else
			{
				TRACE("Not safe to mode switch. Pixel pos: {}, {}", (int32_t)m_PixelPosition.X, (int32_t)m_PixelPosition.Y);
				m_SafeToModeSwitchX = false;
			}
			if (m_PixelPosition.Y % m_Specification.TileSize == 0)
			{
				m_Position.Y = static_cast<int32_t>(std::floor(m_PixelPosition.Y / m_Specification.TileSize));
				TRACE("Safe to mode switch!");
				m_SafeToModeSwitchY = true;
			}
			else
			{
				TRACE("Not safe to mode switch. Pixel pos: {}, {}", (int32_t)m_PixelPosition.X, (int32_t)m_PixelPosition.Y);
				m_SafeToModeSwitchY = false;
			}

		}
		{
			//update timers and modes ready for next frame only scatter and chase here not frightned(isblue status) or eaten (Eyesonly status this is handled at the top of function currently 
			if (m_ModeTimer > 10.0f)
			{
				m_ModeTimerUp = true;
			}
			if (m_ModeTimerUp && m_SafeToModeSwitchX && m_SafeToModeSwitchY)
			{
				TRACE("Mode Switch");
				if (m_Mode == GhostMode::Chase)
				{
					m_Mode = GhostMode::Scatter;
					m_ModeTimer = 0.0f;
					m_Direction.ReverseDirection();

				}
				else if (m_Mode == GhostMode::Scatter)
				{
					m_Mode = GhostMode::Chase;
					m_ModeTimer = 0.0f;
					m_Direction.ReverseDirection();
				}
				m_ModeTimerUp = false;
			}
			m_ModeTimer += ts;
		}

	}
	void Ghost::OnRender()
	{
		switch (m_Status)
		{
		case GhostStatus::NotStarted:
		case GhostStatus::Running:
			Application::Get().GetRenderer().RenderSprite(m_Specification.MainSprite, m_PixelPosition.X, m_PixelPosition.Y, m_Specification.TileSize);
			break;
		case GhostStatus::IsBlue:
			Application::Get().GetRenderer().RenderSprite(m_Specification.BlueSprite, m_PixelPosition.X, m_PixelPosition.X, m_Specification.TileSize);
			break;
		case GhostStatus::EyesOnly:
			Application::Get().GetRenderer().RenderSprite(m_Specification.EyesSprite, m_PixelPosition.X, m_PixelPosition.X, m_Specification.TileSize);
			break;
		}
	}
	void Ghost::Reset()
	{

	}
	void Ghost::StartGame()
	{
		m_Status = GhostStatus::Running;
	}

	void Ghost::SetPowerPelletActivated()
	{
		m_Status = GhostStatus::IsBlue;
		m_Mode = GhostMode::Scatter;
		m_FrightenedTimer = 10.0f; // ts is in seconds 1 minute 
	}
	void Ghost::SetEaten()
	{
		m_Status = GhostStatus::EyesOnly;
		m_Mode = GhostMode::Eaten;
		m_Target = m_Specification.InitialPosition;
	}

	void Ghost::UpdateTarget()
	{
		if (m_CanUseDoor)
		{
			if (m_Position == m_Target)
			{
				if (m_Specification.DoorPosition == m_Target) //ghost is at the door 
				{
					m_CanUseDoor = false;
				}
				else if (m_Specification.InitialPosition == m_Target) //ghost is back at its starting position
				{
					m_Status = GhostStatus::Running;
					m_Mode = GhostMode::Scatter;
					m_Target = m_Specification.DoorPosition;
					return;
				}
			}
		}
		else
		{
			//scatter mode 
			if (m_Mode == GhostMode::Scatter)
			{
				//The ghost has a specific corner of the map send it there
				m_Target = m_Specification.ScatterPosition;
				TRACE("Scatter mode target set to {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
				return;
			}
			else if (m_Mode == GhostMode::Chase)
			{
				//chase mode is different for each ghost type!
				switch (m_Specification.Type)
				{
				case GhostType::Red: // Targets pacman 
					m_Target = m_Specification.LevelCallback->GetPacmanPosition();
					TRACE("Red Ghost chase mode target set to be: {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
					return;
					break;
				case GhostType::Cyan: // 2nd cell in fromt of the pacman then double the vector from red ghost
				{
					//need to check that selected tile is not a wall and actually reachable 
					auto pmanpos = m_Specification.LevelCallback->GetPacmanPosition();
					auto direction = m_Specification.LevelCallback->GetPacmanDirection();
					pmanpos.X += direction.X * 2;
					pmanpos.Y += direction.Y * 2;
					auto redghostpos = m_Specification.LevelCallback->GetRedGhostPosition();
					int32_t answerX = redghostpos.X - pmanpos.X;
					int32_t answerY = redghostpos.Y - pmanpos.Y;
					m_Target.X = pmanpos.X + (2 * answerX);
					m_Target.Y = pmanpos.Y + (2 * answerY);
					m_Target.X = std::clamp(m_Target.X, static_cast<int32_t>(0), static_cast<int32_t>(m_Specification.LevelCallback->GetLevelWidthInTiles()));
					m_Target.Y = std::clamp(m_Target.Y, static_cast<int32_t>(0), static_cast<int32_t>(m_Specification.LevelCallback->GetLevelHeightInTiles()));
					TRACE("Cyan Ghost chase mode target set to be: {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
					return;
					break;
				}
				case GhostType::Orange: // chase pacman until it gets close(3 tiles) then goes to scatter mode
				{
					auto pacmanposition = m_Specification.LevelCallback->GetPacmanPosition();
					int32_t xdifference = std::abs(pacmanposition.X - m_Position.X);
					int32_t ydifference = std::abs(pacmanposition.Y - m_Position.Y);
					if (xdifference >= 3 || ydifference >= 3)
					{
						if (m_SafeToModeSwitchX && m_SafeToModeSwitchY)
						{
							m_Target = pacmanposition;
							TRACE("Orange Ghost chase mode target set to be: {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
							return;
						}
						else
						{
							TRACE("*******************Orange ghost not safe to change mode. Leave target as was");
							return;	
						}
					}
					else
					{
						if (m_SafeToModeSwitchX && m_SafeToModeSwitchY)
						{
							m_Target = m_Specification.ScatterPosition;
							TRACE("Orange Ghost chase mode target set to be: {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
							return;
						}
						else
						{
							TRACE("####################Orange ghost not safe to change mode. Leave target as was");
							return;
						}

					}
					break;
				}
				case GhostType::Pink: //chase 4 tiles in front of pacman. Need to do clamping as could be possible to get value outside of the map!!
				{
					m_Target = m_Specification.LevelCallback->GetPacmanPosition();
					auto direction = m_Specification.LevelCallback->GetPacmanDirection();
					if (direction.X != 0)
					{
						m_Target.X += direction.X * 4;
					}
					else if (direction.Y != 0)
					{
						m_Target.Y += direction.Y * 4;
					}
					else
					{
						// pacpman isn't moving
						m_Target.X += 4;
					}
					TRACE("Pink Ghost chase mode target set to be: {},{}", (int32_t)m_Target.X, (int32_t)m_Target.Y);
					return;
					break;
				}
				}
			}
		}
	}

	void Ghost::FindPath(const Vec2<int32_t>& starttile)
	{
		//check we are not already at the target
		if (GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles()) == GetTileIndex(m_Target, m_Specification.LevelCallback->GetLevelWidthInTiles()))
		{
			//we are on the target tile so do nothing
			m_TileToMoveToIndex = GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles());
			return;
		}
		//setup for breadth first search
		size_t numberoftiles = m_Specification.LevelCallback->GetLevelWidthInTiles() * m_Specification.LevelCallback->GetLevelHeightInTiles();
		std::queue<uint32_t> queue;
		queue.push(GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles()));
		std::vector<bool> visited(numberoftiles, false);
		visited[GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles())] = true;
		std::vector<int32_t>prev(numberoftiles, -1);

		//search
		while (!queue.empty())
		{
			//need to access using front and then pop from queue to remove it and move to next in queue.
			uint32_t tileindex = queue.front();
			queue.pop();
			const auto& neighbours = m_Specification.LevelCallback->GetAdjacentTileList(tileindex);
			for (const auto& nexttile : neighbours)
			{
				//zero is a valid tile index therefore -1 is the invalid, cannot move there state!  
				if (nexttile != -1 && visited[nexttile] == false)
				{
					queue.push(nexttile);
					visited[nexttile] = true;
					prev[nexttile] = tileindex;
				}
			}
		}
		//construct the path
		std::vector<uint32_t> pathtotarget;
		ASSERT((GetTileIndex(m_Target, m_Specification.LevelCallback->GetLevelWidthInTiles()) <= 441), "Index is invalid");
		for (int32_t at = GetTileIndex(m_Target, m_Specification.LevelCallback->GetLevelWidthInTiles()); at != -1; at = prev[at])
		{
			pathtotarget.emplace_back(at);
		}
		//reverse the path vector 
		std::reverse(pathtotarget.begin(), pathtotarget.end());
		
		//check that the path does actually start at the start point

		if (pathtotarget[0] == GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles()))
		{
			m_TileToMoveToIndex = pathtotarget[1];
		}
		else
		{
			ASSERT(false, "cannot reach the end point from the start");
		}

	}
}