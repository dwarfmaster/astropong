// Gère le chemin d'accès aux ressources

#include "config.h"

const char* rcdir = RCDIR;

const char* getPath(const std::string& path)
{
	std::string dir = rcdir;
	dir += "/";
	dir += path;
	return dir.c_str();
}

