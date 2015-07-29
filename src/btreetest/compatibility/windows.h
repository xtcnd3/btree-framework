/************************************************************
**
** file:	windows.h
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains declarations for functions compatible to
** the windows API, when including windows.h in Visual Studio.
** Further declarations will be added as needed.
**
************************************************************/

#ifndef	WINDOWS_H
#define	WINDOWS_H

#include <stdint.h>
#include <assert.h>

#include <stdlib.h>

#if 0

 #include <sys/time.h>

#else

 #include <time.h>

#endif

typedef	union
{
	struct
	{
		uint32_t	LowPart;
		int32_t		HighPart;
	} DUMMYSTRUCTNAME;
	struct
	{
		uint32_t	LowPart;
		int32_t		HighPart;
	} u;

	int64_t			QuadPart;
} LARGE_INTEGER;

bool QueryPerformanceCounter (LARGE_INTEGER *psPerformanceCount);
bool QueryPerformanceFrequency (LARGE_INTEGER *psFrequency);

#endif // WINDOWS_H
