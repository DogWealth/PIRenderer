#include "Window.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Application.h"
#include "test/Test_Blinn_Phong.h"
#include "test/Test_IBL.h"
#include "test/Test_PBR.h"
int main()
{
	PIRenderer::Test_PBR test;
	
	test.Run();

}