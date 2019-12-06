#pragma once
#include <map>
#include <string>
using namespace std;

class CTexture
{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;	//로컬 변수
	static map<wstring, LPDIRECT3DTEXTURE9> s_TexturePool;

public:
	CTexture();
	CTexture(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename);
	~CTexture();

	bool Create(LPDIRECT3DDEVICE9 pD3DDevice,const TCHAR* filename);
	void Release();

	LPDIRECT3DTEXTURE9 GetTexture() const
	{
		return m_pTexture;
	}
	
	static void ReleaseAll();
};

