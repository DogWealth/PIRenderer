#pragma once
#include <SDL.h>
#include "Window.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "Mesh.h"
#include "CameraController.h"
#include "Light.h"
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
		void Render(Mesh* mesh);

	private:
		Window* m_Window;
		Renderer* m_Renderer;

		Mesh* m_Mesh;
		Mesh* m_HeadMesh;
		Mesh* m_LightMesh;

		Shader* m_Shader;
		Shader* m_HeadShader;
		Shader* m_LightShader;

		Texture* m_Texture;
		Texture* m_HeadTexture;

		PerspectiveCameraController* m_Controller;
		//OrthographicCameraController* m_Controller;

		bool m_Running = true;


	};
}

