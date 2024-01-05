#pragma once
#include<fstream>
#include<sstream>
#include<filesystem>
#include"../../../includes/json/json.hpp"
#include "../memory/winapi/winapi.hpp"
using json = nlohmann::json;
class File
{
public:
	bool DoesFileExist(const std::string& file) {
		struct stat buffer;
		return (stat(file.c_str(), &buffer) == false);
	}
};
class Json : public File
{
public:
	__inline std::string ReadJsonFile(std::string file, std::string section)
	{
		if (!std::filesystem::exists(file))
			pWinApi->GetMsgBox(NULL, skCrypt("File Not Found").decrypt(), skCrypt("Read Json").decrypt(), NULL);
		std::ifstream filepath(file);
		json data = json::parse(filepath);
		return data[section];
	}
	__inline bool CheckIfJsonExits(std::string file, std::string section)
	{
		if (!std::filesystem::exists(file))
			pWinApi->GetMsgBox(NULL, skCrypt("File Not Found").decrypt(), skCrypt("Check If Json").decrypt(), NULL);
		std::ifstream filepath(file);
		json data = json::parse(filepath);
		return data.contains(section);
	}
	__inline bool WriteJsonFile(std::string file, std::string name,
		std::string value)
	{
		json file_path;
		std::ofstream jsonFile(file, std::ios::out | std::ios::binary);
		jsonFile << file;
		jsonFile.close();
		if (!std::filesystem::exists(file))
			return false;
		return true;
	}
}; inline Json g_Json;