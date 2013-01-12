#include <string>
#include <fstream>
#include <stdexcept>
#include "tools.h"
#include "ResourceMgr.h"
#include "Tile.h"
#include "Tileset.h"
#include "Level.h"

#include <iostream>

Level::Level(std::string filename):
	tileset(""), background(""),
	width(0), height(0),
	blocks(0)
{
	std::cout << "== Loading level: " << filename << "..." << std::endl;
	std::ifstream ifs(filename.c_str());
	std::string fl;
	getline(ifs,fl);
	std::vector<std::string> fls = split(stripspaces(fl),';');
	for (unsigned int i = 0; i < fls.size(); i++)
	{
		std::vector<std::string> kv = split(fls[i],':');
		if (kv.size() != 2) continue;
		std::string key = kv[0], value = kv[1];
		if (key == "width") width = toint(value);
		if (key == "height") height = toint(value);
		if (key == "tileset") tileset = value;
		if (key == "background") background = value;
	}
	if (tileset == "" || width == 0 || height == 0)
		throw std::runtime_error("Invalid level properties.");
	std::cout << "Tileset: " << tileset << std::endl;
	std::cout << "Background: " << background << std::endl;
	std::cout << "Size: " << width << "x" << height << std::endl;
	blocks = new int*[width];
	for (int x = 0; x < width; x++)
	{
		blocks[x] = new int[height];
		for (int y = 0; y < height; y++)
			blocks[x][y] = 0;
	}
	int x=0, y=0;
	std::string b("");
	char c(0);
	while (ifs.good())
	{
		if (x >= width || y >= height) break;
		c = ifs.get();
		if (c == '\n' || c == '\r')
		{
			if (b != "")
			{
				blocks[x][y] = toint(b);
				x=0;
				y++;
				b = "";
			}
		}
		else if (c == ' ' || c == '\t')
		{
			if (b != "")
			{
				blocks[x][y] = toint(b);
				x++;
				b = "";
			}
		}
		else
			b += c;
	}
	std::cout << "== Level loaded !" << std::endl;
}

Tile &Level::get(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		return getResourceMgr().getTileset(tileset)->get(blocks[x][y]);
	}
	return getAirTile();
}

Image *Level::getBackground()
{
	return getResourceMgr().getImage("backgrounds/"+background);
}
