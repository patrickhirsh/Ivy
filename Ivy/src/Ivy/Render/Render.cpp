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
            for (int i = 0; i < mesh->NV(); i++)
            {
                vertices.push_back(mesh->V(i).elem[0]);
                vertices.push_back(mesh->V(i).elem[1]);
                vertices.push_back(mesh->V(i).elem[2]);
            }

            // generate and bind vertex buffer
            unsigned int bufferID;
            glGenBuffers(1, &bufferID);
            glBindBuffer(GL_ARRAY_BUFFER, bufferID);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
            
            // define vertex attributes
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

            // execute draw call
            glDrawArrays(GL_TRIANGLES, 0, mesh->NV());
        }

        /* We have a color array and a vertex array */
        //glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_COLOR_ARRAY);
        //glVertexPointer(3, GL_FLOAT, 0, vertices);
        //glColorPointer(3, GL_FLOAT, 0, colors);

        /* Send data : 24 vertices */
        //glDrawArrays(GL_QUADS, 0, 24);

        /* Cleanup states */
        //glDisableClientState(GL_COLOR_ARRAY);
        //glDisableClientState(GL_VERTEX_ARRAY);
	}
}