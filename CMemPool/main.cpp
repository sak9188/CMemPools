// CMemPool.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "mpool.h"

#include <stdio.h>

int main(int argc, char * argv[])
{
	CreateMemoryPool(1 * 1024 * 1024);
	void *a = MallocFromPool(1);
	auto b = (int *)a;
	*b = 9999;

	a = MallocFromPool(1);
	b = (int *)a;
	*b = -1;

	a = MallocFromPool(1);
	b = (int *)a;
	*b = -8888;

	getchar();
	return 0;
}

