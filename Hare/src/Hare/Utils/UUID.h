#pragma once

namespace Hare
{
	// UUID stands for Universally unique Identifier. 
	// Every time we need an ID we just increment a uint64 count.
	class UUID
	{
	public:
		UUID();
		~UUID() = default;
		UUID(uint64_t id);
		UUID(const UUID& other);

	public:
		operator uint64_t() { return m_ID; }
		operator const uint64_t() const { return m_ID; }

	private:
		uint64_t m_ID = 0;
	};
}