#pragma once

#include <string>
#include <vector>
#include "../CodeUtilities/CodeUtilities.h"
#include "../Converter/Converter.h"
#include "../Display/Display.h"

class CodePublisher
{
public:
	CodePublisher();
	bool processCommandLineArgs(int argc, char** argv);
	bool extractFiles();
	void publish();

private:
	Utilities::ProcessCmdLine* pcl_;
	CodeConverter cconv_;
	Display display_;

	std::string dirIn_;
	std::string dirOut_;
	std::vector<std::string> files_;
};