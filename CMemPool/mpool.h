#include <cstdlib>

#ifndef MPOOL_H
#define MPOOL_H

typedef struct _MemoryNode
{
	bool IsUsed;	// �Ƿ�ʹ��
	void *StartAddr; // �ڵ��ͷ��ַ
	unsigned int DataSize; // ��ռ�õĿռ��С
	_MemoryNode *PreNode;
	_MemoryNode *NextNode;
}MemoryNode;

typedef struct _MemoryPool
{
	void *AllocAddr;//��һ���ڴ�ķ����ַ
	void *EndAddr;//�ڴ�ص�β��ַ
	unsigned int Size;//�ڴ�ص��ܴ�С
	unsigned int FreeSize;//ʣ���ڴ�ؿռ��С

	unsigned int UsedNodeNum;
	MemoryNode *FirstUsedNode;

	unsigned int FreeNodeNum;
	MemoryNode *FirstFreeNode;
}MemoryPool;

extern MemoryPool GMemoryPool;

void CreateMemoryPool(const unsigned int MemorySize);
void* MallocFromPool(const unsigned int MemorySize);
bool FreeFromMemoryPool(void* MemoryAddres);
static void* MP_MallocFromMemoryPool(unsigned int MemorySize);

//void PX_Free(void *pAddress)
//{
//	unsigned int i, sIndex;
//	MemoryNode *itNode;//����ָ��
//	MemoryNode FreeNode;//�ͷ��ڴ�ڵ�
//	unsigned char *pcTempStart, *pcTempEnd;
//	unsigned char bExist;
//
//	bExist = 0;
//	���ݵ�ַ�õ��ڴ���Ϣ�ڵ�
//	FreeNode = *(MemoryNode *)((unsigned char *)pAddress - sizeof(MemoryNode));
//	�����ڴ�ڵ�Ŀ�ʼλ��
//	FreeNode.StartAddr = (unsigned char *)FreeNode.StartAddr - sizeof(MemoryNode);
//
//	��������ڵ������һ���ڵ�,Ҳ���ǽ�����ַ��һ������AllocAddr
//	if ((char *)FreeNode.EndAddr + 1 == (char *)G_MemoryPool.AllocAddr)
//	{
//		��ǰ�����Ƿ��п���ƴ�ӵĽڵ�
//		for (i = 0; i<G_MemoryPool.FreeTableCount; i++)
//		{
//			itNode = PX_MemoryPool_GetFreeTable(i);
//			if ((char *)itNode->EndAddr + 1 == (char *)FreeNode.StartAddr)
//			{
//				�����,��AllocAddr�ĵ�ַ�Ƶ��Ǹ��ڵ�Ŀ�ʼ��ַ
//				G_MemoryPool.AllocAddr = itNode->StartAddr;
//				�ָ��ͷŽڵ�Ŀռ䵽FreeSizeͬʱɾ��Ԥ���ռ�ĵ�ַ
//				G_MemoryPool.FreeSize += ((char*)FreeNode.EndAddr - (char *)FreeNode.StartAddr + sizeof(MemoryNode) + 1);
//				�ָ��ϲ��Ŀռ䵽FreeSize
//				G_MemoryPool.FreeSize += (char*)itNode->EndAddr - (char *)itNode->StartAddr + 1 + sizeof(MemoryNode);
//				�Ƴ�����ڵ�
//				PX_MemoryPoolRemoveFreeNode(i);
//				�������,����
//				return;
//
//			}
//		}
//		���û����Ҫƴ�ӵĽڵ�,��AllocAddrֱ�����ӵ���ʼ��ַ
//		G_MemoryPool.AllocAddr = (char *)FreeNode.StartAddr;
//		�ָ�ռ�õĿռ��Ԥ���ռ�
//		G_MemoryPool.FreeSize += ((char*)FreeNode.EndAddr - (char *)FreeNode.StartAddr + sizeof(MemoryNode) + 1);
//		return;
//	}
//
//	�����������
//
//	��¼ǰһ�����������Ľڵ�,��Ϊ0xfffffff��ʾû��,�������һ�����ӽڵ�����һ���ڵ����,��ɾ��ǰһ�εĽڵ�
//	sIndex = 0xffffffff;
//
//	�����Ƿ��п����ӵĽڵ�
//	for (i = 0; i<G_MemoryPool.FreeTableCount; i++)
//	{
//		itNode = PX_MemoryPool_GetFreeTable(i);
//		pcTempStart = (unsigned char *)itNode->StartAddr;
//		pcTempEnd = (unsigned char *)itNode->EndAddr;
//		�����Ƿ��п���������ӵĽڵ�
//		if (pcTempStart == (unsigned char *)FreeNode.EndAddr + 1)
//		{
//			�����,����֮ǰ�������ӽڵ�
//			if (sIndex == 0xffffffff)
//			{
//				sIndex = i;
//				Refresh this node
//				itNode->StartAddr = FreeNode.StartAddr;
//				FreeNode = *itNode;
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//			}
//			else
//			{
//				itNode->StartAddr = FreeNode.StartAddr;
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//				PX_MemoryPoolRemoveFreeNode(sIndex);
//			}
//			bExist = 1;
//		}
//
//		�����Ƿ��п�����ǰ���ӵĽڵ�
//		if (pcTempEnd + 1 == (unsigned char *)FreeNode.StartAddr)
//		{
//			�����,����֮ǰ�������ӽڵ�
//			if (sIndex == 0xffffffff)
//			{
//				sIndex = i;
//				Refresh this node
//				itNode->EndAddr = FreeNode.EndAddr;
//				FreeNode = *itNode;
//				�Ƴ���һ���ڵ�
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//			}
//			else
//			{
//				itNode->EndAddr = FreeNode.EndAddr;
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//				�Ƴ���һ���ڵ�
//				PX_MemoryPoolRemoveFreeNode(sIndex);
//			}
//			bExist = 1;
//		}
//	}
//	���û�����ӽڵ�,�����ͷ��ڴ�ڵ�
//	if (bExist == 0)
//		*PX_AllocFreeMemoryNode() = FreeNode;
//
//	����������ڸ��������ͷ��ڴ�ڵ��С
//	PX_UpdateMaxFreqSize();
//
//}
#endif