#include "usedHdr.h"
#include "freeHdr.h"
#include "BlockType.h"
#include <assert.h>

usedHdr::usedHdr(freeHdr * pFree)
	: usedNext(0), 	
	  usedPrev(0),		// prev used block
	  blockSize(pFree->blockSize),		// size of block
	  blockType((memU8)BlockType::Used),	// block type is used -> 0xAA  if block type is free -> 0xCC
	  aboveBlockFree(false),	// if(block is free) -> true or if(block is used) -> false 
	  pad0(0),           // future use
	  pad1(0)			// future use
{
}

usedHdr::usedHdr(memU32 size, BlockType type, memBool aboveFree)
	: usedNext(0),
	  usedPrev(0),		// prev used block
	  blockSize(size),		// size of block
	  blockType((memU8)type),	// block type is used -> 0xAA  if block type is free -> 0xCC
	  aboveBlockFree(aboveFree),	// if(block is free) -> true or if(block is used) -> false 
	  pad0(0),           // future use
	  pad1(0)
{
}

usedHdr::usedHdr()
{
}

usedHdr::usedHdr(const usedHdr &pUsed)
{
	usedNext = pUsed.usedNext;		
	usedPrev = pUsed.usedPrev;
	blockSize = pUsed.blockSize;
	blockType = pUsed.blockType;
	aboveBlockFree = pUsed.aboveBlockFree;	
	pad0 = pUsed.pad0;           
	pad1 = pUsed.pad1;			
}

usedHdr & usedHdr::operator = (const usedHdr &pUsed)
{
	usedNext = pUsed.usedNext;
	usedPrev = pUsed.usedPrev;
	blockSize = pUsed.blockSize;
	blockType = pUsed.blockType;
	aboveBlockFree = pUsed.aboveBlockFree;
	pad0 = pUsed.pad0;
	pad1 = pUsed.pad1;
	return *this;
}

usedHdr::~usedHdr()
{
}