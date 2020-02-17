#include "Core/IvyPCH.h"
#include "Render/Renderer.h"
#include "Resource/Resource.h"

namespace _Ivy
{
    Renderer::Renderer(GLFWwindow* window)
    {
        // Hardcoded Shader Binding... TODO: abstract this.
        Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultLitVertex.shader");
        Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultLitFragment.shader");
        Resource::_unlitVertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultUnlitVertex.shader");
        Resource::_unlitFragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultUnlitFragment.shader");
        Resource::_litShaders.push_back(Resource::_vertexShader);
        Resource::_litShaders.push_back(Resource::_fragmentShader);
        Resource::_unlitShaders.push_back(Resource::_unlitVertexShader);
        Resource::_unlitShaders.push_back(Resource::_unlitFragmentShader);

        GLint width, height;
        glfwGetWindowSize(window, &width, &height);

        /* Temp Quad stuff */

        GL(glGenFramebuffers(1, &_fbo));
        GL(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));

        // generate texture
        GL(glGenTextures(1, &_tcb));
        GL(glBindTexture(GL_TEXTURE_2D, _tcb));
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GL(glBindTexture(GL_TEXTURE_2D, 0));

        // attatch to fbo
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tcb, 0);

        // generate render buffer
        GL(glGenRenderbuffers(1, &_rbo));
        GL(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
        GL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));

        // safe to unbind after memory is allocated
        GL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

        // attatch rbo to the depth and stencil attatchment of the fbo
        GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo));

        // ensure framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            LOG_ERROR("Frame buffer not complete!");

        _quad =
        {
            // first triangle
             20.0f,  20.0f, 0.0f,   1.0f,  1.0f, 0.0f, // top right
             20.0f,  20.0f, 0.0f,   1.0f,  1.0f, 0.0f, // top right
             20.0f, -20.0f, 0.0f,   1.0f,  0.0f, 0.0f, // bottom right
            -20.0f,  20.0f, 0.0f,   0.0f,  1.0f, 0.0f, // top left 
            // second triangle
             20.0f, -20.0f, 0.0f,   1.0f,  0.0f, 0.0f, // bottom right
            -20.0f, -20.0f, 0.0f,   0.0f,  0.0f, 0.0f, // bottom left
            -20.0f,  20.0f, 0.0f,   0.0f,  1.0f, 0.0f  // top left
        };

        // vertex buffer layout
        _vbl.Push<float>(3);       // pos
        _vbl.Push<float>(3);       // tex coord

        // create vao and vbo for quad
        _vao = VertexArray::Create();
        _vbo = VertexBuffer::Create(_quad.data(), _quad.size() * sizeof(float));
        _vao->SetVertexBuffer(_vbo, _vbl);

        // rebind GLFW's FrameBuffer
        GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        
    }

	void Renderer::DrawRequest(Ivy::Ref<Ivy::StaticMesh> object)
	{
        _staticMeshDrawRequests[object->GetMeshPath()].push_back(object);
	}

    void Renderer::SetSceneTranslation(const cy::Vec3f& translation)
    {
        _sceneTranslation = cy::Matrix4f::Translation(translation);
    }

    void Renderer::SetSceneRotation(float angleX, float angleY, float angleZ)
    {
        _sceneRotation = cy::Matrix4f::RotationXYZ(angleX, angleY, angleZ);
    }

	void Renderer::ProcessRequests(GLFWwindow* window)
	{
        glEnable(GL_DEPTH_TEST);

        GLint width, height;
        glfwGetWindowSize(window, &width, &height);

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
                    Shader::Bind(Resource::_litShaders);
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

                    /* Begin Project 5 Render buff impl (temporary)*/

                    GL(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));

                    // clear frame buffer
                    glClearColor(0.7f, 0.7f, 0.7f, 1.0f); // use a different clear color so the background stands out
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glEnable(GL_DEPTH_TEST);

                    // execute teapot draw call
                    GL(glDrawArrays(GL_TRIANGLES, 0, resourceMeta->BufferCount));
                    Resource::UnbindStaticMesh(requestInstance);


                    glBindFramebuffer(GL_FRAMEBUFFER, 0); // bind default on-screen buffer

                    // === Draw quad ===
                    glDisable(GL_DEPTH_TEST);

                    _vbo->Bind();
                    _vao->Bind();

                    Shader::Bind(Resource::_unlitShaders);
                    GLuint mLocu = glGetUniformLocation(Shader::GetActiveProgram(), "model");
                    GLuint vLocu = glGetUniformLocation(Shader::GetActiveProgram(), "view");
                    GLuint pLocu = glGetUniformLocation(Shader::GetActiveProgram(), "projection");
                    GL(glUniformMatrix4fv(mLocu, 1, GL_FALSE, _model.cell));
                    GL(glUniformMatrix4fv(vLocu, 1, GL_FALSE, _view.cell));
                    GL(glUniformMatrix4fv(pLocu, 1, GL_FALSE, _projection.cell));

                    // bind quad vbo and vao, then bind texture from off-screen frame buffer
                    
                    GL(glBindTexture(GL_TEXTURE_2D, _tcb));
                    GL(glGenerateMipmap(GL_TEXTURE_2D));

                    // draw quad
                    GL(glDrawArrays(GL_TRIANGLES, 1, 6));
                }
            }
        }
	}
}