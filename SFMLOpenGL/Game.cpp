#include <Game.h>
#include <Cube.h>
#include <Easing.h>
#include <Model.h>

// Helper to convert Number to String for HUD
template <typename T>
string toString(T number)
{
	ostringstream oss;
	oss << number;
	return oss.str();
}

GLuint	vao = 0,	// Vertex Array ID
		vbo,		// Vertex Buffer ID
		vib,		// Vertex Index Buffer
		vertexbuffer,
		uvbuffer,
		to[1];		// Texture ID
GLint	positionID,	// Position ID
		colorID,	// Color ID
		textureID,	// Texture ID
		uvID,		// UV ID
		mvpID,		// Model View Projection ID
		mvpplayerID,// Model View Player Projection ID
		x_offsetID, // X offset ID
		y_offsetID,	// Y offset ID
		z_offsetID;	// Z offset ID
GLenum	error;		// OpenGL Error Code

//Please see .//Assets//Textures// for more textures
const string filename = ".//Assets//Textures//grid.tga";
unsigned char* img_data;			// Image data

int		width;						// Width of texture
int		height;						// Height of texture
int		comp_count;					// Component of texture

mat4	mvp, 
		projection, 
		view, 
		model;	// Model View Projection

mat4	mvpPlayer, 
		modelPlayer;

mat4	mvpObstacle,
		modelObstacle;

mat4	mvpRamps, 
		modelRamps;

mat4	mvpMovingobs, 
		modelMovingobs;

mat4	mvpObjective,
		modelObjective;

//Fonts
Font	font;

//Shaders
BasicShader m_shader;
BasicShader m_modelShader;

Model m_testmodel;
Model m_testlego; 
Model m_spike;
Model m_movingHazard;
Model m_floatingPlatform;
Model m_player;
const float PLAYER_WIDTH = 0.6f;

//Audio
sf::SoundBuffer m_audioBuffer;
sf::Music		m_music;
sf::Sound		m_soundEffect;
bool			music_Loaded = true;
bool			sound_Loaded = true;
Mutex			renderLock;

Game::Game() : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing")
{
}

Game::Game(sf::ContextSettings settings) : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing", 
	sf::Style::Default, 
	settings)

	//blocks are 2.0 wide
{
	//Load audio files
	if (!m_audioBuffer.loadFromFile("sound.wav"))
	sound_Loaded = false;	
	
	if (!m_music.openFromFile("music.ogg"))
	music_Loaded = false;

	//set up the array of ground / stationary objects and their positions

	//set up the objects
	float xPosition{ -4.0f };

	float xpositionRamps{ 17.0f };
	float ypositionRamps{ -0.8f };
	float obstacleXPosition{ 6.0f };
	for (int i = 0; i < 100; i++)
	{
		game_object[i] = new GameObject();
		game_object[i]->setPosition(vec3(xPosition, -2.8, 0.0f));
		game_object[i]->setRotation(rand() % 4 * 1.5708f);
		xPosition += 2.0f;
	}

	//Gaps in the floor, move them far away so they're not z fighting the start block
	float farX = -1000.0f;
	game_object[1] = new GameObject();
	game_object[1]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[2] = new GameObject();
	game_object[2]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[6] = new GameObject();
	game_object[6]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[7] = new GameObject();
	game_object[7]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[20] = new GameObject();
	game_object[20]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[19] = new GameObject();
	game_object[19]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[41] = new GameObject();
	game_object[41]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[42] = new GameObject();
	game_object[42]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[46] = new GameObject();
	game_object[46]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[47] = new GameObject();
	game_object[47]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[36] = new GameObject();
	game_object[36]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[37] = new GameObject();
	game_object[37]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[47] = new GameObject();
	game_object[47]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[48] = new GameObject();
	game_object[48]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[55] = new GameObject();
	game_object[55]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[63] = new GameObject();
	game_object[64]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[72] = new GameObject();
	game_object[73]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[85] = new GameObject();
	game_object[86]->setPosition(vec3(farX, -2.8, 0.0f));
	game_object[91] = new GameObject();
	game_object[91]->setPosition(vec3(farX, -2.8, 0.0f));

	//loop to set up the ojects ramp
	for (int i = 0; i < 3; i++)
	{
		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps, ypositionRamps, 0.0f));
		rampsObjects[i]->setRotation(rand() % 4 * 1.5708f);
		xpositionRamps += 2.0f;
		ypositionRamps += 2.0f;
	}
	/// <summary>
	/// loop between 3 and 7 to set up ramps
	/// </summary>
	float xpositionRamps1{ 43.0f };
	float ypositionRamps1{ -0.8f };
	for (int i = 3; i < 7; i++)
	{
		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps1, ypositionRamps1, 0.0f));
		rampsObjects[i]->setRotation(rand() % 4 * 1.5708f);
		xpositionRamps1 += 3.0f;
		ypositionRamps1 += 3.0f;
	}

	float xpositionRamps2{ 55.0f };
	float ypositionRamps2{ -0.8f };

	for (int i = 7; i < 10; i++)
	{
		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps2, ypositionRamps2, 0.0f));
		rampsObjects[i]->setRotation(rand() % 4 * 1.5708f);
		xpositionRamps1 += 2.5f;
		ypositionRamps1 += 2.5f;
	}
	float xpositionRamps3{ 65.0f };
	float ypositionRamps3{ 5.5f };
	for (int i = 10; i < 15; i++)
	{

		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps3, ypositionRamps3, 0.0f));
		rampsObjects[i]->setRotation(rand() % 4 * 1.5708f);
		xpositionRamps3 += 3.0f;
		ypositionRamps3 += 3.1f;
	}

	//set up the player
	playerObject = new GameObject();
	playerObject->setPosition(vec3(-4.0f, -0.8, 0.0f));
	/// <summary>
	/// loop to set up obstacles
	/// </summary>
	for (int i = 0; i < 5; i++)
	{

		obstacleObject[i] = new GameObject();
		obstacleObject[i]->setPosition(vec3(obstacleXPosition, -0.8f, 0.0f));
		obstacleObject[i]->setRotation(rand() % 4 * 1.5708f);
		obstacleXPosition += 25.0f;

	}
	//loop to set up moving obstacles
	float posmoveX{ 27.5f };
	for (int i = 0; i < 6; i++)
	{

		spikeObjects[i] = new GameObject();
		spikeObjects[i]->setPosition(vec3(posmoveX, -0.8f, 0.0f));
		posmoveX += 30.0f;
		
	}
	//set up objective
	objective[0] = new GameObject();
	objective[0]->setPosition(vec3(180.0f, -0.8f, 0.0f));
	objective[1] = new GameObject();
	objective[1]->setPosition(vec3(180.0f, 1.2f, 0.0f));
	
	//set the initial state
	m_moveState = MoveStates::Stationary;
	m_blockMove = AiMove::MoveUp;

}

Game::~Game()
{
}

void Game::run()
{
	initialize();
	setUpcontent();
	Event event;

	float rotation = 0.01f;
	float start_value = 0.0f;
	float end_value = 1.0f;

	if (sound_Loaded) { m_soundEffect.setBuffer(m_audioBuffer); }
	if (music_Loaded) { m_music.play(); }	
	m_music.setVolume(50.0f);

	std::thread updateThread(&Game::update, this);
	window.setActive(false);
	std::thread renderThread(&Game::render, this);

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// Set Model Rotation
				if (!animate)
				{
					animate = true;
					

				}
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				// Set Model Rotation
				if (!animate)
				{
					animate = true;
				
				}

				// https://www.sfml-dev.org/documentation/2.0/classsf_1_1Clock.php
				// https://github.com/acron0/Easings
				// http://robotacid.com/documents/code/Easing.cs
				// http://st33d.tumblr.com/post/94243475686/easing-equations-for-unity-c
				// http://easings.net/
				// http://upshots.org/actionscript/jsas-understanding-easing
				// https://www.kirupa.com/html5/animating_with_easing_functions_in_javascript.htm
				// https://medium.com/motion-in-interaction/animation-principles-in-ui-design-understanding-easing-bea05243fe3#.svh4gczav
				// http://thednp.github.io/kute.js/easing.html
				// http://gizma.com/easing/#quad1
				// https://github.com/warrenm/AHEasing

				// VR
				// https://www.sfml-dev.org/documentation/2.4.2/classsf_1_1Sensor.php
				// http://en.sfml-dev.org/forums/index.php?topic=9412.msg65594
				// https://github.com/SFML/SFML/wiki/Tutorial:-Building-SFML-for-Android-on-Windows
				// https://github.com/SFML/SFML/wiki/Tutorial:-Building-SFML-for-Android
				// https://www.youtube.com/watch?v=n_JSi6ihDFs
				// http://en.sfml-dev.org/forums/index.php?topic=8010.0
				// 				
			}			
		}
		//update();
		//render();
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();
	updateThread.join();
	renderThread.join();
}

void Game::initialize()
{	
	x_offset = { 0.0f }, y_offset = { -2.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)
	m_maxHeight = { playerObject->getPosition().y + 5.0f };
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (!(!glewInit())) { DEBUG_MSG("glewInit() failed"); }

	glClearColor(0.0f,0.777f,1.0f,1.0f);

	m_shader = BasicShader(".//Assets//Shaders//basicShader.vs", ".//Assets//Shaders//basicShader.fs");
	m_modelShader = BasicShader(".//Assets//Shaders//modelShader.vs", ".//Assets//Shaders//modelShader.fs");
	m_testlego = Model(".//Assets//Models//legoGround//ground.obj");
	m_spike = Model(".//Assets//Models//legoSpike//spike.obj");
	m_movingHazard = Model(".//Assets//Models//legoMovingHazard//movingHazard.obj");
	m_player = Model(".//Assets//Models//legoPlayer//LEGO_Man.obj");
	m_floatingPlatform = Model(".//Assets//Models//legoFloatingPlatform//floatingPlatform.obj");
	   
	// Copy UV's to all faces
	for (int i = 1; i < 6; i++)
		memcpy(&uvs[i * 4 * 2], &uvs[0], 2 * 4 * sizeof(GLfloat));

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	// Vertex Array Buffer
	glGenBuffers(1, &vbo);		// Generate Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS) + (2 * UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Generate Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);


	/*int countINDICES = game_object[0]->getIndexCount();*/
	// Indices to be drawn
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * INDICES * sizeof(GLuint), indices, GL_STATIC_DRAW);		

	// Set image data
	// https://github.com/nothings/stb/blob/master/stb_image.h
	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to[0]);
	glBindTexture(GL_TEXTURE_2D, to[0]);

	// Wrap around
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Filtering
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind to OpenGL
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_data				// Image Data
		);

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
		);

	
	view = lookAt(
		vec3(0.0f, 4.0f, 10.0f),	// Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);


	// Model matrix
	model = mat4(
		1.0f					// Identity Matrix
		);

	modelPlayer = mat4(
		1.0f					// Identity Matrix for the player
	);

	modelObstacle = mat4(
		1.0f					// Identity Matrix for the obstacle
	);

	modelRamps = mat4(
		1.0f					// Identity Matrix for the obstacle
	);

	modelMovingobs = mat4(
		1.0f					// Identity Matrix for the obstacle
	);
	
	modelObjective = mat4(
			1.0f					// Identity Matrix for the obstacle
		);

	//Player Model matrix
	//modelPlayer = mat4(
	//	1.0f					//Player Identity Matrix
	//);	
	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Load Font
	font.loadFromFile(".//Assets//Fonts//BBrick.ttf");
}

void Game::update()
{	
	sf::Clock clock;
	while (isRunning)
	{
		sf::Time dt = clock.restart();
		std::cout << playerObject->getPosition().x << std::endl;
		//function calls for the move , ramps,objective,player move 
		//,obstacle move,camera, obstacle collisions
		if (m_moveState == MoveStates::Stationary)
		{
			m_maxHeight = { playerObject->getPosition().y + 5.0f };
			playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y - 5.f * dt.asSeconds(), playerObject->getPosition().z));
		}
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (playerObject->getPosition().x + PLAYER_WIDTH >= game_object[i]->getPosition().x - PLAYER_WIDTH &&
					game_object[i]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
					playerObject->getPosition().y + 1 >= game_object[i]->getPosition().y - 1 &&
					game_object[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1 ||
					playerObject->getPosition().x + PLAYER_WIDTH >= rampsObjects[j]->getPosition().x - PLAYER_WIDTH &&
					rampsObjects[j]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
					playerObject->getPosition().y + 1 >= rampsObjects[j]->getPosition().y - 1 &&
					rampsObjects[j]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
				{
					playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y + 5.f * dt.asSeconds(), playerObject->getPosition().z));
				}
			}
		}

		switch (m_moveState)
		{
			//the initial state the not jumping state
		case MoveStates::Stationary:

			//checks if the space bar is pressd and switchs the state to jumping
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				m_moveState = MoveStates::Jumping;
				if (sound_Loaded) { m_soundEffect.play(); }
			}
			break;
			//jumping state
		case MoveStates::Jumping:
			//moves the player model up the screen
			playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y + 5.f * dt.asSeconds(), playerObject->getPosition().z));

			//if the player model is greater than or equal to 0.5
			if (playerObject->getPosition().y >= m_maxHeight)
			{
				//then set the state to falling
				m_moveState = MoveStates::Falling;
			}
			break;
		case MoveStates::Falling:

			playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y - 5.f * dt.asSeconds(), playerObject->getPosition().z));

			m_moveState = MoveStates::Stationary;
			break;
		}
		playerMove(dt.asSeconds());
		movingblockCollision();
		rampsCollision();
		objectiveCollision();
		obstacleMove();
		camera();
		obstacleCollision();

#if (DEBUG >= 2)
		DEBUG_MSG("Updating...");
#endif
		// Update Model View Projection
		// For mutiple objects (cubes) create multiple models
		// To alter Camera modify view & projection
		mvp = projection * view * model;
		mvpPlayer = projection * view * modelPlayer;
		mvpObstacle = projection * view * modelObstacle;
		mvpRamps = projection * view * modelRamps;
		mvpMovingobs = projection * view * modelMovingobs;
		mvpObjective = projection * view * modelObjective;
	}
	
}

void Game::render()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif	
	while (isRunning)
	{
		//renderLock.lock();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Save current OpenGL render states
		// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7
		window.pushGLStates();

		// Find mouse position using sf::Mouse
		/*int x = Mouse::getPosition(window).x;
		int y = Mouse::getPosition(window).y;

		string hud = "Heads Up Display ["
			+ string(toString(x))
			+ "]["
			+ string(toString(y))
			+ "]";

		Text text(hud, font);
		text.setFillColor(sf::Color(255, 255, 255, 170));
		text.setPosition(50.f, 50.f);
		window.draw(text);*/

		// Restore OpenGL render states
		// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7

		window.popGLStates();

		// Rebind Buffers and then set SubData
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

		// Use Progam on GPU
		m_shader.use();

		// Find variables within the shader
		// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
		positionID = glGetAttribLocation(m_shader.getID(), "sv_position");
		if (positionID < 0) { DEBUG_MSG("positionID not found"); }

		colorID = glGetAttribLocation(m_shader.getID(), "sv_color");
		if (colorID < 0) { DEBUG_MSG("colorID not found"); }

		uvID = glGetAttribLocation(m_shader.getID(), "sv_uv");
		if (uvID < 0) { DEBUG_MSG("uvID not found"); }

		textureID = glGetUniformLocation(m_shader.getID(), "f_texture");
		if (textureID < 0) { DEBUG_MSG("textureID not found"); }

		mvpID = glGetUniformLocation(m_shader.getID(), "sv_mvp");
		if (mvpID < 0) { DEBUG_MSG("mvpID not found"); }

		//added this for the player mvp dont know if correct
		mvpplayerID = glGetUniformLocation(m_shader.getID(), "sv_mvp");
		if (mvpplayerID < 0) { DEBUG_MSG("mvpID not found"); }

		x_offsetID = glGetUniformLocation(m_shader.getID(), "sv_x_offset");
		if (x_offsetID < 0) { DEBUG_MSG("x_offsetID not found"); }

		y_offsetID = glGetUniformLocation(m_shader.getID(), "sv_y_offset");
		if (y_offsetID < 0) { DEBUG_MSG("y_offsetID not found"); }

		z_offsetID = glGetUniformLocation(m_shader.getID(), "sv_z_offset");
		if (z_offsetID < 0) { DEBUG_MSG("z_offsetID not found"); };

		// VBO Data....vertices, colors and UV's appended
		// Add the Vertices for all your GameOjects, Colors and UVS

		glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), game_object[0]->getVertex());

		//glBufferSubData(GL_ARRAY_BUFFER, 0 * VERTICES * sizeof(GLfloat), 3 * VERTICES * sizeof(GLfloat), vertices);
		glBufferSubData(GL_ARRAY_BUFFER, 3 * VERTICES * sizeof(GLfloat), 4 * COLORS * sizeof(GLfloat), colors);
		glBufferSubData(GL_ARRAY_BUFFER, ((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat), 2 * UVS * sizeof(GLfloat), uvs);

		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		// Set Active Texture .... 32 GL_TEXTURE0 .... GL_TEXTURE31
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(textureID, 0); // 0 .... 31

		// Set pointers for each parameter (with appropriate starting positions)
		// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
		glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * VERTICES * sizeof(GLfloat)));
		glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * VERTICES) + (4 * COLORS)) * sizeof(GLfloat)));
		// Enable Arrays
		glEnableVertexAttribArray(positionID);
		glEnableVertexAttribArray(colorID);
		glEnableVertexAttribArray(uvID);


		m_modelShader.use();
		// view/projection transformations	
		m_modelShader.setMat4("projection", projection);
		m_modelShader.setMat4("view", view);

		//run through a for loop to draw  cubes
		for (int i = 0; i < 100; i++)
		{
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(game_object[i]->getPosition().x, game_object[i]->getPosition().y, game_object[i]->getPosition().z)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			model = glm::rotate(model, game_object[i]->getRotation(), glm::vec3(0, 1, 0));
			m_modelShader.setMat4("model", model);
			m_testlego.Draw(m_modelShader);
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(playerObject->getPosition().x, playerObject->getPosition().y, playerObject->getPosition().z)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, 1.5708f, glm::vec3(0, 1, 0));
		m_modelShader.setMat4("model", model);
		m_player.Draw(m_modelShader);

		//run through a for loop to draw  cubes
		for (int i = 0; i < 5; i++)
		{
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(obstacleObject[i]->getPosition().x, obstacleObject[i]->getPosition().y, obstacleObject[i]->getPosition().z)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			model = glm::rotate(model, obstacleObject[i]->getRotation(), glm::vec3(0, 1, 0));
			m_modelShader.setMat4("model", model);
			m_movingHazard.Draw(m_modelShader);
		}

		//run through a for loop to draw  cubes
		for (int i = 0; i < 15; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(rampsObjects[i]->getPosition().x, rampsObjects[i]->getPosition().y, rampsObjects[i]->getPosition().z)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			model = glm::rotate(model, rampsObjects[i]->getRotation(), glm::vec3(0, 1, 0));
			m_modelShader.setMat4("model", model);
			m_floatingPlatform.Draw(m_modelShader);
		}

		//run through a for loop to draw  cubes
		for (int i = 0; i < 5; i++)
		{
			// render the loaded model
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(spikeObjects[i]->getPosition().x, spikeObjects[i]->getPosition().y, spikeObjects[i]->getPosition().z)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			m_modelShader.setMat4("model", model);
			m_spike.Draw(m_modelShader);
		}

		m_shader.use();
		//set the mvp of the objective
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpObjective[0][0]);
		//loop the through the objectives
		for (int i = 0; i < 2; i++)
		{
			glUniform1f(x_offsetID, objective[i]->getPosition().x);
			glUniform1f(y_offsetID, objective[i]->getPosition().y);
			glUniform1f(y_offsetID, objective[i]->getPosition().y);

			glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
		}

		/*window.draw(playerRect);*/
		window.display();

		// Disable Arrays
		glDisableVertexAttribArray(positionID);
		glDisableVertexAttribArray(colorID);
		glDisableVertexAttribArray(uvID);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Unbind Buffers with 0 (Resets OpenGL States...important step)
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		// Reset the Shader Program to Use
		glUseProgram(0);

		// Check for OpenGL Error code
		error = glGetError();
		if (error != GL_NO_ERROR) {
			DEBUG_MSG(error);
		}
		//renderLock.unlock();
	}
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	//glDetachShader(progID, vsid);	// Shader could be used with more than one progID
	//glDetachShader(progID, fsid);	// ..
	//glDeleteShader(vsid);			// Delete Vertex Shader
	//glDeleteShader(fsid);			// Delete Fragment Shader
	//glDeleteProgram(progID);		// Delete Shader
	glDeleteBuffers(1, &vbo);		// Delete Vertex Buffer
	glDeleteBuffers(1, &vib);		// Delete Vertex Index Buffer
	stbi_image_free(img_data);		// Free image stbi_image_free(..)
}
void Game::setUpcontent()
{
}

void Game::obstacleCollision()
{	
	// checks the collision between the player and obstacle 0	
	if(playerObject->getPosition().x  + PLAYER_WIDTH >= obstacleObject[0]->getPosition().x - PLAYER_WIDTH &&
		obstacleObject[0]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
		playerObject->getPosition().y + 1 >= obstacleObject[0]->getPosition().y - 1 &&
		obstacleObject[0]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
	{	
		playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
	}
	// checks the collision between the player and obstacle 1	
	else if (playerObject->getPosition().x + PLAYER_WIDTH >= obstacleObject[1]->getPosition().x - PLAYER_WIDTH &&
		obstacleObject[1]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
		playerObject->getPosition().y + 1 >= obstacleObject[1]->getPosition().y - 1 &&
		obstacleObject[1]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
	{
		playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
	}
	// checks the collision between the player and obstacle 2
	else if (playerObject->getPosition().x + PLAYER_WIDTH >= obstacleObject[2]->getPosition().x - PLAYER_WIDTH &&
		obstacleObject[2]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
		playerObject->getPosition().y + 1 >= obstacleObject[2]->getPosition().y - 1 &&
		obstacleObject[2]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
	{
		playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
	}	
}

void Game::rampsCollision()
{
	for (int i = 0; i < 15; i++)
	{
		if (playerObject->getPosition().x + PLAYER_WIDTH >= rampsObjects[i]->getPosition().x - PLAYER_WIDTH &&
			rampsObjects[i]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
			playerObject->getPosition().y + 1 >= rampsObjects[i]->getPosition().y - 1 &&
			rampsObjects[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
		{
			playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y + 0.01f, playerObject->getPosition().z));
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				playerObject->setPosition(vec3(playerObject->getPosition().x -0.01f, playerObject->getPosition().y , playerObject->getPosition().z));
			}
			m_moveState = MoveStates::Stationary;
		}
	}
}

void Game::movingblockCollision()
{
	for (int i = 0; i < 5; i++)
	{		
		// checks the collision between the player and moving obstacles 0		
		if (playerObject->getPosition().x + PLAYER_WIDTH >= spikeObjects[i]->getPosition().x - PLAYER_WIDTH &&
			spikeObjects[i]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
			playerObject->getPosition().y + 1 >= spikeObjects[i]->getPosition().y - 1 &&
			spikeObjects[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
		{
			playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
		}
	}  
}

void Game::objectiveCollision()
{
	//loop for the objective
	for (int i = 0; i < 2; i++)
	{
		//collision between player and objective
		if (playerObject->getPosition().x + PLAYER_WIDTH >= objective[i]->getPosition().x - PLAYER_WIDTH &&
			objective[i]->getPosition().x + PLAYER_WIDTH >= playerObject->getPosition().x - PLAYER_WIDTH &&
			playerObject->getPosition().y + 1 >= objective[i]->getPosition().y - 1 &&
			objective[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
		{
			//set the position of the player
			playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
		}
	}
}

void Game::camera()
{
	//it does the key presses for the view
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))	
		m_view = 1;	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))	
		m_view = 2;	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	
		m_view = 3;
	
	if (m_view == 2)
	{
		view = lookAt(
			vec3(playerObject->getPosition().x - x_offset, playerObject->getPosition().y - y_offset, playerObject->getPosition().z - z_offset),	// Camera (x,y,z), in World Space
			vec3(playerObject->getPosition().x, playerObject->getPosition().y, playerObject->getPosition().z),		// Camera looking at player
			vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);
	}
	else if (m_view == 1)
	{
		view = lookAt(
			vec3(playerObject->getPosition().x - x_offset, playerObject->getPosition().y - y_offset, playerObject->getPosition().z - z_offset),	// Camera (x,y,z), in World Space
			vec3(playerObject->getPosition().x, playerObject->getPosition().y, playerObject->getPosition().z),		// Camera looking at player
			vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);
	}

	view = lookAt(
		vec3(playerObject->getPosition().x - x_offset, playerObject->getPosition().y - y_offset, playerObject->getPosition().z - z_offset),	// Camera (x,y,z), in World Space
		vec3(playerObject->getPosition().x, playerObject->getPosition().y, playerObject->getPosition().z),		// Camera looking at player
		vec3(0.0f, 1.0f, 0.0f)
	);	
}
void Game::obstacleMove()
{
	if (m_timer < 1500)	
		m_timer++;	

	for (int i = 0; i < 5; i++)
	{
		switch (m_blockMove)
		{
			//switch to move up the screen 
		case AiMove::MoveUp:
			//if the obstacle is less than 1.8
			if (obstacleObject[i]->getPosition().y <= 1.8f)
			{
				//moves up the screen 
				obstacleObject[i]->setPosition(vec3(obstacleObject[i]->getPosition().x, obstacleObject[i]->getPosition().y + 0.01f, obstacleObject[i]->getPosition().z));
			}
			//if the obstacle is greater than 1.8
			if (obstacleObject[i]->getPosition().y >= 1.8f)
			{
				//moves down  the screen
				obstacleObject[i]->setPosition(vec3(obstacleObject[i]->getPosition().x, obstacleObject[i]->getPosition().y - 0.01f, obstacleObject[i]->getPosition().z));
				//switch to the move down screen 
				m_blockMove = AiMove::MoveDown;
			}
			break;

		case AiMove::MoveDown:
			//the timer is 1500 and is gretaer than the ground
			if (m_timer == 1500 && obstacleObject[i]->getPosition().y >= -0.8f)
			{
				//move down
				obstacleObject[i]->setPosition(vec3(obstacleObject[i]->getPosition().x, obstacleObject[i]->getPosition().y - 0.01f, obstacleObject[i]->getPosition().z));
			}
			//when at the ground 
			else if (obstacleObject[i]->getPosition().y <= -0.8f)
			{
				//switch to move up
				m_blockMove = AiMove::MoveUp;
				//reset timer
				m_timer = 0;
			}
			break;
		}
	}
}
void Game::playerMove(float dt)
{
	if (m_view == 2)	
		x_offset = { 10.0f }, y_offset = { -10.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)	

	if (m_view == 1)	
		x_offset = { 0.0f }, y_offset = { -2.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)	

	//if the d key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//move the player model to the right
		playerObject->setPosition(vec3(playerObject->getPosition().x + 5.f * dt, playerObject->getPosition().y, playerObject->getPosition().z));
	}

	//when the a is pressed move to the left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//move the player model to the left
		playerObject->setPosition(vec3(playerObject->getPosition().x - 5.f * dt, playerObject->getPosition().y, playerObject->getPosition().z));
	}
	//switch statement for the jumping states
}