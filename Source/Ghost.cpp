#include "Ghost.h"
#include "Level.h"
#include "Application.h"
#include "Assert.h"
#include "Renderer.h"

//TODO remove me!
#include <raylib.h>


#include <queue>

namespace Pman
{
	static uint32_t InTileIndex(float position, uint32_t tilesize, float& leftover)
	{
		leftover = std::fmod(position, (float)tilesize);
		position = position - leftover;
		return (position / tilesize);
	}
	static float PortionInTile(float position, uint32_t tilesize)
	{
		return std::fmod(position, (float)tilesize);
	}
	static uint32_t GetTileIndex(const Vec2<uint32_t>& tile, uint32_t levelwidth)
	{
		return (tile.Y * levelwidth) + tile.X;
	}
	static uint32_t GetTileIndexFromPosition(const Vec2<float>& position, const uint32_t& tilesize, const uint32_t& levelwidth)
	{
		float leftoverx = 0.0f;
		float leftovery = 0.0f;
		uint32_t tilex = InTileIndex(position.X, tilesize, leftoverx);
		uint32_t tiley = InTileIndex(position.Y, tilesize, leftovery);
		return (tiley * levelwidth) + tilex;
	}
	Ghost::Ghost(const GhostSpecification& spec) : m_Status(GhostStatus::NotStarted), m_Mode(GhostMode::Scatter), m_Specification(spec), m_Position(spec.InitialPosition)
	{
	}
	Ghost::~Ghost()
	{
	}
	void Ghost::OnUpdate(float ts)
	{
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
		//work out direction to take
		{
			if (ts < 0.001)
			{
				ts = 0.0166668f;
			}
			//m_Target = { (float)9 * m_Specification.TileSize,(float)17 * m_Specification.TileSize };
			Vec2<uint32_t> currentcoordinates = GetTileCoordinate();
			FindPath(currentcoordinates);
			const auto& tile = m_Specification.LevelCallback->GetTile(m_TileToMoveToIndex);
			if (currentcoordinates.X > tile.GetTileXPosition() && m_Direction.X != 1)
			{
				//need togo left
				m_Direction.X = -1;
				m_Direction.Y = 0;
			}
			else if (currentcoordinates.X < tile.GetTileXPosition() && m_Direction.X != -1)
			{
				//need togo right
				m_Direction.X = 1;
				m_Direction.Y = 0;
			}
			else if (currentcoordinates.Y > tile.GetTileYPosition() && m_Direction.Y != 1)
			{
				//need togo up
				m_Direction.X = 0;
				m_Direction.Y = -1;
			}
			else if (currentcoordinates.Y < tile.GetTileYPosition() && m_Direction.Y != -1)
			{
				//need togo down
				m_Direction.X = 0;
				m_Direction.Y = 1;
			}
			if (GetTileIndexFromPosition(m_Target,m_Specification.TileSize,m_Specification.LevelCallback->GetLevelWidthInTiles()) == m_TileToMoveToIndex)
			{
				//stop moveing as we are all ready at target
				m_Direction.X = 0;
				m_Direction.Y = 0;
			}
		}
		{
			//now move the ghost 
			m_Position.X = m_Position.X + (m_Direction.X * (m_Specification.MoveSpeed * ts));
			m_Position.Y = m_Position.Y + (m_Direction.Y * (m_Specification.MoveSpeed * ts));
			//check for going though the tunnel from 1 size to the other
			if (m_Position.X <= 0)
			{
				m_Position.X = (m_Specification.MoveSpeed * ts) - m_Specification.LevelCallback->GetAbsoluteWidth();
			}
			else if (m_Position.X >= m_Specification.LevelCallback->GetAbsoluteWidth())
			{
				m_Position.X = 0 + (m_Specification.MoveSpeed * ts);
			}
		}
		{
			//update timers and modes ready for next frame only scatter and chase here not frightned(isblue status) or eaten (Eyesonly status this is handled at the top of function currently 
			if (m_ModeTimer > 5.0f) //not mode switching after 30 seconds not prototypical, for testing 
			{
				std::cout << "Mode switch!" << std::endl;
				if (m_Mode == GhostMode::Chase)
				{
					m_Mode = GhostMode::Scatter;
					m_ModeTimer = 0.0f;
				}
				else if (m_Mode == GhostMode::Scatter)
				{
					m_Mode = GhostMode::Chase;
					m_ModeTimer = 0.0f;
				}
			}
			m_ModeTimer += ts;
		}

	}
	void Ghost::OnRender()
	{
		//DrawPixel(m_Position.X, m_Position.Y, YELLOW);
		switch (m_Status)
		{
		case GhostStatus::NotStarted:
		case GhostStatus::Running:
			Application::Get().GetRenderer().RenderSprite(m_Specification.MainSprite, m_Position.X, m_Position.Y, m_Specification.TileSize);
			break;
		case GhostStatus::IsBlue:
			Application::Get().GetRenderer().RenderSprite(m_Specification.BlueSprite, m_Position.X, m_Position.Y, m_Specification.TileSize);
			break;
		case GhostStatus::EyesOnly:
			Application::Get().GetRenderer().RenderSprite(m_Specification.EyesSprite, m_Position.X, m_Position.Y, m_Specification.TileSize);
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

	float Ghost::GetDistanceToTarget(const Vec2<int32_t>& direction, float ts)
	{
		Vec2<float> position{ 0.0f,0.0f };
		position.X = m_Position.X + (direction.X * (m_Specification.MoveSpeed * ts));
		position.Y = m_Position.Y + (direction.Y * (m_Specification.MoveSpeed * ts));
		float diffx = position.X - m_Target.X;
		float diffy = position.Y - m_Target.Y;
		float hypsquared = (diffx * diffx) + (diffy * diffy);
		return std::sqrt(hypsquared);
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
				return;
			}
			else if (m_Mode == GhostMode::Chase)
			{
				//chase mode is different for each ghost type!
				switch (m_Specification.Type)
				{
				case GhostType::Red: // Targets pacman 
					m_Target = m_Specification.LevelCallback->GetPacmanPosition();
					return;
					break;
				case GhostType::Cyan: // 2nd cell in fromt of the pacman then double the vector from red ghost
				{
					m_Target = m_Specification.LevelCallback->GetPacmanPosition();
					auto direction = m_Specification.LevelCallback->GetPacmanDirection();
					m_Target.X += direction.X * (m_Specification.TileSize * 2);
					m_Target.Y += direction.Y * (m_Specification.TileSize * 2);
					auto redghostpos = m_Specification.LevelCallback->GetRedGhostPosition();
					m_Target.X += m_Target.X - redghostpos.X;
					m_Target.Y += m_Target.Y - redghostpos.Y;
					return;
					break;
				}
				case GhostType::Orange: // chase pacman until it gets close(3 tiles) then goes to scatter mode
				{
					float mindistance = 3 * m_Specification.TileSize;
					auto pacmanposition = m_Specification.LevelCallback->GetPacmanPosition();
					float distancetopacman = std::sqrt(std::pow(m_Position.X - pacmanposition.X, 2) + std::pow(m_Position.Y - pacmanposition.Y, 2));
					if (mindistance <= distancetopacman)
					{
						m_Target = pacmanposition;
						return;
					}
					else
					{
						m_Target = m_Specification.ScatterPosition;
						return;
					}
					break;
				}
				case GhostType::Pink: //chase 4 tiles in front of pacman. Need to do clamping as could be possible to get value outside of the map!!
				{
					m_Target = m_Specification.LevelCallback->GetPacmanPosition();
					auto direction = m_Specification.LevelCallback->GetPacmanDirection();
					m_Target.X += direction.X * (m_Specification.TileSize * 4);
					m_Target.Y += direction.Y * (m_Specification.TileSize * 4);
					return;
					break;
				}
				}
			}
		}
	}
	Vec2<uint32_t> Ghost::GetTileCoordinate() const
	{
		
		float leftoverX = 0;
		float leftovery = 0;
		uint32_t tilex = InTileIndex(m_Position.X, m_Specification.TileSize, leftoverX);
		uint32_t tiley = InTileIndex(m_Position.Y, m_Specification.TileSize, leftovery);
		if (leftoverX > 30 && m_Direction.X == 1)
		{
			tilex++;
		}
		if (leftoverX > 1 && m_Direction.X == -1)
		{
			tilex++;
		}
		if (leftovery > 1 && m_Direction.Y == -1)
		{
			tiley++;
		}
		if (leftovery > 30 && m_Direction.Y == 1)
		{
			//tiley--;
		}
		return { tilex,tiley };
	}
	void Ghost::FindPath(const Vec2<uint32_t>& starttile)
	{
		//check we are not already at the target
		if (GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles()) == GetTileIndexFromPosition(m_Target, m_Specification.TileSize, m_Specification.LevelCallback->GetLevelWidthInTiles()))
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
				if (nexttile != -1 && visited[nexttile] == false)
				{
					queue.push(nexttile);
					visited[nexttile] = true;
					prev[nexttile] = tileindex;
				}
			}
		}
		//construct the path
		std::stack<uint32_t> pathtotarget;
		for (int32_t at = GetTileIndexFromPosition(m_Target, m_Specification.TileSize, m_Specification.LevelCallback->GetLevelWidthInTiles()); at != -1; at = prev[at])
		{
			pathtotarget.push(at);
		}
		//check that the path does actually start at the start point

		if (pathtotarget.top() == GetTileIndex(starttile, m_Specification.LevelCallback->GetLevelWidthInTiles()))
		{
	
			//set m_TileToMoveTo
			pathtotarget.pop();
			if (!pathtotarget.empty())
			{
				m_TileToMoveToIndex = pathtotarget.top();
			}
		}
		else
			ASSERT(false, "cannot reach the end point from the start");
	}
}