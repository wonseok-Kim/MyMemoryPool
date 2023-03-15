#pragma once

#include <cstdint>
#include <new>

#define _SAFEPOOL

class __CheckCode
{
protected:
	static uint32_t NextCheckCode() { return s_nextCheckCode++; }

private:
	inline static uint32_t s_nextCheckCode = 0xcccccccc;
};

//inline uint32_t NextCheckCode()
//{
//	static uint32_t nextCheckCode = 0xcccccccc;
//
//	return nextCheckCode++;
//}

template <typename T>
class MemoryPool : public __CheckCode
{
	struct FreeNode
	{
		T Data;
		FreeNode* Next;
	};

public:
	MemoryPool(int initAllocCount, bool bNeedToCallConstructorDestructorEveryTime = true)
	{
		m_checkCode = NextCheckCode();		
		m_bNeedToCallConstructorDestructorEveryTime = bNeedToCallConstructorDestructorEveryTime;

		for (int i = 0; i < initAllocCount; ++i)
		{
			FreeNode* node = NewNode();

			node->Next = m_pFreeNode;
			m_pFreeNode = node;
		}
	}

	~MemoryPool()
	{
		if (m_usageCount != 0)
			__debugbreak();

		for (int i = 0; i < m_allocCount; ++i)
		{
			FreeNode* toDelete = m_pFreeNode;
			if (toDelete == nullptr)
				__debugbreak();

			m_pFreeNode = m_pFreeNode->Next;			
			
#ifdef _SAFEPOOL
			delete[]((uint8_t*)toDelete - sizeof(uintptr_t));
#else
			delete toDelete;
#endif // _SAFEPOOL
		}
	}

	T* Alloc()
	{		
		m_usageCount++;

		FreeNode* node;
		if (m_pFreeNode == nullptr)
		{
			node = NewNode();
		}
		else
		{
			node = m_pFreeNode;
			m_pFreeNode = m_pFreeNode->Next;
		}

#ifdef _SAFEPOOL
		InsertCheckCode(node);
#endif // _SAFEPOOL

		if (m_bNeedToCallConstructorDestructorEveryTime)
			return new((T*)node) T();
		else
			return (T*)node;
	}

	void Free(T* p)
	{
		m_usageCount--;

#ifdef _SAFEPOOL
		if (!IsValidCheckCode(p))
			__debugbreak();
#endif // _SAFEPOOL

		if (m_bNeedToCallConstructorDestructorEveryTime)
			p->~T();

		FreeNode* node = (FreeNode*)p;
		node->Next = m_pFreeNode;
		m_pFreeNode = node;
	}

	int GetAllocCount() { return m_allocCount; }
	int GetUsageCount() { return m_usageCount; }

private:
	FreeNode* NewNode()
	{
		m_allocCount++;
		
#ifdef _SAFEPOOL
		uint8_t* temp = new uint8_t[sizeof(FreeNode) + sizeof(uintptr_t)];
		FreeNode* node = (FreeNode*)(temp + sizeof(uintptr_t));
#else
		FreeNode* node = new FreeNode;
#endif // _SAFEPOOL

		if (!m_bNeedToCallConstructorDestructorEveryTime)
			new((T*)node) T();

		return node;
	}

	void InsertCheckCode(FreeNode* node)
	{
		uint8_t* buf = (uint8_t*)node;

		*(uint32_t*)(buf - 4) = m_checkCode;
		*(uint32_t*)(buf + sizeof(T)) = m_checkCode;
	}

	bool IsValidCheckCode(T* node)
	{
		uint8_t* buf = (uint8_t*)node;

		if (*(uint32_t*)(buf - 4) != m_checkCode)
			return false;
		if (*(uint32_t*)(buf + sizeof(T)) != m_checkCode)
			return false;

		return true;
	}

private:
	FreeNode* m_pFreeNode = nullptr;

	int m_usageCount = 0;
	int m_allocCount = 0;

	uint32_t m_checkCode;
	bool m_bNeedToCallConstructorDestructorEveryTime;
};


