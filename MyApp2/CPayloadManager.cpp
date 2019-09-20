#include "stdafx.h"

#include <d3d9.h>
#include <d3dx9.h>

#include "CSprite.h"
#include "CPayload.h"

#include "CPayloadManager.h"


CPayloadManager::CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECT3DTEXTURE9 texture
	, const D3DXVECTOR3& center, float speed, const D3DXVECTOR3& dir, const D3DXVECTOR2& screensize)
{
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[i] = new CPayload(pD3DDevice, texture, center, speed, dir, screensize);
	}
}


CPayloadManager::~CPayloadManager()
{
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		delete m_Payload[i];
	}
}

int CPayloadManager::GetEmptyPayload() {
	for(int i = 0; i < PAYLOAD_MAX; i++) {
		if (m_Payload[i]->isAlive() == false) {
			return i;
		}
	}

	return -1;
}

void CPayloadManager::Update(float dt) {
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[i]->Update(dt);
	}
}
	
void CPayloadManager::Draw() {
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		m_Payload[i]->Draw();
	}
}

void CPayloadManager::onFire(const D3DXVECTOR3& pos) {
	int id = GetEmptyPayload();

	if (id != -1) {
		m_Payload[id]->onFire(pos);
		//m_Payload[id]->setDir(D3DXVECTOR3(0, -1, 0));
	}

	//id = GetEmptyPayload();

	//if (id != -1) {
	//	m_Payload[id]->onFire(pos - D3DXVECTOR3(20, 0, 0));
	//	m_Payload[id]->setDir(D3DXVECTOR3(0.5, -0.7, 0));
	//}

	//id = GetEmptyPayload();

	//if (id != -1) {
	//	m_Payload[id]->onFire(pos + D3DXVECTOR3(20, 0, 0));
	//	m_Payload[id]->setDir(D3DXVECTOR3(-0.5, -0.7, 0));
	//}
}

void CPayloadManager::onFire(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir) {
	int id = GetEmptyPayload();

	if (id != -1) {
		m_Payload[id]->onFire(pos, dir);
	}
}

bool CPayloadManager::IsCollision(const D3DXVECTOR3& pos, float radius) {
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		if (m_Payload[i]->isAlive()) {
			D3DXVECTOR3 dist =  m_Payload[i]->GetPos() - pos;
			if (D3DXVec3Length(&dist) < radius) {
				m_Payload[i]->setAlive(false);
				return true;
			}
		}
	}

	return false;
}

D3DXVECTOR3 CPayloadManager::GetPostion() {
	for (int i = 0; i < PAYLOAD_MAX; i++) {
		if (m_Payload[i]->isAlive()) {
			return m_Payload[i]->GetPos();
		}
	}
}

