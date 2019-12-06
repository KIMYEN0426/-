#pragma once

class CEntity
{
public:
	CEntity();
	CEntity(D3DXVECTOR3 pos, D3DXVECTOR3 cen, D3DXVECTOR3 dir, float speed);
	~CEntity();

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 cen, D3DXVECTOR3 dir, float speed);

	const D3DXVECTOR3& getPos();
	const D3DXVECTOR3& getCen();
	const D3DXVECTOR3& getDir();	
	float getSpeed() const;

	void SetDir(const D3DXVECTOR3& dir)
	{
		m_dir = dir;
	}
	void SetSpeed(float speed)
	{
		m_speed = speed;
	}
	void SetPos(const D3DXVECTOR3& pos)
	{
		m_pos = pos;
	}
	void Update(float deltatime);

protected:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_center;
	D3DXVECTOR3 m_dir;
	float m_speed;
};

