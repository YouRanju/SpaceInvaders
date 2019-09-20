#include "stdafx.h"
#include "Cinput.h"

#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>

CInput::CInput(HWND hwnd, HINSTANCE hInstance)
{
	m_lpDInput = NULL;
	m_lpKeyboard = NULL;

	InitInput(hwnd, hInstance);
}

CInput::~CInput()
{
	ShutdownInput();
}

int CInput::InitInput(HWND hwnd, HINSTANCE hInstance)
{
	try
	{
		auto result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_lpDInput, NULL);
		if (FAILED(result))
			throw 0L;

		result = m_lpDInput->CreateDevice(GUID_SysKeyboard, &m_lpKeyboard, NULL);
		if (FAILED(result))
			throw 1L;

		result = m_lpKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(result))
			throw 0L;
		
		result = m_lpKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(result))
			throw 0L;

		result = m_lpKeyboard->Acquire();
		if (FAILED(result))
			throw 0L;
	}
	catch (DWORD dwError)
	{
		LPTSTR errorMsg = NULL;

		switch (dwError)
		{
		case 0L:
			errorMsg = (LPTSTR)_T("디바이스 생성에 실패 하였습니다.");
			break;
		case 1L:
			errorMsg = (LPTSTR)_T("키보드 생성에 실패 하였습니다.");
			break;
		}

		MessageBox(hwnd, errorMsg, _T("Error"), MB_OK);
	}

	return 0;
}

void CInput::ShutdownInput() {
	if (m_lpKeyboard != NULL) {
		m_lpKeyboard->Unacquire();
		m_lpKeyboard->Release();
		m_lpKeyboard = NULL;
	}

	if (m_lpDInput != NULL) {
		m_lpDInput->Release();
		m_lpDInput = NULL;
	}
}

bool CInput::ReadKeyboard() {
	auto result = m_lpKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_lpKeyboard->Acquire();
		else
			return false;
	}
	return true;
}

bool CInput::IsEscapePressed() {
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
		return true;
	else
		return false;
}

bool CInput::IsPausePressed() {
	static DWORD dwPrevTime = 0;
	DWORD dwCurTime = GetTickCount();
	int dt = dwCurTime - dwPrevTime;

	if ((dt > 500) && m_keyboardState[DIK_F10] & 0x80) {
		m_keyboardState[DIK_F10] = 0;
		dwPrevTime = dwCurTime;
		return true;
	}
		
	else
		return false;
}

bool CInput::IsSpacePressed() {
	static DWORD dwPrevTime = 0;
	DWORD dwCurTime = GetTickCount();
	int dt = dwCurTime - dwPrevTime;

	if ((dt > 200) && m_keyboardState[DIK_SPACE] & 0x80) {
		m_keyboardState[DIK_SPACE] = 0;
		dwPrevTime = dwCurTime;
		return true;
	}

	else
		return false;
}

void CInput::GetArrowDir(D3DXVECTOR3& vecDir) {
	vecDir.x -= IsKeyPressed(DIK_LEFTARROW);
	vecDir.x += IsKeyPressed(DIK_RIGHTARROW);
	//vecDir.y -= IsKeyPressed(DIK_UPARROW);
	//vecDir.y += IsKeyPressed(DIK_DOWNARROW);
	vecDir.z = 0.0f;

	D3DXVec3Normalize(&vecDir, &vecDir);
}