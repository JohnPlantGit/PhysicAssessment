#include "LengthConstraint.h"
#include "Rigidbody.h"

LengthConstraint::LengthConstraint()
{
}

LengthConstraint::LengthConstraint(Rigidbody* body1, Rigidbody* body2, float length)
{
	m_body1 = body1;
	m_body2 = body2;
	m_length = length;
}

LengthConstraint::~LengthConstraint()
{
}

void LengthConstraint::SatisfyConstraint()
{
	glm::vec2 direction = m_body2->GetPosition() - m_body1->GetPosition();
	float length = glm::length(direction);

	if (glm::all(glm::notEqual(direction, glm::vec2(0,0))))
	{
		direction = glm::normalize(direction);

		glm::vec2 moveVector = 0.5f * (length - m_length) * direction * m_stiffness;

		if (!m_body1->GetKinematic() && !m_body2->GetKinematic())
		{
			m_body1->SetPosition(m_body1->GetPosition() + moveVector);
			m_body2->SetPosition(m_body2->GetPosition() - moveVector);
		}
		if (m_body1->GetKinematic() && !m_body2->GetKinematic())
		{
			m_body2->SetPosition(m_body2->GetPosition() - moveVector * 2.0f);
		}
		if (!m_body1->GetKinematic() && m_body2->GetKinematic())
		{
			m_body1->SetPosition(m_body1->GetPosition() + moveVector * 2.0f);
		}
	}
}
