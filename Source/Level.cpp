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

#include "Level.h"
#include "Assert.h"
#include "Application.h"
#include "Renderer.h"
#include "utills.h"

#include <raylib.h>


namespace Pman
{

	static bool CheckCollision(const Rect<int32_t>& player_, const Rect<int32_t>& tile_)
	{
		return !(player_.Right <= tile_.Left || player_.Left >= tile_.Right || player_.Bottom <= tile_.Top || player_.Top >= tile_.Bottom);
	}
	static void GetGhostRectangle(Rect<int32_t>& rect_, const Vec2<int32_t>& ghost_position, const int32_t tile_size)
	{
		rect_.Left = ghost_position.X;
		rect_.Right = ghost_position.X + tile_size;
		rect_.Top = ghost_position.Y;
		rect_.Bottom = ghost_position.Y + tile_size;
	}
	Level::Level(const uint32_t tile_size) : m_TileSize(tile_size)
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

	void Level::LoadLevel(const std::vector<std::string>& level_data)
	{
		ASSERT(m_TileSize, "Tilesize cannot be 0!");
		ASSERT(level_data.size() != 0, "Level data cannot be empty!");

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
		uint32_t rows = (uint32_t)level_data.size();
		uint32_t columns = (uint32_t)level_data[0].length(); //all rows must be this length or we will assert
		m_LevelHeight = rows;
		m_LevelWidth = columns;
		m_Tiles.reserve(m_LevelWidth * m_LevelHeight);
		uint32_t tilecount = 0;
		
		for (size_t row = 0; row < rows; row++)
		{
			ASSERT((level_data[row].length() == columns), "All rows must be the same length as the first");
			for (size_t column = 0; column < columns; column++)
			{
				ASSERT((tilecount < rows * columns), "Error !!!! Too many tiles have been processed!");
				auto it = m_Tiles.begin() + tilecount;
							
				switch (level_data[row][column])
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
					ASSERT((m_RedGhost == nullptr), "Can only have 1 of each type of ghost");
					GhostSpecification gspec{};
					gspec.MainSprite = m_RedGhostSprite;
					gspec.BlueSprite = m_BlueGhostSprite;
					gspec.EyesSprite = m_GhostEyesSprite;
					gspec.InitialPosition = { static_cast<int32_t>(column),static_cast<int32_t>(row) };
					gspec.LevelCallback = this;
					gspec.MoveSpeed = 61.0f;
					gspec.ScatterPosition = { 18,1 }; 
					gspec.TileSize = m_TileSize;
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
					ASSERT((m_PinkGhost == nullptr), "Can only have 1 of each type of ghost");
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
					ASSERT((m_CyanGhost == nullptr), "Can only have 1 of each type of ghost");
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
					ASSERT((m_OrangeGhost == nullptr), "Can only have 1 of each type of ghost");
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
					pspec.MoveSpeed = 60.0f;
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
					ERROR("Invalid level data supplied at col: {}, row: {}", column, row);
					break;
				}
				tilecount++;
				
			}
		}
		ProcessAdjacentTiles();
	}
	void Level::OnUpdate(float time_step)
	{
		m_Player->OnUpdate(time_step);
		m_CyanGhost->OnUpdate(time_step);
		m_RedGhost->OnUpdate(time_step);
		m_PinkGhost->OnUpdate(time_step);
		m_OrangeGhost->OnUpdate(time_step);
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

	/// <summary>
	/// Checks if you will collide with a wall 
	/// </summary>
	/// <param name="position">the pixel position of the player</param>
	/// <param name="direction">The direction vector -1 is up/ left and 1 is right/down, 0 no movement in that axis</param>
	/// <param name="canusedoor">is able to use the ghost house door</param>
	/// <returns>true on collision with wall</returns>
	bool Level::CollideWithWall(const Vec2<int32_t>& position_, const Vec2<int32_t>& direction_) const
	{
		// Get the player's bounding box in world space
		Rect<int32_t> playerrect = { position_.X,position_.X + m_TileSize,position_.Y,position_.Y + m_TileSize };
		
		// Calculate the tile coordinates the player is interacting with
		//Reduce tile area that player interacts with by 4 pixels in each direction 2 on each side! This is to make it easier to get round the map. 
		int32_t tileXLeft = (playerrect.Left + 2) / m_TileSize;
		int32_t tileXRight = (playerrect.Right - 2) / m_TileSize;
		int32_t tileYTop = (playerrect.Top + 2) / m_TileSize;
		int32_t tileYBottom = (playerrect.Bottom - 2) / m_TileSize;
		bool retval = false;
		// Iterate over the tiles the player's bounding box overlaps with
		for (int32_t x = tileXLeft; x <= tileXRight; ++x)
		{
			for (int32_t y = tileYTop; y <= tileYBottom; ++y)
			{
				// Get the tile boundaries in pixels
				Rect tilerect = { x * m_TileSize,(x + 1) * m_TileSize,y * m_TileSize,(y + 1) * m_TileSize };
				// Check if the player's bounding box intersects with the tile
				if (CheckCollision(playerrect, tilerect))
				{
					// If tile is a wall, prevent movement in that direction
					if (m_Tiles[GetTileIndex({ x, y }, m_LevelWidth)].GetTileType() == TileType::Wall || m_Tiles[GetTileIndex({ x, y }, m_LevelWidth)].GetTileType() == TileType::Door)
					{
						//check collision left	
						if (playerrect.Left < tilerect.Right)
						{
							if (direction_.X == -1)
							{
								retval = true;
							}
						}
						//check collision right
						if (playerrect.Right > tilerect.Left)
						{
							if (direction_.X == 1)
							{
								retval = true;
							}
						}
						//check collision above
						if (playerrect.Top < tilerect.Bottom)
						{
							if (direction_.Y == -1)
							{
								retval = true;
							}
						}
						//check collision below
						if (playerrect.Bottom > tilerect.Top)
						{
							if (direction_.Y == 1)
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
	
	bool Level::CollectGem(const Vec2<int32_t>& position_)
	{
		size_t tileindex = GetTileArrayIndexofTile(position_.X, position_.Y, m_LevelWidth);
		if (m_Tiles[tileindex].GetTileType() == TileType::Gem)
		{
			m_Tiles[tileindex].CollectGem();
			return true;
		}
		return false;
	}
	bool Level::CollectPowerPellet(const Vec2<int32_t>& position_)
	{
		size_t tileindex = GetTileArrayIndexofTile(position_.X, position_.Y, m_LevelWidth);
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
	void Level::CollideWithGhost(const Vec2<int32_t>& position_) const
	{
		//calculate player and Ghost rectangles
		Rect<int32_t> playerrect = { position_.X, position_.X + m_TileSize,position_.Y,position_.Y + m_TileSize };
		Rect<int32_t> redghostrect = { 0,0,0,0 };
		Rect<int32_t> cyanghostrect = { 0,0,0,0 };
		Rect<int32_t> pinkghostrect = { 0,0,0,0 };
		Rect<int32_t> orangeghostrect = { 0,0,0,0 };
		GetGhostRectangle(redghostrect, m_RedGhost->GetPixelPosition(), m_TileSize);
		GetGhostRectangle(cyanghostrect, m_CyanGhost->GetPixelPosition(), m_TileSize);
		GetGhostRectangle(pinkghostrect, m_PinkGhost->GetPixelPosition(), m_TileSize);
		GetGhostRectangle(orangeghostrect, m_OrangeGhost->GetPixelPosition(), m_TileSize);
		if (CheckCollision(playerrect, redghostrect) && m_RedGhost->GetStatus() != GhostStatus::EyesOnly)
		{
			
			if (m_RedGhost->GetStatus() == GhostStatus::IsBlue) 
			{
				m_RedGhost->SetEaten();
				m_Player->EatenGhost();
			}
			else
			{
				m_Player->LooseALife();
			}
		}
		if (CheckCollision(playerrect, cyanghostrect) && m_CyanGhost->GetStatus() != GhostStatus::EyesOnly)
		{
			if (m_CyanGhost->GetStatus() == GhostStatus::IsBlue)
			{
				m_CyanGhost->SetEaten();
				m_Player->EatenGhost();
			}
			else
			{
				m_Player->LooseALife();
			}
		}
		if (CheckCollision(playerrect, pinkghostrect) && m_PinkGhost->GetStatus() != GhostStatus::EyesOnly)
		{
			if (m_PinkGhost->GetStatus() == GhostStatus::IsBlue)
			{
				m_PinkGhost->SetEaten();
				m_Player->EatenGhost();
			}
			else
			{
				m_Player->LooseALife();
			}
		}
		if (CheckCollision(playerrect, orangeghostrect) && m_OrangeGhost->GetStatus() != GhostStatus::EyesOnly)
		{
			if (m_OrangeGhost->GetStatus() == GhostStatus::IsBlue)
			{

				m_OrangeGhost->SetEaten();
				m_Player->EatenGhost();
			}
			else
			{
				m_Player->LooseALife();
			}
		}
	}
	bool Level::IsGameOver() const
	{
		if (m_Player->GetPlayerLives() == 0)
		{
			return true;
		}
		return false;
	}
}