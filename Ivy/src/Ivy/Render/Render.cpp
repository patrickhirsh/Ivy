#include "IvyPCH.h"
#include "Render.h"

namespace _Ivy
{
	void Render::DrawRequest(Ivy::Ref<Ivy::Object> object)
	{
        Resource::AddOBJResource(object->_objPath);
		_drawRequests.push_back(object);
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
        for (auto request : _drawRequests)
        {
            // Hardcoded Teapot Resources...
            if (Resource::_vertices.size() == 0)
            {
                Ivy::Ref<cy::TriMesh> mesh = Resource::GetOBJResource(request->_objPath);
                for (int i = 0; i < mesh->NV(); i++)
                {
                    Resource::_vertices.push_back(mesh->V(i).elem[0]);
                    Resource::_vertices.push_back(mesh->V(i).elem[1]);
                    Resource::_vertices.push_back(mesh->V(i).elem[2]);
                }
                for (int i = 0; i < mesh->NF(); i++)
                {
                    Resource::_indeces.push_back(mesh->F(i).v[0]);
                    Resource::_indeces.push_back(mesh->F(i).v[1]);
                    Resource::_indeces.push_back(mesh->F(i).v[2]);
                }
                
                Resource::_va = VertexArray::Create();
                Resource::_vb = VertexBuffer::Create(Resource::_vertices.data(), Resource::_vertices.size() * sizeof(float));
                Resource::_vbLayout.Push<float>(3);
                Resource::_va->SetVertexBuffer(Resource::_vb, Resource::_vbLayout);
                
                Resource::_ib = IndexBuffer::Create(Resource::_indeces.data(), Resource::_indeces.size());
                Resource::_ib->Bind();

                Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\simple_vertex.shader");
                Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\simple_fragment.shader");
                std::vector<Ivy::Ref<Shader>> shaders;
                shaders.push_back(Resource::_vertexShader);
                shaders.push_back(Resource::_fragmentShader);
                Shader::Bind(shaders);
            }

            GLint width, height;
            glfwGetWindowSize(window, &width, &height);

            // scene transformations
            _projection = cy::Matrix4f::Perspective(1.0f, width / height, 0, 10000);
            _model = cy::Matrix4f::Scale(1);
            cy::Matrix4f MVP = _projection * (_sceneTranslation * _sceneRotation) * _model;
            GLuint mvpLoc = glGetUniformLocation(Shader::GetActiveProgram(), "MVP");
            glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, MVP.cell);

            // execute draw call
            glDrawElements(GL_TRIANGLES, Resource::_indeces.size(), GL_UNSIGNED_INT, 0);
        }
	}
}