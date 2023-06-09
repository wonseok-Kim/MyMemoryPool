#include <Windows.h>

#include <stdio.h>
#include <stdlib.h>

#include <list>

#include "MemoryPool.h"
#include "Test.h"

class Star
{
public:
	Star()
	{
		wprintf(L"Star()\n");
	}

	Star(int x, int y)
		: m_x{ x }, m_y{ y }
	{
		wprintf(L"Star()\n");
	}

	~Star()
	{
		wprintf(L"~Star()\n");
	}

private:
	int m_x = 0;
	int m_y = 0;
};

int main()
{	
	MemoryPool<Star> starPool1(2);
	MemoryPool<Star> starPool2(1);

	Star* pStar1 = starPool1.Alloc();
	Star* pStar2 = starPool1.Alloc();
	Star* pStar3 = starPool1.Alloc();
	Star* pStar4 = starPool1.Alloc();
	Star* pStar5 = starPool1.Alloc();

	Star* pStar6 = starPool2.Alloc();
	Star* pStar7 = starPool2.Alloc();

	//memset(pStar2, 0, sizeof(Star) + 1);
	//memset((char*)pStar4 - 1, 0, sizeof(Star));

	starPool1.Free(pStar1);
	starPool1.Free(pStar2);
	starPool1.Free(pStar3);
	starPool1.Free(pStar4);
	
	starPool1.Free(pStar5);

	starPool2.Free(pStar6);
	starPool2.Free(pStar7);

	//starPool2.Free(pStar5);

	{
		MemoryPool<int> intPool(1, false);

		int* pInt1 = intPool.Alloc();
		int* pInt2 = intPool.Alloc();

		intPool.Free(pInt1);
		intPool.Free(pInt2);
	}

	MemoryPool<double> doublePool(2, false);

	double* pDouble1 = doublePool.Alloc();
	double* pDouble2 = doublePool.Alloc();
	double* pDouble3 = doublePool.Alloc();
	double* pDouble4 = doublePool.Alloc();

	doublePool.Free(pDouble1);
	doublePool.Free(pDouble2);
	doublePool.Free(pDouble3);
	doublePool.Free(pDouble4);


	MemoryPool<double> doublePool2(3, false);

	TestFunc();

	return 0;
}