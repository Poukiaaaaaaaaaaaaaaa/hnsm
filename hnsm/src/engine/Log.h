#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

//SDL headers
#include <SDL2\SDL.h>

//libsndfile headers
#include <libsndfile\sndfile.h>

/*
 *	Classe dont les méthodes statiques permettent l'écriture
 *	simplifié dans des fichiers.
 *
 */
class Log
{
private:
	Log();
public:
	static void toFile(std::string filePath, std::string msg);
	static void toSdlError(std::string errorPath, std::string msg, const char *file, const int line);
	static void toSfError(std::string errorPath, std::string msg, SNDFILE *sndfile, const char *file, const int line);
};
