#ifndef GAME_H
#define GAME_H

#include <string>
#include <sstream>

//#define TINYC2_IMPL
#include <tinyc2.h>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <Debug.h>

#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

#include <GameObject.h>
#include <BasicShader.h>

#include <thread>

using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();
	void update();
	void render();
	
private:
	GameObject* game_object[100];
	GameObject* playerObject;
	GameObject* obstacleObject[5];
	GameObject* spikeObjects[5];
	GameObject* rampsObjects[15];
	GameObject* objective[2];
	RenderWindow window;
	sf::Texture texture;
	


	int result{ 0 };

	Clock clock;
	Time time;
	bool animate = false;
	vec3 animation = vec3(0.0f);
	//variables for the game
	float rotation = 0.0f;
	float m_groundPos{ -0.8f };
	float xPos{ 0.0f };
	float yPos{ 0.0f };
	float x_offset, y_offset, z_offset;
	int m_view{ 0 };
	float m_maxHeight{ 0.0f };
	int m_doOnce{ 0 };
	//bools for the game
	bool newHeight{ false };
	bool isRunning = false;
	int m_timer{ 0 };
	//all functions fo the game
	void initialize();	
	void unload();
	void setUpcontent();
	void obstacleCollision();
	void rampsCollision();
	void movingblockCollision();
	void objectiveCollision();
	void camera();
	void obstacleMove();
	void playerMove(float dt);
	enum AiMove
	{
		MoveUp,
		MoveDown
	}
	m_blockMove;
	enum MoveStates
	{
		Jumping,
		Falling,
		Stationary

	}
	m_moveState;


};

#endif  // ! GAME_H