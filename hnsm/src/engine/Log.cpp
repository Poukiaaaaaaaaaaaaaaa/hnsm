#include "Log.h"

/*
 *	M�thode d'�criture basique: �crit une cha�ne de
 *	caract�res dans un fichier.
 *
 */
void Log::toFile(std::string filePath, std::string msg)
{
	std::ofstream out(filePath, std::ios::app);
	out << msg << std::endl;
	out.close();
}

/*
 *	M�thode d'�criture avec v�rification d'erreur SDL:
 *	R�cup�re une potentielle erreur li�es � SDL puis l'�crit dans
 *	un fichier, pr�c�d�e par le fichier et la ligne � laquelle
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
 *	M�me principe que la v�rification d'erreurs via SDL,
 *	cette fois ci utilisant la fonction de v�rification d'erreurs
 *	fournie par libsndfile.
 *
 */
void Log::toSfError(std::string errorPath, std::string msg, SNDFILE *sndfile, const char *file, const int line)
{
	std::ofstream error(errorPath, std::ios::app);
	error << "[" << file << ":" << line << "]:" << msg << sf_error(sndfile) << std::endl;
	error.close();
}
