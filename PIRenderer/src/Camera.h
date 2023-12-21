#pragma once
#include "Vector.h"
#include "Matrix.h"
namespace PIRenderer {
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float far, float near);

		void SetProjection(float left, float right, float bottom, float top, float far, float near);

		void LookAt(const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upAxis)
		{

			m_ViewMatrix =  Matrix4::LookAt(eyePos, lookAt, upAxis);
		}

		void SetPosition(const Vector3f& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}
		const Vector3f& GetPosition() const { return m_Position; }

		void SetLookDir(const Vector3f& lookDir)
		{
			m_LookDir = lookDir;
			RecalculateViewMatrix();
		}
		const Vector3f& GetLookDir() const { return m_LookDir; }

		void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}
		float GetRotation() const { return m_Rotation; }

		const Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Matrix4& GetViewMatrix() const { return m_ViewMatrix; }
		const Matrix4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

	private:
		void RecalculateViewMatrix();

	private:
		Matrix4 m_ProjectionMatrix;
		Matrix4 m_ViewMatrix;
		Matrix4 m_ViewProjectionMatrix;

		Vector3f m_Position = { 0.0f, 0.0f, 0.0f };
		Vector3f m_LookDir = { 0.0f, 0.0f, -1.0f };
		float m_Rotation = 0.0f;


	};

}
