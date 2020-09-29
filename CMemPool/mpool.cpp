#include "mpool.h"

MemoryPool GMemoryPool;

void CreateMemoryPool(const unsigned int MemorySize)
{
	void* MemoryAddr = malloc(MemorySize);
	GMemoryPool.AllocAddr = MemoryAddr;//�ʼ�ķ����ַ������ʼ��ַ
	GMemoryPool.EndAddr = ((unsigned char*)MemoryAddr) + MemorySize - 1;//������ڴ����ֹ��ַ
	GMemoryPool.Size = MemorySize;//�ڴ�ش�С
	GMemoryPool.FreeSize = MemorySize;//һ��ʼʣ���ڴ�ؿռ��С�����ڴ�ش�С
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
			if (Iter->PreNode == NULL) // ˵����ͷ���ڵ�
			{
				GMemoryPool.FirstUsedNode = Iter->NextNode;
				if (Iter->NextNode != NULL)
				{
					Iter->NextNode->PreNode = NULL;
				}
			}
			else // ��ͷ���ڵ�
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
	//��ʼ���µ��ڴ�ڵ�
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