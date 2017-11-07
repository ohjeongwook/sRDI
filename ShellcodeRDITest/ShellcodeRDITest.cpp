// ShellcodeRDITest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

extern "C" {
	ULONG_PTR ExecutePayload(ULONG_PTR uiLibraryAddress, DWORD dwFunctionHash, LPVOID lpUserData, DWORD nUserdataLen);
}

void DumpFile(LPSTR filename)
{
	HANDLE hFile;
	HANDLE hFileMapping;
	LPVOID lpFileBase;

	hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Couldn't open file with CreateFile()\n");
		return;
	}

	hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFileMapping == 0)
	{
		CloseHandle(hFile);
		printf("Couldn't open file mapping with CreateFileMapping()\n");
		return;
	}

	lpFileBase = MapViewOfFile(hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if (lpFileBase == 0)
	{
		CloseHandle(hFileMapping);
		CloseHandle(hFile);
		printf("Couldn't map view of file with MapViewOfFile()\n");
		return;
	}

	ExecutePayload((ULONG_PTR)lpFileBase, 0, NULL, 0);

	/*
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
	if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE)
	{
		// DumpExeFile(dosHeader);
	}
	else if ((dosHeader->e_magic == 0x014C)    // Does it look like a i386
		&& (dosHeader->e_sp == 0))        // COFF OBJ file???
	{
		// The two tests above aren't what they look like.  They're
		// really checking for IMAGE_FILE_HEADER.Machine == i386 (0x14C)
		// and IMAGE_FILE_HEADER.SizeOfOptionalHeader == 0;
		// DumpObjFile((PIMAGE_FILE_HEADER)lpFileBase);
	}
	else
		printf("unrecognized file format\n");
	*/

	UnmapViewOfFile(lpFileBase);
	CloseHandle(hFileMapping);
	CloseHandle(hFile);
}

int main()
{
	// DumpFile("c:\\mat\\bin\\hex2bin.exe");
	DumpFile("bin\\cmd.exe");
	// DumpFile("bin\\calc.exe");
	// DumpFile("bin\\notepad.exe");
    return 0;
}

