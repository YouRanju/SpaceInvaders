#pragma once

#define PAYLOAD_MAX 10

class CSprite;
class CPayload;

class CPayloadManager
{
protected:
	CPayload * m_Payload[PAYLOAD_MAX];
public:
	explicit CPayloadManager(LPDIRECT3DDEVICE9 pD3DDevice, LPDIRECT3DTEXTURE9 texture, const D3DXVECTOR3& center
		, float speed, const D3DXVECTOR3& dir, const D3DXVECTOR2& screensize);
	virtual ~CPayloadManager();

	int GetEmptyPayload();

	void Update(float dt);
	void Draw();

	void onFire(const D3DXVECTOR3& pos);
	void onFire(const D3DXVECTOR3& pos, const D3DXVECTOR3& dir);

	D3DXVECTOR3 GetPostion();

	bool IsCollision(const D3DXVECTOR3& pos, float radius);

};

