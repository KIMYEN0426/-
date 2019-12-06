#pragma once

#include <dinput.h>
class CInput
{
public:
	explicit CInput(HWND hwnd, HINSTANCE hInstance);
	virtual ~CInput();

	int InitInput(HWND hwnd, HINSTANCE hInstance);
	void ShutdownInput();

	bool ReadKeyboard();

	void ClearKeyBuffer()
	{
		ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	}

	bool IsEscapePressed()
	{
		return (m_keyboardState[DIK_ESCAPE] & 0x80) ? true : false;
	}

	bool IsKeyPressed(int key) 
	{
		return (m_keyboardState[key] & 0x80) ? true : false;
	}

	void GetInputDir(D3DXVECTOR3& vDIR);
	bool IsSpaceKeyPressed();

protected:
	LPDIRECTINPUT8 m_lpDInput;
	LPDIRECTINPUTDEVICE8 m_lpKeyboard;
	unsigned char m_keyboardState[256];
};

