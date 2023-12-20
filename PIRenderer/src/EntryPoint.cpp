#include "Window.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Shader.h"

#define WIDTH 600
#define HEIGHT 600

int main()
{
	PIRenderer::Window Window(WIDTH, HEIGHT, "PI Renderer");

	PIRenderer::Renderer* renderer = new PIRenderer::Renderer((uint32_t*)(Window.GetSurface()->pixels), WIDTH, HEIGHT);
	PIRenderer::Mesh* mesh = new PIRenderer::Mesh("obj/african_head.obj");//注意相对路径时相对PIRenderer这个文件夹开始的
	PIRenderer::Shader* shader = PIRenderer::Shader::Create("BasicShader");

	renderer->AddMesh(mesh);
	renderer->BindShader(shader);

	Window.SetRenderer(renderer);
	Window.Run();
}