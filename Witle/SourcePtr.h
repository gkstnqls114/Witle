#pragma once
#ifndef TEMPLATE_SOURCE_PTR
#define TEMPLATE_SOURCE_PTR

class SourcePtr
{
	void* m_Ptr{ nullptr };
	UINT m_byteSize{ 0 };

public:
	template <typename T>
	SourcePtr(T* ptr);

	~SourcePtr();

	void* GetPtr() const { return m_Ptr; }
	UINT GetbyteSize() const { return m_byteSize; }
};


template<typename T>
inline SourcePtr::SourcePtr(T * ptr)
{
	m_Ptr = static_cast<void *>(ptr);
	m_byteSize = sizeof(T);
}
#endif // !1
