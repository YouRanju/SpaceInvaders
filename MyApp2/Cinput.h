#pragma once

#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

class CInput
{
public:
	explicit CInput(HWND hwnd, HINSTANCE hInstance);
	//explicit :  자동 형 변환을 의도하지 않은 사용법인 경우, 버그를 야기할 수도 있기 때문에 explicit키워드를 써서 컴파일러가 파라미터 형 변환을 위해 생성자를 호출하는 것을 막아 버그를 방지
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

