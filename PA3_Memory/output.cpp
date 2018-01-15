//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <assert.h>
#include <string.h>

// locals
#include "output.h"


io::io()
	:fileHandle(0)
{
};

io::~io()
{
	if (0 != fileHandle)
	{
		privDestroy();
	}
};

void io::create(const char * const name)
{
	privGetInstance()->privCreate(name);
}

void io::create(const char * const name, const char * const filename)
{
	privGetInstance()->privCreate(name,filename);
}

void io::destroy()
{
	privGetInstance()->privDestroy();
}

FILE *io::getHandle()
{
	return privGetInstance()->privGetHandle();
}

io *io::privGetInstance()
{
	static io instance;
	return &instance;
}

void io::privDestroy()
{
	errno_t fileError;
	assert(fileHandle);

	fileError = fflush(fileHandle);
	assert(!fileError);

	fileError = fclose(fileHandle);
	fileHandle = 0;
	assert(!fileError);
}

void io::privCreate(const char * const name)
{
	errno_t fileError;
	char fileName[64];
	char *out_string = "Output_";

	strcpy_s(fileName, 64, out_string);

#ifdef _DEBUG
	char *debug_string = "Debug.txt";
	strcat_s(fileName, 64, debug_string);
#endif

#ifdef NDEBUG
	char *debug_string = "Release.txt";
	strcat_s(fileName, 64, debug_string);
#endif

	fileError = fopen_s(&fileHandle, fileName, "wt");
	assert(fileHandle);

	fprintf(fileHandle, "Program Assignment: 2 \n");
	fprintf(fileHandle, "Name: %s\n", name);
	fprintf(fileHandle, "\n");
	fprintf(fileHandle, "-------------------------------------------------\n");
	fprintf(fileHandle, "\n");
}

void io::privCreate(const char * const name, const char * const filename)
{
	errno_t fileError;
	char fileName[64];
	const char *out_string = filename;

	strcpy_s(fileName, 64, out_string);

#ifdef _DEBUG
	char *debug_string = "_Debug.txt";
	strcat_s(fileName, 64, debug_string);
#endif

#ifdef NDEBUG
	char *debug_string = "_Release.txt";
	strcat_s(fileName, 64, debug_string);
#endif

	fileError = fopen_s(&fileHandle, fileName, "wt");
	assert(fileHandle);

	fprintf(fileHandle, "Program Assignment: 2 \n");
	fprintf(fileHandle, "Name: %s\n", name);
	fprintf(fileHandle, "\n");
	fprintf(fileHandle, "-------------------------------------------------\n");
	fprintf(fileHandle, "\n");
}

FILE *io::privGetHandle()
{
	assert(fileHandle);
	return fileHandle;
}


