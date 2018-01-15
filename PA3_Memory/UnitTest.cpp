//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "UnitTest.h"

Test::Test(const char * pTestName)
	:SLink()
{
	// initialized data
	this->pName = pTestName;
	this->testFunc = this;

	// register it
	TestRegistry::AddTest(this);

}