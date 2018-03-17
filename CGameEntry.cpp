#include "stdafx.h"
#include "CGameEntry.h"

using namespace std;
CGameEntry::CGameEntry()
{
}


CGameEntry::~CGameEntry()
{
}
//���ƹ��캯��
CGameEntryMoveable::CGameEntryMoveable(const CGameEntryMoveable &gem)
{
	if (this != &gem)
	{
		this->m_bActive = gem.m_bActive;
		this->m_direction = gem.m_direction;
		this->m_directionTurn = gem.m_directionTurn;
		this->m_fSpeed = gem.m_fSpeed;
		this->m_rect = gem.m_rect;
	}
}

//���ظ�ֵ����
CGameEntryMoveable& CGameEntryMoveable::operator=(const CGameEntryMoveable &gem)
{
	if (this != &gem)
	{
		this->m_bActive = gem.m_bActive;
		this->m_direction = gem.m_direction;
		this->m_directionTurn = gem.m_directionTurn;
		this->m_fSpeed = gem.m_fSpeed;
		this->m_rect = gem.m_rect;
	}
	return *this;
}

//��ת
void CGameEntryMoveable::RotateRight()
{
	m_direction += m_directionTurn;       //��ǰ�Ƕ�����
	if (m_direction >= 360)               //����Ƕ���ֵ����
	{
		m_direction -= 360;
	}
}

//��ת
void CGameEntryMoveable::RotateLeft()
{
	m_direction -= m_directionTurn; //��ǰ�Ƕ�����
	if (m_direction < 0)            //���ֽǶȴ���0
	{
		m_direction = static_cast<float>(360 + m_direction);
	}
}

//ǰ��
void CGameEntryMoveable::Forward()
{
	//���ݵ�ǰ�Ƕȣ���x,y�����Ϸֱ��������
	m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//����
void CGameEntryMoveable::Backward()
{
	//���ݵ�ǰ�Ƕȣ���x,y�����Ͻ�������
	m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//��ȡˮƽ������ٶȷ���
float CGameEntryMoveable:: GetXSpeed() const
{
	return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
}

//��ȡ��ֱ�����ϵ��ٶȷ���
float CGameEntryMoveable::GetYSpeed() const
{
	return -static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//��һ��(ǰ����)λ��
RectF CGameEntryMoveable::ForwardNextRect()const
{
	RectF rc = m_rect;
	rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	return rc;
}

//��һ��(���˵�)λ��
RectF CGameEntryMoveable::BackwardNextRect()const
{
	RectF rc = m_rect;
	rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	return rc;
}

//��ȡͷ����λ��
PointF CGameEntryMoveable::GetHeadPos() const
{
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	float fRadius = sqrt(pow(m_rect.Width / 2, 2) + pow(m_rect.Height / 2, 2));//����뾶
	ptHead.X += fRadius * sin(GetDirectionArc());    //����ͷ������x
	ptHead.Y -= fRadius * cos(GetDirectionArc());	 //����ͷ������y
	return ptHead;
}

//��ȡ���ĵ�
PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF center = PointF(m_rect.X+m_rect.Width/2,m_rect.Y+m_rect.Height/2);
	return center;
}

//�������ĵ�
void CGameEntryMoveable::SetCenterPoint(const PointF &ptCenter)
{
	PointF center =GetCenterPoint();
	center.X = ptCenter.X - center.X;
	center.Y = ptCenter.Y - center.Y;
	m_rect.Offset(center);
}

//���ýǶ�
void CGameEntryMoveable::SetDirectionArc(float dir)
{
	m_direction = dir * 180.0f / PI;
}
//�Ƕȣ����ȣ�
void CGameEntryMoveable::SetDirection(float dir)
{
	m_direction = dir;
}
//��õ�ǰ�Ƕ�
float CGameEntryMoveable::GetDirectionArc()const
{
	return PI * m_direction / 180.0f;
}

float CGameEntryMoveable::GetDirection()const
{
	return m_direction;
}

//����ÿ����ת�ĽǶ�
void CGameEntryMoveable::SetDirectionTurnArc(float dir)
{
	m_directionTurn = PI*dir/180.0f;
}
void CGameEntryMoveable::SetDirectionTurn(float dir)
{
	m_directionTurn = dir;
}

//��ȡ��ǰ�ĽǶȣ���λ��PI��
float CGameEntryMoveable::GetDirectionTurnArc() const
{
	return PI * m_directionTurn / 180.0f;
}

//��ȡ��ǰ�Ļ��ȣ���λ���ȣ�
float CGameEntryMoveable::GetDirectionTurn()const
{
	return m_direction;
}

//�Ƿ���Ч
bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}

void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}

//ռ�÷�Χ
void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_rect = rect;
}

RectF CGameEntryMoveable::GetRect() const
{
	return m_rect;
}

//�ƶ��ٶ�
void CGameEntryMoveable::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

float CGameEntryMoveable::GetSpeed() const
{
	return m_fSpeed;
}








