#include <iostream>
#include "CodePublisher.h"
#include "../Converter/Converter.cpp"
#include "../DirExplorerN/DirExplorerN.cpp"
#include "../Display/Display.cpp"

using namespace Utilities;

ProcessCmdLine::Usage customUsage();

CodePublisher::CodePublisher()
{
}

bool CodePublisher::processCommandLineArgs(int argc, char** argv)
{
    pcl_ = new ProcessCmdLine(argc, argv);
    pcl_->usage(customUsage());
    preface("Command Line: ");
    pcl_->showCmdLine();
    if (pcl_->parseError())
    {
        pcl_->usage();
        std::cout << "\n\n";
        return false;
    }
    dirIn_ = pcl_->path();
    return true;
}

bool CodePublisher::extractFiles()
{
    DirExplorerN de(pcl_->path());

    for (auto patt : pcl_->patterns())
    {
        de.addPattern(patt);
    }

    if (pcl_->hasOption("s"))
    {
        de.recurse();
    }

    bool res = de.search() && de.match_regexes(pcl_->regexes());
    de.showStats();
    files_ = de.filesList();
    return res;
}

void CodePublisher::publish()
{
    display_.display(cconv_.convert(files_));
}

ProcessCmdLine::Usage customUsage()
{
    std::string usage;
    usage += "\n  Command Line: path [/option]* [/pattern]* [/regex]*";
    usage += "\n    path is relative or absolute path where processing begins";
    usage += "\n    [/option]* is of the form:";
    usage += "\n      /s     - walk directory recursively";
    usage += "\n    [pattern]* are one or more pattern strings of the form:";
    usage += "\n      *.h *.cpp *.cs *.txt or *.*";
    usage += "\n    [regex] regular expression(s), i.e. [A-B](.*)";
    usage += "\n";
    return usage;
}

int Process_ExtractFiles(CodePublisher& cp, int argc, char** argv) {
    if (argc < 2) return 1;
    if (!cp.processCommandLineArgs(argc, argv)) return 1;
    if (!cp.extractFiles()) { 
        return 2;
    }
    return 0;
}

void Convert_PublishFiles(CodePublisher& cp) {
    cp.publish();
}

int main(int argc, char** argv) {
    CodePublisher cp;
    int check = Process_ExtractFiles(cp, argc, argv);
    if (check == 1) {
        return 1;
    }
    else if (check == 2) {
        return 2;
    }
    
    Convert_PublishFiles(cp);
    return 0;
}