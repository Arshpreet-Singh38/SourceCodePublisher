#include "Process.h"
#include "../StringUtilities/StringUtilities.h"

using namespace Utilities;

CBP Process::cbp_ = []() {
};

Process::Process()
{
    GetStartupInfo(&startInfo_);
    startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    startInfo_.dwX = 200;
    startInfo_.dwY = 250;
    startInfo_.dwYSize = 300;
}

void Process::application(const std::string& appName)
{
    appName_ = sToW(appName);
}

void Process::commandLine(const std::string& cmdLine)
{
    commandLine_ = sToW(cmdLine);
}

void Process::title(const std::string& title)
{
    title_ = sToW(title);
    startInfo_.lpTitle = const_cast<LPWSTR>(title_.c_str());
}

bool Process::create(const std::string& appName)
{
    std::wstring app;
    if (appName.size() == 0)
        app = appName_;
    else
    {
        app = sToW(appName);
    }
    LPCTSTR applic = app.c_str();
    LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
    LPSECURITY_ATTRIBUTES prosec = nullptr;
    LPSECURITY_ATTRIBUTES thrdsec = nullptr;
    BOOL inheritHandles = false;
    DWORD createFlags = CREATE_NO_WINDOW;
    LPVOID environment = nullptr;
    LPCTSTR currentPath = nullptr;
    LPSTARTUPINFO pStartInfo = &startInfo_;
    LPPROCESS_INFORMATION pPrInfo = &procInfo_;

    BOOL OK =
        CreateProcess(
            applic, cmdLine, prosec, thrdsec, inheritHandles,
            createFlags, environment, currentPath, pStartInfo, pPrInfo
        );

    return OK;
}

void Process::setCallBackProcessing(CBP cbp)
{
    cbp_ = cbp;
}

void Process::callback()
{
    cbp_();
}

void CALLBACK WaitOrTimerCallback(_In_ PVOID lpParameter, _In_ BOOLEAN TimerOrWaitFired)
{
    Process p;
    p.callback();
    return;
}

void Process::registerCallback()
{
    HANDLE hNewHandle;
    HANDLE hProcHandle = procInfo_.hProcess;
    RegisterWaitForSingleObject(
        &hNewHandle, hProcHandle, WaitOrTimerCallback,
        NULL, INFINITE, WT_EXECUTEONLYONCE
    );
}

HANDLE Process::getProcessHandle()
{
    return procInfo_.hProcess;
}