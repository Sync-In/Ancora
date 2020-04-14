#include "aepch.h"
#include "Renderer3D.h"

#include "Ancora/Renderer/VertexArray.h"
#include "Ancora/Renderer/Shader.h"
#include "Ancora/Renderer/RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Ancora {

  struct Renderer3DStorage
  {
    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;
    Ref<IndexBuffer> QuadIndexBuffer;
    Ref<Shader> QuadShader;
    Ref<Shader> CubeMapShader;
  };

  static Renderer3DStorage s_Data;

  void Renderer3D::Init()
  {
    s_Data.QuadVertexArray = VertexArray::Create();

    float vertices[] = {
      -0.5f, -0.5f,  0.5f,
       0.5f, -0.5f,  0.5f,
       0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,

       0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
       0.5f, -0.5f, -0.5f
    };
    s_Data.QuadVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
    BufferLayout layout = {
      { ShaderDataType::Float3, "a_Position" }
    };
    s_Data.QuadVertexBuffer->SetLayout(layout);
    s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

    uint32_t indices[36] = {
      0, 1, 2, 2, 3, 0,
      4, 5, 6, 6, 7, 4,
      1, 2, 4, 4, 7, 1,
      6, 0, 3, 3, 5, 6,
      3, 2, 4, 4, 5, 3,
      0, 1, 7, 7, 6, 0
    };
    s_Data.QuadIndexBuffer = IndexBuffer::Create(indices, 36);
    s_Data.QuadVertexArray->SetIndexBuffer(s_Data.QuadIndexBuffer);

    s_Data.QuadShader = Shader::Create("Sandbox/assets/shaders/FlatColor.glsl");
    s_Data.CubeMapShader = Shader::Create("Sandbox/assets/shaders/CubeMap.glsl");
  }

  void Renderer3D::Shutdown()
  {
  }

  void Renderer3D::BeginScene(const Renderer3DSceneData& sceneData)
  {
    s_Data.QuadShader->Bind();
    s_Data.QuadShader->SetMat4("u_ViewProjection", sceneData.Camera->GetViewProjectionMatrix());
    s_Data.CubeMapShader->Bind();
    s_Data.CubeMapShader->SetMat4("u_ViewProjection", sceneData.Camera->GetViewProjectionMatrix());
  }

  void Renderer3D::EndScene()
  {
  }

  void Renderer3D::SkyBox(Ref<CubeMap> cubeMap, const glm::vec3& position, const glm::vec3& size)
  {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
    s_Data.CubeMapShader->SetMat4("u_Transform", transform);

    cubeMap->Bind(0);
    s_Data.CubeMapShader->SetInt("u_Skybox", 0);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

  void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
  {
    s_Data.QuadShader->SetFloat4("u_Color", color);

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), size);
    s_Data.QuadShader->SetMat4("u_Transform", transform);

    RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
  }

}
