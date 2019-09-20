#pragma once

class CSprite;

class CPayload
{
protected:
	CSprite *	m_Spr;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_Dir;
	D3DXVECTOR2 m_screensize;
	float		m_speed;
	float		m_Alive;

public:
	explicit CPayload(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECT3DTEXTURE9 texture, const D3DXVECTOR3& center, float speed,
						const D3DXVECTOR3& dir, const D3DXVECTOR2& screensize);
	virtual ~CPayload();

	bool isAlive() const { return m_Alive; }

	void setAlive(bool flag) { m_Alive = flag; }

	const D3DXVECTOR3& GetPos() { return m_Pos; }

	void onFire(const D3DXVECTOR3& pos) {
		m_Alive = true;
		m_Pos = pos;
	}

	void onFire(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir) {
		m_Alive = true;
		m_Pos = pos;
		D3DXVec3Normalize(&m_Dir, &dir);
	}

	void Update(float dt);
	void Draw();

	void setDir(const D3DXVECTOR3& dir) {
		m_Dir = dir;
	}
};

