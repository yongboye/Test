#pragma once

#ifdef _WIN32
	#ifdef EXPORTDLL
	#define DLLEXPORT __declspec(dllexport)
	#else
	#define DLLEXPORT __declspec(dllimport)
	#endif
#else
	#define DLLEXPORT
#endif