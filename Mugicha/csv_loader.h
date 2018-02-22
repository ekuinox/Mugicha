#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

bool csv_loader(const std::string& filename, std::vector<std::vector<std::string>>& table, const char delimiter = ',')
{
	std::fstream filestream(filename);

	if (!filestream.is_open()) return false;

	while (!filestream.eof())
	{
		std::string buffer;
		filestream >> buffer;

		std::vector<std::string> record;
		std::istringstream streambuffer(buffer);
		std::string token;
		
		while (std::getline(streambuffer, token, delimiter))
		{
			if (token != "") record.push_back(token); // 空白をスキップ
		}

		table.push_back(record);
	}

	return true;
}