
#include "../pch.h"

#include <glad/glad.h>

#include "RenderUtil.h"

namespace Clumsy 
{
	void RenderUtil::ClearScreen()
	{
		glClear(GL_COLOR_BUFFER_BIT || GL_DEPTH_BUFFER_BIT);
	}

	void RenderUtil::InitGraphics()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_FRAMEBUFFER_SRGB);
	}
}