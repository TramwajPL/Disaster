
#include "../pch.h"
#include "PhysicsObject.h"

namespace Clumsy
{
	PhysicsObject::PhysicsObject(const PhysicsObject& other) :
		m_Position(other.m_Position),
		m_OldPosition(other.m_OldPosition),
		m_Velocity(other.m_Velocity),
		m_Collider(other.m_Collider)
	{
		m_Collider->AddReference();
	}
	
	void PhysicsObject::operator=(PhysicsObject other)
	{
		char* temp[sizeof(PhysicsObject) / sizeof(char)];
		memcpy(temp, this, sizeof(PhysicsObject));
		memcpy(this, &other, sizeof(PhysicsObject));
		memcpy(&other, temp, sizeof(PhysicsObject));
	}

	PhysicsObject::~PhysicsObject()
	{
		if (m_Collider->RemoveReference())
		{
			delete m_Collider;
		}
	}

	void PhysicsObject::Integrate(float delta)
	{
		m_Position += m_Velocity * delta;
	}
}