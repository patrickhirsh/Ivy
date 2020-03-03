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
    Renderer::Renderer()
    {
        // Hardcoded Shader Binding... TODO: abstract this.
        Resource::_vertexShader = Shader::Create(GL_VERTEX_SHADER, "shader\\PBRVertex.shader");
        Resource::_fragmentShader = Shader::Create(GL_FRAGMENT_SHADER, "shader\\PBRFragment.shader");
        Resource::_vertexShaderCM = Shader::Create(GL_VERTEX_SHADER, "shader\\DefaultUnlitVertexCM.shader");
        Resource::_fragmentShaderCM = Shader::Create(GL_FRAGMENT_SHADER, "shader\\DefaultUnlitFragmentCM.shader");
        Resource::_shaders.push_back(Resource::_vertexShader);
        Resource::_shaders.push_back(Resource::_fragmentShader);
        Resource::_shadersCM.push_back(Resource::_vertexShaderCM);
        Resource::_shadersCM.push_back(Resource::_fragmentShaderCM);
    }

    void Renderer::Update(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs)
    {
        glDisable(GL_DEPTH_TEST);
        auto windowDimensions = window->GetDimensions();

        for (auto entity : Entities)
        {
            // required components
            Ivy::Transform& transform =     ecs->GetComponent<Ivy::Transform>(entity);
            Ivy::Mesh& mesh =               ecs->GetComponent<Ivy::Mesh>(entity);
            
            // optional material
            bool hasMaterial = false;
            auto dummyMat = Ivy::Material();    // TODO: not this...
            Ivy::Material& material = (hasMaterial = ecs->HasComponent<Ivy::Material>(entity)) ?
                ecs->GetComponent<Ivy::Material>(entity) : 
                dummyMat;
            
            // optional cubemap
            bool hasCubemap = false;
            auto dummyCube = Ivy::Cubemap();    // TODO: not this...
            Ivy::Cubemap& cubemap = (hasCubemap = ecs->HasComponent<Ivy::Cubemap>(entity)) ?
                ecs->GetComponent<Ivy::Cubemap>(entity) :
                dummyCube;

            if (!mesh.Loaded) { LoadMesh(mesh); }
            if (mesh.Loaded)
            {
                BindVBO(mesh.VBO);
                BindVAO(mesh.VAO);

                glEnable(GL_DEPTH_TEST);
                GL(glDepthMask(GL_TRUE));

                if (hasCubemap)
                {
                    Shader::Bind(Resource::_shadersCM);
                    if (!cubemap.Loaded) { LoadCubemap(cubemap); }
                    if (!cubemap.Loaded) { LOG_ERROR("Couldn't load cubemap!"); }
                    BindCubemap(cubemap);
                }
                else if (hasMaterial)
                {
                    Shader::Bind(Resource::_shaders);
                    if (!material.Loaded) { LoadMaterial(material); }
                    if (!material.Loaded) { LOG_ERROR("Couldn't load material!"); }
                    BindMaterial(material);
                }
                if (!hasMaterial && !hasCubemap)
                {
                    LOG_ERROR("No material provided! (TODO: Add default materials...)");
                }

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
                auto projection = cy::Matrix4f::Perspective(1.0f, (std::get<0>(windowDimensions) / std::get<1>(windowDimensions)), 1, 10000);

                // normal transformation
                cy::Matrix4f NormalTransform = (meshTranslation * meshRotation) * model;
                NormalTransform.Invert();
                NormalTransform.Transpose();

                // shader matrix uniforms
                GLuint mLoc = glGetUniformLocation(Shader::GetActiveProgram(), "Model");
                GLuint vLoc = glGetUniformLocation(Shader::GetActiveProgram(), "View");
                GLuint pLoc = glGetUniformLocation(Shader::GetActiveProgram(), "Projection");
                GLuint nLoc = glGetUniformLocation(Shader::GetActiveProgram(), "NormalTransform");
                GL(glUniformMatrix4fv(mLoc, 1, GL_FALSE, model.cell));
                GL(glUniformMatrix4fv(vLoc, 1, GL_FALSE, view.cell));
                GL(glUniformMatrix4fv(pLoc, 1, GL_FALSE, projection.cell));
                GL(glUniformMatrix4fv(nLoc, 1, GL_FALSE, NormalTransform.cell));

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

    void Renderer::BindMaterial(Ivy::Material& material)
    {
        GL(glUniform1i(glGetUniformLocation(Shader::GetActiveProgram(), "AlbedoMap"), 0));
        GL(glUniform1i(glGetUniformLocation(Shader::GetActiveProgram(), "NormalMap"), 1));
        GL(glUniform1i(glGetUniformLocation(Shader::GetActiveProgram(), "MetallicMap"), 2));
        GL(glUniform1i(glGetUniformLocation(Shader::GetActiveProgram(), "RoughnessMap"), 3));

        GL(glActiveTexture(GL_TEXTURE0 + 0));
        GL(glBindTexture(GL_TEXTURE_2D, material.AlbetoTBO));
        GL(glActiveTexture(GL_TEXTURE0 + 1));
        GL(glBindTexture(GL_TEXTURE_2D, material.NormalTBO));
        GL(glActiveTexture(GL_TEXTURE0 + 2));
        GL(glBindTexture(GL_TEXTURE_2D, material.MetallicTBO));
        GL(glActiveTexture(GL_TEXTURE0 + 3));
        GL(glBindTexture(GL_TEXTURE_2D, material.RoughnessTBO));

        GL(glDepthMask(GL_TRUE));
    }

    void Renderer::BindCubemap(Ivy::Cubemap& cubemap)
    {
        GL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.TBO));
        GL(glDepthMask(GL_FALSE));

        // need these every bind?
        for (GLuint i = 0; i < cubemap.TextureData.size(); i++)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,     // offset to correct cubemap enum
                0, GL_RGB, cubemap.TextureWidth, cubemap.TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, cubemap.TextureData[i]);
        }
    }

    void Renderer::UnbindAll()
    {
        GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL(glBindVertexArray(0));
        GL(glDisableClientState(GL_VERTEX_ARRAY));
        GL(glBindTexture(GL_TEXTURE_2D, 0));
        // TODO: unbind all maps
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

        // TODO: REMOVE THIS HACK and figure out why a draw index of 0 causes problems...
        //for (int i = 0; i < VBL.GetStride(); i++) { buffer.push_back(0.0f); }

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
   
        mesh.Loaded = true;
    }

    void Renderer::LoadMaterial(Ivy::Material& material)
    {
        LoadSampler2D(material.AlbetoTBO, material.AlbedoPath, 0, material.Loaded);
        LoadSampler2D(material.NormalTBO, material.NormalPath, 1, material.Loaded);
        LoadSampler2D(material.MetallicTBO, material.MetallicPath, 2, material.Loaded);
        LoadSampler2D(material.RoughnessTBO, material.RoughnessPath, 3, material.Loaded);
    }

    void Renderer::LoadSampler2D(GLuint& TBO, std::string path, int textureIndex, bool& loaded)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load((GetResourceDirectory() + path).c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
        GL(glGenTextures(1, &TBO));
        GL(glActiveTexture(GL_TEXTURE0 + textureIndex));
        GL(glBindTexture(GL_TEXTURE_2D, TBO));
        GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
        //stbi_image_free(data);
        loaded = true;
    }

    void Renderer::LoadCubemap(Ivy::Cubemap& cubemap)
    {
        GL(glGenTextures(1, &cubemap.TBO));
        GL(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.TBO));

        // TODO: Check for mismatched texture sizes...
        int width, height, nrChannels;
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathPosX).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathNegX).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathPosY).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathNegY).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathPosZ).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureData.push_back(stbi_load((GetResourceDirectory() + cubemap.SourceTexturePathNegZ).c_str(), &width, &height, &nrChannels, 0));
        cubemap.TextureWidth = width;
        cubemap.TextureHeight = height;

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        cubemap.Loaded = true;
    }
}