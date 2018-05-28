#include "FileConfig.h"

static std::string logFileName = "FileConfig.log";

FileConfig::FileConfig(std::string filePath) : m_filePath(filePath)
{
	std::ifstream file(filePath);
	
	if (!file.is_open())
	{
		m_filePath = "";
	}

	file.close();
}

std::string FileConfig::getStr(const std::string label)
{
	std::string string = "";

	std::ifstream file(m_filePath, std::ios::in);

	if (file.is_open())
	{
		while (!file.eof() && string == "")
		{
			std::string line;

			getline(file, line, '\n');

			if (line.length() < 1 || line[0] == '>')
				continue; // Si la ligne est un commentaire ou est vide: ignorer

			size_t eqPos = line.find('=');

			if (eqPos == std::string::npos)
				continue; // Si il n'y a pas de caractère égal: ignorer

			if (eqPos > line.find(' '))
				continue; // Si il y a un espace avant le caractère égal: ignorer

			if (label == line.substr(0, eqPos))
			{
				string = line.substr(eqPos + 1);
			}
		}
	}

	file.close();

	if (string == "")
	{
		std::ofstream logFile(logFileName, std::ios::app);

		logFile << "Une erreur est survenue lors de la recherche dans le fichier: " << m_filePath << std::endl;
		logFile << "La recherche pour le label " << label << " n'a rien donné." << std::endl << std::endl;

		logFile.close();
	}

	return string;
}

std::string FileConfig::getPath(const std::string label)
{
	return m_filePath.substr(0, m_filePath.rfind('/') + 1) + getStr(label);
}

int FileConfig::getVal(const std::string label)
{
	int value = -1;

	try
	{
		value = stoi(this->getStr(label));
	}
	catch (std::invalid_argument)
	{
		value = -1;
	}

	return value;
}

FileConfig::operator bool()
{
	return m_filePath != "";
}
