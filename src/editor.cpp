#include "Video.h"
#include "Sound.h"
#include "Level.h"
#include "Tileset.h"
#include "Environment.h"
#include "tools.h"
#include "ResourceMgr.h"

#include "editor.h"

#define EDITOR_FADE_RESET editor_fade = 0; editor_fadetime = SDL_GetTicks()+1000;
#define EDITOR_TILE_PREV if (editor_currenttile > 0) editor_currenttile --; else editor_currenttile = tiles.size()-1;
#define EDITOR_TILE_NEXT if (editor_currenttile < tiles.size()-1) editor_currenttile ++; else editor_currenttile = 0;

void startEditor()
{
	Device &d = getDevice();
	unsigned int editor_currenttile(0), editor_fade(BLOCK_WIDTH), editor_fadetime(0);
	PLAYER->enablePhysics(false);
	ENV.allowSprites = false;
	Level &lvl = ENV.lvl;
	SDL_Event e;
	while (d.run())
	{
		// draw background
		for (int x = 0; x <= d.getWidth();
			x += lvl.getBackground()->getWidth())
		{
			for (int y = 0; y <= d.getHeight();
				y += lvl.getBackground()->getHeight())
			{
				int dx = x-PPOS.x%BLOCK_WIDTH, dy = y-PPOS.y%BLOCK_HEIGHT;
				if (PPOS.x < d.getWidth()/2 || PPOS.x > lvl.getWidth()*BLOCK_WIDTH-d.getWidth()/2)
					dx = x;
				if (PPOS.y < d.getHeight()/2 || PPOS.y+BLOCK_HEIGHT > lvl.getHeight()*BLOCK_WIDTH-d.getHeight()/2)
					dy = y;
				d.drawImage(lvl.getBackground(),dx,dy);
			}
		}
		// update player position
		ENV.step();
		vec2 min (
			d.getWidth()/2/BLOCK_WIDTH*BLOCK_WIDTH,
			d.getHeight()/2/BLOCK_HEIGHT*BLOCK_HEIGHT);
		vec2 max (
			lvl.getWidth()*BLOCK_WIDTH-d.getWidth()/2/BLOCK_WIDTH*BLOCK_WIDTH,
			lvl.getHeight()*BLOCK_HEIGHT-d.getHeight()/2/BLOCK_HEIGHT*BLOCK_HEIGHT);
		if (PPOS.x < min.x) PLAYER->setPos(min.x, PPOS.y);
		if (PPOS.x > max.x) PLAYER->setPos(max.x, PPOS.y);
		if (PPOS.y < min.y) PLAYER->setPos(PPOS.x, min.y);
		if (PPOS.y > max.y) PLAYER->setPos(PPOS.x, max.y);
		// render
		ENV.lvl.render();
		// level editor
		std::vector<int> tiles = lvl.getTileset()->getValidTiles();
		for (int i = -5; i <= 5; i++)
		{
			d.drawImage(getResourceMgr().getImage("common/editor/slot"),
				d.getWidth()-BLOCK_WIDTH+editor_fade,
				d.getHeight()/2-BLOCK_HEIGHT/2+i*BLOCK_HEIGHT);
			int ci = (int)editor_currenttile + i;
			if (ci < 0) ci = tiles.size()+ci;
			if (ci >= (int)tiles.size()) ci = ci%tiles.size();
			Tile &t = lvl.getTileset()->get(tiles[ci]);
			if (!t.isAir())
			{
				d.drawImage(t.getImage(),
					d.getWidth()-BLOCK_WIDTH+editor_fade,
					d.getHeight()/2-BLOCK_HEIGHT/2+i*BLOCK_HEIGHT);
			}
		}
		d.drawImage(getResourceMgr().getImage("common/editor/select"),
			d.getWidth()-BLOCK_WIDTH+editor_fade,
			d.getHeight()/2-BLOCK_HEIGHT/2);
		if (SDL_GetTicks() > editor_fadetime && editor_fade < BLOCK_WIDTH)
			editor_fade+=2;
		int x, y;
		SDL_GetMouseState(&x, &y);
		d.drawImage(lvl.getTileset()->get(tiles[editor_currenttile]).getImage(), x-BLOCK_WIDTH/2, y-BLOCK_HEIGHT/2);
		// we're done, let's render
		d.render();
		// handle events
		while(d.hasEvent())
		{
			e = d.nextEvent();
			switch(e.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					vec2 p = getRealPos(vec2(e.button.x, e.button.y));
					switch (e.button.button)
					{
						case SDL_BUTTON_LEFT:
							lvl.set(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT,
								lvl.getTileset()->getValidTiles()[editor_currenttile]);
							break;
						case SDL_BUTTON_RIGHT:
							lvl.set(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT, 0);
							break;
						case SDL_BUTTON_WHEELUP:
							EDITOR_TILE_PREV
							EDITOR_FADE_RESET
							break;
						case SDL_BUTTON_WHEELDOWN:
							EDITOR_TILE_NEXT
							EDITOR_FADE_RESET
							break;
						case SDL_BUTTON_MIDDLE:
							for (int i = 0; i < tiles.size(); i++)
							{
								if (tiles[i]==lvl.getId(p.x/BLOCK_WIDTH, p.y/BLOCK_HEIGHT))
									editor_currenttile = i;
							}
							break;
						default:
							break;
					}
				} break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							PLAYER->setVel(-5,PLAYER->getVel().y);
							break;
						case SDLK_RIGHT:
							PLAYER->setVel(5,PLAYER->getVel().y);
							break;
						case SDLK_UP:
							PLAYER->setVel(PLAYER->getVel().x,-5);
							break;
						case SDLK_DOWN:
							PLAYER->setVel(PLAYER->getVel().x,5);
							break;
						case SDLK_PAGEDOWN:
							lvl.load_next();
							break;
						case SDLK_MINUS:
						case SDLK_KP_MINUS:
							EDITOR_TILE_PREV
							EDITOR_FADE_RESET
							break;
						case SDLK_PLUS:
						case SDLK_KP_PLUS:
							EDITOR_TILE_NEXT
							EDITOR_FADE_RESET
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (e.key.keysym.sym)
					{
						case SDLK_LEFT:
							if (PLAYER->getVel().x < 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						case SDLK_RIGHT:
							if (PLAYER->getVel().x > 0)
								PLAYER->setVel(0,PLAYER->getVel().y);
							break;
						case SDLK_UP:
							if (PLAYER->getVel().y < 0)
								PLAYER->setVel(PLAYER->getVel().x, 0);
							break;
						case SDLK_DOWN:
							if (PLAYER->getVel().y > 0)
								PLAYER->setVel(PLAYER->getVel().x, 0);
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
		}
	}
}
