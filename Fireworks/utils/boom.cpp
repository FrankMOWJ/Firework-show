#include "boom.h"



void boom(){
	// std::cout << "begin to play music!\n";
	std::string path = FileSystem::getPath("boom.wav");
	const wchar_t* pathWideString = std::wstring(path.begin(), path.end()).c_str();
	PlaySound(pathWideString, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
	// std::cout << "music is over.\n";
}

void boom_shooting()
{
	std::string path = FileSystem::getPath("fireworks-shooting.wav");
	const wchar_t* pathWideString = std::wstring(path.begin(), path.end()).c_str();
	PlaySound(pathWideString, NULL, SND_FILENAME | SND_ASYNC | SND_NOSTOP);
}
