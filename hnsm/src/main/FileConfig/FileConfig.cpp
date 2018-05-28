#include "FileConfig.h"

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
	std::string string;

	std::ifstream file(m_filePath, std::ios::in);

	if (file.is_open())
	{
		while (!file.eof())
		{
			std::string line;

			getline(file, line, '\n');

			if (line.length() < 1 || line[0] == '>')
				continue; // Si la ligne est un commentaire ou est vide: ignorer

			size_t eqPos = line.find('=');

			if (eqPos == std::string::npos)
				continue; // Si il n'y a pas de caract�re �gal: ignorer

			if (eqPos > line.find(' '))
				continue; // Si il y a un espace avant le caract�re �gal: ignorer

			if (label == line.substr(0, eqPos))
			{
				string = line.substr(eqPos + 1);
				break;
			}
		}
	}

	file.close();

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
