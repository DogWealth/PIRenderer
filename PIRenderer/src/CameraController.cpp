#include "CameraController.h"
#include "Input.h"
#define MIN(a, b) (a >= b)? b : a 
#define MAX(a, b) (a <= b)? b : a 
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

		if (Input::IsKeyPressed(SDL_SCANCODE_W))
			m_CameraPosition.z -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_S))
			m_CameraPosition.z += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
			m_CameraRotation.y += m_CameraRotationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_CameraRotation.y -= m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}

	PerspectiveCameraController::PerspectiveCameraController(float n, float f, float fov, float aspectRatio)
		: m_Camera(n, f, fov, aspectRatio)
	{
	}

	void PerspectiveCameraController::OnUpdate(double tick)
	{
		if (Input::IsKeyPressed(SDL_SCANCODE_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_D))
			m_CameraPosition.x += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_W))
			m_CameraPosition.z -= m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_S))
			m_CameraPosition.z += m_CameraTranslationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
			m_CameraRotation.y -= m_CameraRotationSpeed;

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
			m_CameraRotation.y += m_CameraRotationSpeed;

		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);
	}

	void OrbitController::OnUpdate(double tick)
	{
		if (Input::IsKeyPressed(SDL_SCANCODE_LEFT))
		{
			Theta -= m_CameraRotationSpeed * tick;
		}

		if (Input::IsKeyPressed(SDL_SCANCODE_RIGHT))
		{
			Theta += m_CameraRotationSpeed * tick;
		}

		if (Input::IsKeyPressed(SDL_SCANCODE_UP))
		{
			Phi += m_CameraRotationSpeed * tick;
			Phi = MIN(90, Phi);
		}

		if (Input::IsKeyPressed(SDL_SCANCODE_DOWN))
		{
			Phi -= m_CameraRotationSpeed * tick;
			Phi = MAX(-90, Phi);
		}

		if (Input::IsKeyPressed(SDL_SCANCODE_W))
			Radius -= m_CameraTranslationSpeed * tick;

		if (Input::IsKeyPressed(SDL_SCANCODE_S))
			Radius += m_CameraTranslationSpeed * tick;


		m_CameraPosition.x = Radius * sin(Theta * PI / 180.0f) * cos(Phi * PI / 180.0f);
		m_CameraPosition.y = Radius * sin(Phi * PI / 180.0f);
		m_CameraPosition.z = Radius * cos(Theta * PI / 180.0f) * cos(Phi * PI / 180.0f);

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.LookAt(m_CameraPosition, {0, 0, 0}, {0, 1, 0});
	}
}