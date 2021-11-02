
#include "../pch.h"
#include "Input.h"

namespace Clumsy {

	Input::Input() :
		m_mouseX(0),
		m_mouseY(0)
	{
		memset(m_inputs, 0, NUM_KEYS * sizeof(bool));
		memset(m_downKeys, 0, NUM_KEYS * sizeof(bool));
		memset(m_upKeys, 0, NUM_KEYS * sizeof(bool));

		memset(m_mouseInput, 0, NUM_MOUSEBUTTONS * sizeof(bool));
		memset(m_downMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
		memset(m_upMouse, 0, NUM_MOUSEBUTTONS * sizeof(bool));
	}


}