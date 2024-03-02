#pragma once
#include "Camera.h"

#define PI 3.1415
namespace PIRenderer {
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float left, float right, float bottom, float top, float far, float near);

		void OnUpdate();

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

	private:
		OrthographicCamera m_Camera;
		Vector3f m_CameraPosition = { 0.f, 0.f, 0.f };
		Vector3f m_CameraRotation = { 0.f, 0.f, 0.f };

		float m_CameraTranslationSpeed = 0.01f;
		float m_CameraRotationSpeed = 3.f;
	};

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float n, float f, float fov, float aspectRatio);

		virtual void OnUpdate();

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

	protected:
		PerspectiveCamera m_Camera;
		Vector3f m_CameraPosition = { 0.f, 0.f, 30.f };
		Vector3f m_CameraRotation = { 0.f, 0.f, 0.f };

		float m_CameraTranslationSpeed = 0.05f;
		float m_CameraRotationSpeed = 1.f;
	};

	class OrbitController : public PerspectiveCameraController
	{
	public:
		OrbitController(float n, float f, float fov, float aspectRatio)
			: PerspectiveCameraController(n, f, fov, aspectRatio)
		{

		}

		virtual void OnUpdate() override;

	private:
		float Radius = 5;
		float Theta = 0;
		float Phi = 0;

	};
}

