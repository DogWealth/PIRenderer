#pragma once
#include "Vector.h"
#include "Matrix.h"
namespace PIRenderer {
	class Camera
	{
	public:
		Camera(Matrix4 projectionMatrix, Matrix4 viewMatrix)
			: m_ProjectionMatrix(projectionMatrix), m_ViewMatrix(viewMatrix)
		{
		}

		
		void LookAt(const Vector3f& eyePos, const Vector3f& lookAt, const Vector3f& upAxis)
		{

			m_ViewMatrix = Matrix4::LookAt(eyePos, lookAt, upAxis);
			m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
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

		void SetRotation(Vector3f rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}
		Vector3f GetRotation() const { return m_Rotation; }

		const Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const Matrix4& GetViewMatrix() const { return m_ViewMatrix; }
		const Matrix4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


	protected:
		void RecalculateViewMatrix()
		{
			Matrix4 transform = Matrix4::Translate(-m_Position.x, -m_Position.y, -m_Position.z);
			Matrix4 rotation = Matrix4::RotateEuler(m_Rotation.y, m_Rotation.x, m_Rotation.z);

			m_ViewMatrix = transform * Matrix4::Transpose(rotation);
			m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
		}

	protected:
		Matrix4 m_ProjectionMatrix;
		Matrix4 m_ViewMatrix;
		Matrix4 m_ViewProjectionMatrix;
		           
		Vector3f m_Position = { 0.0f, 0.0f, 2.0f };
		Vector3f m_LookDir = { 0.0f, 0.0f, -1.0f };
		Vector3f m_Rotation = { 0.0f, 0.0f, 0.0f };
	};

	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float far, float near);

		void SetProjection(float left, float right, float bottom, float top, float far, float near);
	};


	class PerspectiveCamera : public Camera
	{
	public:
		PerspectiveCamera(float n, float f, float fov, float aspectRatio);

		void SetProjection(float n, float f, float fov, float aspectRatio);
	};

}
