#include "Log.h"

/*
 *	Méthode d'écriture basique: écrit une chaîne de
 *	caractères dans un fichier.
 *
 */
void Log::toFile(std::string filePath, std::string msg)
{
	std::ofstream out(filePath, std::ios::app);
	out << msg << std::endl;
	out.close();
}

/*
 *	Méthode d'écriture avec vérification d'erreur SDL:
 *	Récupère une potentielle erreur liées à SDL puis l'écrit dans
 *	un fichier, précédée par le fichier et la ligne à laquelle
 *	l'erreur est intervenue.
 *
 */
void Log::toSdlError(std::string errorPath, std::string msg, const char *file, const int line)
{
	std::ofstream error(errorPath, std::ios::app);
	error << "[" << file << ":" << line << "]:" << msg << SDL_GetError() << std::endl;
	error.close();
}

/*
 *	Même principe que la vérification d'erreurs via SDL,
 *	cette fois ci utilisant la fonction de vérification d'erreurs
 *	fournie par libsndfile.
 *
 */
void Log::toSfError(std::string errorPath, std::string msg, SNDFILE *sndfile, const char *file, const int line)
{
	std::ofstream error(errorPath, std::ios::app);
	error << "[" << file << ":" << line << "]:" << msg << sf_error(sndfile) << std::endl;
	error.close();
}
