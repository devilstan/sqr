#ifndef __WIN32_TO_ANSI__
#define __WIN32_TO_ANSI__

#ifndef WIN32
	#include <string.h>
	#include <stdlib.h>

	typedef bool            BOOL;
	typedef unsigned char   BYTE;
	typedef unsigned short  WORD;
	typedef char*           LPCSTR;
	typedef unsigned char * LPBYTE;

	#ifndef TRUE
		#define TRUE  true
	#endif
	#ifndef FALSE
		#define FALSE false
	#endif
	
	#define lstrlen strlen
	#define ZeroMemory(x, y) memset(x, 0, y)
	#define min(x, y) x < y ? x : y

#else
	#define XMD_H
	#include <windows.h>
#endif

#endif
