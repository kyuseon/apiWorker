#include <iostream>

#include <unistd.h>

#include "Application.h"

extern int AppMain();

int main(int argc, char **argv, char **envp)
{
	Application::Get().initArgs(argc, argv, envp);

	return AppMain();
}
