#include "freeHdr.h"


freeHdr::freeHdr(memU32 size, BlockType type, memBool aboveFree)
	: freeNext(0),
	freePrev(0),
	blockSize(size),		// size of block
	blockType((memU8)type),		// block type is used -> 0xAA  if block type is free -> 0xCC 
	aboveBlockFree(aboveFree),	// if(block is free) -> true or if(block is used) -> false
	pad1(0),			// future use
	pad2(0)
{
}

freeHdr::freeHdr()
{
}

freeHdr::freeHdr(const freeHdr &pFree)
{
	freeNext = pFree.freeNext;
	freePrev = pFree.freePrev;
	blockSize = pFree.blockSize;
	blockType = pFree.blockType;
	aboveBlockFree = pFree.aboveBlockFree;
	pad1 = pFree.pad1;
	pad2 = pFree.pad2;
}

freeHdr & freeHdr::operator = (const freeHdr &pFree)
{
	freeNext = pFree.freeNext;
	freePrev = pFree.freePrev;
	blockSize = pFree.blockSize;
	blockType = pFree.blockType;
	aboveBlockFree = pFree.aboveBlockFree;
	pad1 = pFree.pad1;
	pad2 = pFree.pad2;
	return *this;
}

freeHdr::~freeHdr()
{
}