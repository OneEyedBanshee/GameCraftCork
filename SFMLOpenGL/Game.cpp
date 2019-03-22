#include <Game.h>
#include <Cube.h>
#include <Easing.h>

// Helper to convert Number to String for HUD
template <typename T>
string toString(T number)
{
	ostringstream oss;
	oss << number;
	return oss.str();
}

GLuint	vsid,		// Vertex Shader ID
		fsid,		// Fragment Shader ID
		progID,		// Program ID
		vao = 0,	// Vertex Array ID
		vbo,		// Vertex Buffer ID
		vib,		// Vertex Index Buffer
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

int width;						// Width of texture
int height;						// Height of texture
int comp_count;					// Component of texture

unsigned char* img_data;		// image data

mat4 mvp, projection, 
		view, model;	// Model View Projection

mat4 mvpPlayer,
 modelPlayer;

mat4 mvpObstacle,
modelObstacle;

mat4 mvpRamps,
modelRamps;

mat4 mvpMovingobs,
modelMovingobs;


mat4 mvpObjective,
modelObjective;

Font font;						// Game font



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
		xPosition += 2.0f;
	}
	game_object[1] = new GameObject();
	game_object[1]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[2] = new GameObject();
	game_object[2]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[6] = new GameObject();
	game_object[6]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[7] = new GameObject();
	game_object[7]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[20] = new GameObject();
	game_object[20]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[19] = new GameObject();
	game_object[19]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[41] = new GameObject();
	game_object[41]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[42] = new GameObject();
	game_object[42]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[46] = new GameObject();
	game_object[46]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[47] = new GameObject();
	game_object[47]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[36] = new GameObject();
	game_object[36]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[37] = new GameObject();
	game_object[37]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[47] = new GameObject();
	game_object[47]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[48] = new GameObject();
	game_object[48]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[55] = new GameObject();
	game_object[55]->setPosition(vec3(-4.0f, -2.8, 0.0f));
	game_object[56] = new GameObject();
	game_object[56]->setPosition(vec3(-4.0f, -2.8, 0.0f));

	//loop to set up the ojects ramp
	for (int i = 0; i < 3; i++)
	{
		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps, ypositionRamps, 0.0f));
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
		xpositionRamps1 += 3.0f;
		ypositionRamps1 += 3.0f;
	}

	float xpositionRamps2{ 55.0f };
	float ypositionRamps2{ -0.8f };

	for (int i = 7; i < 10; i++)
	{
		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps2, ypositionRamps2, 0.0f));
		xpositionRamps1 += 2.5f;
		ypositionRamps1 += 2.5f;
	}
	float xpositionRamps3{ 65.0f };
	float ypositionRamps3{ 5.5f };
	for (int i = 10; i < 15; i++)
	{

		rampsObjects[i] = new GameObject();
		rampsObjects[i]->setPosition(vec3(xpositionRamps3, ypositionRamps3, 0.0f));
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
		obstacleXPosition += 25.0f;

	}
	//loop to set up moving obstacles
	float posmoveX{ 27.5f };
	for (int i = 0; i < 5; i++)
	{

		movingobstacleObject[i] = new GameObject();
		movingobstacleObject[i]->setPosition(vec3(posmoveX, -0.8f, 0.0f));
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
		update();
		render();
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();

}

void Game::initialize()
{
	
	x_offset = { 0.0f }, y_offset = { -2.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)
	m_maxHeight = { playerObject->getPosition().y + 5.0f };
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (!(!glewInit())) { DEBUG_MSG("glewInit() failed"); }

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

	// NOTE: uniforms values must be used within Shader so that they 
	// can be retreived
	const char* vs_src =
		"#version 400\n\r"
		""
		"in vec3 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_uv;"
		""
		"out vec4 color;"
		"out vec2 uv;"
		""
		"uniform mat4 sv_mvp;"
		"uniform float sv_x_offset;"
		"uniform float sv_y_offset;"
		"uniform float sv_z_offset;"
		""
		"void main() {"
		"	color = sv_color;"
		"	uv = sv_uv;"
		//"	gl_Position = vec4(sv_position, 1);"
		"	gl_Position = sv_mvp * vec4(sv_position.x + sv_x_offset, sv_position.y + sv_y_offset, sv_position.z + sv_z_offset, 1 );"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	// Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	const char* fs_src =
		"#version 400\n\r"
		""
		"uniform sampler2D f_texture;"
		""
		"in vec4 color;"
		"in vec2 uv;"
		""
		"out vec4 fColor;"
		""
		"void main() {"
		"	fColor = color - texture2D(f_texture, uv);"
		""
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);

	// Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	// Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

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
	std::cout << playerObject->getPosition().x << std::endl;
	//function calls for the move , ramps,objective,player move 
	//,obstacle move,camera, obstacle collisions
	if (m_moveState == MoveStates::Stationary)
	{
		m_maxHeight = { playerObject->getPosition().y + 5.0f };
		playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y - 0.01f, playerObject->getPosition().z));
	}
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (playerObject->getPosition().x + 1 >= game_object[i]->getPosition().x - 1 &&
				game_object[i]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
				playerObject->getPosition().y + 1 >= game_object[i]->getPosition().y - 1 &&
				game_object[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1 ||
				playerObject->getPosition().x + 1 >= rampsObjects[j]->getPosition().x - 1 &&
				rampsObjects[j]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
				playerObject->getPosition().y + 1 >= rampsObjects[j]->getPosition().y - 1 &&
				rampsObjects[j]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
			{
				playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y + 0.01f, playerObject->getPosition().z));
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
		}
		break;
		//jumping state
	case MoveStates::Jumping:
		//moves the player model up the screen
		playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y + 0.01f, playerObject->getPosition().z));

		//if the player model is greater than or equal to 0.5
		if (playerObject->getPosition().y >= m_maxHeight)
		{
			//then set the state to falling
			m_moveState = MoveStates::Falling;
		}
		break;
	case MoveStates::Falling:

		playerObject->setPosition(vec3(playerObject->getPosition().x, playerObject->getPosition().y - 0.01f, playerObject->getPosition().z));
	
		m_moveState = MoveStates::Stationary;
		break;
	}
	playerMove();
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

void Game::render()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save current OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7
	window.pushGLStates();

	// Find mouse position using sf::Mouse
	int x = Mouse::getPosition(window).x;
	int y = Mouse::getPosition(window).y;

	string hud = "Heads Up Display ["
		+ string(toString(x))
		+ "]["
		+ string(toString(y))
		+ "]";

	Text text(hud, font);

	text.setFillColor(sf::Color(255, 255, 255, 170));
	text.setPosition(50.f, 50.f);

	
	window.draw(text);

	// Restore OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7

	window.popGLStates();

	// Rebind Buffers and then set SubData
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	// Use Progam on GPU
	glUseProgram(progID);

	// Find variables within the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	if (positionID < 0) { DEBUG_MSG("positionID not found"); }

	colorID = glGetAttribLocation(progID, "sv_color");
	if (colorID < 0) { DEBUG_MSG("colorID not found"); }

	uvID = glGetAttribLocation(progID, "sv_uv");
	if (uvID < 0) { DEBUG_MSG("uvID not found"); }

	textureID = glGetUniformLocation(progID, "f_texture");
	if (textureID < 0) { DEBUG_MSG("textureID not found"); }

	mvpID = glGetUniformLocation(progID, "sv_mvp");
	if (mvpID < 0) { DEBUG_MSG("mvpID not found"); }

	//added this for the player mvp dont know if correct
	mvpplayerID = glGetUniformLocation(progID, "sv_mvp");
	if (mvpplayerID < 0) { DEBUG_MSG("mvpID not found"); }

	x_offsetID = glGetUniformLocation(progID, "sv_x_offset");
	if (x_offsetID < 0) { DEBUG_MSG("x_offsetID not found"); }

	y_offsetID = glGetUniformLocation(progID, "sv_y_offset");
	if (y_offsetID < 0) { DEBUG_MSG("y_offsetID not found"); }

	z_offsetID = glGetUniformLocation(progID, "sv_z_offset");
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
	//run through a for loop to draw  cubes
	for (int i = 0; i <100; i++)
	{
		glUniform1f(x_offsetID, game_object[i]->getPosition().x);
		glUniform1f(y_offsetID, game_object[i]->getPosition().y);
		glUniform1f(z_offsetID, game_object[i]->getPosition().z);
	
		// Draw Element Arrays
		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}
	
	//set up mvp for player block
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpPlayer[0][0]);

	glUniform1f(x_offsetID, playerObject->getPosition().x);
	glUniform1f(y_offsetID, playerObject->getPosition().y);
	glUniform1f(y_offsetID, playerObject->getPosition().y);

	glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	//set up mvp for obstacles blocks
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpObstacle[0][0]);
	//run through a for loop to draw  cubes
	for (int i = 0; i < 5; i++)
	{
		glUniform1f(x_offsetID, obstacleObject[i]->getPosition().x);
		glUniform1f(y_offsetID, obstacleObject[i]->getPosition().y);
		glUniform1f(y_offsetID, obstacleObject[i]->getPosition().y);

		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}


	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpRamps[0][0]);
	//run through a for loop to draw  cubes
	for (int i = 0; i < 15; i++)
	{
		glUniform1f(x_offsetID, rampsObjects[i]->getPosition().x);
		glUniform1f(y_offsetID, rampsObjects[i]->getPosition().y);
		glUniform1f(y_offsetID, rampsObjects[i]->getPosition().y);

		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}
	//set up mvp for moving blocks
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpMovingobs[0][0]);
	//run through a for loop to draw  cubes
	for (int i = 0; i < 5; i++)
	{
		glUniform1f(x_offsetID, movingobstacleObject[i]->getPosition().x);
		glUniform1f(y_offsetID, movingobstacleObject[i]->getPosition().y);
		glUniform1f(y_offsetID, movingobstacleObject[i]->getPosition().y);

		glDrawElements(GL_TRIANGLES, 3 * INDICES, GL_UNSIGNED_INT, NULL);
	}
	//set the mvp of the objective
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvpObjective[0][0]);
	//loop the through the objectives
	for (int i = 0; i <2; i++)
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
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDetachShader(progID, vsid);	// Shader could be used with more than one progID
	glDetachShader(progID, fsid);	// ..
	glDeleteShader(vsid);			// Delete Vertex Shader
	glDeleteShader(fsid);			// Delete Fragment Shader
	glDeleteProgram(progID);		// Delete Shader
	glDeleteBuffers(1, &vbo);		// Delete Vertex Buffer
	glDeleteBuffers(1, &vib);		// Delete Vertex Index Buffer
	stbi_image_free(img_data);		// Free image stbi_image_free(..)
}
void Game::setUpcontent()
{



}
void Game::obstacleCollision()
{
	/// <summary>
	/// checks the collision between the player and obstacle 0
	/// </summary>
	if(playerObject->getPosition().x  + 1 >= obstacleObject[0]->getPosition().x -1  &&
		obstacleObject[0]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
		playerObject->getPosition().y + 1 >= obstacleObject[0]->getPosition().y - 1 &&
		obstacleObject[0]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
	{
	
		playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));

	}
	/// <summary>
	/// checks the collision between the player and obstacle 1
	/// </summary>
	else if (playerObject->getPosition().x + 1 >= obstacleObject[1]->getPosition().x - 1 &&
		obstacleObject[1]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
		playerObject->getPosition().y + 1 >= obstacleObject[1]->getPosition().y - 1 &&
		obstacleObject[1]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
	{
		playerObject->setPosition(vec3(-4.0f, -0.8f, 0.0f));
	}
	/// <summary>
	/// checks the collision between the player and obstacle 2
	/// </summary>
	else if (playerObject->getPosition().x + 1 >= obstacleObject[2]->getPosition().x - 1 &&
		obstacleObject[2]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
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

		if (playerObject->getPosition().x + 1 >= rampsObjects[i]->getPosition().x - 1 &&
			rampsObjects[i]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
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
		/// <summary>
		/// checks the collision between the player and moving obstacles 0
		/// </summary>
		if (playerObject->getPosition().x + 1 >= movingobstacleObject[i]->getPosition().x - 1 &&
			movingobstacleObject[i]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
			playerObject->getPosition().y + 1 >= movingobstacleObject[i]->getPosition().y - 1 &&
			movingobstacleObject[i]->getPosition().y + 1 >= playerObject->getPosition().y - 1)
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
		if (playerObject->getPosition().x + 1 >= objective[i]->getPosition().x - 1 &&
			objective[i]->getPosition().x + 1 >= playerObject->getPosition().x - 1 &&
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
	{
		m_view = 1;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_view = 2;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_view = 3;
	}

	if (m_view == 2)
	{
		view = lookAt(
			vec3(playerObject->getPosition().x - x_offset, playerObject->getPosition().y - y_offset, playerObject->getPosition().z - z_offset),	// Camera (x,y,z), in World Space
			vec3(playerObject->getPosition().x, playerObject->getPosition().y, playerObject->getPosition().z),		// Camera looking at player
			vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);

	}
	if (m_view == 1)
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
	{
		m_timer++;
	}

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
void Game::playerMove()
{
	if (m_view == 2)
	{
		x_offset = { 10.0f }, y_offset = { -10.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)
	}

	if (m_view == 1)
	{
		x_offset = { 0.0f }, y_offset = { -2.0f }, z_offset = { -10.0f }; // offset on screen (Vertex Shader)
	}

	//if the d key is pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//move the player model to the right
		playerObject->setPosition(vec3(playerObject->getPosition().x + 0.01f, playerObject->getPosition().y, playerObject->getPosition().z));

	}
	//when the a is pressed move to the left
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//move the player model to the left
		playerObject->setPosition(vec3(playerObject->getPosition().x - 0.01f, playerObject->getPosition().y, playerObject->getPosition().z));
	}
	//switch statement for the jumping states
	

}