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

	this->m_NetUpdatePending = false;

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
	for (auto& player : this->m_netPlayersController)
		player.Update();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->m_PlayerController->TakeDamage(1);
	}

	if (this->m_netPlayers.size() > this->m_netPlayersController.size()) 
		this->m_netPlayersController.push_back(NetPlayer());
	else 
		for (auto i = 0; i < this->m_netPlayersController.size(); ++i) 
			if (this->m_netPlayersController[i].getUid() != this->m_netPlayers[i].uid)
				this->m_netPlayersController[i].AssignStruct(this->m_netPlayers[i]);
}

void Game::NetUpdate()
{
		this->m_NetUpdatePending = true;
		if (this->m_netClock.getElapsedTime().asMilliseconds() >= 20) {
			this->m_sendPacket.clear();
			this->m_pStruct = this->m_PlayerController->getPlayerStruct();
			this->m_sendPacket << this->m_pStruct.packetType << 
				this->m_pStruct.posx << this->m_pStruct.posy <<
				this->m_pStruct.speed << this->m_pStruct.isMoving <<
				this->m_pStruct.isBehindTile << this->m_pStruct.name <<
				this->m_pStruct.uid << this->m_pStruct.animx << this->m_pStruct.animy;

			netw::sendPacket(this->m_sendPacket);

			if (netw::getPacket(this->m_netPacket)) {
				this->m_inPacketType = 0xFF;
				this->m_netPacket >> this->m_netpStruct.packetType >>
					this->m_netpStruct.posx >> this->m_netpStruct.posy >>
					this->m_netpStruct.speed >> this->m_netpStruct.isMoving >>
					this->m_netpStruct.isBehindTile >> this->m_netpStruct.name >>
					this->m_netpStruct.uid  >> this->m_netpStruct.animx >> this->m_netpStruct.animy;

				this->m_inPacketType = this->m_netpStruct.packetType;

				if (this->m_inPacketType == PLAYERJOINRESPONSE) {
					std::cout << this->m_netpStruct.uid << " added to player list." << std::endl;
					this->m_netPlayers.push_back(this->m_netpStruct);
				}
				else if (this->m_inPacketType == PLAYERUPDATERESPONSE) {
					if (this->m_netpStruct.uid != this->m_PlayerController->getUid()) {
						for (auto& player : this->m_netPlayersController) {
							if (player.getUid() == this->m_netpStruct.uid) {
								player.AssignStruct(this->m_netpStruct);
							}
						}	
					}
				}

				this->m_netPacket.clear();
			}


			this->m_NetUpdatePending = false;
			this->m_netClock.restart();
		}
}


void Game::Render()
{
	this->m_Window->clear(sf::Color::White);

	this->m_Map->RenderMainLayer(*this->m_Window.get());
	this->m_Map->RenderSecondLayer(*this->m_Window.get());
	
	this->m_Window->draw(*this->m_PlayerController.get(), &m_LightShader);
	for (auto& player : this->m_netPlayersController)
		this->m_Window->draw(player);

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