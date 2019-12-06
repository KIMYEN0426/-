#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CTexture.h"

map<wstring, LPDIRECT3DTEXTURE9> CTexture::s_TexturePool;

CTexture::CTexture() : m_pTexture(NULL)
{
}

CTexture::CTexture(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename)
{
	Create(pD3DDevice, filename);
}


CTexture::~CTexture()
{
	Release();
}


bool CTexture::Create(LPDIRECT3DDEVICE9 pD3DDevice, const TCHAR* filename) {

	bool bReturn = false;
	auto iter = s_TexturePool.find(wstring(filename));

	if (iter != s_TexturePool.end())
	{
		m_pTexture = iter->second;
		bReturn = true;
	}
	else
	{
		auto hr = D3DXCreateTextureFromFileEx(pD3DDevice, filename
			, 0, 0, 0, 0
			, D3DFMT_A8R8G8B8
			, D3DPOOL_DEFAULT
			, D3DX_FILTER_NONE
			, D3DX_DEFAULT
			, D3DCOLOR_XRGB(0, 0, 0)
			, NULL
			, NULL
			, &m_pTexture);

		bReturn = FAILED(hr) ? false : true;
		/*if (FAILED(hr))
			bReturn = false;
		bReturn =  true;*/

		if (bReturn == true) 
		{
			s_TexturePool[wstring(filename)] = m_pTexture; //1
			//s_TexturePool.insert(pair<wstring, LPDIRECT3DTEXTURE9>(wstring(filename), m_pTexture); //2;
		}
	}

	return bReturn;
}

void CTexture::Release() {
	//if (m_pTexture != NULL){
	//	m_pTexture->Release();
	//m_pTexture = NULL;
	//}
}

void CTexture::ReleaseAll()
{
	for (auto iter = s_TexturePool.begin(); iter != s_TexturePool.end(); ++iter)
	{
		iter->second->Release();
		iter->second = NULL;
	}
	s_TexturePool.clear();
}