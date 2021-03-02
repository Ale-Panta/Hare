#include "hrpch.h"
#include "UUID.h"

#include <random>

namespace Hare
{
	static std::random_device s_RandomDevice;
	static std::mt19937_64 eng(s_RandomDevice());
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

	UUID::UUID()
		: m_ID(s_UniformDistribution(eng))
	{

	}

	UUID::UUID(uint64_t uuid)
		: m_ID(uuid)
	{

	}

	UUID::UUID(const UUID& other)
		: m_ID(other.m_ID)
	{

	}

}