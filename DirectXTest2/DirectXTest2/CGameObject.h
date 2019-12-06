#pragma once

//#include "CEntity.h"
//#include "CSprite.h"
//#include "CTexture.h"


class CGameObject
{
public:
	CGameObject();
	CGameObject(LPDIRECT3DDEVICE9 pD3DDevice
		, const TCHAR* filename
		, D3DXVECTOR3 pos
		, D3DXVECTOR3 cen = {32.f,32.f,0.f}
		, D3DXVECTOR3 dir ={0.f,0.f,0.f}
		, float speed=100.f);
	virtual ~CGameObject();

	bool Create(LPDIRECT3DDEVICE9 pD3DDevice
		, const TCHAR* filename
		, D3DXVECTOR3 pos
		, D3DXVECTOR3 cen
		, D3DXVECTOR3 dir
		, float speed);

	void Draw();
	void Update(float deltaTime)
	{
		m_pEntity->Update(deltaTime);
	}

	void SetDir(const D3DXVECTOR3& dir)
	{
		m_pEntity->SetDir(dir);
	}

	void SetSpeed(float speed)
	{
		m_pEntity->SetSpeed(speed);
	}
	const D3DXVECTOR3& GetPos() {
		return m_pEntity->getPos();
	}
	void SetPos(const D3DXVECTOR3& pos) {
		m_pEntity->SetPos(pos);
	}
	void SetAlive(bool flag) {
		m_bAlive = flag;
	}
	bool GetAlive() const {
		return m_bAlive;
	}
protected:
	CSprite* m_pSpr;
	CTexture* m_pTex;
	CEntity* m_pEntity;
	bool m_bAlive;
};

