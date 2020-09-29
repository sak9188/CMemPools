#include "mpool.h"

MemoryPool GMemoryPool;

void CreateMemoryPool(const unsigned int MemorySize)
{
	void* MemoryAddr = malloc(MemorySize);
	GMemoryPool.AllocAddr = MemoryAddr;//最开始的分配地址就是起始地址
	GMemoryPool.EndAddr = ((unsigned char*)MemoryAddr) + MemorySize - 1;//计算出内存池终止地址
	GMemoryPool.Size = MemorySize;//内存池大小
	GMemoryPool.FreeSize = MemorySize;//一开始剩余内存池空间大小就是内存池大小
}

void* MallocFromPool(const unsigned int MemorySize)
{
	unsigned int mSize = 8;
	if (MemorySize > GMemoryPool.FreeSize)
	{
		return NULL;
	}
	else if (MemorySize > mSize)
	{
		mSize = MemorySize;
	}

	return MP_MallocFromMemoryPool(mSize);
}

bool FreeFromMemoryPool(void* MemoryAddres)
{
	MemoryNode *Iter = GMemoryPool.FirstUsedNode;
	for (size_t i = 0; i < GMemoryPool.UsedNodeNum; i++)
	{
		if (Iter->StartAddr == MemoryAddres)
		{
			Iter->IsUsed = false;
			if (Iter->PreNode == NULL) // 说明是头部节点
			{
				GMemoryPool.FirstUsedNode = Iter->NextNode;
				if (Iter->NextNode != NULL)
				{
					Iter->NextNode->PreNode = NULL;
				}
			}
			else // 非头部节点
			{
				Iter->PreNode->NextNode = Iter->NextNode;

				Iter->PreNode = NULL;
				Iter->NextNode = NULL;
			}

			if (GMemoryPool.FirstUsedNode == NULL)
			{
				GMemoryPool.FirstUsedNode = Iter;
			}
			else
			{
				Iter->NextNode = GMemoryPool.FirstUsedNode;
				GMemoryPool.FirstUsedNode->PreNode = Iter;
				GMemoryPool.FirstUsedNode = Iter;
			}
			GMemoryPool.FreeNodeNum += 1;
		}
		Iter += 1;
	}
	return true;
}

static void* MP_MallocFromMemoryPool(unsigned int MemorySize)
{
	MemoryNode *AllocateNode;
	//初始化新的内存节点
	AllocateNode = (MemoryNode *)GMemoryPool.AllocAddr;
	AllocateNode->StartAddr = (void *)AllocateNode;
	AllocateNode->StartAddr = (char *)AllocateNode->StartAddr + sizeof(MemoryNode);
	AllocateNode->DataSize = MemorySize;
	AllocateNode->IsUsed = true;
	AllocateNode->PreNode = NULL;
	AllocateNode->NextNode = GMemoryPool.FirstUsedNode;
	if (GMemoryPool.FirstUsedNode != NULL)
	{
		GMemoryPool.FirstUsedNode->PreNode = AllocateNode;
	}
	GMemoryPool.UsedNodeNum += 1;
	GMemoryPool.FirstFreeNode = AllocateNode;

	GMemoryPool.AllocAddr = (char *)AllocateNode + sizeof(MemoryNode) + MemorySize;
	return AllocateNode->StartAddr;
}