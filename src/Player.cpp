#include <list>
#include <stack>
#include <algorithm>
#include "Sprite.h"
#include "Environment.h"
#include "Script.h"
#include "Tile.h"
#include "Player.h"

#include <iostream>

Player::Player() : Sprite("common.character", 32, 64) {}

void Player::doHit()
{
	for (int x = 0; x <= PSIZE.x; x += TILE_WIDTH)
	for (int y = 0; y <= PSIZE.y; y += TILE_HEIGHT)
	{
		vec2 t (
			((x+PPOS.x)/TILE_WIDTH),
			((y+PPOS.y)/TILE_HEIGHT));
		if (!(t.x*TILE_WIDTH+getSize().x >= p.x
		 && t.x*TILE_WIDTH <= p.x + getSize().x-1
		 && t.y*TILE_HEIGHT+getSize().y >= p.y
		 && t.y*TILE_HEIGHT <= p.y + getSize().y-1)) continue;
		runScript(ENV.lvl.get(t.x,t.y).getScripts()["on_hit"],0,t);
	}
	for (std::list<Sprite*>::iterator i = ENV.sprites.begin();
		i != ENV.sprites.end(); i++)
	{
		vec2 t = (*i)->getPos();
		if (t.x+getSize().x >= p.x
		 && t.x <= p.x + getSize().x-1
		 && t.y+getSize().y >= p.y
		 && t.y <= p.y + getSize().y-1)
			(*i)->hit();
	}
}

void Player::die()
{
	ENV.lvl.reload();
}

void Player::step()
{
	if (!physics) { Sprite::step(); return; }
	// check if player shall die
	if (
		PPOS.y > ENV.lvl.getHeight()*TILE_HEIGHT ||
		ENV.lvl.get(PPOS.x/TILE_WIDTH, (PPOS.y-1+TILE_HEIGHT*2)/TILE_HEIGHT).getHurt() ||
		ENV.lvl.get(PPOS.x/TILE_WIDTH+1, (PPOS.y-1+TILE_HEIGHT*2)/TILE_HEIGHT).getHurt()
		)
			die();
	// run tiles on_contact scripts
	// TODO maybe this needs some more work...
	std::stack<vec2> tokeep;
	for (int x = 0; x <= PSIZE.x; x += TILE_WIDTH)
	for (int y = 0; y <= PSIZE.y; y += TILE_HEIGHT)
	{
		vec2 t (
			((x+PPOS.x)/TILE_WIDTH),
			((y+PPOS.y)/TILE_HEIGHT));
		if (!(t.x*TILE_WIDTH+getSize().x >= p.x
		 && t.x*TILE_WIDTH <= p.x + getSize().x-1
		 && t.y*TILE_HEIGHT+getSize().y >= p.y
		 && t.y*TILE_HEIGHT <= p.y + getSize().y-1)) continue;
		bool c = false;
		for (std::list<vec2>::iterator i = contact.begin(); i != contact.end(); i++)
			if ((*i) == t)
				c = true;
		if (!c && !ENV.lvl.get(t.x,t.y).isSolid())
			runScript(ENV.lvl.get(t.x,t.y).getScripts()["on_contact"],0,t);
		tokeep.push(t);
	}
	contact.clear();
	while (!tokeep.empty())
	{
		contact.push_back(tokeep.top());
		tokeep.pop();
	}
	Sprite::step();
}

