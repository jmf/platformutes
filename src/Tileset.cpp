/*
 Platformutes
 Copyright (C) 2012-2013 Zeg9 <dazeg9@gmail.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <fstream>
#include "tools.h"
#include "Tile.h"
#include "Tileset.h"

#include <iostream>

Tileset::Tileset(std::string filename)
{
	std::cout << "== Loading tileset: " << filename << "..." << std::endl;
	std::ifstream ifs(filename.c_str());
	std::string contents;
	while (ifs.good())
		contents += ifs.get();
	std::vector<std::string> tokens = split(contents,'[');
	for (unsigned int i = 0; i < tokens.size(); i++)
	{
		std::vector<std::string> ls = split(tokens[i],']',2);
		if (ls.size() != 2) continue;
		int num = toint(stripspaces(ls[0]));
		if (!num) continue;
		tiles[num].parse(ls[1]);
		//std::cout << "Loaded tile " << num << " with image " << tiles[num].img << std::endl;
	}
	//std::cout << "== Tileset loaded !" << std::endl;
}

Tile &Tileset::get(int num)
{
	if (num >0 && tiles.find(num)!=tiles.end()) return tiles[num];
	return getAirTile();
}

std::vector<int> Tileset::getValidTiles()
{
	std::vector<int> r;
	for (std::map<int, Tile>::iterator i = tiles.begin(); i != tiles.end(); i++)
		r.push_back(i->first);
	return r;
}

