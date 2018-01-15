//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "heapHdr.h"
#include "mem.h"
#include "freeHdr.h"
#include <assert.h>
#include "usedHdr.h"

heapHdr::heapHdr(void * ptr)
:	usedHead(0),
	freeHead(0),
	heapInitialize(false),
	bytePad1(0),
	bytePad2(0),
	bytePad3(0),
	pad1(0),
	pad2(0),
	pad3(0)
{
	stats.peakNumUsed = 0;			// number of peak used allocations
	stats.peakUsedMemory  = 0;		// peak size of used memory

	stats.currNumUsedBlocks =0;		// number of current used allocations
	stats.currUsedMem =0;			// current size of the total used memory

	stats.currNumFreeBlocks =0;		// number of current free blocks
	stats.currFreeMem =0 ;			// current size of the total free memory

	stats.heapTopAddr = reinterpret_cast<memVoid *> ( (memU8 *)ptr + sizeof(heapHdr) );		// start address available heap
	stats.heapBottomAddr = reinterpret_cast<memVoid *> ( (memU8 *)ptr + HEAP_SIZE );		// last address available heap
	
	stats.sizeHeap = (memU32)stats.heapBottomAddr - (memU32)stats.heapTopAddr + sizeof(heapHdr);				// size of Heap total space, including header
	stats.sizeHeapHeader = sizeof(heapHdr);		// size of heap header
}

memStats::memStats()
{
}

memStats::memStats(const memStats & pStats)
{
	peakNumUsed = pStats.peakNumUsed;		
	peakUsedMemory = pStats.peakUsedMemory;

	currNumUsedBlocks = pStats.currNumUsedBlocks;
	currUsedMem = pStats.currUsedMem;

	currNumFreeBlocks = pStats.currNumFreeBlocks;
	currFreeMem = pStats.currFreeMem;

	sizeHeap = pStats.sizeHeap;
	sizeHeapHeader = pStats.sizeHeapHeader;

	heapTopAddr = pStats.heapTopAddr;
	heapBottomAddr = pStats.heapBottomAddr;
}

memStats & memStats::operator = (const memStats &pStats)
{
	
	peakNumUsed = pStats.peakNumUsed;
	peakUsedMemory = pStats.peakUsedMemory;

	currNumUsedBlocks = pStats.currNumUsedBlocks;
	currUsedMem = pStats.currUsedMem;

	currNumFreeBlocks = pStats.currNumFreeBlocks;
	currFreeMem = pStats.currFreeMem;

	sizeHeap = pStats.sizeHeap;
	sizeHeapHeader = pStats.sizeHeapHeader;

	heapTopAddr = pStats.heapTopAddr;
	heapBottomAddr = pStats.heapBottomAddr; 
	
	return *this;
}

memStats::~memStats()
{
}

heapHdr::heapHdr()
{
}

heapHdr::heapHdr(const heapHdr &pHeap)
{
	this->usedHead = pHeap.usedHead;
	this->freeHead = pHeap.freeHead;
	this->heapInitialize = pHeap.heapInitialize;
	this->bytePad1 = pHeap.bytePad1;
	this->bytePad2 = pHeap.bytePad2;
	this->bytePad3 = pHeap.bytePad3;
	this->pad1 = pHeap.pad1;
	this->pad2 = pHeap.pad2;
	this->pad3 = pHeap.pad3;
	this->stats = pHeap.stats;
}

heapHdr & heapHdr::operator = (const heapHdr &pHeap)
{
	this->usedHead = pHeap.usedHead;
	this->freeHead = pHeap.freeHead;
	this->heapInitialize = pHeap.heapInitialize;
	this->bytePad1 = pHeap.bytePad1;
	this->bytePad2 = pHeap.bytePad2;
	this->bytePad3 = pHeap.bytePad3;
	this->pad1 = pHeap.pad1;
	this->pad2 = pHeap.pad2;
	this->pad3 = pHeap.pad3;
	this->stats = pHeap.stats;
	return *this;
}

heapHdr::~heapHdr()
{
}

void heapHdr::AddStats(const freeHdr * const pFree)
{
	this->stats.currNumFreeBlocks++;
	this->stats.currFreeMem += pFree->blockSize;
}

void heapHdr::AddStats(const usedHdr * const pUsed)
{	
	stats.currNumUsedBlocks++;
	stats.currUsedMem += pUsed->blockSize;
	
	if (stats.currNumUsedBlocks > stats.peakNumUsed)
	{
		stats.peakNumUsed = stats.currNumUsedBlocks;
	}

	if (stats.currUsedMem > stats.peakUsedMemory)
	{
		stats.peakUsedMemory = stats.currUsedMem;
	}
}

void heapHdr::MinusStats(const freeHdr * const pFree)
{
	this->stats.currNumFreeBlocks--;
	this->stats.currFreeMem -= pFree->blockSize;
}

void heapHdr::MinusStats(const usedHdr * const pUsed)
{
	stats.currNumUsedBlocks--;
	stats.currUsedMem -= pUsed->blockSize;
}

void heapHdr::AddFreeBlock(freeHdr * const pFree)
{
	assert(pFree);
	
	if (this->freeHead == 0)
	{
		this->freeHead = pFree;
		pFree->freeNext = 0;
		pFree->freePrev = 0;
	}
	else if (pFree < this->freeHead)
	{
		pFree->freeNext = this->freeHead;
		this->freeHead->freePrev = pFree;
		this->freeHead = pFree;
		pFree->freePrev = 0;
	}
	else
	{
		freeHdr *pTemp = this->freeHead;

		while (pTemp != 0)
		{
			if (pFree < pTemp->freeNext || pTemp->freeNext == 0)
			{
				break;
			}
			pTemp = pTemp->freeNext;
		}

		if (pTemp->freeNext == 0)
		{
			pTemp->freeNext = pFree;
			pFree->freePrev = pTemp;
			pFree->freeNext = 0;
		}
		else
		{
			pFree->freePrev = pTemp;
			pFree->freeNext = pTemp->freeNext;
			pTemp->freeNext->freePrev = pFree;
			pTemp->freeNext = pFree;
		}
	}

	//Update stats
	this->AddStats(pFree);
}

void heapHdr::RemoveFreeBlock(freeHdr * const pFree)
{
	assert(pFree);

	if (pFree->freeNext != 0)
	{	
		if (pFree->freePrev == 0)
		{
			this->freeHead = pFree->freeNext;
			pFree->freeNext->freePrev = 0;
		}
		else
		{
			pFree->freeNext->freePrev = pFree->freePrev;
			pFree->freePrev->freeNext = pFree->freeNext;
		}
	}

	else
	{
		if (pFree->freePrev == 0)
		{
			this->freeHead = 0;
		}
		else
		{
			pFree->freePrev->freeNext = 0;
		}
	}

	//Update stats
	this->MinusStats(pFree);
}

freeHdr * heapHdr::FindFreeBlock(memU32 size) const
{
	freeHdr *pTemp = this->freeHead;

	while (pTemp != 0)
	{
		if (pTemp->blockSize >= size)
		{
			break;
		}
		pTemp = pTemp->freeNext;
	}

	return pTemp;
}

void heapHdr::AddUsedBlock(usedHdr * const pUsed)
{
	assert(pUsed);

	//Always add to the front
	if (this->usedHead == 0)
	{
		this->usedHead = pUsed;
		pUsed->usedNext = 0;
		pUsed->usedPrev = 0;
	}

	else
	{
		pUsed->usedNext = this->usedHead;
		this->usedHead->usedPrev = pUsed;
		this->usedHead = pUsed;
		pUsed->usedPrev = 0;
	}

	this->AddStats(pUsed);
}

void heapHdr::RemoveUsedBlock(usedHdr * const pUsed)
{
	assert(pUsed);

	if (pUsed->usedNext != 0)
	{
		if (pUsed->usedPrev == 0)
		{
			this->usedHead = pUsed->usedNext;
			pUsed->usedNext->usedPrev = 0;
		}
		else
		{
			pUsed->usedPrev->usedNext = pUsed->usedNext;
			pUsed->usedNext->usedPrev = pUsed->usedPrev;
		}
	}

	else
	{
		if (pUsed->usedPrev == 0)
		{
			this->usedHead = 0;
			pUsed->usedNext = 0;
		}
		else
		{
			pUsed->usedPrev->usedNext = 0;
		}
	}

	//Update stats
	this->MinusStats(pUsed);
}

freeHdr * heapHdr::getAboveBlock(freeHdr *pFree) const
{
	memU32 *abovePtr = (memU32 *)((memU32)pFree - 4);
	freeHdr * aboveBlock = (freeHdr*)*abovePtr;
	aboveBlock->blockSize += sizeof(freeHdr) + pFree->blockSize;
	return aboveBlock;
}