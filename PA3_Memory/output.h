//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

class io
{
public:

	static void create(const char * const name);
	static void create(const char * const name, const char * const filename);

	static void destroy();
	static FILE *getHandle();

private:
	// big four
	io();
	~io();
	io(const io &) = delete;
	io & operator=(const io &) = delete;

	// methods
	static io	*privGetInstance();
	void		privDestroy();
	void		privCreate(const char * const name);
	void		privCreate(const char * const name, const char * const filename);
	FILE		*privGetHandle();

	// data
	FILE *fileHandle;

};

#endif //output.h