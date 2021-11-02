#pragma once

#include <map>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/quaternion.hpp>
#include "Mesh.h"
#include "Shader.h"

namespace Clumsy 
{
	class Model 
    {
	public:
        std::vector<Texture> textures_loaded;	
        std::string directory;
        bool gammaCorrection;

        static const unsigned int MAX_BONES = 100;
		Model();
		~Model();
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	
        void initShaders(Shader shader);
        void loadModel(const std::string& path);
        void Draw(Shader shader);
        void Draw2(Shader shader);
		void Draw3(Shader shader, unsigned int amount);
        void showNodeName(aiNode* node);

        glm::mat4 aiToGlm(aiMatrix4x4 ai_matr);
        aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend); // super super n lerp =)
		bool hasBones = false;
		std::vector<Mesh> meshes; // one mesh in one object
    private:
		
		bool init = false;
		bool init2 = false;
		float m_LastFrameTime = 0.0f;
		Assimp::Importer import;
		const aiScene* scene;
		

		std::map<std::string, unsigned int> m_bone_mapping; // maps a bone name and their index
		unsigned int m_num_bones = 0;
		std::vector<BoneMatrix> m_bone_matrices;
		aiMatrix4x4 m_global_inverse_transform;

		unsigned int m_bone_location[MAX_BONES];
		float ticks_per_second = 0.0f;

		std::vector<Texture> LoadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string type_name);


		unsigned int findPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		unsigned int findRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		unsigned int findScaling(float p_animation_time, const aiNodeAnim* p_node_anim);
		const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const std::string p_node_name);
		// calculate transform matrix
		aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim);
		aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim);

		void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform);
		void boneTransform(double time_in_sec, std::vector<aiMatrix4x4>& transforms);

		// rotate Head
		glm::quat rotate_head_xz = glm::quat(cos(glm::radians(0.0f)), sin(glm::radians(0.0f)) * glm::vec3(1.0f, 0.0f, 0.0f));
	};
}