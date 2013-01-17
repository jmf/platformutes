#include <SDL/SDL_mixer.h>
#include "Sound.h"

#include <iostream>

Sound::Sound(std::string filename)
{
	chunk = Mix_LoadWAV(filename.c_str());
	if (!chunk)
		std::cerr << Mix_GetError() << std::endl;
}
Sound::~Sound()
{
	Mix_FreeChunk(chunk);
}

SoundManager::SoundManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		std::cerr << "[!] Error opening audio: " << Mix_GetError() << std::endl;
	Mix_AllocateChannels(10);
}
SoundManager::~SoundManager()
{
	Mix_CloseAudio();
}

void SoundManager::playSound(Sound *s)
{
	Mix_PlayChannel(-1, s->chunk, 0);
}

SoundManager &getSoundManager()
{
	static SoundManager soundManager;
	return soundManager;
}
