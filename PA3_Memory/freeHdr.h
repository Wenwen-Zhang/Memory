//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef FREEHDR_H
#define FREEHDR_H

#include "memTypes.h"
#include "BlockType.h"

struct freeHdr
{
	freeHdr(memU32 , BlockType , memBool );

	//Big Four
	freeHdr();
	freeHdr(const freeHdr &);
	freeHdr & operator = (const freeHdr &);
	~freeHdr();
	

	//Data
	freeHdr *freeNext;		// next free block
	freeHdr *freePrev;		// prev free block
	memU32  blockSize;		// size of block
	memU8	blockType;		// block type is used -> 0xAA  if block type is free -> 0xCC
	memBool	aboveBlockFree;	// if(block is free) -> true or if(block is used) -> false
	memU8	pad1;			// future use
	memU8	pad2;			// future use

};

#endif //FREEHDR_H