#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"

class FileConfig
{
public:
	FileConfig();
	FileConfig(std::string filePath);
	std::string getStr(const std::string label);
	std::string getPath(const std::string label);
	int	    getVal(const std::string label);

	explicit operator bool();

private:
	std::string m_filePath;
};
