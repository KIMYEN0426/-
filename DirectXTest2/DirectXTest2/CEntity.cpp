#include "stdafx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CEntity.h"


CEntity::CEntity()
{
}

CEntity::CEntity(D3DXVECTOR3 pos, D3DXVECTOR3 cen, D3DXVECTOR3 m_dir, float m_speed)
{
	Set(pos, cen, m_dir, m_speed);
}

CEntity::~CEntity()
{
}

void CEntity::Set(D3DXVECTOR3 pos, D3DXVECTOR3 cen, D3DXVECTOR3 dir, float speed)
{
	m_pos = pos;
	m_center = cen;
	m_dir = dir;
	m_speed = speed;
}
const D3DXVECTOR3& CEntity::getPos() { return m_pos; }
const D3DXVECTOR3& CEntity::getCen() { return m_center; }
const D3DXVECTOR3& CEntity::getDir() { return m_dir; }
float CEntity::getSpeed() const { return m_speed; }

void CEntity::Update(float deltatime) {
	m_pos = m_pos + m_dir * m_speed*deltatime;
}