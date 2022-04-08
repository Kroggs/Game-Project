#pragma once
#include "Player.h"
#include "assets.h"
#include "levels.h"
#include "text.h"
#include "Gui.h"
#include <filesystem>

#include <memory>

class Game
{
public :

	Game();

	void Init();
	void InitShaders();
	void Update();
	void Render();
	void End();
	
	bool IsClosed() const;
	bool IsRunning() const;

private :

	std::unique_ptr<sf::RenderWindow> m_Window;
	std::unique_ptr<sf::Event> m_EventHandler;
	std::unique_ptr<sf::View> m_Camera;
	std::unique_ptr<Map> m_Map;

	sf::Shader m_LightShader;

	std::vector<Gui> m_Gui;
	std::unique_ptr<Item_Slot> g_Inventory;
	std::unique_ptr<LifeBar> g_LifeBar;

	std::unique_ptr<Player> m_PlayerController;
	std::vector<Item> m_PickupItems;

	bool m_IsRunning;
	bool m_IsPaused;

	bool m_DebugMode;
	int m_FrameRateLimit;
};