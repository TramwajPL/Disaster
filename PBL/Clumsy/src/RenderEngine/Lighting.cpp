#include "Lighting.h"
#include "../Core/CoreEngine.h"

namespace Clumsy {

	void BaseLight::AddToEngine(CoreEngine* engine) const
	{
		RenderEngine::GetInstance()->AddLights(*this);
	}
}