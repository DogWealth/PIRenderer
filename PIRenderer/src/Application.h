#pragma once
#include <SDL.h>
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CameraController.h"
namespace PIRenderer {
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
		void OnUpdate();
		void OnEvent();

		void Render();

	private:
		Window* m_Window;
		Renderer* m_Renderer;
		Mesh* m_Mesh;
		Shader*	m_Shader;
		Texture* m_Texture;
		PerspectiveCameraController* m_Controller;
		//OrthographicCameraController* m_Controller;

		bool m_Running = true;


	};
}

