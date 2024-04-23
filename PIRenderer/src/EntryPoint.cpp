#include "Window.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Application.h"
#include "test/Test_Blinn_Phong.h"
#include "test/Test_IBL.h"
#include "test/Test_PBR.h"
#include "math.h"

using namespace PIRenderer;
int main()
{
	PIRenderer::Test_IBL test;
	
	test.Run();

}