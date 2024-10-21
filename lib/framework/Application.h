#ifndef __APPLICAION_H__
#define __APPLICAION_H__

#include <linux/limits.h>
#include <unistd.h>

#include <string>

class Application
{
	Application() {}
	Application(const Application &app) {}
	Application &operator=(const Application &app) { return Get(); }
	~Application() {}

	int argc_;
	char **argv_;
	char **envp_;
	bool isInit_;

public:
	static Application &Get()
	{
		static Application app;
		return app;
	}
	void initArgs(int argc, char **argv, char **envp)
	{
		argc_ = argc;
		argv_ = argv;
		envp_ = envp;
		isInit_ = true;
	}
	int GetArgc() { return argc_; }
	char **GetArgv() { return argv_; }
	char **GetEnvp() { return envp_; }
	std::string GetArgPath()
	{
		if(!isInit_)
		{
		}
		std::string fullPath = argv_[0];
		auto slash = fullPath.rfind('/', -1UL);
		if(slash == -1UL)
			return "";
		return fullPath.substr(0, slash+1);
	}
	std::string GetArgFilename()
	{
		if(!isInit_)
		{
		}
		std::string fullPath = argv_[0];
		auto slash = fullPath.rfind('/', -1UL);
		if(slash == -1UL)
			return fullPath;
		return fullPath.substr(slash+1);
	}
	std::string GetProcPath()
	{
		std::string fullPath = GetProcFullPath();
		auto slash = fullPath.rfind('/', -1UL);
		if(slash == -1UL)
			return "";
		return fullPath.substr(0, slash+1);
	}
	std::string GetProcFilename()
	{
		std::string fullPath = GetProcFullPath();
		auto slash = fullPath.rfind('/', -1UL);
		if(slash == -1UL)
			return fullPath;
		return fullPath.substr(slash+1);
	}
	std::string GetProcFullPath()
	{
		char pBuf[PATH_MAX];
		ssize_t len = sizeof(pBuf);
		ssize_t bytes = readlink("/proc/self/exe", pBuf, len);
		if (bytes >= 0)
			pBuf[bytes] = '\0';
		return pBuf;
	}
};

#endif