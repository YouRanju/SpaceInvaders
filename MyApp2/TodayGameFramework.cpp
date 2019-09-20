#include "stdafx.h"

#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

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

#define BACK1 

float g_SpawnXPos[10] = { 60.f, 120.f, 180.f, 240.f, 300.f, 360.0f, 420.0f, 480.0f, 540.0f, 600.0f };
float g_SpawnYPos[5] = { -50.f, -100.f, -150.f, -200.f, -250.f };
int g_ifire = 0;

TodayGameFramework::TodayGameFramework()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_ClearColor = D3DCOLOR_XRGB(0, 0, 0);

	m_PCPos = D3DXVECTOR3(320, 420, 0);
	m_BlockDir = D3DXVECTOR3(1, 0 , 0);

	m_Pause = false;
	m_level = 0;
	m_Score = 0;

	m_dwPrevTime = 0;
	m_ItemKeepTime = 0;

	m_gameMode = 1;
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

	RECT rc;
	GetClientRect(hWnd, &rc);

	int ClientWidth = rc.right - rc.left + 1;
	int ClientHeight = rc.bottom - rc.top + 1;
	m_ScreenWidth = (float)ClientWidth;
	m_ScreenHeight = (float)ClientHeight;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = ClientWidth;
	d3dpp.BackBufferHeight = ClientHeight;

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
	m_Text = new CText(m_pD3DDevice, 12, 24);
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

	const D3DXVECTOR3 pcPos = D3DXVECTOR3(m_ScreenWidth * 0.5f, m_ScreenHeight - 64.f, 0);
	const D3DXVECTOR3 center = D3DXVECTOR3(32, 32, 0);
	const D3DXVECTOR3 payloadcenter = D3DXVECTOR3(8, 16, 0);
	const D3DXVECTOR3 pcshootdir = D3DXVECTOR3(0, -1, 0);
	const D3DXVECTOR3 invadershootdir = D3DXVECTOR3(0, 1, 0);
	const float pcSpeed = 200.f;
	const float invaderSpeed = 100.f;
	const float pcshootspeed = 400.f;
	const float invadershootspeed = 200.f;
	
	m_PCSpeed = 200.0f;

	m_TakeCount = 0;
	m_bInvaderRightDir = true;
	bfire = false;

	m_ScoreLife = 0.f;
	thisScore = 0.f;

	m_dwPrevTime = GetTickCount();
	m_InvaderDir = D3DXVECTOR3(1, 0, 0);

	m_Player = new CGameObject(m_pD3DDevice, m_Texture->GetTexture(0), center, pcPos, pcSpeed, 1);

	for (int i = 0; i < 20; ++i) {
		m_Invader[i] = new CGameObject(m_pD3DDevice, m_Texture->GetTexture(1), center
			, D3DXVECTOR3(40.f + 32.f + (float)((i % 5) * 100), 20.f + 32.f + (float)((i / 5) * 64), 0.f)
			, invaderSpeed, m_life);
		m_Invader[i]->SetAlive(true);
	}

	m_PCPayloadManager = new CPayloadManager(m_pD3DDevice, m_Texture->GetTexture(2), payloadcenter
		, pcshootspeed, pcshootdir, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));
	m_InvaderPayloadManager = new CPayloadManager(m_pD3DDevice, m_Texture->GetTexture(3), payloadcenter
		, invadershootspeed, invadershootdir, D3DXVECTOR2(m_ScreenWidth, m_ScreenHeight));

	
}

void TodayGameFramework::RealeaseGameData() {
	delete m_InvaderPayloadManager;
	delete m_PCPayloadManager;

	m_InvaderPayloadManager = NULL;
	m_PCPayloadManager = NULL;

	delete m_Player;
	m_Player = NULL;

	for (int i = 0; i < 20; ++i) {
		delete m_Invader[i];
		m_Invader[i] = NULL;
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

		DWORD dwCurtime = GetTickCount();
		DWORD dwDt = dwCurtime - m_dwPrevTime;
		float fDt = (float)(dwDt) * 0.001f;
		m_dwPrevTime = dwCurtime;

		if (m_Pause == false) {
			switch (m_gameMode) {
			case 1:
				TitleUpdate(fDt);
				break;
			case 2:
				ReadyUpdate(fDt);
				break;
			case 3:
				if (m_Pause == false && m_Input->IsSpacePressed() && !bfire) {
					if (g_ifire >= 2) {
						bfire = true;
						g_ifire = 0;
					}
					m_PCPayloadManager->onFire(m_Player->GetPosition());
					g_ifire++;
				}

				Update(fDt);
				ProjectileUpdate(fDt);
				InvaderCollisionUpdate(fDt);
				PlayerCollisionUpdate(fDt);
				break;
			case 4:
				GameOverUpdate(fDt);
				break;
			case 5:
				NextLevelUpdate(fDt);
				break;
			}
			
		}
	}
}

void TodayGameFramework::GameRender() {
	switch (m_gameMode)
	{
	case 1:
		TitleRender();
		break;
	case 2:
		ReadyRender();
		break;
	case 3:
		//PlaySound(TEXT(BACK1), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Render();
		break;
	case 4:
		GameOverRender();
		break;
	case 5:
		NextLevelRender();
		break;
	}
}

void TodayGameFramework::TitleUpdate(float dt) {
	if (m_Input->IsSpacePressed()) {
		m_Input->ClearKeyBuffer();
		m_gameMode = 2;

		m_life = 1;
		m_level = 0;
		m_Score = 0;
		m_BlockSpeed = 1.2f;
		InitGameData();
	}
}

void TodayGameFramework::TitleRender() {

	if (m_pD3DDevice == NULL)
		return;
	
	//백버퍼 지우기
	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {
		m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)(m_ScreenHeight*0.5f),
			_T("Space Invaders"), D3DCOLOR_XRGB(255, 255, 0));

		m_Text->Draw(0, (int)(m_ScreenHeight*0.5f), (int)m_ScreenWidth,
			(int)(m_ScreenHeight*0.5f), _T("PRESS SPACE KEY TO START"), 
			D3DCOLOR_XRGB(255, 255, 255));

		m_pD3DDevice->EndScene();

	}

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

void TodayGameFramework::ReadyUpdate(float dt) {
	static float fAccTime = 0.f;
	fAccTime += dt;
	if (fAccTime > 1.f) {
		m_gameMode = 3;
		fAccTime = 0.f;
	}
}

void TodayGameFramework::ReadyRender() {
	if (m_pD3DDevice == NULL) {
		return;
	}

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	DrawRenderObjects();

	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("Ready"),
		D3DCOLOR_XRGB(255, 255, 0));

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void TodayGameFramework::GameOverUpdate(float dt) {
	if (m_Input->IsSpacePressed()) {
		m_Input->ClearKeyBuffer();
		m_gameMode = 1;

		RealeaseGameData();
	}
}

void TodayGameFramework::GameOverRender() {
	if (m_pD3DDevice == NULL) {
		return;
	}

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	DrawRenderObjects();

	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("Game Over"),
		D3DCOLOR_XRGB(255, 255, 0));

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void TodayGameFramework::NextLevelUpdate(float dt) {
	if (m_Input->IsSpacePressed()) {
		m_Input->ClearKeyBuffer();
		m_gameMode = 3;
		m_life = 2;

		RealeaseGameData();
		InitGameData();
	}
}

void TodayGameFramework::NextLevelRender() {
	if (m_pD3DDevice == NULL) {
		return;
	}

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	DrawRenderObjects();

	m_Text->Draw(0, 0, (int)m_ScreenWidth, (int)m_ScreenHeight, _T("NEXT LEVEL"),
		D3DCOLOR_XRGB(255, 255, 0));

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
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

void TodayGameFramework::Update(float dt) {

	D3DXVECTOR3 pcDir(0.f, 0.f, 0.f);
	D3DXVECTOR3 pcPos = m_Player->GetPosition();
	m_Input->GetArrowDir(pcDir);

	m_Player->SetDirection(pcDir.x, pcDir.y);
	m_Player->Update(dt);
	m_Player->ArrangePosition(32.f, m_ScreenWidth - 32.f);

	for (int i = 0; i < 20; i++) {
		m_Invader[i]->SetDirection(m_InvaderDir.x, m_InvaderDir.y);
		m_Invader[i]->Update(dt);
	}

	for (int i = 0; i < 20; i++) {
		if (m_Invader[i]->IsTouched(32.f, m_ScreenWidth - 32.f, m_bInvaderRightDir) == true) {
			m_InvaderDir = D3DXVECTOR3((m_bInvaderRightDir) ? m_InvaderDir.x * (-1 * m_BlockSpeed) : m_InvaderDir.x * (-1 * m_BlockSpeed), 0.f, 0.f);
			m_bInvaderRightDir = !m_bInvaderRightDir;

			for (int n = 0; n < 20; ++n) {
				D3DXVECTOR3 pos = m_Invader[n]->GetPosition();
				if (pos.y >= m_ScreenHeight - 122.f && m_Invader[n]->GetAlive()) {
					m_gameMode = 4;
				}
				m_Invader[n]->SetPosition(pos + D3DXVECTOR3(0, 32.f, 0));
			}
			break;
		}
	}
}

void TodayGameFramework::ProjectileUpdate(float dt) {
	m_PCPayloadManager->Update(dt);
	m_InvaderPayloadManager->Update(dt);
}

void TodayGameFramework::InvaderCollisionUpdate(float dt) {

	for (int i = 0; i < 20; i++) {
		if (m_Invader[i]->GetAlive() == true) {
			if (m_PCPayloadManager->IsCollision(m_Invader[i]->GetPosition(), (32.f + 8.f))) {
				if (m_Invader[i]->DecLife()) {
					m_Invader[i]->SetAlive(false);
					m_Score += 100;
					thisScore = 100;
					m_TakeCount++;

					m_ScorePos = m_Invader[i]->GetPosition();
					m_ScoreLife = 0.3f;
				}
				else {
					m_Invader[i]->SetColor(D3DCOLOR_XRGB(255, 0, 0));
				}
			}
		}
	}

	if (m_ScoreLife > 0.0f) {
		m_ScoreLife -= dt;
		m_ScorePos.y -= 80.f * dt;
	}

	if (bfire) {
		D3DXVECTOR3 pos = m_PCPayloadManager->GetPostion();
		if (pos.y <= 0) {
			bfire = false;
		}
	}

	if (m_TakeCount == 20) {
		m_level++;
		m_BlockSpeed += .3f;
		m_gameMode = 5;
	}
}
void TodayGameFramework::PlayerCollisionUpdate(float dt) {
	int aliveidx[20];
	int cnt = 0;
	int randominvader = 0;

	srand((unsigned)time(NULL));

	for (int i = 0; i < 20; i++) {
		if (m_Invader[i]->GetAlive()) {
			aliveidx[cnt++] = i;
		}
	}

	if (cnt > 0) {

		randominvader = rand() % cnt;

		if (CanBlockPayloadReload(1000)) {
			m_InvaderPayloadManager->onFire(m_Invader[aliveidx[randominvader]]->GetPosition());
		}
	}

	if (m_InvaderPayloadManager->IsCollision(m_Player->GetPosition(), (32.f + 8.f))) {
		m_Score -= 100;
		
		///플레이어가 총알에 맞았을 때 점수 애니메이션
		//thisScore = -100;
		//m_ScorePos = m_Player->GetPosition();
		//m_ScoreLife = 0.3f;

		m_gameMode = 4;
	}

}

void TodayGameFramework::Render() {

	if (m_pD3DDevice == NULL) {
		return;
	}

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, m_ClearColor, 1.0f, 0);

	DrawRenderObjects();
	DrawScores();

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

void TodayGameFramework::DrawRenderObjects() {
	if (SUCCEEDED(m_pD3DDevice->BeginScene())) {

		m_Player->Render();

		for (int i = 0; i < 20; ++i) {
			if (m_Invader[i]->GetAlive())
				m_Invader[i]->Render();
		}

		m_PCPayloadManager->Draw();
		m_InvaderPayloadManager->Draw();

		m_pD3DDevice->EndScene();
	}
}

void TodayGameFramework::DrawScores() {
	TCHAR szScore[100];
	_stprintf_s(szScore, 100, _T("Lv. %d, Score : %d"), m_level + 1, m_Score);
	m_Text->DrawLT(10, 10, 500, 80, szScore);

	if (m_ScoreLife > 0.f) {
		TCHAR Score[100];
		_stprintf_s(Score, 100, _T("%d"), thisScore);
		m_Text->Draw((int)(m_ScorePos.x), (int)(m_ScorePos.y - 10.f), 52, 32, Score);
	}
}