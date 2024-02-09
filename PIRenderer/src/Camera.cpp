#include "Camera.h"

namespace PIRenderer {
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float far, float near)
		: Camera(Matrix4::Orthographic(left, right, bottom, top, far, near), Matrix4::Identity())
	{
		RecalculateViewMatrix();
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top, float far, float near)
	{
		m_ProjectionMatrix = Matrix4::Orthographic(left, right, bottom, top, far, near);
		RecalculateViewMatrix();
	}

	PerspectiveCamera::PerspectiveCamera(float n, float f, float fov, float aspectRatio)
		: Camera(Matrix4::Perspective(n, f, fov, aspectRatio), Matrix4::Identity())
	{
		RecalculateViewMatrix();
	}

	void PerspectiveCamera::SetProjection(float n, float f, float fov, float aspectRatio)
	{
		m_ProjectionMatrix = Matrix4::Perspective(n, f, fov, aspectRatio);
		RecalculateViewMatrix();
	}

}