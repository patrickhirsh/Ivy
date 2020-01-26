#include "IvyPCH.h"
#include "Render.h"

namespace _Ivy
{
	void Render::DrawRequest(Ivy::Ref<Ivy::Object> object)
	{
        Resource::AddOBJResource(object->_objPath);
		_drawRequests.push_back(object);
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
            }

            // execute draw call
            glDrawElements(GL_TRIANGLES, Resource::_indeces.size(), GL_UNSIGNED_INT, 0);
        }
	}
}