#include "Ghost.h"
#include "Level.h"
#include "Application.h"
#include "Assert.h"
#include "Renderer.h"
#include "utills.h"



#include <queue>

namespace Pman
{
	bool Ghost::TileIsAbleToMoveTo(const Vec2<int32_t>& target) const
	{
		if (target.X >= 0 && target.X < m_Specification.LevelCallback->GetLevelWidthInTiles() && target.Y >= 0 && target.Y < m_Specification.LevelCallback->GetLevelHeightInTiles())
		{
			const Tile& tile = m_Specification.LevelCallback->GetTile(GetTileIndex(target, m_Specification.LevelCallback->GetLevelWidthInTiles()));
			if (tile.GetTileType() == TileType::Gem || tile.GetTileType() == TileType::PowerPellet || tile.GetTileType() == TileType::Empty)
			{
				return true;
			}
		}
		return false;
	}
	
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
			m_PixelPosition.X = m_PixelPosition.X + static_cast<int32_t>(m_Direction.X * (m_Specification.MoveSpeed * ts));
			m_PixelPosition.Y = m_PixelPosition.Y + static_cast<int32_t>(m_Direction.Y * (m_Specification.MoveSpeed * ts));
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
				m_SafeToModeSwitchX = true;
			}
			else
			{
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
				TRACE("Not safe to mode switch. Pixel pos: {}", m_PixelPosition);
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
			Application::Get().GetRenderer().RenderSprite(m_Specification.BlueSprite, m_PixelPosition.X, m_PixelPosition.Y, m_Specification.TileSize);
			break;
		case GhostStatus::EyesOnly:
			Application::Get().GetRenderer().RenderSprite(m_Specification.EyesSprite, m_PixelPosition.X, m_PixelPosition.Y, m_Specification.TileSize);
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
		m_FrightenedTimer = 15.0f;

	}
	void Ghost::SetEaten()
	{
		m_Status = GhostStatus::EyesOnly;
		m_Mode = GhostMode::Eaten;
		m_Target = m_Specification.InitialPosition;
	}

	void Ghost::UpdateTarget()
	{
		static bool tooclose = false;
		//scatter mode 
		if (m_Mode == GhostMode::Scatter)
		{
			//The ghost has a specific corner of the map send it there
			m_Target = m_Specification.ScatterPosition;
			TRACE("Scatter mode target set to {}", m_Target);
			tooclose = false;
			return;
		}
		else if (m_Mode == GhostMode::Chase)
		{
			//chase mode is different for each ghost type!
			switch (m_Specification.Type)
			{
			case GhostType::Red: // Targets pacman 
				m_Target = m_Specification.LevelCallback->GetPacmanPosition();
				TRACE("Red Ghost chase mode target set to be: {}", m_Target);
				return;
				break;
			case GhostType::Cyan: // yes I know the prototype is 2nd cell in fromt of the pacman then double the vector from red ghost but I can not get this to work reliably so am changing it to be the same as red ghost ie. target pacman
			{
				m_Target = m_Specification.LevelCallback->GetPacmanPosition();
				TRACE("Cyan Ghost chase mode target set to be: {}", m_Target);
				return;
				break;
			}
			case GhostType::Orange: // chase pacman until it gets close(3 tiles) then goes to scatter mode
			{
				auto pacmanposition = m_Specification.LevelCallback->GetPacmanPosition();
				Vec2<int32_t> diff = pacmanposition - m_Position;
				float difflength = diff.Length();
				if (difflength <= 3.0f)
				{
					tooclose = true;
				}
				else if(m_Position == m_Specification.ScatterPosition)
				{
					tooclose = false;
				}
				if (m_SafeToModeSwitchX && m_SafeToModeSwitchY)
				{
					if (!tooclose)
					{
						m_Target = pacmanposition;
					}
					else
					{
						m_Target = m_Specification.ScatterPosition;
					}
				}
				TRACE("Orange ghost chase mode target set to be {}", m_Target);
				break;
			}
			case GhostType::Pink: //chase 4 tiles in front of pacman. Need to do clamping as could be possible to get value outside of the map!!
			{
				auto pmanpos = m_Specification.LevelCallback->GetPacmanPosition();
				auto direction = m_Specification.LevelCallback->GetPacmanDirection();
				if (direction.X != 0)
				{
					m_Target.X = pmanpos.X + (direction.X * 4);
					m_Target.Y = pmanpos.Y;
				}
				else if (direction.Y != 0)
				{
					m_Target.Y = pmanpos.Y + (direction.Y * 4);
					m_Target.X = pmanpos.X;
				}
				else
				{
					// pacpman isn't moving
					m_Target.X = pmanpos.X + 4;
					m_Target.Y = pmanpos.Y;
				}
				//check that the tile is vaild if not reduce number of tile in front so that it is valid
				int32_t adjustment = 3;
				while (!TileIsAbleToMoveTo(m_Target))
				{
					if (direction.X != 0)
					{
						m_Target.X = pmanpos.X + (direction.X * adjustment);
					}
					else if (direction.Y != 0)
					{
						m_Target.Y = pmanpos.Y + (direction.Y * adjustment);
					}
					else
					{
						m_Target.X = pmanpos.X + adjustment;
					}
					adjustment--;
				}
				TRACE("Pink Ghost chase mode target set to be: {}", m_Target);
				return;
				break;
			}
			}
			
		}
		else if (m_Mode == GhostMode::Eaten)
		{
			if (m_Specification.InitialPosition == m_Position)
			{
				m_Status = GhostStatus::Running;
				m_Mode = GhostMode::Scatter;
				m_Target = m_Specification.ScatterPosition;
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