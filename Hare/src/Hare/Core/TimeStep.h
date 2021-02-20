#pragma once

namespace Hare
{
	class DeltaTime
	{
	public:
		DeltaTime(float time = 0.0f)
			: m_Time(time)
		{

		}

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliSeconds() const { return m_Time * 1000.0f; }

		inline operator float() const { return m_Time; }

	private:
		float m_Time;
	};
}