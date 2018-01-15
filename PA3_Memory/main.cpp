//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "UnitTest.h"
#include "Trace.h"

// Prototypes
void MemStressTest();

int main()
{

	// ---- Grading: ------------------------------------------------------

	// TOTAL of 16 unit tests in total, plus 1 stress test
	//        1-15 worth total:  15 pts
	//        16 - worth 3 pts
	//        Stress tests - 7 pts
	// So you can get a total of 25/25 points

	//----------------------------------------------------------------
	// Leave the following functions allow
	// DO NOT reorder them
	//----------------------------------------------------------------

	TestRegistry::RunTests();

	//----------------------------------------------------------------
	// Leave the following functions allow
	// DO NOT reorder them
	//----------------------------------------------------------------

		// Stress test
	Trace::out("\n");
	Trace::out("----------------------------------------\n");

	Trace::out("Stress test: started\n");

	MemStressTest();

	Trace::out("Stress test: done\n");
	Trace::out("----------------------------------------\n");

	// ---------------------------------------------------------------
	// remember to do many prints and stat updates.
	// enjoy and have fun
	// ---------------------------------------------------------------

}

