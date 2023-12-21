#include "CameraController.h"
#include "Input.h"
namespace PIRenderer {
	OrthographicCameraController::OrthographicCameraController(float left, float right, float bottom, float top, float far, float near)
		: m_Camera(left, right, bottom, top, far, near)
	{
	}

	void OrthographicCameraController::OnUpdate()
	{
		if (Input::IsKeyPressed(SDL_SCANCODE_UP))
			m_CameraPosition.z -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_DOWN))
			m_CameraPosition.z += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
			m_CameraRotation += m_CameraRotationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}
}