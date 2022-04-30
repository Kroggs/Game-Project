#include "game.h"
#include "gnet.h"


Game::Game()
{
	this->m_Window = std::make_unique<sf::RenderWindow>();
	this->m_EventHandler = std::make_unique<sf::Event>();
	this->m_Camera = std::make_unique<sf::View>(sf::FloatRect(0, 0, 960, 768));

	this->m_IsPaused = false;
	this->m_IsRunning = false;

	this->m_FrameRateLimit = 60;

	this->g_Inventory = std::make_unique<Item_Slot>();
	this->g_LifeBar = std::make_unique<LifeBar>();
}


void Game::Init()
{
	srand(time(NULL));
	this->m_Window->create(sf::VideoMode(1290, 860), "SFML RPG");
	
	this->m_Map = std::make_unique<Map>();

	this->m_PlayerController = std::make_unique<Player>();

	this->m_Camera->setCenter(this->m_PlayerController->GetPosition());
	this->m_PlayerController->SetCurrentMapLocation(this->m_Map.get());
	this->m_Window->setFramerateLimit(this->m_FrameRateLimit);
	this->m_Window->setView(*this->m_Camera);
	this->m_PlayerController->SetView(this->m_Camera.get(), this->m_Window.get());

	this->m_PlayerController->SetRefSprite(this->g_Inventory->GetSpritePtr());
	this->m_PlayerController->SetRefSprite(this->g_LifeBar->GetSpritePtr());

	netw::init();

	netw::joinServer(this->m_PlayerController->getPlayerStruct());
}

void Game::NetInit()
{
	for (unsigned int i = 0; i < netw::getPlayerAmount(); i++) {
		int targetUid = 0;
		if (netw::getPlayerUid(i, this->m_PlayerController->getUid(), targetUid) != 0) {
			EPlayer currentPlayer = netw::getPlayerByUid(targetUid);
			if (currentPlayer.uid == targetUid) {
				this->m_netPlayers.push_back(currentPlayer);
				std::cout << currentPlayer.name << " added to player list." << std::endl;
			}
		}
	}
}

void Game::InitShaders()
{
	if (!this->m_LightShader.loadFromFile("shaders/light_shader.frag", sf::Shader::Fragment))
		std::cout << "Error : could not load shaders" << std::endl;
}

void Game::Update()
{
	this->m_PlayerController->Update();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->m_PlayerController->TakeDamage(1);
	}

}

void Game::NetUpdate()
{
	if (this->m_netClock.getElapsedTime().asMilliseconds() >= 50) {
		sf::Packet packet;
		EPlayer pStruct = this->m_PlayerController->getPlayerStruct();
		packet << pStruct;
		netw::sendPacket(packet);

		if (netw::getPacket(this->m_netPacket)) {
			int packetType = 0xFF;
			this->m_netPacket >> packetType;
			if (packetType == PLAYERJOINRESPONSE) {
				EPlayer netpStruct;
				this->m_netPacket >> netpStruct;
				this->m_netPlayers.push_back(netpStruct);
			}

			this->m_netPacket.clear();
		}



		this->m_netClock.restart();
	}
}


void Game::Render()
{
	this->m_Window->clear(sf::Color::White);

	this->m_Map->RenderMainLayer(*this->m_Window.get());
	this->m_Map->RenderSecondLayer(*this->m_Window.get());
	
	this->m_Window->draw(*this->m_PlayerController.get(), &m_LightShader);

	this->m_Map->RenderThirdLayer(*this->m_Window.get());

	if (this->m_PlayerController->IsBehindTile())
		this->m_PlayerController->RenderShadow();


	this->m_PlayerController->DrawInventoryItems();

	this->m_Window->draw(*this->g_Inventory);
	this->m_Window->draw(*this->g_LifeBar);
	this->m_Window->draw(this->m_PlayerController->GetLifebar());
	this->m_Window->draw(this->m_PlayerController->GetItemGui());

	this->m_Window->display();
}

void Game::End()
{
	netw::closeConnextion(this->m_PlayerController->getUid());
	this->m_Window->close();
}

bool Game::IsClosed() const
{
	while (this->m_Window->pollEvent(*this->m_EventHandler))
		if (this->m_EventHandler->type == sf::Event::Closed)
			return true;

	return false;
}

bool Game::IsRunning() const
{
	return this->m_Window->isOpen();
}