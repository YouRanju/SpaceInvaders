#include "stdafx.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "CTexture.h"
#include "CSprite.h"

#include "CGameObject.h"


CGameObject::CGameObject() : m_Spr(NULL), m_Dir(0.f, 0.f, 0.f), m_Speed(0), m_Pos(0.f, 0.f, 0.f), m_Center(0.f, 0.f, 0.f)
{
}

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pD3DDevice,
	LPDIRECT3DTEXTURE9 pTexture,
	const D3DXVECTOR3& center,
	const D3DXVECTOR3& position,
	float speed /*= 100.f*/,
	int life /* = 1 */ ) : m_Spr(NULL), m_Dir(0.f, 0.f, 0.f)
{
	InitGameObject(pD3DDevice, pTexture, center, position, speed , life);
}

CGameObject::~CGameObject()
{
	Release();
}

void CGameObject::InitGameObject(LPDIRECT3DDEVICE9 pD3DDevice,
	LPDIRECT3DTEXTURE9 pTexture,
	const D3DXVECTOR3& center,
	const D3DXVECTOR3& position,
	float speed /*= 100.f*/,
	int life)
{
	Release();

	m_Spr = new CSprite(pD3DDevice, pTexture, center);
	m_Speed = speed;
	m_Pos = position;
	m_Center = center;
	m_bAlive = false;

	SetLife(life);
}

void CGameObject::Release()
{
	if (m_Spr != NULL) {
		delete m_Spr;
		m_Spr = NULL;
	}
}

void CGameObject::Update(float dt) 
{
	m_Pos += m_Dir * m_Speed * dt;
}

void CGameObject::Render() {
	m_Spr->SetPosition(m_Pos);
	m_Spr->Draw();
}

void CGameObject::ArrangePosition(float sx, float ex) {
	if (m_Pos.x < sx)
		m_Pos.x = sx;
	if (m_Pos.x > ex)
		m_Pos.x = ex;
}

bool CGameObject::IsTouched(float leftx, float rightx, bool bUseRight)
{
	if (m_bAlive) {
		if (bUseRight && m_Pos.x >= rightx)
			return true;
		else if (!bUseRight && m_Pos.x <= leftx)
			return true;
	}
	return false;
}