#pragma once
#include <SDL.h>
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CameraController.h"
#include "Light.h"
#include "math.h"
#define WIDTH	800.0f
#define HEIGHT	600.0f
namespace PIRenderer {
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		virtual void OnUpdate(double tick);
		void OnEvent();

		void Render();
		void Render(Mesh* mesh);

	protected:
		Window* m_Window;
		Renderer* m_Renderer;
		float* m_DepthBuffer;
		PerspectiveCameraController* m_Controller;
		//OrthographicCameraController* m_Controller;
		bool m_Running = true;


	};
}

