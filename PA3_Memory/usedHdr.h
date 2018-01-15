//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef USEDHDR_H
#define USEDHDR_H

#include "memTypes.h"
#include "BlockType.h"

struct freeHdr;

struct usedHdr
{
	//Big Four
	usedHdr();
	usedHdr(const usedHdr &);
	usedHdr & operator = (const usedHdr &);
	~usedHdr();

	usedHdr(freeHdr *);
	usedHdr(memU32 size, BlockType type, memBool aboveFree);

	usedHdr *usedNext;		// next used block
	usedHdr *usedPrev;		// prev used block
	memU32  blockSize;		// size of block
	memU8	blockType;		// block type is used -> 0xAA  if block type is free -> 0xCC
	memBool	aboveBlockFree;	// if(block is free) -> true or if(block is used) -> false
	memU8	pad0;           // future use
	memU8	pad1;			// future use
};

#endif //USEDHDR_H