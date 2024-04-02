#include "Test_PBR.h"
#include "../shader/PbrShader.h"
namespace PIRenderer {
    Test_PBR::Test_PBR()
    {
        m_SphereMesh = new Mesh("obj/sphere.obj");
        m_SphereShader = Shader::Create("PbrShader");
        //m_AlbedoMap = new Texture("obj/sphere/rustediron2_basecolor.tga");
        //m_NormalMap = new Texture("obj/sphere/rustediron2_normal.tga");
        m_MetallicMap = new Texture("obj/sphere/rustediron2_metallic.tga");
        m_RoughnessMap = new Texture("obj/sphere/rustediron2_roughness.tga");
        

        m_Renderer->SetDepthSquareBuffer(nullptr);
        m_Renderer->SetDepthBuffer(m_DepthBuffer);

        m_LightMesh = new Mesh("obj/cube.obj");
        m_LightShader = Shader::Create("LightShader");


        m_Renderer->UseBackFaceCulling(true);
        m_Renderer->UseDepthTest(true);


    }

    Test_PBR::~Test_PBR()
    {
        delete m_SphereShader;
        delete m_SphereMesh;
    }

    void Test_PBR::OnUpdate(double tick)
    {
        m_Controller->OnUpdate(tick);
        m_Renderer->Clear();

        Matrix4 ModelMatrix = Matrix4::Scale(0.01, 0.01, 0.01);
        Matrix4 VPMatrix = m_Controller->GetCamera().GetViewProjectionMatrix();
        /*Vector3f lightPos = { (float)(5 * sin(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)),
                            (float)(5 * sin(0 * PI / 180.0f)),
                            (float)(5 * cos(clock() / 10 * PI / 180.0f) * cos(45 * PI / 180.0f)) };*/
        Vector3f lightPos = { 0.0f,  0.0f, 10.0f };
        DirectionLight dLight = { -lightPos, lightPos, 300 };
        Matrix4 LightModelMatrix = Matrix4::Scale(0.1, 0.1, 0.1) * Matrix4::Translate(dLight.GetPosition());

        m_SphereShader->SetLight(dLight);
        m_SphereShader->SetModelMatrix(ModelMatrix);
        m_SphereShader->SetVPMatrix(VPMatrix);
        m_SphereShader->SetEyePos(m_Controller->GetCamera().GetPosition());

        dynamic_cast<PbrShader*>(m_SphereShader)->SetAlbedoMap(m_AlbedoMap);
        dynamic_cast<PbrShader*>(m_SphereShader)->SetNormalMap(m_NormalMap);
        dynamic_cast<PbrShader*>(m_SphereShader)->SetMetallicMap(m_MetallicMap);
        dynamic_cast<PbrShader*>(m_SphereShader)->SetRoughnessMap(m_RoughnessMap);

        dynamic_cast<PbrShader*>(m_SphereShader)->SetAlbedo({ 0.5f, 0.f, 0.f });
        dynamic_cast<PbrShader*>(m_SphereShader)->SetAmbientOcclusion(1.0f);
        dynamic_cast<PbrShader*>(m_SphereShader)->SetMetallic(0.5f);
        dynamic_cast<PbrShader*>(m_SphereShader)->SetRoughness(0.5f);

        m_Renderer->BindShader(m_SphereShader);

        //ShowSphereMatrix();

        m_Renderer->DrawMesh(m_SphereMesh);

        //light
        m_Renderer->BindShader(m_LightShader);
        m_LightShader->SetVPMatrix(VPMatrix);
        m_LightShader->SetModelMatrix(LightModelMatrix);

        Render(m_LightMesh);

        m_Window->OnUpdate();
    }

    void Test_PBR::ShowSphereMatrix()
    {
        int nrRows = 4;
        int nrColumns = 4;
        float spacing = 1.5;

        for (int row = 0; row < nrRows; ++row)
        {
            dynamic_cast<PbrShader*>(m_SphereShader)->SetMetallic((float)row / (float)nrRows);
            for (int col = 0; col < nrColumns; ++col)
            {
                // we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
                // on direct lighting.
                dynamic_cast<PbrShader*>(m_SphereShader)->
                    SetRoughness(std::min(std::max((float)col / (float)nrColumns, 0.05f), 1.0f));

                Matrix4 M = Matrix4::Scale(0.01, 0.01, 0.01) * Matrix4::Translate((col - (nrColumns / 2)) * spacing, (row - (nrRows / 2)) * spacing, 0.0f);

                m_SphereShader->SetModelMatrix(M);
                m_Renderer->DrawMesh(m_SphereMesh);
            }
        }
    }
}
    
