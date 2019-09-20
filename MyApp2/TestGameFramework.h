#pragma once

#include "GameFramework.h"

class CInput;
class CText;
class CTexture;
class CSprite;
class CPayloadManager;

class TestGameFramework :
	public GameFramework
{
public:
	TestGameFramework();
	virtual ~TestGameFramework();

	virtual bool InitFramework(HWND hWnd, HINSTANCE hInstance);
	virtual void ReleaseFramework();

	virtual void LoadTextures();
	virtual void ReleaseTextures();

	virtual void InitGameData();
	virtual void RealeaseGameData();

	virtual void GameUpdate(UINT& escapecode);
	virtual void GameRender();

	void ResetBlock(int index);
	bool CanBlockPayloadReload(DWORD dwTickCount);
	void Update(float dt);
	void Render();
	LPD3DXSPRITE GetSpriteObject();
	void ReleaseSpriteObject(LPD3DXSPRITE pSprite);

protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_ClearColor;

	LPD3DXSPRITE m_pSpr[2];
	D3DXVECTOR3 m_PCPos;
	D3DXVECTOR3 m_BlockPos[10];
	D3DXVECTOR3 m_BlockDir;

	float PCNormalSpeed;
	float PCPowerUpSpeed;

	float m_BlockSpeed;
	float m_PCSpeed;

	CInput* m_Input;
	CText* m_Text;
	int m_Score;
	int m_level;
	int m_TakeCount;
	CTexture* m_Texture;
	CSprite* m_Sprite[10];
	CPayloadManager* m_PCPayloadManager;
	CPayloadManager* m_BlockPayloadManager;

	bool m_Pause;

	DWORD m_dwPrevTime;
	float m_ItemKeepTime;
};

