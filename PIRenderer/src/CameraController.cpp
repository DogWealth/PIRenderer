#include "CameraController.h"
#include "Input.h"
namespace PIRenderer {
	OrthographicCameraController::OrthographicCameraController(float left, float right, float bottom, float top, float far, float near)
		: m_Camera(left, right, bottom, top, far, near)
	{
	}

	void OrthographicCameraController::OnUpdate()
	{
		if (Input::IsKeyPressed(SDL_SCANCODE_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_D))
			m_CameraPosition.x += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
			m_CameraRotation += m_CameraRotationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}

	PerspectiveCameraController::PerspectiveCameraController(float n, float f, float fov, float aspectRatio)
		: m_Camera(n, f, fov, aspectRatio)
	{
	}

	void PerspectiveCameraController::OnUpdate()
	{
		if (Input::IsKeyPressed(SDL_SCANCODE_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_D))
			m_CameraPosition.x += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
			m_CameraRotation += m_CameraRotationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_CameraRotation -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}
}