#include "Log.h"

void Log::toFile(std::string filePath, std::string msg)
{
	std::ofstream out(filePath, std::ios::app);
	out << msg << std::endl;
	out.close();
}

void Log::toSdlError(std::string errorPath, std::string msg, const char *file, const int line)
{
	std::ofstream error(errorPath, std::ios::app);
	error << "[" << file << ":" << line << "]:" << msg << SDL_GetError() << std::endl;
	error.close();
}

/*
 * Different error statuses:
 * SF_ERR_NO_ERROR             = 0
 * SF_ERR_UNRECOGNISED_FORMAT  = 1
 * SF_ERR_SYSTEM			   = 2
 * SF_ERR_MALFORMED_FILE	   = 3
 * SF_ERR_UNSUPPORTED_ENCODING = 4
 *
 */
void Log::toSfError(std::string errorPath, std::string msg, SNDFILE *sndfile, const char *file, const int line)
{
	std::ofstream error(errorPath, std::ios::app);
	error << "[" << file << ":" << line << "]:" << msg << sf_error(sndfile) << std::endl;
	error.close();
}