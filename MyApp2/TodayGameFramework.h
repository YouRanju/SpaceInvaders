#pragma once

#include "GameFramework.h"

class CInput;
class CText;
class CTexture;
class CSprite;
class CPayloadManager;
class CGameObject;

class TodayGameFramework :
	public GameFramework
{
public:
	TodayGameFramework();
	virtual ~TodayGameFramework();

	virtual bool InitFramework(HWND hWnd, HINSTANCE hInstance);
	virtual void ReleaseFramework();

	virtual void LoadTextures();
	virtual void ReleaseTextures();

	virtual void InitGameData();
	virtual void RealeaseGameData();

	virtual void GameUpdate(UINT& escapecode);
	virtual void GameRender();

	bool CanBlockPayloadReload(DWORD dwTickCount);
	void Update(float dt);

	////////////////////////////////////////////////////////////
	void ProjectileUpdate(float dt);
	void InvaderCollisionUpdate(float dt);
	void PlayerCollisionUpdate(float dt);
	////////////////////////////////////////////////////////////

	void Render();
	LPD3DXSPRITE GetSpriteObject();
	void ReleaseSpriteObject(LPD3DXSPRITE pSprite);

	///////////////////////////////////////////////////////////
	void TitleUpdate(float dt);
	void TitleRender();
	
	void ReadyUpdate(float dt);
	void ReadyRender();
	
	void GameOverUpdate(float dt);
	void GameOverRender();

	void NextLevelUpdate(float dt);
	void NextLevelRender();
	
	void DrawRenderObjects();
	void DrawScores();

protected:
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;
	D3DCOLOR m_ClearColor;

	D3DXVECTOR3 m_PCPos;
	D3DXVECTOR3 m_BlockPos[16];
	D3DXVECTOR3 m_BlockDir;

	float m_BlockSpeed;
	float m_PCSpeed;

	CInput* m_Input;
	CText* m_Text;
	int m_Score;
	int m_level;
	int m_TakeCount;
	CTexture* m_Texture;
	CSprite* m_Sprite[20];
	CPayloadManager* m_PCPayloadManager;
	//CPayloadManager* m_BlockPayloadManager;
	CPayloadManager* m_InvaderPayloadManager;

	bool m_Pause;

	DWORD m_dwPrevTime;
	float m_ItemKeepTime;

	//////////////////////////20181222Ãß°¡///////////////////////////////
	D3DXVECTOR3 m_InvaderDir;
	bool m_bInvaderRightDir;
	
	float m_ScreenWidth;
	float m_ScreenHeight;

	CGameObject* m_Player;
	CGameObject* m_Invader[20];

	bool bfire;

	D3DXVECTOR3 m_ScorePos;
	float m_ScoreLife;
	int thisScore;

	int m_gameMode;
	int m_life;
};

