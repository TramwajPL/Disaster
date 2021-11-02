#pragma once
#include "Shader.h"
#include "TextureClass.h"

namespace Clumsy {
	class Transform;
	const int MaxParticles = 100000;
	struct Particle {
		glm::vec3 pos, speed;
		unsigned char r, g, b, a;
		float size, angle, weight;
		float life;
		float startLife, startSize;
		float cameradistance;

		bool operator<(const Particle& that) const {
			return this->cameradistance > that.cameradistance;
		}
	};

	class ParticleSystem
	{
	public:
		ParticleSystem(Shader* shader, TextureClass texture);

		~ParticleSystem();

		int FindUnusedParticle();

		void SortParticles();

		void Render(glm::mat4 view, glm::mat4 projection);

		void Init();

		void Update(GLfloat delta, glm::mat4 view, glm::mat4 projection);

		void GenerateNewParticles(GLfloat delta, Transform& position);

		void GenerateTileIllumination(GLfloat delta, Transform& position);

	private:
		
		Particle ParticlesContainer[MaxParticles];
		int LastUsedParticle = 0;
		Shader* m_Shader;
		TextureClass m_Texture;
		GLuint VertexArrayID;
		GLuint billboard_vertex_buffer;
		GLuint particles_position_buffer;
		GLuint particles_color_buffer;
		GLfloat* g_particule_position_size_data;
		GLubyte* g_particule_color_data;
		int ParticlesCount;

	};
}