//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <malloc.h>
#include <new>
#include <assert.h>
#include "mem.h"
#include "heapHdr.h"
#include "freeHdr.h"
#include "usedHdr.h"
#include "output.h"

#define HEAP_ALIGNMENT			16
#define HEAP_ALIGNMENT_MASK		(HEAP_ALIGNMENT-1)

#define ALLOCATION_ALIGNMENT		16
#define ALLOCATION_ALIGNMENT_MASK	(ALLOCATION_ALIGNMENT-1)

#define UNUSED_VAR(v)  ((void *)v)

#ifdef _DEBUG
#define HEAP_HEADER_GUARDS  16
#define HEAP_SET_GUARDS  	memU32 *pE = (memU32 *)((memU32)pRawMem + HEAP_SIZE); \
								*pE++ = 0xEEEEEEEE;*pE++ = 0xEEEEEEEE;*pE++ = 0xEEEEEEEE;*pE++ = 0xEEEEEEEE;
#define HEAP_TEST_GUARDS	memU32 *pE = (memU32 *)((memU32)pRawMem + HEAP_SIZE); \
								assert(*pE++ == 0xEEEEEEEE);assert(*pE++ == 0xEEEEEEEE); \
								assert(*pE++ == 0xEEEEEEEE);assert(*pE++ == 0xEEEEEEEE);  
#else
#define HEAP_HEADER_GUARDS  0
#define HEAP_SET_GUARDS  	
#define HEAP_TEST_GUARDS			 
#endif

memSystem::~memSystem()
{
	HEAP_TEST_GUARDS
	_aligned_free(this->pRawMem);
}


heapHdr *memSystem::getHeap()
{
	return this->pHeap;
}

memSystem::memSystem()
{
	// now initialize it.
	this->pHeap = 0;
	this->pRawMem = 0;

	// Do a land grab --- get the space for the whole heap
	// Since OS have different alignments... I forced it to 16 byte aligned
	//pRawMem = _aligned_malloc(HEAP_SIZE, HEAP_ALIGNMENT);
	pRawMem = _aligned_malloc(HEAP_SIZE + HEAP_HEADER_GUARDS, HEAP_ALIGNMENT);  // <-- add HEAP_HEADER_GUARDS 
	HEAP_SET_GUARDS

	// verify alloc worked
	assert(pRawMem != 0);

	// Guarantee alignemnt
	assert( ((memU32)pRawMem & HEAP_ALIGNMENT_MASK) == 0x0 ); 

	// instantiate the heap header on the raw memory
	heapHdr *p = new(pRawMem) heapHdr(pRawMem);

	// update it
	this->pHeap = p;
}

memVoid memSystem::InitializeSystem( )
{
	assert(pHeap);

	//Create the free header
	memU32 freeBlockSize = (memU32)pHeap->stats.heapBottomAddr - (memU32)pHeap->stats.heapTopAddr - sizeof(freeHdr);
	freeHdr *pFree = new(pHeap + 1) freeHdr(freeBlockSize, BlockType::Free, false);

	//Update the free linkedlist
	pHeap->AddFreeBlock(pFree);

	pHeap->heapInitialize = true;
}

memVoid memSystem::Free( memVoid * const data )
{
	//Grab the header of the used block
	usedHdr *pUsed = (usedHdr*)data - 1; 

	//Remove the used block from the used list
	pHeap->RemoveUsedBlock(pUsed);	

	//Turn the used block into a free block and initialize a secret pointer
	freeHdr *pFree = new (pUsed) freeHdr(pUsed->blockSize, BlockType::Free, pUsed->aboveBlockFree);
	freeHdr ** secret = 0;

	//Grab the block type of below block
	memU8 nextBlockType = (memU8)(*((memInt *)((memU32)(pFree + 1) + pFree->blockSize + 2 * sizeof(memVoid*) + sizeof(memU32))) & 0xFF);

	if (nextBlockType == (memU8)BlockType::Used)
	{
		usedHdr *nextUsed = (usedHdr *)((memU32)(pFree + 1) + pFree->blockSize);
		nextUsed->aboveBlockFree = true;
		secret = (freeHdr **)((memU32)(pFree + 1) + pFree->blockSize - 4);
		*secret = pFree;

		if (pFree->aboveBlockFree)
		{
			freeHdr * aboveBlock = pHeap->getAboveBlock(pFree);
			pHeap->stats.currFreeMem += sizeof(freeHdr) + pFree->blockSize;
			*secret = aboveBlock;
		}
		else
		{
			pHeap->AddFreeBlock(pFree);
		}
	}

	else if (nextBlockType == (memU8)BlockType::Free)
	{
		freeHdr *nextFree = (freeHdr *)((memU32)(pFree + 1) + pFree->blockSize);

		pHeap->stats.currFreeMem += pFree->blockSize + sizeof(freeHdr);

		pFree->blockSize += sizeof(freeHdr) + nextFree->blockSize;

		if (nextFree->freeNext != 0)
		{
			if (pFree->freePrev == 0)
			{
				pHeap->freeHead = pFree;
				pFree->freePrev = 0;
				pFree->freeNext = nextFree->freeNext;
				nextFree->freeNext->freePrev = pFree;
			}
			else
			{
				nextFree->freeNext->freePrev = pFree;
				pFree->freeNext = nextFree->freeNext;
				pFree->freePrev = nextFree->freePrev;
				nextFree->freePrev->freeNext = pFree;
			}
		}
		else
		{
			if (nextFree->freePrev == 0)
			{
				pHeap->freeHead = pFree;
			}
			else
			{
				pFree->freeNext = 0;
				pFree->freePrev = nextFree->freePrev;
				nextFree->freePrev->freeNext = pFree;
			}
		}

		secret = (freeHdr **)((memU32)(pFree + 1) + pFree->blockSize - 4);
		*secret = pFree;

		if (pFree->aboveBlockFree)
		{
			freeHdr * aboveBlock = pHeap->getAboveBlock(pFree);

			pHeap->stats.currFreeMem += sizeof(freeHdr); 
			pHeap->stats.currNumFreeBlocks--;
			
			if (pFree->freeNext != 0)
			{
				aboveBlock->freeNext = pFree->freeNext;
				pFree->freeNext->freePrev = aboveBlock;
			}
			else
			{
				aboveBlock->freeNext = 0;
			}

			*secret = aboveBlock;
		}
	}

	else
	{
		if (pFree->aboveBlockFree)
		{		
			freeHdr * aboveBlock = pHeap->getAboveBlock(pFree);

			pHeap->stats.currFreeMem += sizeof(freeHdr) + pFree->blockSize;

			if (pFree->freeNext != 0)
			{
				aboveBlock->freeNext = pFree->freeNext;
				pFree->freeNext->freePrev = aboveBlock;
			}
			else
			{
				aboveBlock->freeNext = 0;
			}
		}
		else
		{
			pHeap->AddFreeBlock(pFree);
		}		
	}
}

memVoid * memSystem::Malloc( const memU32 size )
{
	assert((size & HEAP_ALIGNMENT_MASK) == 0X0);

	//Find a free block 
	freeHdr *pFree = pHeap->FindFreeBlock(size);

	//subdivide free block into used
	memU32 remainSize= pFree->blockSize - size;
	usedHdr *pUsed = 0;

	if (remainSize == 0) // Used the whole block
	{
		pHeap->RemoveFreeBlock(pFree);

		pUsed = new (pFree) usedHdr(pFree); 

		//Check below, change aboveBlockFree flag
		memU8 nextBlockType = (memU8)(*((memInt *)((memU32)(pUsed + 1) + pUsed->blockSize + 2 * sizeof(memVoid*) + sizeof(memU32))) & 0xFF);

		if (nextBlockType == (memU8)BlockType::Used)
		{
			usedHdr *nextUsed = (usedHdr *)((memU32)(pUsed + 1) + pUsed->blockSize);
			nextUsed->aboveBlockFree = false;
		}

	}

	//Otherwise, subdivide the free block
	else
	{
		pHeap->RemoveFreeBlock(pFree);

		pUsed = new (pFree) usedHdr(size, BlockType::Used, false);

		pFree = new ((freeHdr *)((memU32)(pUsed + 1) + size)) freeHdr(remainSize-sizeof(freeHdr), BlockType::Free, false);
		
		pHeap->AddFreeBlock(pFree);
	}

	pHeap->AddUsedBlock(pUsed);

	memVoid *pBlock = pUsed + 1;
	assert(pBlock);
	return pBlock;
	
}

memVoid memSystem::dump()
{

	fprintf(io::getHandle(),"\n------- DUMP -------------\n\n");

	fprintf(io::getHandle(),"heapStart: 0x%p     \nheapEnd:   0x%p \n\n",this->pHeap, this->pHeap->stats.heapBottomAddr);
	fprintf(io::getHandle(),"usedHead:  0x%p     \nfreeHead:  0x%p \n\n", this->pHeap->usedHead, this->pHeap->freeHead );

	fprintf(io::getHandle(),"Heap Hdr   s: %p  e: %p                            size: 0x%x \n",(void *)((memU32)this->pHeap->stats.heapTopAddr-sizeof(heapHdr)), this->pHeap->stats.heapTopAddr, this->pHeap->stats.sizeHeapHeader);

	memU32 p = (memU32)pHeap->stats.heapTopAddr;

	char *type;
	char *typeHdr;

	while( p < (memU32)pHeap->stats.heapBottomAddr )
	{
		usedHdr *used = (usedHdr *)p;
		if( used->blockType == 0xAA )
		{
			typeHdr = "USED HDR ";
			type    = "USED     ";
		}
		else
		{
			typeHdr = "FREE HDR ";
			type    = "FREE     ";
		}

		memU32 hdrStart = (memU32)used;
		memU32 hdrEnd   = (memU32)used + sizeof(usedHdr);
		fprintf(io::getHandle(),"%s  s: %p  e: %p  p: %p  n: %p  size: 0x%x    AF: %d \n",typeHdr, (void *)hdrStart, (void *)hdrEnd, used->usedPrev, used->usedNext, sizeof(usedHdr), used->aboveBlockFree );
		memU32 blkStart = hdrEnd;
		memU32 blkEnd   = blkStart + used->blockSize; 
		fprintf(io::getHandle(),"%s  s: %p  e: %p                            size: 0x%x \n",type, (void *)blkStart, (void *)blkEnd, used->blockSize );

		p = blkEnd;
	
	}
}