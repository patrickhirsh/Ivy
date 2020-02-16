#include "Core/IvyPCH.h"
#include "System/Renderer.h"
#include "Resource/Resource.h"
#include "Entity/Entity.h"

namespace _Ivy
{
    void Renderer::Update(Ivy::Ref<Window> window)
    {
        // Hardcoded Shader Binding... TODO: abstract this.
        if (Resource::_vertexShader == nullptr || Resource::_fragmentShader == nullptr)
        {
            Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultLitVertex.shader");
            Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultLitFragment.shader");
            std::vector<Ivy::Ref<Shader>> shaders;
            shaders.push_back(Resource::_vertexShader);
            shaders.push_back(Resource::_fragmentShader);
            Shader::Bind(shaders);
        }

        glEnable(GL_DEPTH_TEST);
        auto windowDimensions = window->GetDimensions();

        //GL(glEnable(GL_CULL_FACE));

        for (auto entity : Entities)
        {
            for (auto requestInstance : request.second)
            {
                Ivy::Ref<StaticMeshResource::MetaData> resourceMeta = Resource::BindStaticMesh(requestInstance);
                if (resourceMeta)
                {
                    //cy::Matrix4f MVP = _projection * (_sceneTranslation * _sceneRotation) * _model;

                    // TODO: Fix scene transformation to properly account for aspect ratio
                    // scene transformations
                    _model = cy::Matrix4f::Scale(1);
                    _view = _sceneTranslation * _sceneRotation;
                    _projection = cy::Matrix4f::Perspective(1.0f, (width / height), 1, 100);
                    cy::Matrix4f NTRANS = (_sceneTranslation * _sceneRotation) * _model;
                    NTRANS.Invert();
                    NTRANS.Transpose();
                    GLuint mLoc = glGetUniformLocation(Shader::GetActiveProgram(), "model");
                    GLuint vLoc = glGetUniformLocation(Shader::GetActiveProgram(), "view");
                    GLuint pLoc = glGetUniformLocation(Shader::GetActiveProgram(), "projection");
                    GLuint ntransLoc = glGetUniformLocation(Shader::GetActiveProgram(), "NTRANS");
                    GL(glUniformMatrix4fv(mLoc, 1, GL_FALSE, _model.cell));
                    GL(glUniformMatrix4fv(vLoc, 1, GL_FALSE, _view.cell));
                    GL(glUniformMatrix4fv(pLoc, 1, GL_FALSE, _projection.cell));
                    GL(glUniformMatrix4fv(ntransLoc, 1, GL_FALSE, NTRANS.cell));

                    // execute draw call
                    GL(glDrawArrays(GL_TRIANGLES, 0, resourceMeta->BufferCount));
                    //Resource::UnbindStaticMesh(requestInstance);
                }
            }
        }
    }

    /*
	void Renderer::ProcessRequests(GLFWwindow* window)
	{
        // Hardcoded Shader Binding... TODO: abstract this.
        if (Resource::_vertexShader == nullptr || Resource::_fragmentShader == nullptr)
        {
            Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultLitVertex.shader");
            Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultLitFragment.shader");
            std::vector<Ivy::Ref<Shader>> shaders;
            shaders.push_back(Resource::_vertexShader);
            shaders.push_back(Resource::_fragmentShader);
            Shader::Bind(shaders);
        }

        glEnable(GL_DEPTH_TEST);

        GLint width, height;
        glfwGetWindowSize(window, &width, &height);

        //GL(glEnable(GL_CULL_FACE));

        for (auto request : _staticMeshDrawRequests)
        {
            for (auto requestInstance : request.second)
            {
                Ivy::Ref<StaticMeshResource::MetaData> resourceMeta = Resource::BindStaticMesh(requestInstance);
                if (resourceMeta)
                {
                    //cy::Matrix4f MVP = _projection * (_sceneTranslation * _sceneRotation) * _model;

                    // TODO: Fix scene transformation to properly account for aspect ratio
                    // scene transformations
                    _model = cy::Matrix4f::Scale(1);
                    _view = _sceneTranslation * _sceneRotation;
                    _projection = cy::Matrix4f::Perspective(1.0f, (width / height), 1, 100);
                    cy::Matrix4f NTRANS = (_sceneTranslation * _sceneRotation) * _model;
                    NTRANS.Invert();
                    NTRANS.Transpose();
                    GLuint mLoc = glGetUniformLocation(Shader::GetActiveProgram(), "model");
                    GLuint vLoc = glGetUniformLocation(Shader::GetActiveProgram(), "view");
                    GLuint pLoc = glGetUniformLocation(Shader::GetActiveProgram(), "projection");
                    GLuint ntransLoc = glGetUniformLocation(Shader::GetActiveProgram(), "NTRANS");
                    GL(glUniformMatrix4fv(mLoc, 1, GL_FALSE, _model.cell));
                    GL(glUniformMatrix4fv(vLoc, 1, GL_FALSE, _view.cell));
                    GL(glUniformMatrix4fv(pLoc, 1, GL_FALSE, _projection.cell));
                    GL(glUniformMatrix4fv(ntransLoc, 1, GL_FALSE, NTRANS.cell));

                    // execute draw call
                    GL(glDrawArrays(GL_TRIANGLES, 0, resourceMeta->BufferCount));
                    //Resource::UnbindStaticMesh(requestInstance);
                }
            }
        }
	}
    */
}