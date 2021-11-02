#include "../pch.h"

#include "ParticleSystem.h"
#include "../Core/Transform.h"

namespace Clumsy {
	ParticleSystem::ParticleSystem(Shader* shader, TextureClass texture) : m_Shader(shader), m_Texture(texture)
	{
		Init();
	}

	ParticleSystem::~ParticleSystem()
	{
		glDeleteBuffers(1, &particles_color_buffer);
		glDeleteBuffers(1, &particles_position_buffer);
		glDeleteBuffers(1, &billboard_vertex_buffer);
		glDeleteVertexArrays(1, &VertexArrayID);
		delete[] g_particule_position_size_data;
		delete[] g_particule_color_data;
	}

	int ParticleSystem::FindUnusedParticle() {

		for (int i = LastUsedParticle; i < MaxParticles; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				return i;
			}
		}

		for (int i = 0; i < LastUsedParticle; i++) {
			if (ParticlesContainer[i].life < 0) {
				LastUsedParticle = i;
				return i;
			}
		}

		return 0;
	}


	void ParticleSystem::SortParticles() {
		std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
	}

	void ParticleSystem::Init()
	{
		g_particule_position_size_data = new GLfloat[MaxParticles * 4];
		g_particule_color_data = new GLubyte[MaxParticles * 4];

		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		for (int i = 0; i < MaxParticles; i++) {
			ParticlesContainer[i].life = -1.0f;
			ParticlesContainer[i].cameradistance = -1.0f;
		}

		static const GLfloat g_vertex_buffer_data[] = {
			 -0.5f, -0.5f, 0.0f,
			  0.5f, -0.5f, 0.0f,
			 -0.5f,  0.5f, 0.0f,
			  0.5f,  0.5f, 0.0f,
		};

		glGenBuffers(1, &billboard_vertex_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &particles_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);


		glGenBuffers(1, &particles_color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);

		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	}

	void ParticleSystem::Render(glm::mat4 view, glm::mat4 projection)
	{
		glBindVertexArray(VertexArrayID);
		glm::mat4 ViewProjectionMatrix = projection * view;
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);


		glEnable(GL_BLEND);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		m_Shader->use();

		glActiveTexture(GL_TEXTURE0);
		this->m_Texture.Bind();

		m_Shader->setVec3("CameraRight_worldspace", glm::vec3(view[0][0], view[1][0], view[2][0]));
		m_Shader->setVec3("CameraUp_worldspace", glm::vec3(view[0][1], view[1][1], view[2][1]));
		m_Shader->setMat4("VP", ViewProjectionMatrix);
		m_Shader->setInt("myTextureSampler", 0);


		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
		glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0
		);


		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
		glVertexAttribPointer(
			2,
			4,
			GL_UNSIGNED_BYTE,
			GL_TRUE,
			0,
			(void*)0
		);


		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticleSystem::Update(GLfloat delta, glm::mat4 view, glm::mat4 projection)
	{
		glm::vec3 CameraPosition(glm::inverse(view)[3]);


		ParticlesCount = 0;
		for (int i = 0; i < MaxParticles; i++) {

			Particle& p = ParticlesContainer[i];

			if (p.life > 0.0f) {


				p.life -= delta;
				if (p.life > 0.0f) {
					p.size = std::min(p.life * 1.6f / p.startLife, 1.0f) * p.startSize;

					p.pos += p.speed * (float)delta;
					p.cameradistance = glm::length(p.pos - CameraPosition);



					g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
					g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
					g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;

					g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

					g_particule_color_data[4 * ParticlesCount + 0] = p.r;
					g_particule_color_data[4 * ParticlesCount + 1] = p.g;
					g_particule_color_data[4 * ParticlesCount + 2] = p.b;
					g_particule_color_data[4 * ParticlesCount + 3] = p.a;

				}
				else {

					p.cameradistance = -1.0f;
				}

				ParticlesCount++;

			}
		}

		SortParticles();
	}

	void ParticleSystem::GenerateNewParticles(GLfloat delta, Transform& position)
	{
		int newparticles = 4;

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 1.7f;
			ParticlesContainer[particleIndex].startLife = ParticlesContainer[particleIndex].life;
			ParticlesContainer[particleIndex].pos = position.GetPos() + glm::vec3((rand() % 2000 - 1000.0f) / 20000.0f, 0.0f, (rand() % 2000 - 1000.0f) / 50000.0f);

			float spread = 1.5f;
			glm::vec3 maindir = glm::vec3(0.0f, 0.8f, 0.0f);
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);

			ParticlesContainer[particleIndex].speed = maindir;

			GLfloat rColor = 0.5 + ((rand() % 100) / 200.0f);
			rColor *= 255.0f;

			ParticlesContainer[particleIndex].r = rColor;
			ParticlesContainer[particleIndex].g = rColor;
			ParticlesContainer[particleIndex].b = rColor;
			ParticlesContainer[particleIndex].a = 255.0f;

			ParticlesContainer[particleIndex].size = (rand() % 1000) / 2000.0f + 0.0f;
			ParticlesContainer[particleIndex].startSize = ParticlesContainer[particleIndex].size;

		}
	}

	void ParticleSystem::GenerateTileIllumination(GLfloat delta, Transform& position)
	{
		int newparticles = 1;

		for (int i = 0; i < newparticles; i++) {
			int particleIndex = FindUnusedParticle();
			ParticlesContainer[particleIndex].life = 0.5f;
			ParticlesContainer[particleIndex].startLife = ParticlesContainer[particleIndex].life;
			ParticlesContainer[particleIndex].pos = position.GetPos();

			float spread = 1.0f;
			glm::vec3 maindir = glm::vec3(0.0f, 0.2f, 0.0f);

			ParticlesContainer[particleIndex].speed = maindir;

			GLfloat rColor = 0.5;
			rColor *= 255.0f;

			ParticlesContainer[particleIndex].r = rColor;
			ParticlesContainer[particleIndex].g = rColor;
			ParticlesContainer[particleIndex].b = rColor;
			ParticlesContainer[particleIndex].a = 155.0f;

			ParticlesContainer[particleIndex].size = 0.3f ;
			ParticlesContainer[particleIndex].startSize = ParticlesContainer[particleIndex].size;

		}
	}

}