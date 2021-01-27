#pragma once

#include <Hare.h>

#include "Color.h"
#include "Random.h"
#include "ParticleSystem.h"

class Player
{
public:
	Player();

public:
	void LoadAsset();
	void OnUpdate(Hare::TimeStep ts);
	void OnRender();
	void OnImGuiRender();
	void Reset();

	inline float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }
	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline uint32_t GetScore() const { return (uint32_t)(m_Position.x + 10.0f) / 10.0f; }

private:
	glm::vec2 m_Position = { -10.0f, 0.0f };
	glm::vec2 m_Velocity = { 5.0f, 0.0f };

	float m_EnginePower = 0.5f;
	float m_Gravity = 0.4f;

	float m_Time = 0.0f;
	float m_SmokeEmitInterval = 0.4f;
	float m_SmokeNextEmitTime = m_SmokeEmitInterval;

	ParticleProps m_SmokeParticle;
	ParticleProps m_EngineParticle;
	ParticleSystem m_ParticleSystem;

	Hare::Ref<Hare::Texture2D> m_ShipTexture;
};