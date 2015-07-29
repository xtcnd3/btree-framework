/************************************************************
**
** file:	windows.cpp
** author:	Andreas Steffens
** license:	GPL v2
**
** description:
**
** This file contains code offering functions compatible to
** the windows API, when including windows.h in Visual Studio.
** Function calls will be added as needed.
**
************************************************************/

#if !defined (WIN32)

#include <windows.h>

bool QueryPerformanceCounter (LARGE_INTEGER *psPerformanceCount)
{
#if 0

    struct timeval		sTime;

    gettimeofday(&time, NULL);
    psPerformanceCount->QuadPart = sTime.tv_usec + // microseconds
                                   sTime.tv_sec * 1000000; // seconds

    return (true);

#else

	struct timespec		sTime;

	int		nRetVal = clock_gettime (CLOCK_MONOTONIC_RAW, &sTime);

	psPerformanceCount->QuadPart = sTime.tv_nsec; // nanoseconds
	psPerformanceCount->QuadPart += sTime.tv_sec * 1000000000; // seconds

	return (nRetVal == 0);

#endif
}

bool QueryPerformanceFrequency (LARGE_INTEGER *psFrequency)
{
#if 0

	psFrequency->QuadPart = 1000000;

#else

	psFrequency->QuadPart = 1000000000;

#endif

	return (true);
}

#endif
