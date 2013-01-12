#include <iostream>
#include <SDL/SDL.h>
#include "Video.h"
#include "Image.h"
#include "Level.h"
#include "Tileset.h"
#include "ResourceMgr.h"

int main(int argc, char ** argv)
{
	if (argc > 1)
	{
		// you have no argument !
		std::cerr << "Usage: " << argv[0] << std::endl;
		return 1;
	}
	Device &d = getDevice();
	// TODO I DON'T WANNA SEE THIS HERE !
	Image *c = getResourceMgr().getImage("sprites/character/stand");
	int cx(32), cy(64), ncx(cx), ncy(cy), cxv(0), cyv(1); // character position and velocity; TODO be implemented elsewhere
	Level lvl("../data/levels/test.pmlvl");
	// </TODO>
	SDL_Event e;
	while (d.run())
	{
		// draw background
		for (int x = 0; x < d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y < d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				d.drawImage(lvl.getBackground(),x,y);
			}
		}
		// draw tiles
		for (int x = 0; x < d.getWidth()/BLOCK_WIDTH+1; x++)
		{
			for (int y = 0; y < d.getHeight()/BLOCK_HEIGHT+1; y++)
			{
				if (!lvl.get(x,y).isAir())
					d.drawImage(lvl.get(x,y).getImage(), x*BLOCK_WIDTH, y*BLOCK_HEIGHT);
			}
		}
		// draw sprites and character
		// TODO real sprite handling
		if (cyv < 2)
			cyv += 1;
		ncx = cx+cxv; ncy = cy+cyv;
		if (cyv < 0)
		{
			if (!(
			lvl.get(cx/BLOCK_WIDTH,ncy/BLOCK_HEIGHT).isSolid() ||
			lvl.get(cx/BLOCK_WIDTH+1,ncy/BLOCK_HEIGHT).isSolid()
			))
				cy = ncy;
			else
				cyv += 1;
		}
		if (cyv > 0)
		{
			if (!(
			lvl.get(cx/BLOCK_WIDTH,(ncy-1)/BLOCK_HEIGHT+2).isSolid() ||
			lvl.get(cx/BLOCK_WIDTH+1,(ncy-1)/BLOCK_HEIGHT+2).isSolid()
			))
				cy = ncy;
			else
				cyv -= 1;
		}
		if (cxv < 0 && !(
			lvl.get(ncx/BLOCK_WIDTH, (cy-1)/BLOCK_HEIGHT+1).isSolid() ||
			lvl.get(ncx/BLOCK_WIDTH, (cy-1)/BLOCK_HEIGHT+2).isSolid()
			))
			cx = ncx;
		if (cxv > 0 && !(
			lvl.get((ncx-1)/BLOCK_WIDTH+1, (cy-1)/BLOCK_HEIGHT+1).isSolid() ||
			lvl.get((ncx-1)/BLOCK_WIDTH+1, (cy-1)/BLOCK_HEIGHT+2).isSolid()
			))
			cx = ncx;
		if (
			lvl.get(cx/BLOCK_WIDTH, (cy-1)/BLOCK_HEIGHT+2).getHurt() ||
			lvl.get(cx/BLOCK_WIDTH+1, (cy-1)/BLOCK_HEIGHT+2).getHurt()
			)
		{ cx = 32; cy = 64; }
		d.drawImage(c, cx, cy);
		// </TODO>
		// handle events
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							cxv = -2;
							break;
						case SDLK_RIGHT:
							cxv = 2;
							break;
						case SDLK_UP:
							cyv = -10;
							break;
						case SDLK_DOWN:
							cyv = 2;
							break;
						case SDLK_p:
							std::cout << cx << ',' << cy << std::endl;
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
						case SDLK_RIGHT:
							cxv = 0;
							break;
						case SDLK_UP:
						case SDLK_DOWN:
							cyv = 0;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
		// we're done, let's render
		d.render();
	}
	// Bye !
	return 0;
}
