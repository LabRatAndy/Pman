#include "Level.h"
#include "Assert.h"
#include "Application.h"
#include "Renderer.h"
#include "utills.h"

#include <raylib.h>


namespace Pman
{
	template<typename T>
	struct Rect
	{
		T Left;
		T Right;
		T Top;
		T Bottom;

		Rect(T left, T right, T top, T bottom): Left(left),Right(right), Top(top), Bottom(bottom)
		{
		}
	};

	static bool CheckCollision(const Rect<int32_t>& player, const Rect<int32_t>& tile)
	{
		return !(player.Right <= tile.Left || player.Left >= tile.Right || player.Bottom <= tile.Top || player.Top >= tile.Bottom);
	}

	Level::Level(const uint32_t tilesize) : m_TileSize(tilesize)
	{

	}
	Level::~Level()
	{
		delete m_Player;
		delete m_CyanGhost;
		delete m_RedGhost;
		delete m_PinkGhost;
		delete m_OrangeGhost;

		//Sprites
		delete m_RedGhostSprite;
		delete m_BlueGhostSprite;
		delete m_CyanGhostSprite;
		delete m_GhostEyesSprite;
		delete m_OrangeGhostSprite;
		delete m_PinkGhostSprite;
		delete m_PlayerSprite;
		delete m_PowerPelletSprite;
		delete m_GemSprite;
	}

	void Level::LoadLevel(const std::vector<std::string>& leveldata)
	{
		ASSERT(m_TileSize, "Tilesize cannot be 0!");
		ASSERT(leveldata.size() != 0, "Level data cannot be empty!");

		//make sure the sprites are loaded 
		Renderer& renderer = Application::Get().GetRenderer();
		m_RedGhostSprite = renderer.LoadSprite("Assets/RedGhost.png", SpriteType::RedGhost);
		ASSERT(m_RedGhostSprite, "Failed to load Red Ghost sprite");
		m_CyanGhostSprite = renderer.LoadSprite("Assets/CyanGhost.png", SpriteType::CyanGhost);
		ASSERT(m_CyanGhostSprite, "Failed to load Cyan Ghost sprite");
		m_BlueGhostSprite = renderer.LoadSprite("Assets/BlueGhost.png", SpriteType::BlueGhost);
		ASSERT(m_BlueGhostSprite, "Failed to load Blue Ghost sprite");
		m_OrangeGhostSprite = renderer.LoadSprite("Assets/OrangeGhost.png", SpriteType::OrangeGhost);
		ASSERT(m_OrangeGhostSprite, "Failed to load Orange Ghost sprite");
		m_PinkGhostSprite = renderer.LoadSprite("Assets/PinkGhost.png", SpriteType::PinkGhost);
		ASSERT(m_PinkGhostSprite, "Failed to load Pink Ghost sprite");
		m_GemSprite = renderer.LoadSprite("Assets/Gem.png", SpriteType::Gem);
		ASSERT(m_GemSprite, "Failed to load Gem sprite");
		m_PowerPelletSprite = renderer.LoadSprite("Assets/PowerPellet.png", SpriteType::PowerPellet);
		ASSERT(m_PowerPelletSprite, "Failed to load Powerpellet sprite");
		m_PlayerSprite = renderer.LoadSprite("Assets/Player.png", SpriteType::Player);
		ASSERT(m_PlayerSprite, "Failed to load Player sprite");
		m_GhostEyesSprite = renderer.LoadSprite("Assets/GhostEyes.png", SpriteType::GhostEyes);
		ASSERT(m_GhostEyesSprite, "Failed to load Ghost eyes sprite");

		m_Tiles.clear();
		//workout size of level
		uint32_t rows = (uint32_t)leveldata.size();
		uint32_t columns = (uint32_t)leveldata[0].length(); //all rows must be this length or we will assert
		m_LevelHeight = rows;
		m_LevelWidth = columns;
		m_Tiles.reserve(m_LevelWidth * m_LevelHeight);
		uint32_t tilecount = 0;
		
		for (size_t row = 0; row < rows; row++)
		{
			ASSERT((leveldata[row].length() == columns), "All rows must be the same length as the first");
			for (size_t column = 0; column < columns; column++)
			{
				ASSERT((tilecount < rows * columns), "Error !!!! Too many tiles have been processed!");
				auto it = m_Tiles.begin() + tilecount;
				size_t doorx = 0;
				size_t doory = 0;
				switch (leveldata[row][column])
				{
				case '#': //wall
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Wall;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
					break;
				}
				case '=': //door
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Door;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
					doorx = column;
					doory = row;
					break;
				}
				case '.': //gem
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Gem;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					spec.Texture = m_GemSprite;
					m_Tiles.emplace(it, spec);
					break;
				}
				case 'o': //powerpellet
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::PowerPellet;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					spec.Texture = m_PowerPelletSprite;
					m_Tiles.emplace(it, spec);
					break;
				}
				case '0': //red ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_RedGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 61.0f;
					gspec.ScatterPosition = { 18,1 }; 
					gspec.TileSize = m_TileSize;
					gspec.DoorPosition = { static_cast<int32_t>(doorx),static_cast<int32_t>(doory) };
					gspec.Type = GhostType::Red;
					m_RedGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '1': //pink ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_PinkGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 61.0f;
					gspec.ScatterPosition = { 2,1 };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Pink;
					gspec.DoorPosition = { static_cast<int32_t>(doorx),static_cast<int32_t>(doory) };
					m_PinkGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '2': //cyan ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_CyanGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 61.0f;
					gspec.ScatterPosition = { 18,19 };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Cyan;
					gspec.DoorPosition = { static_cast<int32_t>(doorx),static_cast<int32_t>(doory) };
					m_CyanGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '3': //orange ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_OrangeGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 60.0f;
					gspec.ScatterPosition = { 2,19 };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Orange;
					gspec.DoorPosition = { static_cast<int32_t>(doorx),static_cast<int32_t>(doory) };
					m_OrangeGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
				}
					break;
				case 'P': // player
				{
					ASSERT(!m_Player, "Already have a player!");
					PlayerSpecification pspec{};
					pspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					pspec.MoveSpeed = 10.0f;
					pspec.PlayerLives = 3;
					pspec.PlayerSprite = m_PlayerSprite;
					pspec.TileSize = m_TileSize;
					pspec.LevelWidth = m_LevelWidth;
					pspec.LevelHeight = m_LevelHeight;
					pspec.LevelCallback = this;
					m_Player = new Player(pspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
				}
					break;
				case ' ':
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
					break;
				}
				case 'i':
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Invaild;
					spec.XPosition = static_cast<int32_t>(column);
					spec.YPosition = static_cast<int32_t>(row);
					m_Tiles.emplace(it, spec);
					break;
				}
				default:
					ERROR("Invalid level data supplied at col: {}, row: {}", (uint32_t)column, (uint32_t)row);
					break;
				}
				tilecount++;
				
			}
		}
		ProcessRectangles();
		ProcessAdjacentTiles();
	}
	void Level::OnUpdate(float ts)
	{
		m_Player->OnUpdate(ts);
		//m_CyanGhost->OnUpdate(ts);
		//m_RedGhost->OnUpdate(ts);
		//m_PinkGhost->OnUpdate(ts);
		//m_OrangeGhost->OnUpdate(ts);
	}
	void Level::OnRender()
	{
		Renderer& renderer = Application::Get().GetRenderer();
		// render the level walls, gems and powerpellets first
		for (const Tile& tile : m_Tiles)
		{
			if (tile.GetTileType() != TileType::Empty)
			{
				renderer.RenderTile(tile);
			}
		}
		//Then render player and ghosts
		m_Player->OnRender();
		m_CyanGhost->OnRender();
		m_RedGhost->OnRender();
		m_PinkGhost->OnRender();
		m_OrangeGhost->OnRender();
	}
#pragma warning (push)
#pragma warning (disable : 4100)
	/// <summary>
	/// Checks if you will collide with a wall 
	/// </summary>
	/// <param name="position">the pixel position of the player</param>
	/// <param name="direction">The direction vector -1 is up/ left and 1 is right/down, 0 no movement in that axis</param>
	/// <param name="canusedoor">is able to use the ghost house door</param>
	/// <returns>true on collision with wall</returns>
	bool Level::CollideWithWall(const Vec2<int32_t>& position, const Vec2<int32_t>& direction)
	{
		TRACE("\nNew collsion check!");
		// Get the player's bounding box in world space
		Rect<int32_t> playerrect = { position.X,position.X + m_TileSize,position.Y,position.Y + m_TileSize };
		
		// Calculate the tile coordinates the player is interacting with
		int32_t tileXLeft = playerrect.Left / m_TileSize;
		int32_t tileXRight = (playerrect.Right - 1) / m_TileSize;
		int32_t tileYTop = playerrect.Top / m_TileSize;
		int32_t tileYBottom = (playerrect.Bottom - 1) / m_TileSize;
		bool retval = false;
		// Iterate over the tiles the player's bounding box overlaps with
		for (int32_t x = tileXLeft; x <= tileXRight; ++x)
		{
			for (int32_t y = tileYTop; y <= tileYBottom; ++y)
			{
				// Get the tile boundaries in pixels
				Rect tilerect = { x * m_TileSize,(x + 1) * m_TileSize,y * m_TileSize,(y + 1) * m_TileSize };
				
				TRACE("Player Position: {}, {}", (int32_t)position.X, (int32_t)position.Y);
				TRACE("Tile Bounds: Left {}, Right {}, Top {}, Bottom {}", (int32_t)tilerect.Left, (int32_t)tilerect.Right, (int32_t)tilerect.Top, (int32_t)tilerect.Bottom);

				// Check if the player's bounding box intersects with the tile
				if (CheckCollision(playerrect, tilerect))
				{
					TRACE("Collsion detectected with tile index: {} at coordinates: {},{}", (uint32_t)GetTileIndex({ x, y }, m_LevelWidth), (int32_t)x, (int32_t)y);
					// If tile is a wall, prevent movement in that direction
					if (m_Tiles[GetTileIndex({ x, y }, m_LevelWidth)].GetTileType() == TileType::Wall || m_Tiles[GetTileIndex({ x, y }, m_LevelWidth)].GetTileType() == TileType::Door)
					{
						TRACE("Tile collided with is a wall");
						//check collision left	
						if (playerrect.Left < tilerect.Right)
						{
							if (direction.X == -1)
							{
								retval = true;
							}
						}
						//check collision right
						if (playerrect.Right > tilerect.Left)
						{
							if (direction.X == 1)
							{
								retval = true;
							}
						}
						//check collision above
						if (playerrect.Top < tilerect.Bottom)
						{
							if (direction.Y == -1)
							{
								retval = true;
							}
						}
						//check collision below
						if (playerrect.Bottom > tilerect.Top)
						{
							if (direction.Y == 1)
							{
								retval = true;
							}
						}
					}
				}
			}
		}
		return retval;
	}
#pragma warning(pop)
	
	bool Level::CollectGem(const Vec2<int32_t>& position)
	{
		size_t tileindex = GetTileArrayIndexofTile(position.X, position.Y, m_LevelWidth);
		if (m_Tiles[tileindex].GetTileType() == TileType::Gem)
		{
			m_Tiles[tileindex].CollectGem();
			return true;
		}
		return false;
	}
	bool Level::CollectPowerPellet(const Vec2<int32_t>& position)
	{
		size_t tileindex = GetTileArrayIndexofTile(position.X, position.Y, m_LevelWidth);
		if (m_Tiles[tileindex].GetTileType() == TileType::PowerPellet)
		{
			m_Tiles[tileindex].CollectPowerPellet();
			m_CyanGhost->SetPowerPelletActivated();
			m_RedGhost->SetPowerPelletActivated();
			m_PinkGhost->SetPowerPelletActivated();
			m_OrangeGhost->SetPowerPelletActivated();
			return true;
		}
		return false;
	}
	void Level::ProcessRectangles()
	{
		for (const Tile& tile : m_Tiles)
		{
			switch (tile.GetTileType())
			{
			case TileType::Wall:
			{
				Rectangle rect;
				rect.x = (float)tile.GetAbsoluteXPosition();
				rect.y = (float)tile.GetAbsoluteYPosition();
				rect.width = (float)m_TileSize - 4.0f;
				rect.height = (float)m_TileSize - 4.0f;
				m_WallsRectangles.emplace_back(rect);
				break;
			}
			case TileType::Door:
			{
				Rectangle rect;
				rect.x = (float)tile.GetAbsoluteXPosition();
				rect.y = (float)tile.GetAbsoluteYPosition();
				rect.width = (float)m_TileSize - 4.0f;
				rect.height = (float)m_TileSize - 4.0f;
				m_DoorRectangles.emplace_back(rect);
				break;
			}
			case TileType::Gem:
			{
				Rectangle rect;
				rect.x = (float)tile.GetAbsoluteXPosition();
				rect.y = (float)tile.GetAbsoluteYPosition();
				rect.width = (float)m_TileSize;
				rect.height = (float)m_TileSize;
				m_GemsRectangles.emplace_back(rect);
				break;
			}
			case TileType::PowerPellet:
			{
				Rectangle rect;
				rect.x = (float)tile.GetAbsoluteXPosition();
				rect.y = (float)tile.GetAbsoluteYPosition();
				rect.width = (float)m_TileSize;
				rect.height = (float)m_TileSize;
				m_PowerPelletRectangles.emplace_back(rect);
				break;
			}
			}
		}
	}
	bool Level::IsTileAWall(const float& x, const float& y, const Vec2<int32_t>& direction, const bool canusedoor) const
	{
		for (const auto& wall : m_WallsRectangles)
		{
			if (direction.X == -1)
			{
				Rectangle rect{ static_cast<float>(x - m_TileSize),y,static_cast<float>(m_TileSize + 1), 5.0f };
				if (CheckCollisionRecs(rect, wall))
				{
					return true;
				}
			}
			else if (direction.X == 1)
			{
				Rectangle rect{ x,y,static_cast<float>(m_TileSize + 1), 5.0f };
				if (CheckCollisionRecs(rect, wall))
				{
					return true;
				}
			}
			else if (direction.Y == -1)
			{
				Rectangle rect{ x,static_cast<float>(y - m_TileSize),5.0f,static_cast<float>(m_TileSize + 1) };
				if (CheckCollisionRecs(rect, wall))
				{
					return true;
				}
			}
			else if (direction.Y == 1)
			{
				Rectangle rect{ x, y,5.0f,static_cast<float>(m_TileSize + 1) };
				if (CheckCollisionRecs(rect, wall))
				{
					return true;
				}
			}
		}
		
		for (const auto& door : m_DoorRectangles)
		{
			if (direction.Y == -1)
			{
				Rectangle rect{ x,static_cast<float>(y - m_TileSize),5.0f,static_cast<float>(m_TileSize + 1) };
				if (CheckCollisionRecs(rect, door))
				{
					if (canusedoor)
					{
						return false;
					}
					return true;
				}
			}
			else if (direction.Y == 1)
			{
				Rectangle rect{ x,y,5.0f,static_cast<float>(m_TileSize + 1) };
				if (CheckCollisionRecs(rect, door))
				{
					if (canusedoor)
					{
						return false;
					}
					return true;
				}
			}
		}
		
		return false;
	}
	void Level::StartGame()
	{
		m_Player->StartGame();
		m_RedGhost->StartGame();
		m_PinkGhost->StartGame();
		m_CyanGhost->StartGame();
		m_OrangeGhost->StartGame();
	}
	void Level::ProcessAdjacentTiles()
	{
		size_t column = 0;
		size_t line = 0;
		for (size_t tileindex = 0; tileindex < m_Tiles.size(); tileindex++)
		{
			//zero is a valid tile index therefore -1 is the invalid cannot move there!  
			int32_t left = -1;
			int32_t right = -1;
			int32_t top = -1;
			int32_t bottom = -1;
			if (column == m_LevelWidth)
			{
				line++;
				column = 0;
			}
			//ASSERT((line != 5), "test");
			if (column >= 1 && column < m_LevelWidth)
			{
				if (m_Tiles[tileindex - 1].GetTileType() != TileType::Wall)
				{
					left = static_cast<int32_t>(tileindex - 1);
				}
			}
			if (column >= 0 && column < m_LevelWidth - 1)
			{
				if (m_Tiles[tileindex + 1].GetTileType() != TileType::Wall)
				{
					right = static_cast<int32_t>(tileindex + 1);
				}
			}

			if (line>=1)
			{
				if (m_Tiles[(tileindex - m_LevelWidth)].GetTileType() != TileType::Wall)
				{
					top = static_cast<int32_t>(tileindex - m_LevelWidth);
				}
			}
			if (line < m_LevelHeight - 1)
			{
				if (m_Tiles[(tileindex + m_LevelWidth)].GetTileType() != TileType::Wall)
				{
					bottom = static_cast<int32_t>(tileindex + m_LevelWidth);
				}
			}
			m_Tiles[tileindex].SetAdjacentTile(left, right, top, bottom);
			column++;
		}


	}
}