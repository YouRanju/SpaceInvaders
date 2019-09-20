#pragma once

#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

class CInput
{
public:
	explicit CInput(HWND hwnd, HINSTANCE hInstance);
	//explicit :  �ڵ� �� ��ȯ�� �ǵ����� ���� ������ ���, ���׸� �߱��� ���� �ֱ� ������ explicitŰ���带 �Ἥ �����Ϸ��� �Ķ���� �� ��ȯ�� ���� �����ڸ� ȣ���ϴ� ���� ���� ���׸� ����
	virtual ~CInput();

protected:
	LPDIRECTINPUT8 m_lpDInput;
	LPDIRECTINPUTDEVICE8 m_lpKeyboard;
	unsigned char m_keyboardState[256];

public:
	int InitInput(HWND hwnd, HINSTANCE hInstance);
	void ShutdownInput();
	bool ReadKeyboard();
	bool IsEscapePressed();
	bool IsPausePressed();
	bool IsSpacePressed();
	void GetArrowDir(D3DXVECTOR3& vecDir);
	
	float IsKeyPressed(int key) {
		return (m_keyboardState[key] & 0x80) ? 1.0f : 0.0f;
	}

	void ClearKeyBuffer() {
		ZeroMemory(m_keyboardState, sizeof(m_keyboardState));
	}
};

