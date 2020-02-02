#include "Core/IvyPCH.h"
#include "Render/Render.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexBufferLayout.h"
#include "Render/VertexArray.h"
#include "Render/IndexBuffer.h"
#include "Resource/Resource.h"

namespace _Ivy
{
	void Render::DrawRequest(Ivy::Ref<Ivy::StaticMesh> object)
	{
        _staticMeshDrawRequests[object->GetMeshPath()].push_back(object);
	}

    void Render::SetSceneTranslation(const cy::Vec3f& translation)
    {
        _sceneTranslation = cy::Matrix4f::Translation(translation);
    }

    void Render::SetSceneRotation(float angleX, float angleY, float angleZ)
    {
        _sceneRotation = cy::Matrix4f::RotationXYZ(angleX, angleY, angleZ);
    }

	void Render::ProcessRequests(GLFWwindow* window)
	{
        // Hardcoded Shader Binding... TODO: abstract this.
        if (Resource::_vertexShader == nullptr || Resource::_fragmentShader == nullptr)
        {
            Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultVertex.shader");
            Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultFragment.shader");
            std::vector<Ivy::Ref<Shader>> shaders;
            shaders.push_back(Resource::_vertexShader);
            shaders.push_back(Resource::_fragmentShader);
            Shader::Bind(shaders);
        }

        GLint width, height;
        glfwGetWindowSize(window, &width, &height);

        for (auto request : _staticMeshDrawRequests)
        {
            for (auto requestInstance : request.second)
            {
                // TODO: Maybe this shouldn't expose the underlying resource type...
                Ivy::Ref<StaticMeshResource> resource = Resource::BindStaticMesh(requestInstance);
                if (resource)
                {
                    // scene transformations
                    _projection = cy::Matrix4f::Perspective(1.0f, width / height, 0, 10000);
                    _model = cy::Matrix4f::Scale(1);
                    cy::Matrix4f MVP = _projection * (_sceneTranslation * _sceneRotation) * _model;
                    GLuint mvpLoc = glGetUniformLocation(Shader::GetActiveProgram(), "MVP");
                    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, MVP.cell);

                    // execute draw call
                    glDrawElements(GL_TRIANGLES, resource->GetIndeceCount(), GL_UNSIGNED_INT, 0);
                    resource->Unbind();
                }
            }
        }
	}
}