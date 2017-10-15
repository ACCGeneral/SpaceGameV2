#include "inputlogger.h"
#include "keystruct.h"

inputlogger::inputlogger()
{
	mousex = 0;
	mousey = 0;
	leftmouseup = false;
	rightmouseup = false;

	createkeylog(SDLK_s);
	createkeylog(SDLK_w);
	createkeylog(SDLK_a);
	createkeylog(SDLK_d);
	createkeylog(SDLK_t);
	createkeylog(SDLK_UP);
	createkeylog(SDLK_DOWN);
	createkeylog(SDLK_LEFT);
	createkeylog(SDLK_RIGHT);

}

inputlogger & inputlogger::instance() //get the instance of our singleton class 
{
	static inputlogger myinstance;

	return myinstance;
}

void inputlogger::update(bool &go) //check for key and mouse inputs that we are conserned with 
{

	while (SDL_PollEvent(&incomingEvent))
	{
		switch (incomingEvent.type)
		{
			case SDL_QUIT:
			go = false;
			break;

			case SDL_KEYDOWN:
			{
					for (int i = 0; i < mykeys.size(); i++)
					{
						if (incomingEvent.key.keysym.sym == mykeys[i].mykey)
						{
							mykeys[i].down = true;
							mykeys[i].up = false;
						}
					}
				
			}
			break;

			case SDL_KEYUP:
			{
					for (int i = 0; i < mykeys.size(); i++)
					{
						if (incomingEvent.key.keysym.sym == mykeys[i].mykey)
						{
							mykeys[i].down = false;
							mykeys[i].up = true;
						}
				}

			}
			break;

			case SDL_MOUSEBUTTONDOWN:
				if (incomingEvent.button.button == SDL_BUTTON_LEFT)
				{
					leftmouseup = true;
				}
			break;
			case SDL_MOUSEBUTTONUP:
				if (incomingEvent.button.button == SDL_BUTTON_LEFT)
				{
					leftmouseup = false;
				}
				break;

		}
	}

	SDL_GetMouseState(&mousex, &mousey);

}

void inputlogger::createkeylog(int key) //crate a log for our key
{
	keystruct temp;
	temp.mykey = key;
	temp.up = true;
	temp.down = false;

	mykeys.push_back(temp);
}

bool inputlogger::getkeyup(int key) //check if key is up
{
	for (int i = 0; i < mykeys.size(); i++)
	{
		if (mykeys[i].mykey == key)
		{
			if (mykeys[i].up == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	}

}

bool inputlogger::getkeydown(int key) //check if key is down
{
	for (int i = 0; i < mykeys.size(); i++)
	{
		if (mykeys[i].mykey == key)
		{
			if (mykeys[i].down == true)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

	}
}

bool inputlogger::getleftmouse() //get if left mouse if down
{
	return leftmouseup;
}

