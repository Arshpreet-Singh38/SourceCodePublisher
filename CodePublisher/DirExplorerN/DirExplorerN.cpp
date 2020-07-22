#include <regex>
#include "DirExplorerN.h"
#include "../FileSystem/FileSystem.cpp"

DirExplorerN::DirExplorerN(const std::string& path) : path_(path)
{
	patterns_.push_back("*.*");
}

void DirExplorerN::addPattern(const std::string& patt)
{
	if (patterns_.size() == 1 && patterns_[0] == "*.*")
		patterns_.pop_back();
	patterns_.push_back(patt);
}

void DirExplorerN::recurse(bool doRecurse)
{
	recurse_ = doRecurse;
}

bool DirExplorerN::search()
{

	find(path_);
	if (files_.size() > 0) return true;
	return false;
}

void DirExplorerN::find(const std::string& path)
{

	std::string fpath = Path::getFullFileSpec(path);
	doDir(fpath);

	for (auto patt : patterns_)
	{
		std::vector<std::string> files = Directory::getFiles(fpath, patt);
		for (auto f : files)
		{
			doFile(fpath, f);
		}
	}

	std::vector<std::string> dirs = Directory::getDirectories(fpath);
	for (auto d : dirs)
	{
		if (d == "." || d == "..")
			continue;
		std::string dpath = fpath + "\\" + d;
		if (recurse_)
		{
			find(dpath);
		}
		else
		{
			doDir(dpath);
		}
	}
}


bool DirExplorerN::match_regexes(const std::vector<std::string>& regexes)
{
	std::vector<std::string> filteredFiles;

	for (auto file : files_) {
		std::string filename = Path::getName(file);
		for (auto regex : regexes) {
			std::regex r(regex);
			if (std::regex_match(filename, r)) {
				filteredFiles.push_back(file);
				break;
			}
		}
	}

	if (filteredFiles.size() > 0) {
		files_ = filteredFiles;
		return true;
	}

	return false;
}

void DirExplorerN::doFile(const std::string& dir, const std::string& filename)
{
	++fileCount_;
	std::string filepath = dir + "\\" + filename;
	files_.push_back(filepath);
}

void DirExplorerN::doDir(const std::string& dirname)
{
	++dirCount_;
}

const DirExplorerN::files& DirExplorerN::filesList() const
{
	return files_;
}

size_t DirExplorerN::fileCount()
{
	return fileCount_;
}

size_t DirExplorerN::dirCount()
{
	return dirCount_;
}

void DirExplorerN::showStats()
{
	std::ostringstream msg;
	msg << "\n\nProcessed " << fileCount_ << " files in " << dirCount_ << " directories";
	std::cout << msg.str();
}
