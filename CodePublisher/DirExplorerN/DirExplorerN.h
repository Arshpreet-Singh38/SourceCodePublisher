#pragma once

#include <vector>
#include <iostream>
#include "../FileSystem/FileSystem.h"

class DirExplorerN
{
public:
    using patterns = std::vector<std::string>;
    using files = std::vector<std::string>;

    DirExplorerN(const std::string& path);

    void addPattern(const std::string& patt);
    void recurse(bool doRecurse = true);

    bool search();
    void find(const std::string& path);
    bool match_regexes(const std::vector<std::string>& regexes);

    void doFile(const std::string& dir, const std::string& filename);
    void doDir(const std::string& dirname);

    const files& filesList() const;
    size_t fileCount();
    size_t dirCount();
    void showStats();

private:
    std::string path_;
    patterns patterns_;
    files files_;
    size_t dirCount_ = 0;
    size_t fileCount_ = 0;
    bool recurse_ = false;
};