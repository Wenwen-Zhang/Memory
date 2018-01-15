//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "UnitTest.h"
#include "output.h"
#include "mem.h"
#include "usedHdr.h"
#include "freeHdr.h"
#include "Trace.h"

#define PTR_FIX(x)  (unsigned int)(x == 0) ? 0: ((unsigned int)x - (unsigned int)h)  
#define UNUSED_VAR(v) ((void *)v)

TEST( Secret_pointers , MemoryShakeOut )
{
	io::create("Student Values", "Test_16");

	fprintf(io::getHandle(),"Test16: \n\n");

	// Part A:
	fprintf(io::getHandle(),"      memSystem mem;\n");
	fprintf(io::getHandle(),"      mem.InitializeSystem();\n");

	fprintf(io::getHandle(),"      void *a1 = mem.Malloc(0x100);\n");
	fprintf(io::getHandle(),"      void *a2 = mem.Malloc(0x20);\n");
	fprintf(io::getHandle(),"      void *a3 = mem.Malloc(0x20);\n");
	fprintf(io::getHandle(),"      void *a4 = mem.Malloc(0x20);\n");
	fprintf(io::getHandle(),"      void *a5 = mem.Malloc(0x20);\n");
	fprintf(io::getHandle(),"      void *b1 = mem.Malloc(0x200);\n");
	fprintf(io::getHandle(),"      void *c1 = mem.Malloc(0x300);\n");
	fprintf(io::getHandle(),"      void *c2 = mem.Malloc(0x30);\n");
	fprintf(io::getHandle(),"      void *c3 = mem.Malloc(0x30);\n");

	fprintf(io::getHandle(),"      void *c4 = mem.Malloc(0x30);\n");
	fprintf(io::getHandle(),"      void *c5 = mem.Malloc(0x30);\n");
	fprintf(io::getHandle(),"      void *c6 = mem.Malloc(0x30);\n");
	fprintf(io::getHandle(),"      void *c7 = mem.Malloc(0x30);\n");
	fprintf(io::getHandle(),"      void *d1 = mem.Malloc(0x200);\n");

	fprintf(io::getHandle(),"      void *d2 = mem.Malloc(0x40);\n");
	fprintf(io::getHandle(),"      void *d3 = mem.Malloc(0x60);\n");
	fprintf(io::getHandle(),"      void *e1 = mem.Malloc(0x100);\n");
	fprintf(io::getHandle(),"      void *f1 = mem.Malloc(0x500);\n");
	fprintf(io::getHandle(),"      void *g1 = mem.Malloc(0x100);\n\n");

	fprintf(io::getHandle(),"      mem.dump();\n\n");

	fprintf(io::getHandle(), "      mem.Free(d1);\n");
	fprintf(io::getHandle(), "      mem.Free(b1);\n");
	fprintf(io::getHandle(), "      mem.Free(f1);\n");
	fprintf(io::getHandle(), "      mem.Free(a1);\n");
	fprintf(io::getHandle(), "      mem.Free(a4);\n");
	fprintf(io::getHandle(), "      mem.Free(c7);\n");
	fprintf(io::getHandle(), "      mem.Free(c5);\n");
	fprintf(io::getHandle(), "      mem.Free(c2);\n\n");

	fprintf(io::getHandle(), "      mem.dump();\n\n");

	fprintf(io::getHandle(), "      mem1.Free(d2);\n");
	fprintf(io::getHandle(), "      mem1.Free(c4);\n");
	fprintf(io::getHandle(), "      mem1.Free(g1);\n\n");

	fprintf(io::getHandle(), "      mem1.dump();\n\n");


	//---------------------------------------------------------
	// Part A:
	//---------------------------------------------------------
	memSystem mem1;
	mem1.InitializeSystem();

   	void *a1 = mem1.Malloc( 0x100 );
	void *a2 = mem1.Malloc(0x20);
	void *a3 = mem1.Malloc(0x20);
	void *a4 = mem1.Malloc(0x20);
	void *a5 = mem1.Malloc(0x20);
   	void *b1 = mem1.Malloc( 0x200 );
	void *c1 = mem1.Malloc( 0x300 );
	void *c2 = mem1.Malloc(0x30);
	void *c3 = mem1.Malloc(0x30);

	void *c4 = mem1.Malloc(0x30);
	void *c5 = mem1.Malloc(0x30);
	void *c6 = mem1.Malloc(0x30);
	void *c7 = mem1.Malloc(0x30);
	void *d1 = mem1.Malloc( 0x200 );

	void *d2 = mem1.Malloc(0x40);
	void *d3 = mem1.Malloc(0x60);
	void *e1 = mem1.Malloc( 0x100);
	void *f1 = mem1.Malloc(0x500);
	void *g1 = mem1.Malloc(0x100);

	UNUSED_VAR(c1);
	UNUSED_VAR(e1);
	UNUSED_VAR(g1);
	UNUSED_VAR(a1);
	UNUSED_VAR(a2);
	UNUSED_VAR(a3);
	UNUSED_VAR(a5);
	UNUSED_VAR(c3);
	UNUSED_VAR(c6);
	UNUSED_VAR(c4);
	UNUSED_VAR(d2);	
	UNUSED_VAR(d3);

	// ---- Verify A: -----------------------------------------------------------

	heapHdr *h = mem1.getHeap();

	CHECK_EQUAL(PTR_FIX(a1), 0x50); 
	CHECK_EQUAL(PTR_FIX(a2), 0x160); 
	CHECK_EQUAL(PTR_FIX(a3), 0x190); 
	CHECK_EQUAL(PTR_FIX(a4), 0x1C0); 
	CHECK_EQUAL(PTR_FIX(a5), 0x1F0); 
	CHECK_EQUAL(PTR_FIX(b1), 0x220); 
	CHECK_EQUAL(PTR_FIX(c1), 0x430); 
	CHECK_EQUAL(PTR_FIX(c2), 0x740); 
	CHECK_EQUAL(PTR_FIX(c3), 0x780); 

	CHECK_EQUAL(PTR_FIX(c4), 0x7c0); 
	CHECK_EQUAL(PTR_FIX(c5), 0x800);
	CHECK_EQUAL(PTR_FIX(c6), 0x840);
	CHECK_EQUAL(PTR_FIX(c7), 0x880); 
	CHECK_EQUAL(PTR_FIX(d1), 0x8c0);

	CHECK_EQUAL(PTR_FIX(d2), 0xad0); 
	CHECK_EQUAL(PTR_FIX(d3), 0xb20); 
	CHECK_EQUAL(PTR_FIX(e1), 0xb90); 
	CHECK_EQUAL(PTR_FIX(f1), 0xca0);
	CHECK_EQUAL(PTR_FIX(g1), 0x11b0); 

	mem1.dump();

	//---------------------------------------------------------
	// Part B:
	//---------------------------------------------------------

	mem1.Free( d1 );
	mem1.Free( b1 );
	mem1.Free(f1);
	mem1.Free(a1);
	mem1.Free(a4);
	mem1.Free(c7);
	mem1.Free(c5);
	mem1.Free(c2);

	usedHdr *used;
	memU32  *secret;
	freeHdr *free;

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)a2 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x150);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free =  (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x40);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)a5 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x1E0);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x1B0);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c1 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x420);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x210);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c3 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x770);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x730);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c6 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x830);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x7f0);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)d2 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0xac0);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x870);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)g1 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x11a0);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0xc90);

	mem1.dump();

	//---------------------------------------------------------
	// Part C:
	//---------------------------------------------------------
	//
	//  READ the next 3 Frees... here is the real test
	//  If it doesn't pass inspection - you lose points 
	//
	//---------------------------------------------------------

	// This should not use While loop on the Free list
	// I will walk into you function and verify - Keenan
	mem1.Free(d2);

	// This should not use While loop on the Free list
	// I will walk into you function and verify - Keenan
	mem1.Free(c4);

	// This should not use While loop on the Free list
	// I will walk into you function and verify - Keenan
	mem1.Free(g1);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)a2 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x150);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x40);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)a5 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x1e0);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x1b0);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c1 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x420);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x210);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c3 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x770);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x730);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)c6 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0x830);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x7b0);

	// Check for Secret Pointers, given pointer
	used = (usedHdr *)d3 - 1;
	CHECK_EQUAL((PTR_FIX(used)), 0xb10);
	CHECK_EQUAL(used->aboveBlockFree, true);
	secret = (memU32 *)((memU32)used - 4);
	free = (freeHdr *)*secret;
	CHECK_EQUAL((PTR_FIX(free)), 0x870);


	mem1.dump();

	io::destroy();

	
}