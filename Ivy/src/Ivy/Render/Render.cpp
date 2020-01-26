#include "IvyPCH.h"
#include "Render.h"

namespace _Ivy
{
    int Render::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            /* ERROR.. */
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            printf("Failed to compile shader! %s", message);

            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    unsigned int Render::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return program;
    }

	void Render::DrawRequest(Ivy::Ref<Ivy::Object> object)
	{
        Resource::AddOBJResource(object->_objPath);
		_drawRequests.push_back(object);
	}

	void Render::ProcessRequests(GLFWwindow* window)
	{
        for (auto request : _drawRequests)
        {
            Ivy::Ref<cy::TriMesh> mesh = Resource::GetOBJResource(request->_objPath);
            
            std::vector<float> vertices;
            for (int i = 0; i < mesh->NF(); i++)
            {
                auto face = mesh->F(i);
                for (int j = 0; j < 3; j++)
                {
                    auto vertex = face.v[j];
                    vertices.push_back(mesh->V(vertex).elem[0]);
                    vertices.push_back(mesh->V(vertex).elem[1]);
                    vertices.push_back(mesh->V(vertex).elem[2]);
                }
            }

            auto vb = VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
            auto va = VertexArray();
            auto layout = VertexBufferLayout();
            layout.Push<float>(3);
            va.SetVertexBuffer(vb, layout);

            // execute draw call
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
	}
}