#include "Camera.h"

namespace PIRenderer {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float far, float near)
		: m_ProjectionMatrix(Matrix4::Orthographic(left, right, bottom, top, far, near)), m_ViewMatrix(Matrix4::Identity())
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float far, float near)
	{
		m_ProjectionMatrix = Matrix4::Orthographic(left, right, bottom, top, far, near);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		Matrix4 transform = Matrix4::Translate(m_Position.x, m_Position.y, m_Position.z);
		Matrix4 rotation = Matrix4::RotateY(m_Rotation);

		m_ViewMatrix = Matrix4::Transpose(rotation) * (Matrix4::Zero() - transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}