
#include "../pch.h"

#include <glad/glad.h> 
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Model.h"
#include "../Core/Timestep.h"

namespace Clumsy 
{
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        return textureID;
    }

    Model::Model()
    {
        scene = nullptr;
		
    }

    Model::~Model()
    {
        import.FreeScene();
    }

    void Model::initShaders(Shader shader)
    {
        for (unsigned int i = 0; i < MAX_BONES; i++) // get location all matrices of bones
        {
            std::string name = "bones[" + std::to_string(i) + "]";// name like in shader
            m_bone_location[i] = glGetUniformLocation(shader.ID, name.c_str());
        }
    }

    // draws the model, and thus all its meshes
    void Model::Draw(Shader shader)
    {
		if (init == false)
		{
			for (unsigned int i = 0; i < MAX_BONES; i++) // get location all matrices of bones
			{
				std::string name = "bones[" + std::to_string(i) + "]";// name like in shader
				m_bone_location[i] = glGetUniformLocation(shader.ID, name.c_str());
			}
			init = true;
		}
        glUniform1i(glGetUniformLocation(shader.ID, "hasBones"), true);
        glUniform1i(glGetUniformLocation(shader.ID, "instanced"), true);

        std::vector<aiMatrix4x4> transforms;

        float time = (float)glfwGetTime();
        Timestep timestep = time - m_LastFrameTime;
        m_LastFrameTime = time;
        if (hasBones) {
            boneTransform((double)m_LastFrameTime, transforms);

            for (unsigned int i = 0; i < transforms.size(); i++) // move all matrices for actual model position to shader
            {
                glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
            }
        }

        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader.ID);
        }
    }

    void Model::Draw2(Shader shader)
    {
        std::vector<aiMatrix4x4> transforms;

        glUniform1i(glGetUniformLocation(shader.ID, "hasBones"), false);
        glUniform1i(glGetUniformLocation(shader.ID, "instanced"), false);
        for (int i = 0; i < meshes.size(); i++)
        {
            meshes[i].Draw(shader.ID);
        }
    }

	void Model::Draw3(Shader shader, unsigned int amount)
	{
		std::vector<aiMatrix4x4> transforms;

		glUniform1i(glGetUniformLocation(shader.ID, "hasBones"), false);
		glUniform1i(glGetUniformLocation(shader.ID, "instanced"), true);
		for (int i = 0; i < meshes.size(); i++)
		{
			meshes[i].DrawInstanced(shader.ID, amount);
		}
	}


    void Model::loadModel(const std::string& path)
    {

        // result: a specific transformation will affect a particular vertex with a certain force.

        scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "error assimp : " << import.GetErrorString() << std::endl;
            return;
        }
        m_global_inverse_transform = scene->mRootNode->mTransformation;
        m_global_inverse_transform.Inverse();

        if (scene->mAnimations != NULL) {
            if (scene->mAnimations[0]->mTicksPerSecond != 0.0)
            {
                ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
            }
            else
            {
                ticks_per_second = 25.0f;
            }
        }

        // directoru = container for model.obj and textures and other files
        directory = path.substr(0, path.find_last_of('/'));
        showNodeName(scene->mRootNode);
        processNode(scene->mRootNode, scene);

        if (scene->mAnimations != NULL) {
            for (unsigned int i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
            {
               // std::cout << scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << std::endl;
            }
        }
    }

    void Model::showNodeName(aiNode* node)
    {
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            showNodeName(node->mChildren[i]);
        }
    }
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        Mesh mesh;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++)
        {
            aiMesh* ai_mesh = scene->mMeshes[i];
            mesh = processMesh(ai_mesh, scene);
            meshes.push_back(mesh); //accumulate all meshes in one vector
        }

    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        std::vector<VertexBoneData> bones_id_weights_for_each_vertex;

        vertices.reserve(mesh->mNumVertices);
        indices.reserve(mesh->mNumVertices);

        bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);

        //vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            if (mesh->mNormals != NULL)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            else
            {
                vertex.Normal = glm::vec3();
            }


            // in assimp model can have 8 different texture coordinates
            // we only care about the first set of texture coordinates
            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

        //indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i]; 
            indices.push_back(face.mIndices[0]); 
            indices.push_back(face.mIndices[1]); 
            indices.push_back(face.mIndices[2]);
        }

        //material
        if (mesh->mMaterialIndex >= 0)
        {
            //all pointers created in assimp will be deleted automaticaly when we call import.FreeScene();
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuse_maps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
            bool exist = false;
            for (int i = 0; (i < textures.size()) && (diffuse_maps.size() != 0); i++)
            {
                if (textures[i].path == diffuse_maps[0].path) 
                {
                    exist = true;
                }
            }
            if (!exist && diffuse_maps.size() != 0) textures.push_back(diffuse_maps[0]); 

            std::vector<Texture> specular_maps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
            exist = false;
            for (int i = 0; (i < textures.size()) && (specular_maps.size() != 0); i++)
            {
                if (textures[i].path == specular_maps[0].path) 
                {
                    exist = true;
                }
            }
            if (!exist && specular_maps.size() != 0) textures.push_back(specular_maps[0]); 

        }

        // load bones
        if (mesh->HasBones()) {
            hasBones = true;
            for (unsigned int i = 0; i < mesh->mNumBones; i++)
            {
                unsigned int bone_index = 0;
                std::string bone_name(mesh->mBones[i]->mName.data);

                if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
                {
                    // Allocate an index for a new bone
                    bone_index = m_num_bones;
                    m_num_bones++;
                    BoneMatrix bi;
                    m_bone_matrices.push_back(bi);
                    m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
                    m_bone_mapping[bone_name] = bone_index;
                }
                else
                {
                    bone_index = m_bone_mapping[bone_name];
                }

                for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
                {
                    unsigned int vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
                    float weight = mesh->mBones[i]->mWeights[j].mWeight;
                    bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight);

                }
            }
        }
        else
            hasBones = false;

        return Mesh(vertices, indices, textures, bones_id_weights_for_each_vertex);
    }

    std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string type_name)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString ai_str;
            mat->GetTexture(type, i, &ai_str);

            std::string filename = std::string(ai_str.C_Str());
            filename = directory + '/' + filename;

            Texture texture;
            texture.id = Clumsy::TextureFromFile(ai_str.C_Str(), this->directory);; // return prepaired openGL texture
            texture.type = type_name;
            texture.path = ai_str;
            textures.push_back(texture);
        }
        return textures;
    }

    unsigned int Model::findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
       
        for (unsigned int i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
        {
            if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime) 
            {
                return i; 
            }
        }

        assert(0);
        return 0;
    }

    unsigned int Model::findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
        
        for (unsigned int i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
        {
            if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime) 
            {
                return i; 
            }
        }

        assert(0);
        return 0;
    }

    unsigned int Model::findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
        
        for (unsigned int i = 0; i < p_node_anim->mNumScalingKeys - 1; i++) 
        {
            if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime) 
            {
                return i; 
            }
        }

        assert(0);
        return 0;
    }

    aiVector3D Model::calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
        if (p_node_anim->mNumPositionKeys == 1) 
        {
            return p_node_anim->mPositionKeys[0].mValue;
        }

        unsigned int position_index = findPosition(p_animation_time, p_node_anim);
        unsigned int next_position_index = position_index + 1;
        assert(next_position_index < p_node_anim->mNumPositionKeys);
       
        float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
        
        float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
        assert(factor >= 0.0f && factor <= 1.0f);
        aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
        aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
        aiVector3D delta = end - start;

        return start + factor * delta;
    }

    aiQuaternion Model::calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
        if (p_node_anim->mNumRotationKeys == 1) 
        {
            return p_node_anim->mRotationKeys[0].mValue;
        }

        unsigned int rotation_index = findRotation(p_animation_time, p_node_anim); 
        unsigned int next_rotation_index = rotation_index + 1; 
        assert(next_rotation_index < p_node_anim->mNumRotationKeys);
       
        float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
       
        float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;


        assert(factor >= 0.0f && factor <= 1.0f);
        aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
        aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

        return nlerp(start_quat, end_quat, factor);
    }

    aiVector3D Model::calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
    {
        if (p_node_anim->mNumScalingKeys == 1) 
        {
            return p_node_anim->mScalingKeys[0].mValue;
        }

        unsigned int scaling_index = findScaling(p_animation_time, p_node_anim); 
        unsigned int next_scaling_index = scaling_index + 1; 
        assert(next_scaling_index < p_node_anim->mNumScalingKeys);
     
        float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
        
        float  factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
        assert(factor >= 0.0f && factor <= 1.0f);
        aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
        aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
        aiVector3D delta = end - start;

        return start + factor * delta;
    }

    const aiNodeAnim* Model::findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name)
    {
        for (unsigned int i = 0; i < p_animation->mNumChannels; i++)
        {
            const aiNodeAnim* node_anim = p_animation->mChannels[i]; 
            if (std::string(node_anim->mNodeName.data) == p_node_name)
            {
                return node_anim;
            }
        }

        return nullptr;
    }
    // start from RootNode
    void Model::readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
    {

        std::string node_name(p_node->mName.data);

        const aiAnimation* animation = scene->mAnimations[0];
        aiMatrix4x4 node_transform = p_node->mTransformation;

        const aiNodeAnim* node_anim = findNodeAnim(animation, node_name); 

        if (node_anim)
        {

            //scaling
            //aiVector3D scaling_vector = node_anim->mScalingKeys[2].mValue;
            aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
            aiMatrix4x4 scaling_matr;
            aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

            //rotation
            //aiQuaternion rotate_quat = node_anim->mRotationKeys[2].mValue;
            aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
            aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

            //translation
            //aiVector3D translate_vector = node_anim->mPositionKeys[2].mValue;
            aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
            aiMatrix4x4 translate_matr;
            aiMatrix4x4::Translation(translate_vector, translate_matr);

            if (std::string(node_anim->mNodeName.data) == std::string("Head"))
            {
                aiQuaternion rotate_head = aiQuaternion(rotate_head_xz.w, rotate_head_xz.x, rotate_head_xz.y, rotate_head_xz.z);

                node_transform = translate_matr * (rotate_matr * aiMatrix4x4(rotate_head.GetMatrix())) * scaling_matr;
            }
            else
            {
                node_transform = translate_matr * rotate_matr * scaling_matr;
            }

        }

        aiMatrix4x4 global_transform = parent_transform * node_transform;

       
        if (m_bone_mapping.find(node_name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
        {
            unsigned int bone_index = m_bone_mapping[node_name];
            m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
        }

        for (unsigned int i = 0; i < p_node->mNumChildren; i++)
        {
            readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
        }

    }

    void Model::boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms)
    {
        aiMatrix4x4 identity_matrix; // = mat4(1.0f);

        double time_in_ticks = time_in_sec * ticks_per_second;
        float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration); 

        readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

        transforms.resize(m_num_bones);

        for (unsigned int i = 0; i < m_num_bones; i++)
        {
            transforms[i] = m_bone_matrices[i].final_world_transform;
        }
    }

    glm::mat4 Model::aiToGlm(aiMatrix4x4 ai_matr)
    {
        glm::mat4 result;
        result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
        result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
        result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
        result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;
        //cout << endl;

        return result;
    }

    aiQuaternion Model::nlerp(aiQuaternion a, aiQuaternion b, float blend)
    {
        //cout << a.w + a.x + a.y + a.z << endl;
        a.Normalize();
        b.Normalize();

        aiQuaternion result;
        float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        float one_minus_blend = 1.0f - blend;

        if (dot_product < 0.0f)
        {
            result.x = a.x * one_minus_blend + blend * -b.x;
            result.y = a.y * one_minus_blend + blend * -b.y;
            result.z = a.z * one_minus_blend + blend * -b.z;
            result.w = a.w * one_minus_blend + blend * -b.w;
        }
        else
        {
            result.x = a.x * one_minus_blend + blend * b.x;
            result.y = a.y * one_minus_blend + blend * b.y;
            result.z = a.z * one_minus_blend + blend * b.z;
            result.w = a.w * one_minus_blend + blend * b.w;
        }

        return result.Normalize();
    }
}