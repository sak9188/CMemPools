#include <cstdlib>

#ifndef MPOOL_H
#define MPOOL_H

typedef struct _MemoryNode
{
	bool IsUsed;	// 是否被使用
	void *StartAddr; // 节点的头地址
	unsigned int DataSize; // 所占用的空间大小
	_MemoryNode *PreNode;
	_MemoryNode *NextNode;
}MemoryNode;

typedef struct _MemoryPool
{
	void *AllocAddr;//下一次内存的分配地址
	void *EndAddr;//内存池的尾地址
	unsigned int Size;//内存池的总大小
	unsigned int FreeSize;//剩余内存池空间大小

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
//	MemoryNode *itNode;//迭代指针
//	MemoryNode FreeNode;//释放内存节点
//	unsigned char *pcTempStart, *pcTempEnd;
//	unsigned char bExist;
//
//	bExist = 0;
//	依据地址得到内存信息节点
//	FreeNode = *(MemoryNode *)((unsigned char *)pAddress - sizeof(MemoryNode));
//	重置内存节点的开始位置
//	FreeNode.StartAddr = (unsigned char *)FreeNode.StartAddr - sizeof(MemoryNode);
//
//	假如这个节点是最后一个节点,也就是结束地址后一个就是AllocAddr
//	if ((char *)FreeNode.EndAddr + 1 == (char *)G_MemoryPool.AllocAddr)
//	{
//		向前搜索是否有可以拼接的节点
//		for (i = 0; i<G_MemoryPool.FreeTableCount; i++)
//		{
//			itNode = PX_MemoryPool_GetFreeTable(i);
//			if ((char *)itNode->EndAddr + 1 == (char *)FreeNode.StartAddr)
//			{
//				如果有,将AllocAddr的地址移到那个节点的开始地址
//				G_MemoryPool.AllocAddr = itNode->StartAddr;
//				恢复释放节点的空间到FreeSize同时删除预留空间的地址
//				G_MemoryPool.FreeSize += ((char*)FreeNode.EndAddr - (char *)FreeNode.StartAddr + sizeof(MemoryNode) + 1);
//				恢复合并的空间到FreeSize
//				G_MemoryPool.FreeSize += (char*)itNode->EndAddr - (char *)itNode->StartAddr + 1 + sizeof(MemoryNode);
//				移除这个节点
//				PX_MemoryPoolRemoveFreeNode(i);
//				处理完成,返回
//				return;
//
//			}
//		}
//		如果没有需要拼接的节点,将AllocAddr直接连接到起始地址
//		G_MemoryPool.AllocAddr = (char *)FreeNode.StartAddr;
//		恢复占用的空间和预留空间
//		G_MemoryPool.FreeSize += ((char*)FreeNode.EndAddr - (char *)FreeNode.StartAddr + sizeof(MemoryNode) + 1);
//		return;
//	}
//
//	不是特殊情况
//
//	记录前一次连接索引的节点,若为0xfffffff表示没有,如果有下一次连接节点且上一个节点存在,则删除前一次的节点
//	sIndex = 0xffffffff;
//
//	查找是否有可连接的节点
//	for (i = 0; i<G_MemoryPool.FreeTableCount; i++)
//	{
//		itNode = PX_MemoryPool_GetFreeTable(i);
//		pcTempStart = (unsigned char *)itNode->StartAddr;
//		pcTempEnd = (unsigned char *)itNode->EndAddr;
//		查找是否有可以向后连接的节点
//		if (pcTempStart == (unsigned char *)FreeNode.EndAddr + 1)
//		{
//			如果有,按照之前所述连接节点
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
//		查找是否有可以向前连接的节点
//		if (pcTempEnd + 1 == (unsigned char *)FreeNode.StartAddr)
//		{
//			如果有,按照之前所述连接节点
//			if (sIndex == 0xffffffff)
//			{
//				sIndex = i;
//				Refresh this node
//				itNode->EndAddr = FreeNode.EndAddr;
//				FreeNode = *itNode;
//				移除上一个节点
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//			}
//			else
//			{
//				itNode->EndAddr = FreeNode.EndAddr;
//				G_MemoryPool.FreeSize += sizeof(MemoryNode);
//				移除上一个节点
//				PX_MemoryPoolRemoveFreeNode(sIndex);
//			}
//			bExist = 1;
//		}
//	}
//	如果没有连接节点,分配释放内存节点
//	if (bExist == 0)
//		*PX_AllocFreeMemoryNode() = FreeNode;
//
//	这个函数用于更新最大的释放内存节点大小
//	PX_UpdateMaxFreqSize();
//
//}
#endif