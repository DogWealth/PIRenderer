#pragma once
#include "Camera.h"
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
		float m_CameraRotation = 0.f;

		float m_CameraTranslationSpeed = 1.f;
		float m_CameraRotationSpeed = 3.f;
	};
}

