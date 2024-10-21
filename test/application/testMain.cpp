#include <iostream>
#include <exception>

#include <unistd.h>

#include "framework/Application.h"
#include "framework/Logger.h"

int AppMain()
{
	LogSinks		logSinks(LogSinkConsole::GetDefaultSink());
	LogBuffer<char>	logBuffer(&logSinks);
	std::ostream	log(&logBuffer);

	log << "Hello, Testing world." << std::endl;

	log << "- ProcFullPath: " << Application::Get().GetProcFullPath() << std::endl;
	log << "-     ProcPath: " << Application::Get().GetProcPath() << std::endl;
	log << "- ProcFilename: " << Application::Get().GetProcFilename() << std::endl;
	log << "-      ArgPath: " << Application::Get().GetArgPath() << std::endl;
	log << "-  ArgFilename: " << Application::Get().GetArgFilename() << std::endl;

	return 0;
}
