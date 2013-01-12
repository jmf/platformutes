#ifndef __VIDEO_H__
#define __VIDEO_H__

#include <stack>
#include <SDL/SDL.h>

// You can change the resolution here
// It is recommended to keep a relatively small one,
// because the game become really laggy with higher ones.
// Default: 640x480
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480

// Set this to 8 to get an old-school 8 bit style
// Default: 32
#define VIDEO_BPP 32

// SDL_FULLSCREEN if you want fullscreen,
// 0 if you don't want it.
#define VIDEO_SDL_FLAGS SDL_FULLSCREEN

// You can change the FPS here, but you shouldn't
// If you set higher FPS all the game will be faster,
// even the character, ennemies etc. and your CPU needs to handle it.
#define MAX_FPS 60

// You shouldn't modify this.
#define TBF 1000/MAX_FPS // Time between frames


class Image;

class Device
{
	friend Device &getDevice();
	public:
		int getWidth();
		int getHeight();
		void drawImage(Image *i, int x=0, int y=0);
		void render();
		bool run();
		bool hasEvent();
		SDL_Event nextEvent();
	private:
		Device();
		~Device();
		SDL_Surface *screen;
		std::stack<SDL_Event> eventStack;
		int lastticks; // last frame SDL_GetTicks()
		bool done; // When set to true, the application exits
};
Device &getDevice();

#endif//__VIDEO_H__
