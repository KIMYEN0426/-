#pragma once


class CSprite
{
protected:
	LPD3DXSPRITE m_Sprite;
	LPDIRECT3DDEVICE9 pD3DDevice;
public:
	CSprite();
	CSprite(LPDIRECT3DDEVICE9 pD3DDevice);
	virtual ~CSprite();
	bool Create(LPDIRECT3DDEVICE9 pD3DDevice);
	void Release();
	void Draw(LPDIRECT3DTEXTURE9 pTex,const D3DXVECTOR3 &cen, const D3DXVECTOR3 &position);
};

