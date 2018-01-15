//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef HEAPHDR_H
#define HEAPHDR_H

#include"memTypes.h"

// forward declares
struct usedHdr;
struct freeHdr;

struct memStats
{
	memUInt peakNumUsed;		// number of peak used allocations
	memUInt peakUsedMemory;		// peak size of used memory

	memUInt currNumUsedBlocks;	// number of current used allocations
	memUInt currUsedMem;		// current size of the total used memory

	memUInt	currNumFreeBlocks;	// number of current free blocks
	memUInt currFreeMem;		// current size of the total free memory

	memUInt sizeHeap;			// size of Heap total space, including header
	memUInt sizeHeapHeader;		// size of heap header

	memVoid *heapTopAddr;		// start address available heap
	memVoid *heapBottomAddr;    // bottom of address of heap

	//Big Four
	memStats();
	memStats(const memStats &);
	memStats & operator = (const memStats &);
	~memStats();
};


struct heapHdr
{
	// Make sure that the heapHdr is 16 byte aligned.

	// allocation links
	usedHdr		*usedHead;
	freeHdr		*freeHead;

	memBool		heapInitialize;
	memU8		bytePad1;
	memU8		bytePad2;
	memU8		bytePad3;

	memU32		pad1;
	memU32		pad2;
	memU32		pad3;

	// recording stats
	memStats	stats;	

	//Big Four
	heapHdr();
	heapHdr(const heapHdr &);
	heapHdr & operator = (const heapHdr &);
	~heapHdr();

	// methods

	// Stats methods
	void AddStats(const freeHdr * const);
	void MinusStats(const freeHdr * const);
	void AddStats(const usedHdr * const);
	void MinusStats(const usedHdr * const);

	//Free blocks methods
	void AddFreeBlock(freeHdr * const);
	void RemoveFreeBlock(freeHdr * const);
	freeHdr * FindFreeBlock(memU32) const;
	freeHdr * getAboveBlock(freeHdr *) const;


	//Used blocks methods
	void AddUsedBlock(usedHdr * const pUsed);
	void RemoveUsedBlock(usedHdr * const);

	// specialize constructor
	heapHdr(void * ptr);
	
};

#endif //heapHdr.h