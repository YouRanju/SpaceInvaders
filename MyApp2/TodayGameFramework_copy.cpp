#include "stdafx.h"

#include <tchar.h>
#include <time.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Cinput.h"
#include "CText.h"
#include "CTexture.h"
#include "CSprite.h"
#include "CPayload.h"
#include "CPayloadManager.h"
#include "CGameObject.h"

#include "TodayGameFramework.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

float g_SpawnXPos[10] = { 60.f, 120.f, 180.f, 240.f, 300.f, 360.0f, 420.0f, 480.0f, 540.0f, 600.0f };
float g_SpawnYPos[5] = { -50.f, -100.f, -150.f, -200.f, -250.f };

TodayGameFramework::TodayGameFramework()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_ClearColor = D3DCOLOR_XRGB(0, 0, 255);

	m_PCPos = D3DXVECTOR3(320, 420, 0);
	m_BlockDir = D3DXVECTOR3(1, 0, 0);

	PCNormalSpeed = 200.0f;
	PCPowerUpSpeed = 400.0f;

	m_BlockSpeed = 60.0f;
	m_PCSpeed = PCNormalSpeed;

	m_Pause = false;

	m_dwPrevTime = 0;
	m_ItemKeepTime = 0;

	m_Score = 0;
	m_level = 0;
	m_TakeCount = 0;
}


TodayGameFramework::~TodayGameFramework()
{
}

bool TodayGameFramework::InitFramework(HWND hWnd, HINSTANCE hInstance) {
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL) {
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480;

	if (m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice) == E_FAIL)
	{
		return false;
	}

	srand((unsigned int)time(NULL));

	m_Input = new CInput(hWnd, hInstance);
	m_Text = new CText(m_pD3DDevice, 20, 40);
	m_Texture = new CTexture(m_pD3DDevice);

	return true;
}

void TodayGameFramework::ReleaseFramework() {
	delete m_Texture;
	delete m_Text;
	delete m_Input;

	m_Texture = NULL;
	m_Text = NULL;
	m_Input = NULL;

	if (m_pD3DDevice != NULL) {
		m_pD3DDevice->Release();
	}

	if (m_pD3D != NULL) {
		m_pD3D->Release();
	}

	m_pD3DDevice = NULL;
	m_pD3D = NULL;
}

void TodayGameFramework::LoadTextures() {
	m_Texture->LoadTexture(-1, TEXT("myship.bmp"));
	m_Texture->LoadTexture(-1, TEXT("enemy.bmp"));
	m_Texture->LoadTexture(-1, TEXT("myshoot.bmp"));
	m_Texture->LoadTexture(-1, TEXT("enemyshoot.bmp"));
}

void TodayGameFramework::ReleaseTextures() {

}

void TodayGameFramework::InitGameData() {
	D3DXVECTOR3 center = D3DXVECTOR3(32, 32, 0);
	m_Sprite[0] = new CSprite(m_pD3DDevice, m_Texture->GetTexture(0), center);
	D3DXVECTOR3 point = D3DXVECTOR3(32, 50, 0);

	//m_Player = new CGameObject(m_pD3DDevice, m_Texture->GetTexture(0), center, D3DXVECTOR3(320, 420, 0), 200.f);

	for (int i = 1; i < 16; ++i) {
		m_Sprite[i] = new CSprite(m_pD3DDevice, m_Texture->GetTexture(1), center);
		m_BlockPos[i] = point;
		point += D3DXVECTOR3(100, 0, 0);

		if ((i) % 5 == 0) {
			point = D3DXVECTOR3(32, point.y + 70, 0);
		}
	}

	m_PCPayloadManager = new CPayloadManager(m_pD3DDevice, m_Texture->GetTexture(2), D3DXVECTOR3(20, 20, 0), 400.0f, D3DXVECTOR3(0, -1, 0));
	m_BlockPayloadManager = new CPayloadManager(m_pD3DDevice, m_Texture->GetTexture(3), D3DXVECTOR3(20, 20, 0), 400.0f, D3DXVECTOR3(0, 1, 0));

	m_dwPrevTime = GetTickCount();
}

void TodayGameFramework::RealeaseGameData() {
	delete m_BlockPayloadManager;
	delete m_PCPayloadManager;

	m_BlockPayloadManager = NULL;
	m_PCPayloadManager = NULL;

	for (int i = 0; i < 10; ++i) {
		delete m_Sprite[i];
		m_Sprite[i] = NULL;
	}
}

void TodayGameFramework::GameUpdate(UINT& escapecode) {

	if (m_Input->IsEscapePressed()) {
		escapecode = WM_QUIT;
		return;
	}
	else {
		m_Input->ReadKeyboard();

		if (m_Input->IsPausePressed())
			m_Pause = !m_Pause;

		if (m_Input->IsSpacePressed()) {
			m_PCPayloadManager->onFire(m_PCPos + D3DXVECTOR3(20, 0, 0));
		}

		DWORD dwCurtime = GetTickCount();
		DWORD dwDt = dwCurtime - m_dwPrevTime;
		float fDt = (float)(dwDt) * 0.001f;
		m_dwPrevTime = dwCurtime;

		if (m_Pause == false) {
			Update(fDt);
		}
	}
}

void TodayGameFramework::GameRender() {
	Render();
}

///////////////////////////////////////////////////////////////////////////////

bool TodayGameFramework::CanBlockPayloadReload(DWORD dwTickCount) {
	static DWORD dwPrewTime = 0;
	DWORD dt = GetTickCount() - dwPrewTime;
	if (dt > dwTickCount) {
		dwPrewTime = GetTickCount();
		return true;
	}

	return false;
}

void  TodayGameFramework::Update(float dt) {

	bool flag = false;

	D3DXVECTOR3 pcDir(0.f, 0.f, 0.f);
	m_Input->GetArrowDir(pcDir);
	m_PCPos += pcDir * m_PCSpeed * dt;

	if (m_PCPos.x < 32.0f) {
		m_PCPos.x = 32.0f;
	}

	if (m_PCPos.x > 600.0f) {
		m_PCPos.x = 600.0f;
	}

	for (int i = 1; i < 16; i++) {

		if (m_BlockPos[i].y >= 420) {
			m_BlockDir = D3DXVECTOR3(0, 0, 0);
		}

		m_BlockPos[i] += (m_BlockDir * m_BlockSpeed * dt);

		if (m_BlockPos[i].x >= 608) {
			m_BlockDir = D3DXVECTOR3(-1, 0, 0);
			flag = true;
		}
		else if (m_BlockPos[i].x <= 32) {
			m_BlockDir = D3DXVECTOR3(1, 0, 0);
			flag = true;
		}
	}

	if (flag) {
		for (int i = 1; i < 16; i++) {
			m_BlockPos[i] += D3DXVECTOR3(0, 30, 0);
			flag = false;
		}
	}

	m_PCPayloadManager->Update(dt);
	//m_BlockPayloadManager->Update(dt);

	//for (int i = 1; i < 6; i++) {
	//	if (m_BlockPos[i].y > 0.0f && m_BlockPos[i].y < 400.0f) {
	//		if (CanBlockPayloadReload(1000)) {
	//			D3DXVECTOR3 dir = m_PCPos - m_BlockPos[i];
	//			m_BlockPayloadManager->onFire(m_BlockPos[i], dir);
	//		}
	//	}
	//}

	for (int i = 1; i < 6; i++) {

		bool bCollision = false;
		D3DXVECTOR3 dist = m_PCPos - m_BlockPos[i];

		//if (D3DXVec3Length(&dist) < (32.f + 32.f)) {
		//	if (m_Sprite[i]->IsItem()) {
		//		m_Sprite[0]->SetPTexture(m_Texture->GetTexture(3));
		//		m_PCSpeed = PCPowerUpSpeed;
		//		m_ItemKeepTime = 0.f;
		//	}

		//	m_Score += 200;
		//	m_TakeCount++;
		//	bCollision = true;
		//}

		if (m_PCPayloadManager->IsCollision(m_BlockPos[i], (32.f + 32.f))) {
			m_Score += 50;
			m_TakeCount++;
			bCollision = true;
		}

		/*if (m_BlockPayloadManager->IsCollision(m_PCPos, (50.f + 20.f))) {
		m_Score -= 50;
		}*/

		/*if (m_TakeCount != 0 && (m_TakeCount % 5) == 0 && bCollision == true) {
		m_level++;
		m_BlockSpeed *= 1.5f;

		if (m_level % 7 == 0)
		m_BlockSpeed = 60.0f;

		bCollision = false;
		}*/
	}
}

void TodayGameFramework::Render() {

	if (m_pD3DDevice == NULL) {
		return;
	}

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	D3DXVECTOR3 center = D3DXVECTOR3(32, 32, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		m_Sprite[0]->SetPosition(m_PCPos);
		m_Sprite[0]->Draw();

		for (int i = 1; i < 16; ++i) {
			m_Sprite[i]->SetPosition(m_BlockPos[i]);
			m_Sprite[i]->Draw();
		}

		m_PCPayloadManager->Draw();
		m_BlockPayloadManager->Draw();

		m_pD3DDevice->EndScene();
	}

	TCHAR szScore[100];
	_stprintf_s(szScore, 100, _T("Lv. %d, Score : %d"), m_level + 1, m_Score);
	m_Text->Draw(10, 10, 500, 80, szScore);
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

LPD3DXSPRITE TodayGameFramework::GetSpriteObject() {
	LPD3DXSPRITE pSprite = NULL;

	if (FAILED(D3DXCreateSprite(m_pD3DDevice, &pSprite))) {
		return NULL;
	}
	return pSprite;
}

void TodayGameFramework::ReleaseSpriteObject(LPD3DXSPRITE pSprite) {
	if (pSprite != NULL) {
		pSprite->Release();
	}
}