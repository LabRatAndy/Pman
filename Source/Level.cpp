#include "Level.h"
#include "Assert.h"
#include "Application.h"
#include "Renderer.h"

#include <raylib.h>

namespace Pman
{
	static size_t GetTileArrayIndexofTile(uint32_t x, uint32_t y, uint32_t levelwidth)
	{
		return (y * levelwidth) + x;
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
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
					break;
				}
				case '=': //door
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Door;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
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
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					spec.Texture = m_GemSprite;
					m_Tiles.emplace(it, spec);
					break;
				}
				case 'o': //powerpellet
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::PowerPellet;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
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
					gspec.InitialPosition = { (float)(column * m_TileSize),(float)(row * m_TileSize) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 20.0f;
					gspec.ScatterPosition = { (float)(18 * m_TileSize),(float)(1 * m_TileSize) };
					gspec.TileSize = m_TileSize;
					gspec.DoorPosition = { (float)doorx * m_TileSize, (float)doory * m_TileSize };
					gspec.Type = GhostType::Red;
					m_RedGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '1': //pink ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_PinkGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { (float)(column * m_TileSize),(float)(row * m_TileSize) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 10.0f;
					gspec.ScatterPosition = { (float)2 * m_TileSize,(float)1 * m_TileSize };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Pink;
					gspec.DoorPosition = { (float)doorx * m_TileSize, (float)doory * m_TileSize };
					m_PinkGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '2': //cyan ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_CyanGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { (float)(column * m_TileSize),(float)(row * m_TileSize) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 10.0f;
					gspec.ScatterPosition = { (float)18 * m_TileSize,(float)19 * m_TileSize };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Cyan;
					gspec.DoorPosition = { (float)doorx * m_TileSize, (float)doory * m_TileSize };
					m_CyanGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
				}
					break;
				case '3': //orange ghost
				{
					GhostSpecification gspec{};
					gspec.MainSprite = m_OrangeGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { (float)(column * m_TileSize),(float)(row * m_TileSize) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 10.0f;
					gspec.ScatterPosition = { (float)2 * m_TileSize,(float)19 * m_TileSize };
					gspec.TileSize = m_TileSize;
					gspec.Type = GhostType::Orange;
					gspec.DoorPosition = { (float)doorx * m_TileSize, (float)doory * m_TileSize };
					m_OrangeGhost = new Ghost(gspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
				}
					break;
				case 'P': // player
				{
					ASSERT(!m_Player, "Already have a player!");
					PlayerSpecification pspec{};
					pspec.InitialPosition = { (float)(column * m_TileSize),(float)(row * m_TileSize) };
					pspec.MoveSpeed = 50.0f;
					pspec.PlayerLives = 3;
					pspec.PlayerSprite = m_PlayerSprite;
					pspec.TileSize = m_TileSize;
					pspec.LevelCallback = this;
					m_Player = new Player(pspec);
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
				}
					break;
				case ' ':
				{
					TileSpecification spec{};
					spec.TileSize = m_TileSize;
					spec.Type = TileType::Empty;
					spec.XPosition = static_cast<uint32_t>(column * m_TileSize);
					spec.YPosition = static_cast<uint32_t>(row * m_TileSize);
					m_Tiles.emplace(it, spec);
					break;
				}
				default:
					ASSERT(false, "Invalid level data supplied!");
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
		m_RedGhost->OnUpdate(ts);
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
	bool Level::CollideWithWall(const Vec2<uint32_t>& position, const Vec2<int32_t>& direction, bool canusedoor)
	{
		//check we are still within bounds
		ASSERT((position.X >= 0), "next tile is less than 0");
		ASSERT((position.X <= m_LevelWidth * m_TileSize), "next tile is more than width of level");
		ASSERT((position.Y >= 0), "next tile is less than 0");
		ASSERT((position.Y <= m_LevelHeight * m_TileSize), "next tile is more than height of level");
		Rectangle testrect;
		testrect.x = (float)position.X;
		testrect.y = (float)position.Y;
		testrect.width = (float)(m_TileSize - 4);
		testrect.height = static_cast<float>(m_TileSize - 4);
		for (const Rectangle& wallrect : m_WallsRectangles)
		{
			if (CheckCollisionRecs(testrect, wallrect))
			{
				std::cout << "Test rectangle x,y,w,h: " << testrect.x << "," << testrect.y << "," << testrect.width << "," << testrect.height << std::endl;
				std::cout << "Wall rectangle x,y,w,h: " << wallrect.x << "," << wallrect.y << "," << wallrect.width << "," << wallrect.height << std::endl;
				if (direction.X == -1 && testrect.x >= wallrect.x)
				{
					return true;
				}
				else if (direction.X == 1 && testrect.x <= wallrect.x)
				{
					return true;
				}
				else if (direction.Y == -1 && testrect.y >= wallrect.y)
				{
					return true;
				}
				else if (direction.Y == 1 && testrect.y <= wallrect.y)
				{
					return true;
				}
			}
		}
		//door check
		if (!canusedoor)
		{
			for (const Rectangle& doorrect : m_DoorRectangles)
			{
				if (CheckCollisionRecs(testrect, doorrect))
				{
					//only need up/down check as can't approach the door from the side
					if (direction.Y == -1 && testrect.y >= doorrect.y)
					{
						return true;
					}
					else if (direction.Y == 1 && testrect.y <= doorrect.y)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool Level::CollideWithWall(const uint32_t& tilex, const uint32_t& tiley, const Vec2<int32_t>& direction, const bool& canusedoor)
	{
		size_t tileindex = -1;
		if (direction.X == -1)
		{
			tileindex = GetTileArrayIndexofTile(tilex - 1, tiley, m_LevelWidth);
		}
		else if (direction.X == 1)
		{
			tileindex = GetTileArrayIndexofTile(tilex + 1, tiley, m_LevelWidth);
		}
		else if (direction.Y == -1)
		{
			tileindex = GetTileArrayIndexofTile(tilex, tiley - 1, m_LevelWidth);
		}
		else if (direction.Y == 1)
		{
			tileindex = GetTileArrayIndexofTile(tilex, tiley + 1, m_LevelWidth);
		}
		switch (m_Tiles[tileindex].GetTileType())
		{
		case TileType::Empty:
		case TileType::Gem:
		case TileType::PowerPellet:
			return false;
			break;
		case TileType::Wall:
			return true;
			break;
		case TileType::Door:
			if (canusedoor)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	bool Level::CollectGem(const Vec2<float>& position)
	{
		uint32_t x = (uint32_t)std::round((position.X / (float)m_TileSize));
		uint32_t y = (uint32_t)std::round((position.Y / (float)m_TileSize));
		size_t tileindex = GetTileArrayIndexofTile(x, y, m_LevelWidth);
		if (m_Tiles[tileindex].GetTileType() == TileType::Gem)
		{
			m_Tiles[tileindex].CollectGem();
			return true;
		}
		return false;
	}
	bool Level::CollectPowerPellet(const Vec2<float>& position)
	{
		uint32_t x = (uint32_t)std::round((position.X / (float)m_TileSize));
		uint32_t y = (uint32_t)std::round((position.Y / (float)m_TileSize));
		size_t tileindex = GetTileArrayIndexofTile(x, y, m_LevelWidth);
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
			int32_t left = -1;
			int32_t right = -1;
			int32_t top = -1;
			int32_t bottom = -1;
			if (column == m_LevelWidth)
			{
				line++;
				column = 0;
			}
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
				if (m_Tiles[tileindex - m_LevelWidth].GetTileType() != TileType::Wall)
				{
					top = static_cast<int32_t>(tileindex - m_LevelWidth);
				}
			}
			if (line < m_LevelHeight - 1)
			{
				if (m_Tiles[tileindex + m_LevelWidth].GetTileType() != TileType::Wall)
				{
					bottom = static_cast<int32_t>(tileindex + m_LevelWidth);
				}
			}
			m_Tiles[tileindex].SetAdjacentTile(left, right, top, bottom);
			column++;
		}


	}
}