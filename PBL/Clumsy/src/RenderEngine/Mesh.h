#pragma once

#include "assimp\Importer.hpp"
#include "Shader.h"

namespace Clumsy 
{
    #define NUM_BONES_PER_VEREX 4

    struct Vertex 
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture 
    {
        unsigned int id;
        std::string type;
        aiString path;
    };

    struct BoneMatrix
    {
        aiMatrix4x4 offset_matrix;
        aiMatrix4x4 final_world_transform;

    };

    struct VertexBoneData
    {
        unsigned int ids[NUM_BONES_PER_VEREX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
        float weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            memset(ids, 0, sizeof(ids));    // init all values in array = 0
            memset(weights, 0, sizeof(weights));
        }

        void addBoneData(unsigned int bone_id, float weight);
    };

	class Mesh 
    {
    public:
        Mesh(std::vector<Vertex> vertic, std::vector<unsigned int> ind, std::vector<Texture> textur, std::vector<VertexBoneData> bone_id_weights);
        Mesh() {};
		~Mesh();
        void Draw(unsigned int shaders_program);
		void DrawInstanced(unsigned int shaders_program, unsigned int amount);

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        std::vector<VertexBoneData> bones_id_weights_for_each_vertex;
        
        unsigned int VAO;
        unsigned int VBO_vertices;
        unsigned int VBO_bones;
        unsigned int EBO_indices;

    private:
        void setupMesh();
	};
}