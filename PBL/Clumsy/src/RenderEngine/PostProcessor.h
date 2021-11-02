#pragma once

#include <glad\glad.h>

#include "Shader.h"
#include "TextureClass.h"

namespace Clumsy
{
	class PostProcessor
	{
	public:
		Shader m_PostProcessingShader;
		TextureClass m_Texture, m_Texture2;
		GLuint m_Width, m_Height;
		GLboolean m_Grey, m_Shake;

		PostProcessor(Shader shader, GLuint width, GLuint height);
		~PostProcessor() {}
		void BeginRender();
		void EndRender();
		void Render(GLfloat time);

		GLuint MSFBO, FBO, DepthFBO;

	private:
		GLuint RBO, DepthRBO;
		GLuint VAO;

		void initRenderData();
	};
}