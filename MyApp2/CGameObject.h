#pragma once

class CTexture;
class CSprite;

class CGameObject
{
public:
	CGameObject();
	CGameObject(LPDIRECT3DDEVICE9 pD3DDevice,
		LPDIRECT3DTEXTURE9 pTexture,
		const D3DXVECTOR3& center,
		const D3DXVECTOR3& position,
		float speed = 100.f,
		int life = 1);
	virtual ~CGameObject();

	void InitGameObject(LPDIRECT3DDEVICE9 pD3DDevice, 
						LPDIRECT3DTEXTURE9 pTexture, 
						const D3DXVECTOR3& center, 
						const D3DXVECTOR3& position,
						float speed = 100.f, int life = 1);

	void Release();

	void Update(float dt);
	void Render();

	void ArrangePosition(float sx, float ex);
	
	bool IsTouched(float leftx, float rightx, bool bUseRight);

	void SetDirection(float x, float y) {
		m_Dir = D3DXVECTOR3(x, y, 0.f);
	}

	const D3DXVECTOR3& GetDirection() const {
		return m_Dir;
	}

	void SetPosition(D3DXVECTOR3 pos) {
		m_Pos = pos;
	}

	D3DXVECTOR3 GetPosition() {
		return m_Pos;
	}

	void SetAlive(bool flag) {
		m_bAlive = flag;
	}

	bool GetAlive() {
		return m_bAlive;
	}

	void SetLife(int life) {
		m_life = life;
	}

	bool DecLife() {
		m_life--;

		if (m_life == 0) {
			return true;
		}

		return false;
	}

	void SetColor(D3DCOLOR color) {
		m_Spr->SetColor(color);
	}

protected:
	CSprite* m_Spr;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Center;
	D3DXVECTOR3 m_Dir;

	bool m_bAlive;
	float m_Speed;
	int m_life;
};

