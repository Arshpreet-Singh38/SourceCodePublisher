#include "Display.h"
#include "../Process/Process.h"
#include "../Process/Process.cpp"
#include "../FileSystem/FileSystem.cpp"

Display::Display() { }

void Display::display(const std::string& file)
{
    CBP callback = []() {
    };

    Process p;
    p.application("C:\\Windows\\System32\\cmd.exe");
    std::string cmd = "/C " + Path::getFullFileSpec(file);

    p.commandLine(cmd);
    p.create();
    p.setCallBackProcessing(callback);
    p.registerCallback();
    WaitForSingleObject(p.getProcessHandle(), INFINITE);
}

void Display::display(const std::vector<std::string>& files)
{
    for (auto file : files)
        display(file);
}