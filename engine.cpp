#include "engine.h"
#include "SceneManager.h"
#include "entitymanager.h"
#include "inputlogger.h"
#include "TextManager.h"
#include "MainGame.h"
#include "EventSystem.h"

bool InitGL();

bool ACC::engine::OpenglSdlSetup(int width, int height) //set up all of the SDL and opengl settings
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL failed to initialise" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	window = SDL_CreateWindow("ACC Engine",  // The first parameter is the window title
		100, 100,
		width, height,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_RENDERER_ACCELERATED);

	renderer = SDL_CreateRenderer(window, -1, 0);
	glcontext = SDL_GL_CreateContext(window);


	if (!InitGL())
	{
		std::cout << "OpenGL failed to initialise" << std::endl;
		return false;
	}


	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}



void ACC::engine::init() //define what we need for our engine
{ 
	device = alcOpenDevice(NULL);
	if (device == NULL)
	{
		std::cout << "Failed to open device" << std::endl;
		abort();
	}
	context = alcCreateContext(device, NULL);
	if (context == NULL)
	{
		alcCloseDevice(device);
		std::cout << "Failed to create context" << std::endl;
		abort();
	}
	if (!alcMakeContextCurrent(context))
	{
		alcDestroyContext(context);
		alcCloseDevice(device);
		std::cout << "Failed to make context current" << std::endl;
		abort();
	}

	Resourecmanager::instance().modelsinit();
	Resourecmanager::instance().obbinit();
	Resourecmanager::instance().shaderinit();
	Resourecmanager::instance().soundsinit();
	TextManager::instance().setshader(Resourecmanager::instance().getfromshadholder("textshader"));
	TextManager::instance().setup();

	Eventsmanager = std::make_shared<EventSystem>();

	srand(time(NULL));
	go = true;
	myentman = std::make_shared<entitymanager>();
	sceneMan = std::make_shared<SceneManager>(); //set our scene
	sceneMan->addstate(std::make_shared<MainGame>(sceneMan->getself(), myentman, Eventsmanager));
	sceneMan->init();
	frametimelast = SDL_GetTicks();
	framespersecond = 0;
	current = SDL_GetTicks();
	lastTime = current;

}


void ACC::engine::update()
{	
	current = SDL_GetTicks();
	deltaTs = (float)(current - lastTime) / 1000.0f; //track fps
	lastTime = current;

	inputlogger::instance().update(go);

	sceneMan->update(deltaTs,go);
	sceneMan->draw(deltaTs, go);

	framespersecond = current - frametimelast;
	frametimelast = current;
	framespersecond = (float)(1000.f / framespersecond);
	std::string fpstext = "FPS " + std::to_string(framespersecond);
	TextManager::instance().rendertext(fpstext, glm::vec3(1.0f, 0.0, 0.0), 1, 700.0f, 0.5f);

	if (go == false && sceneMan != NULL) // if we want to shut down
	{
		sceneMan->removescene(); //do clean up
		sceneMan.reset();
	}

	SDL_GL_SwapWindow(window);

}

void ACC::engine::cleanup() //clean up our engine
{
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();
	TextManager::instance().releaseshader();

	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
}


bool InitGL() // setup for opengl
{
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "GLEW failed to initialise due to: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return true;
}