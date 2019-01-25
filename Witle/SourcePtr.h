#pragma once
#ifndef TEMPLATE_SOURCE_PTR
#define TEMPLATE_SOURCE_PTR

class SourcePtr
{
public:
	const void* const m_Ptr{ nullptr };
	UINT m_byteSize{ 0 };

	template <typename T>
	SourcePtr(T* ptr) : m_Ptr(static_cast<void *>(ptr)), m_byteSize(sizeof(T)) {};
	~SourcePtr();

	//const void* const GetPtr() const { return m_Ptr; }
	//const UINT GetbyteSize() const { return m_byteSize; }
};

#endif // !TEMPLATE_SOURCE_PTR
