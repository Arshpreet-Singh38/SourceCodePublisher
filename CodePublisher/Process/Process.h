#pragma once

#include <windows.h>
#include <string>
#include <functional>

using CBP = std::function<void(void)>;

class Process
{
public:
	Process();
	virtual ~Process() {}
	bool create(const std::string& path = "");
	void title(const std::string& title);
	void application(const std::string& app);
	void commandLine(const std::string& cmdLine);
	void registerCallback();
	void setCallBackProcessing(CBP cbp);
	void callback();
	HANDLE getProcessHandle();

private:
	STARTUPINFO startInfo_;
	PROCESS_INFORMATION procInfo_;
	std::wstring title_;
	std::wstring appName_;
	std::wstring commandLine_;
	static CBP cbp_;
};