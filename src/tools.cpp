#include <sstream>
#include <vector>
#include "Environment.h"
#include "Video.h"
#include "tools.h"

#include <iostream>

std::string tostring(int i)
{
	std::ostringstream oss;
	oss << i;
	return oss.str();
}

int toint(std::string s)
{
	std::istringstream iss(s);
	int i;
	iss >> i;
	return i;
}

bool tobool(std::string s)
{
	if (toint(s)) return true;
	if (lowercase(s)=="true") return true;
	return false;
}

int round(int i, int f)
{
	if (i%f > f/2)
		return (i/f+1)*f;
	return (i/f)*f;
}

std::vector<std::string> split(std::string in, char sep, unsigned int max)
{
	std::vector<std::string> r;
	std::string current("");
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (in[i] == sep && (max==0 || r.size()+1 < max))
		{
			if (current != "")
				r.push_back(current);
			current = "";
		}
		else
			current += in[i];
	}
	if (current != "")
		r.push_back(current);
	return r;
}

std::string strip(std::string in, std::string what)
{
	std::string out("");
	for (unsigned int i = 0; i < in.size(); i++)
	{
		if (what.find(in[i]) == what.npos)
			out += in[i];
	}
	return out;
}

std::string stripspaces(std::string in)
{
	return strip(in, " \t\n\r");
}

std::string lowercase(std::string in)
{
	std::string out;
	for (unsigned int i = 0; i < in.size(); i++)
		out += tolower(in[i]);
	return out;
}

bool startswith(std::string s, std::string what)
{
	if (s.size() < what.size()) return false;
	if (s.substr(0,what.size()) == what) return true;
	return false;
}

vec2::vec2(int _x, int _y) :
	x(_x), y(_y)
	{}

bool operator==(vec2 const&a, vec2 const&b)
{
	return a.x == b.x && a.y == b.y;
}

vec2 getDrawPos(vec2 p)
{
	Device &d = getDevice();
	vec2 dp (
		p.x - PPOS.x + d.getWidth()/2/TILE_WIDTH*TILE_WIDTH,
		p.y - PPOS.y + d.getHeight()/2/TILE_HEIGHT*TILE_HEIGHT
	);
	int lw = ENV.lvl.getWidth()*TILE_WIDTH,
	    lh = ENV.lvl.getHeight()*TILE_HEIGHT;
	if (PPOS.x < d.getWidth()/2)
		dp.x = p.x;
	else if (PPOS.x > lw-d.getWidth()/2)
		dp.x = p.x - lw + d.getWidth();
	if (PPOS.y < d.getHeight()/2)
		dp.y = p.y;
	else if (PPOS.y+TILE_HEIGHT > lh-d.getHeight()/2)
		dp.y = p.y - lh + d.getHeight();
	return dp;
}

vec2 getRealPos(vec2 dp)
{
	Device &d = getDevice();
	vec2 p (
		dp.x + PPOS.x - d.getWidth()/2/TILE_WIDTH*TILE_WIDTH,
		dp.y + PPOS.y - d.getHeight()/2/TILE_HEIGHT*TILE_HEIGHT
	);
	int lw = ENV.lvl.getWidth()*TILE_WIDTH,
	    lh = ENV.lvl.getHeight()*TILE_HEIGHT;
	if (PPOS.x < d.getWidth()/2)
		p.x = dp.x;
	else if (PPOS.x > lw-d.getWidth()/2)
		p.x = dp.x + lw - d.getWidth();
	if (PPOS.y < d.getHeight()/2)
		p.y = dp.y;
	else if (PPOS.y+TILE_HEIGHT > lh-d.getHeight()/2)
		p.y = dp.y + lh - d.getHeight();
	return p;
}

