#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <SFML/Audio.hpp>

struct LevelInformation
{
    int width, height;
    int layers;
    std::vector<int> collidableTiles;
};


class Asset : public sf::Drawable
{
public :

    virtual void SetPosition(const sf::Vector2f newPos) = 0;
    virtual sf::Sprite GetSprite() const = 0;

    virtual void Interact() = 0;

    virtual bool HasCollision() const = 0;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
    bool m_hasCollision;
    bool m_isDestroyable;
    bool m_isMovable;
};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height);

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};


class Map
{
public:
    
    enum Levels { SPAWN, NONE };

    Map();

    void RenderAssets(sf::RenderWindow& window);
    void RenderMainLayer(sf::RenderWindow& window);
    void RenderSecondLayer(sf::RenderWindow& window);
    void RenderThirdLayer(sf::RenderWindow& window);
    void AddAsset(Asset* asset);

    void ChangeLevel(const int newLevel);

    int GetCurrentLevel() const;
    LevelInformation GetLevelInformations(short int level);

    std::vector<Asset*> GetAssets() const;

private:
    std::vector<Asset*> m_assets;
    std::vector<TileMap> m_TileMap;
    std::vector<int> m_CollidableTiles;
    TileMap m_spawn_l1, m_spawn_l2;
    sf::Music music;

    int m_currentLevel, m_previousLevel, m_mapWidth, m_mapHeight;
};