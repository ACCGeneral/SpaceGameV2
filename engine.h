#pragma once


#include <time.h> 
#include <SDL.h>
#include "SDL.h"
#include "glew.h"
#include <memory>
#include <AL/al.h> 
#include <AL/alc.h>

class entitymanager;
class SceneManager;
class scene;

//base class for our engine setup

namespace ACC
{

	class engine
	{
	private:

		SDL_Renderer *renderer;
		SDL_Window *window;
		SDL_GLContext glcontext;
		unsigned int lastTime;
		unsigned int current;
		float deltaTs;
		std::shared_ptr<SceneManager> sceneMan;
		std::shared_ptr<entitymanager> myentman;
		ALCdevice *device;
		ALCcontext *context;

		int frametimelast;
		int framespersecond;

	public:

		bool OpenglSdlSetup(int width, int height);
		void init();
		void update();
		void cleanup();
		bool go;

	};

}