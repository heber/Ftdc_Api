#pragma once

#define WINVER 0x0501             
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x501
#define _WIN32_IE 0x0600

#ifndef AI_ADDRCONFIG
#define AI_ADDRCONFIG 0x0020
#endif

//#define SOCKHANDLE_USE_OVERLAPPED

#define ASSERT(f) ((void)0)

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <windows.h>
#include <stdio.h>
#include <winsock2.h>
#include <tchar.h>
#include <strsafe.h>
#include <ws2tcpip.h>
#include <Iphlpapi.h>
#include <comdef.h>
#include "global.h"
//#include <iostream>
//using namespace std;
