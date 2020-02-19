#include "Core/IvyPCH.h"
#include "System/Renderer.h"
#include "World/ECS.h"
#include "Entity/Entity.h"
#include "Resource/Resource.h"

// STB: https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace _Ivy
{
    void Renderer::Update(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs)
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

        for (auto entity : Entities)
        {
            Ivy::Transform& transform =     ecs->GetComponent<Ivy::Transform>(entity);
            Ivy::Mesh& mesh =               ecs->GetComponent<Ivy::Mesh>(entity);

            if (!mesh.Loaded) { LoadMesh(mesh); }
            if (mesh.Loaded)
            {
                BindVBO(mesh.VBO);
                BindVAO(mesh.VAO);
                BindTexture(mesh.Texture, mesh.TextureWidth, mesh.TextureHeight, mesh.TextureData);

                // TODO: Fix scene transformation to properly account for aspect ratio

                // mesh transformation
                auto meshTranslation = cy::Matrix4f::Translation(cy::Vec3f(
                    transform.Position.X,
                    transform.Position.Y,
                    transform.Position.Z));
                auto meshRotation = cy::Matrix4f::RotationXYZ(
                    transform.Rotation.X,
                    transform.Rotation.Y,
                    transform.Rotation.Z);

                // model / view / projection matrices
                auto model = cy::Matrix4f::Scale(1);
                auto view = meshTranslation * meshRotation;
                auto projection = cy::Matrix4f::Perspective(1.0f, (std::get<0>(windowDimensions) / std::get<1>(windowDimensions)), 1, 100);

                // normal transformation
                cy::Matrix4f NTRANS = (meshTranslation * meshRotation) * model;
                NTRANS.Invert();
                NTRANS.Transpose();

                // set shader uniforms
                GLuint mLoc = glGetUniformLocation(Shader::GetActiveProgram(), "model");
                GLuint vLoc = glGetUniformLocation(Shader::GetActiveProgram(), "view");
                GLuint pLoc = glGetUniformLocation(Shader::GetActiveProgram(), "projection");
                GLuint ntransLoc = glGetUniformLocation(Shader::GetActiveProgram(), "NTRANS");
                GL(glUniformMatrix4fv(mLoc, 1, GL_FALSE, model.cell));
                GL(glUniformMatrix4fv(vLoc, 1, GL_FALSE, view.cell));
                GL(glUniformMatrix4fv(pLoc, 1, GL_FALSE, projection.cell));
                GL(glUniformMatrix4fv(ntransLoc, 1, GL_FALSE, NTRANS.cell));

                // execute draw call
                GL(glDrawArrays(GL_TRIANGLES, 0, mesh.VBOSize));

                UnbindAll();
            }
        }
    }

    void Renderer::BindVAO(GLuint VAO)
    {
        GL(glBindVertexArray(VAO));
        GL(glEnableClientState(GL_VERTEX_ARRAY));
    }

    void Renderer::BindVBO(GLuint VBO)
    {
        GL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    }

    void Renderer::BindTexture(GLuint TBO, int TextureWidth, int TextureHeight, unsigned char* TextureData)
    {
        GL(glBindTexture(GL_TEXTURE_2D, TBO));
        // need these every bind?
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TextureWidth, TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TextureData));
        GL(glGenerateMipmap(GL_TEXTURE_2D));
    }

    void Renderer::UnbindAll()
    {
        GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL(glBindVertexArray(0));
        GL(glDisableClientState(GL_VERTEX_ARRAY));
        GL(glBindTexture(GL_TEXTURE_2D, 0));
    }

    void Renderer::LoadMesh(Ivy::Mesh& mesh)
    {
        std::string path = _Ivy::GetResourceDirectory() + mesh.SourceMeshPath;
        cy::TriMesh cymesh = cy::TriMesh();
        if (!cymesh.LoadFromFileObj(path.c_str()))
        {
            LOG_ERROR("failed to load static mesh from path: " << path);
            return;
        }

        // build vb layout based on mesh data
        VertexBufferLayout VBL;
        mesh.HasVertexPositions =       cymesh.NV() > 0;
        mesh.HasVertexNormals =         cymesh.NVN() > 0;
        mesh.HasVertexTextures =        cymesh.NVT() > 0;
        if (mesh.HasVertexPositions)    { VBL.Push<float>(3); } // xp, yp, zp
        if (mesh.HasVertexNormals)      { VBL.Push<float>(3); } // xn, yn, zn
        if (mesh.HasVertexTextures)     { VBL.Push<float>(3); } // xt, yt, zt

        std::vector<float> buffer;

        // for each face...
        for (int face = 0; face < cymesh.NF(); face++)
        {
            // for each vertex...
            for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            {
                // add vertex position coords
                if (mesh.HasVertexPositions)
                {
                    if (cymesh.F(face).v[vertexIndex] >= cymesh.NV())
                    {
                        LOG_ERROR("Couldn't load mesh: " << mesh.SourceMeshPath << " (invalid vertex position data)");
                        ASSERT(false);
                        mesh.Loaded = false;
                        return;
                    }
                    buffer.push_back(cymesh.V(cymesh.F(face).v[vertexIndex])[0]);
                    buffer.push_back(cymesh.V(cymesh.F(face).v[vertexIndex])[1]);
                    buffer.push_back(cymesh.V(cymesh.F(face).v[vertexIndex])[2]);
                }

                // add vertex normal coords
                if (mesh.HasVertexNormals)
                {
                    if (cymesh.FN(face).v[vertexIndex] >= cymesh.NVN())
                    {
                        LOG_ERROR("Couldn't load mesh: " << mesh.SourceMeshPath << " (invalid vertex normal data)");
                        ASSERT(false);
                        mesh.Loaded = false;
                        return;
                    }
                    buffer.push_back(cymesh.VN(cymesh.FN(face).v[vertexIndex])[0]);
                    buffer.push_back(cymesh.VN(cymesh.FN(face).v[vertexIndex])[1]);
                    buffer.push_back(cymesh.VN(cymesh.FN(face).v[vertexIndex])[2]);
                }

                // add vertex texture coords
                if (mesh.HasVertexTextures)
                {
                    if (cymesh.FT(face).v[vertexIndex] >= cymesh.NVT())
                    {
                        LOG_ERROR("Couldn't load mesh: " << mesh.SourceMeshPath << " (invalid vertex texture data)");
                        ASSERT(false);
                        mesh.Loaded = false;
                        return;
                    }
                    buffer.push_back(cymesh.VT(cymesh.FT(face).v[vertexIndex])[0]);
                    buffer.push_back(cymesh.VT(cymesh.FT(face).v[vertexIndex])[1]);
                    buffer.push_back(cymesh.VT(cymesh.FT(face).v[vertexIndex])[2]);
                }
            }
        }

        // create VAO
        GL(glGenVertexArrays(1, &mesh.VAO));

        // create VBO
        GL(glGenBuffers(1, &mesh.VBO));
        GL(glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO));
        GL(glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(float), buffer.data(), GL_DYNAMIC_DRAW));

        // set VBO
        GL(glBindVertexArray(mesh.VAO));
        GL(glEnableClientState(GL_VERTEX_ARRAY));
        GL(glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO));
        const auto& elements = VBL.GetElements();
        unsigned int offset = 0;
        for (unsigned int i = 0; i < elements.size(); i++)
        {
            const auto& element = elements[i];
            GL(glEnableVertexAttribArray(i));
            GL(glVertexAttribPointer(i, element.Count, element.Type, element.Normalized,
                VBL.GetStride(), (const void*)offset));

            offset += element.Count * VertexBufferLayoutElement::SizeOf(element.Type);
        }

        // set VBO size
        mesh.VBOSize = (buffer.size() * sizeof(float)) / VBL.GetStride();

        // hardcoded texture (TODO: not this)
        mesh.TextureData = stbi_load((GetResourceDirectory() + "brick.png").c_str(), &mesh.TextureWidth, &mesh.TextureHeight, &mesh.TextureNRChannels, STBI_rgb_alpha);
        GL(glGenTextures(1, &mesh.Texture));
        GL(glBindTexture(GL_TEXTURE_2D, mesh.Texture));
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mesh.TextureWidth, mesh.TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mesh.TextureData));

        mesh.Loaded = true;
    }
}