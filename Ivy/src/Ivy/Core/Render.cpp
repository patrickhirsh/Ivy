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
            Ivy::Ref<cy::TriMesh> mesh = Resource::GetOBJResource(request->_objPath);
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