#pragma once

#include <iostream>
#include <fstream>
#include <string>

#include "Log.h"

class FileConfig
{
public:
	FileConfig(std::string filePath);
	std::string getStr(const std::string label);
	std::string getPath(const std::string label);
	int	        getVal(const std::string label);

	explicit operator bool();

	//static bool init(std::string configPath);

private:
	std::string m_filePath;

	//static std::string _configPath;

};