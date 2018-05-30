#include "FileConfig.h"

FileConfig::FileConfig() : m_filePath("") {}

FileConfig::FileConfig(std::string filePath) : m_filePath(filePath) // Le membre est initialisé avec la valeur du paramètre
{
    // On crée l'objet en ouvrant le fichier au chemin donné
	std::ifstream file(filePath);
	
	if (!file.is_open()) // Si l'objet n'existe pas, ou qu'il y a un probleme pour l'ouvrir, on annule la procédure
	{
		m_filePath = "";
	}

	file.close(); // On referme le fichier
}

std::string FileConfig::getStr(const std::string label)
{
	std::string string = "";

	std::ifstream file(m_filePath, std::ios::in); // On ouvre le fichier en lecture seule

	if (file.is_open())
	{
        // Tant que l'on est pas arrivé à la fin du fichier
        // ou que l'on n'a pas trouvé la chaîne correspondant au label
		while (!file.eof() && string == "")
		{
			std::string line;

			getline(file, line, '\n'); // On stocke la ligne entière dans une chaîne de caractère

			if (line.length() < 1 || line[0] == '>')
				continue; // Si la ligne est un commentaire ou est vide: ignorer

			size_t eqPos = line.find('='); // On récupère la position du caractère égal dans la ligne

			if (eqPos == std::string::npos)
				continue; // Si il n'y a pas de caractère égal: ignorer

			if (eqPos > line.find(' '))
				continue; // Si il y a un espace avant le caractère égal: ignorer

			if (label == line.substr(0, eqPos)) // Si le label trouvé correspond à celui recherché
			{
				string = line.substr(eqPos + 1); // ...on récupère la chaîne associée au label
			}
		}
	}

	file.close(); // Fermeture du fichier

    // Indication d'erreur si la recherche n'a rien donné
	if (string == "")
	{
		std::ofstream logFile(logFileName, std::ios::app);

		logFile << "Une erreur est survenue lors de la recherche dans le fichier: " << m_filePath << std::endl;
		logFile << "La recherche pour le label " << label << " n'a rien donné." << std::endl << std::endl;

		logFile.close();
	}

	return string; // On retourne la chaîne de caractère associée au label si elle a été trouvée, sinon la chaîne retournée est vide
}


std::string FileConfig::getPath(const std::string label)
{
	return m_filePath.substr(0, m_filePath.rfind('/') + 1) + getStr(label);
} // Renvoie la chaîne de caractère associée au label plus le chemin relatif au fichier par rapport à l'executable


int FileConfig::getVal(const std::string label)
{
	int value = -1;

	try // Essaie de convertir la chaîne correspondant au label en une valeur numérique
	{
		value = stoi(this->getStr(label));
	}
	catch (std::invalid_argument)
	{
		value = -1;
	} // Si il y a eu une erreur de conversion, on renvoie la valeur par défaut

	return value;
} // Renvoie la valeur associée au label


FileConfig::operator bool()
{
	return m_filePath != "";
} // Renvoie faux quand l'objet n'est relié à aucun fichier
