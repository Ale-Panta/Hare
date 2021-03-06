//15-02-2021 Diego Cugno diego@cugno.org

#ifndef HR_PLATFORM_LINUX
	#ifndef __linux
		#warn "Compiler reports that the current compilation target isn't Linux. This file should be excluded from a compilation target that isn't Linux, UNIX, or POSIX."
	#endif
	#error "HR_PLATFORM_LINUX is not defined. Linux or a POSIX system is required to compile this file. Please define macro before compiling!"
#endif

#pragma once

#include "hrpch.h"
#include "Hare/Utils/PlatformUtils.h"
#include "Hare/Core/Application.h"

#include <GLFW/glfw3.h>

#define LINUX_PATH_MAX 4096 + 255 + 1

namespace Hare
{
    std::optional<std::string> FileDialogs::OpenFile(const char* filter)
	{

		const char* fileExtension = std::strchr(filter, 0) + 1;

		FILE *fp;
		char path[LINUX_PATH_MAX];

		std::string cmd = std::string("zenity --file-selection --file-filter=\"") +
			std::string(fileExtension) + std::string("\" --title=\"Open File\"");
		fp = popen(cmd.c_str(), "r");
		if (fp == NULL)
			HR_CORE_ERROR("Null file pointer in FileDialogs::OpenFile().\n");

		//Check for File dialog cancellation.
		if (fgets(path, LINUX_PATH_MAX, fp) == NULL)
		{
			pclose(fp);
			return std::nullopt;
		}

		pclose(fp);

		std::string fileChoice = std::string(path);
		int endLinePos = fileChoice.find_first_of("\n");
		fileChoice.resize(endLinePos);
		return fileChoice;
    }

    std::optional<std::string> FileDialogs::SaveFile(const char* filter)
	{
		FILE *fp;
		char path[LINUX_PATH_MAX];
		const char* fileExtension = std::strchr(filter, 0) + 1;
		std::string shortenedExt(fileExtension);
		shortenedExt.erase(0,1);

		std::string cmd = std::string("zenity --file-selection --file-filter=\"") +
			std::string(fileExtension) + std::string("\" --filename=\"") + shortenedExt
			+ ("\" --title=\"Save As File\" --save --confirm-overwrite");
		fp = popen(cmd.c_str(), "r");
		if (fp == NULL)
			HR_CORE_ERROR("Null file pointer in FileDialogs::OpenFile().\n");

		//Check for File dialog cancellation.
		if (fgets(path, LINUX_PATH_MAX, fp) == NULL)
		{
			pclose(fp);
			return std::nullopt;
		}

		pclose(fp);

		std::string fileChoice = std::string(path);
		int endLinePos = fileChoice.find_first_of("\n");
		fileChoice.resize(endLinePos);
		return fileChoice;
    }
}