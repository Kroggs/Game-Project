#pragma once
#include "Player.h"
#include "assets.h"
#include "levels.h"
#include "text.h"
#include "Gui.h"
#include <filesystem>
#include <cstdlib>

#include <memory>
#include <SFML/Network.hpp>
#include "eplayer_struct.h"
#include "netplayer.h"

class Game
{
public :

	Game();

	void Init();
	void NetInit();
	void InitShaders();
	void Update();
	void NetUpdate();
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

	sf::Clock m_netClock;
	sf::Packet m_netPacket, m_sendPacket;
	std::vector<EPlayer> m_netPlayers;
	std::vector<NetPlayer> m_netPlayersController;

	EPlayer m_pStruct, m_netpStruct;
	int m_inPacketType;

	bool m_IsRunning;
	bool m_IsPaused;
	bool m_NetUpdatePending; 

	bool m_DebugMode;
	int m_FrameRateLimit;
};