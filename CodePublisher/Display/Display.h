#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../FileSystem/FileSystem.h"

class Display
{
public:
	Display();

	void display(const std::string& file);
	void display(const std::vector<std::string>& files);
};