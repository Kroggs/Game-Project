#include "map.h"
#include "levels.h"

bool TileMap::load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
{
    if (!m_tileset.loadFromFile(tileset))
        return false;

    m_vertices.setPrimitiveType(sf::Quads);
    m_vertices.resize(width* height * 4);

    for (unsigned int i = 0; i < width; ++i)
        for (unsigned int j = 0; j < height; ++j)
        {
            int tileNumber = tiles[i + j * width];
            if (tileNumber != 0)
            {
                tileNumber--;
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }
        }

    return true;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    states.texture = &m_tileset;

    target.draw(m_vertices, states);
}



Map::Map()
{
	this->m_currentLevel = NONE;
    this->m_previousLevel = NONE;

    this->m_mapWidth = 0;
    this->m_mapHeight = 0;

    ChangeLevel(SPAWN);
}

void Map::ChangeLevel(const int newLevel)
{
    this->m_previousLevel = this->m_currentLevel;
    this->m_currentLevel = newLevel;

    if (this->m_currentLevel != this->m_previousLevel) {
        this->m_TileMap.clear();
        switch (this->m_currentLevel) {
        case SPAWN:
            std::vector<TileMap> layers;
            for (unsigned int i = 0; i < SPAWN_LAYERS; ++i) {
                TileMap layer;
                layer.load("images/tileset/32x32/Tilesets/Rural Village Old Tiles32.png", sf::Vector2u(32, 32), L::SPAWN_LAYER[i], 148, 148);
                this->m_TileMap.push_back(layer);
            }
            std::vector<int> CollidableTiles = { 67, 68, 69, 154, 155, 156, 121, 122, 179, 180, 241, 270, 273, 244,
                                                 212, 183, 187, 216, 70, 71, 72, 157, 158, 159, 63, 64, 162, 24, 25, 26, 53, 54, 55,
                                                 82, 83, 84 , 173, 174, 175, 202, 203, 204, 231, 232, 233, 178, 207, 236, 268, 269, 297, 298, 263, 264, 293, 294, 176, 177, 205, 206 };
            this->m_CollidableTiles.clear();
            this->m_CollidableTiles = CollidableTiles;
            break;
        }
    }

}

LevelInformation Map::GetLevelInformations(short int level)
{
    LevelInformation levelInfo;

    switch (level)
    {
    case SPAWN:
        levelInfo.width = 148;
        levelInfo.height = 148;
        levelInfo.layers = 3;
        levelInfo.collidableTiles = this->m_CollidableTiles;
        break;
    }

    return levelInfo;
}

int Map::GetCurrentLevel() const
{
    return this->m_currentLevel;
}

void Map::RenderMainLayer(sf::RenderWindow& window)
{
    window.draw(this->m_TileMap[0]);
}

void Map::RenderSecondLayer(sf::RenderWindow& window)
{
    window.draw(this->m_TileMap[1]);
}

void Map::RenderThirdLayer(sf::RenderWindow& window)
{
    window.draw(this->m_TileMap[2]);
}


void Map::AddAsset(Asset* asset)
{
	this->m_assets.push_back(asset);
}

std::vector<Asset*> Map::GetAssets() const
{
	return this->m_assets;
}
