#include "Test.h"

#include "MemoryPool.h"

void TestFunc()
{
	MemoryPool<double> doublePool3(3, false);


	double* pDouble1 = doublePool3.Alloc();
}
