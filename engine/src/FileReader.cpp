#include "FileReader.h"
#include <fstream>
#include <sstream>

namespace RoomEng{
	namespace FileReader{
		std::string loadAsString(std::string path){
			std::ifstream file(path);
			if(!file){
				return "";
			}
			std::stringstream buff;
			buff << file.rdbuf();
			file.close();
			return buff.str();
		}

		bool fileExists(std::string path){
			std::ifstream file(path);
    		return file.good();
		}
	}
}