//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef CPP_UNIT_H
#define CPP_UNIT_H

#include <assert.h>
#include <stdio.h>

//---------------------------------------------------------------------------------------
// UnitTrace - output to debug window
//             isolated from other systems
//
// NOTE: You need to set your project settings
//       Character Set -> Use Multi-Byte Character Set
//
//---------------------------------------------------------------------------------------

// Needs to be included because Window.h sucks for advanced warnings
#pragma warning( push )
#pragma warning( disable : 4820 )
#pragma warning( disable : 4668 )
#include <Windows.h>
#pragma warning( pop ) 

#define UnitTraceBuffSize 256

// Singleton helper class
class UnitTrace
{
public:
	// displays a printf to the output window
	static void out(char* fmt, ...)
	{
		UnitTrace *pTrace = UnitTrace::privGetInstance();
		assert(pTrace);

		va_list args;
		va_start(args, fmt);

		vsprintf_s(pTrace->privBuff, UnitTraceBuffSize, fmt, args);
		OutputDebugString(pTrace->privBuff);

		va_end(args);
	}

	// Big four
	UnitTrace() = default;
	UnitTrace(const UnitTrace &) = default;
	UnitTrace & operator = (const UnitTrace &) = default;
	~UnitTrace() = default;

private:
	static UnitTrace *privGetInstance()
	{
		// This is where its actually stored (BSS section)
		static UnitTrace helper;
		return &helper;
	}
	char privBuff[UnitTraceBuffSize];
};

//---------------------------------------------------------------------------------------
// Utility class - for testing floats
//
// Needs <cmath> - fabs()
//
//---------------------------------------------------------------------------------------
#include <cmath>

class Utility
{
public:
	static bool AreEqual(float a, float b, float epsilon = 0.001f)
	{
		return (fabs(a - b) < epsilon);
	}

	static bool AreEqual(double a, double b, double epsilon = 0.001f)
	{
		return (fabs(a - b) < epsilon);
	}
};

//---------------------------------------------------------------------------------------
// Unit Tests
//
// Needs this header
//
//---------------------------------------------------------------------------------------
struct UnitStats
{
	UnitStats()
		:testCount(0),
		testPass(0),
		testFail(0), 
		indvAsserts(0)
	{
	}

	// Big four
	UnitStats(const UnitStats &) = default;
	UnitStats & operator = (const UnitStats &) = default;
	~UnitStats() = default;

	// data: ------------------
	int testCount;
	int testPass;
	int testFail;
	int indvAsserts;
};
struct UnitData
{
	UnitData()
		:result(false),
		pMemberName(0),
		pSourceFilePath(0),
		sourceLineNumber(0),
		pad0(0),
		pad1(0),
		pad2(0)
	{
	}

	// Big four
	UnitData(const UnitData &) = default;
	UnitData & operator = (const UnitData &) = default;
	~UnitData() = default;

	// data: -----------------
	bool result;
	char pad0;
	char pad1;
	char pad2;
	const char *pMemberName;
	char *pSourceFilePath;
	int sourceLineNumber;
};

class SLink
{
public:
	SLink()
	{
		this->Clear();
	}
	// Big four
	SLink(const SLink &) = default;
	SLink & operator = (const SLink &) = default;
	~SLink() = default;

	void Clear()
	{
		this->pNext = 0;
	}

	static void AddToFront(SLink *&pRoot, SLink *pNode )
	{
		if (pRoot == 0)
		{		
			pRoot = pNode;
			assert(pNode->pNext == 0);
		}
		else
		{
			SLink *pTmp = pRoot;
			pRoot = pNode;
			pNode->pNext = pTmp;
		}
	}

public:
	// Data
	SLink *pNext;
};

class Test : public SLink
{
public:
	Test(const char * pTestName);

	// Big four
	Test() = delete;
	Test(const Test &) = default;
	Test & operator = (const Test &) = default;
	~Test() = default;


	virtual void run(UnitData &, UnitStats &) = 0;

public:
	const char * pName;
	Test *testFunc;
};

class TestRegistry
{
public:
	// Big four
	TestRegistry(const TestRegistry &) = delete;
	TestRegistry & operator = (const TestRegistry &) = delete;
	~TestRegistry() = default;


	static void AddTest(SLink *pTest)
	{
		//UnitTrace::out("TestRegistry:AddTest(%s) \n", pTest->pName);
		assert(pTest != 0);

		TestRegistry *pRegistry = TestRegistry::privGetInstance();

		// add to front
		SLink::AddToFront((SLink *&)pRegistry->pRoot, pTest);
	}
	static void RunTests()
	{
		//UnitTrace::out("\nTestRegistry:RunTests()\n");

		UnitTrace::out("---- Testing ----\n");
		TestRegistry *pRegistry = TestRegistry::privGetInstance();
		SLink *pTmp = (SLink *)pRegistry->pRoot;

		while (pTmp != 0)
		{
			pRegistry->stats.testCount++;

			// downcast to the test
			Test *pTest = (Test *)pTmp;

			//UnitTrace::out("\n%s - start\n", pTest->pName);

			// run the test
			pRegistry->data.result = true;
			
			pTest->testFunc->run(pRegistry->data, pRegistry->stats);
		
			

			if (pRegistry->data.result == true)
			{
				pRegistry->stats.testPass++;
			}
			else
			{
				pRegistry->stats.testFail++;
			}

			//UnitTrace::out("%s - end\n", pTest->pName);

			// next test
			pTmp = pTmp->pNext;
		}

		UnitTrace::out("\n");
		UnitTrace::out(" testCount: %d\n", pRegistry->stats.testCount);
		UnitTrace::out("  testPass: %d\n", pRegistry->stats.testPass);
		UnitTrace::out("  testFail: %d\n", pRegistry->stats.testFail);
		UnitTrace::out("indvChecks: %d\n", pRegistry->stats.indvAsserts);
	}

private:
	TestRegistry::TestRegistry()
	{
		this->pRoot = 0;
	}
	static TestRegistry * TestRegistry::privGetInstance()
	{
		static TestRegistry tRegistry;
		return &tRegistry;
	}

// Data: ------------------------
	UnitData data;
	UnitStats stats;
	SLink *pRoot;
};


// a trick to create a c-string
#define STRING_ME(s) #s

// Allow conditional expressions that are constant.
#pragma warning( disable : 4127 )

// Increments total check count
// Creates a clickable format in the output window for failure
// Abort test on first fail
#define CHECK( condition ) \
{ \
	stats.indvAsserts++;\
	if( !( condition ) ) \
	{ \
		data.result = false;  \
		data.pMemberName = this->pName; \
		data.pSourceFilePath = __FILE__; \
		data.sourceLineNumber = __LINE__; \
		UnitTrace::out("%s(%d): %s\n", data.pSourceFilePath, data.sourceLineNumber, data.pMemberName  ); \
		return; \
	} \
	else\
	{\
	}\
}


#define CHECK_EQUAL( value1, value2 ) \
{ \
	stats.indvAsserts++;\
	if( !( value1 == value2 ) ) \
	{ \
		data.result = false;  \
		data.pMemberName = this->pName; \
		data.pSourceFilePath = __FILE__; \
		data.sourceLineNumber = __LINE__; \
		UnitTrace::out("%s(%d): %s\n", data.pSourceFilePath, data.sourceLineNumber, data.pMemberName  ); \
		return; \
	} \
	else\
	{\
	}\
}

//#define CHECK_EQUAL( expectedValue, actualValue ) \
//   { \
//      /*lint -e( 506, 774 ) : Allow constant conditions. */ \
//      if( !( (expectedValue) == (actualValue) ) ) \
//      { \
//         result.addFailure( \
//            Failure( \
//               this->getName(), \
//               SimpleString( __FILE__ ), \
//               __LINE__, \
//               StringFrom( expectedValue ), \
//               StringFrom( actualValue ) \
//            ) \
//         ); \
//         \
//         return; \
//      } \
//   }


// Here is the magic
//
// 1) it creates a class, <TestName>_Test
//    it is derived from Test class
// 2) it defines the constructor and calls the base constructor with the name
//    it registers the name of the test to a single class (TestRegistry)
// 3) It also overloads the Test::run() method with the body of the macro

#define TEST( TestName, GroupName ) \
class TestName##GroupName##_Test : public Test \
{ \
	public: \
		TestName##GroupName##_Test(): \
		Test( STRING_ME(TestName##GroupName##_Test) ) \
		{ \
		}; \
	\
	void run(UnitData &data, UnitStats &stats);\
} TestName##GroupName##_instance; \
\
void TestName##GroupName##_Test::run( UnitData &data, UnitStats &stats ) 





#endif
