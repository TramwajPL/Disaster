#pragma once

namespace Clumsy
{
	class ReferenceCounter
	{
	public:
		ReferenceCounter() :
			m_RefCount(1) {}

		inline int GetReferenceCount() { return m_RefCount; }

		inline void AddReference() { m_RefCount++; }
		inline bool RemoveReference() { m_RefCount--; return m_RefCount == 0; }

	private:
		int m_RefCount;
	};
}
