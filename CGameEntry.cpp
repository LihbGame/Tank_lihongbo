#include "stdafx.h"
#include "CGameEntry.h"

using namespace std;
CGameEntry::CGameEntry()
{
}


CGameEntry::~CGameEntry()
{
}
//复制构造函数
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

//重载赋值运算
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

//右转
void CGameEntryMoveable::RotateRight()
{
	m_direction += m_directionTurn;       //当前角度增加
	if (m_direction >= 360)               //避免角度数值过大
	{
		m_direction -= 360;
	}
}

//左转
void CGameEntryMoveable::RotateLeft()
{
	m_direction -= m_directionTurn; //当前角度增加
	if (m_direction < 0)            //保持角度大于0
	{
		m_direction = static_cast<float>(360 + m_direction);
	}
}

//前移
void CGameEntryMoveable::Forward()
{
	//根据当前角度，在x,y方向上分别进行增减
	m_rect.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	m_rect.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//后移
void CGameEntryMoveable::Backward()
{
	//根据当前角度，在x,y方向上进行增减
	m_rect.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	m_rect.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//获取水平方向的速度分量
float CGameEntryMoveable:: GetXSpeed() const
{
	return static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
}

//获取竖直方向上的速度分量
float CGameEntryMoveable::GetYSpeed() const
{
	return -static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
}

//下一步(前进的)位置
RectF CGameEntryMoveable::ForwardNextRect()const
{
	RectF rc = m_rect;
	rc.X += static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	rc.Y -= static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	return rc;
}

//下一步(后退的)位置
RectF CGameEntryMoveable::BackwardNextRect()const
{
	RectF rc = m_rect;
	rc.X -= static_cast<Gdiplus::REAL>((m_fSpeed)*sin(GetDirectionArc()));
	rc.Y += static_cast<Gdiplus::REAL>((m_fSpeed)*cos(GetDirectionArc()));
	return rc;
}

//获取头部的位置
PointF CGameEntryMoveable::GetHeadPos() const
{
	PointF ptCenter = GetCenterPoint();
	PointF ptHead = ptCenter;
	float fRadius = sqrt(pow(m_rect.Width / 2, 2) + pow(m_rect.Height / 2, 2));//计算半径
	ptHead.X += fRadius * sin(GetDirectionArc());    //计算头部坐标x
	ptHead.Y -= fRadius * cos(GetDirectionArc());	 //计算头部坐标y
	return ptHead;
}

//获取中心点
PointF CGameEntryMoveable::GetCenterPoint() const
{
	PointF center = PointF(m_rect.X+m_rect.Width/2,m_rect.Y+m_rect.Height/2);
	return center;
}

//设置中心点
void CGameEntryMoveable::SetCenterPoint(const PointF &ptCenter)
{
	PointF center =GetCenterPoint();
	center.X = ptCenter.X - center.X;
	center.Y = ptCenter.Y - center.Y;
	m_rect.Offset(center);
}

//设置角度
void CGameEntryMoveable::SetDirectionArc(float dir)
{
	m_direction = dir * 180.0f / PI;
}
//角度（弧度）
void CGameEntryMoveable::SetDirection(float dir)
{
	m_direction = dir;
}
//获得当前角度
float CGameEntryMoveable::GetDirectionArc()const
{
	return PI * m_direction / 180.0f;
}

float CGameEntryMoveable::GetDirection()const
{
	return m_direction;
}

//设置每次旋转的角度
void CGameEntryMoveable::SetDirectionTurnArc(float dir)
{
	m_directionTurn = PI*dir/180.0f;
}
void CGameEntryMoveable::SetDirectionTurn(float dir)
{
	m_directionTurn = dir;
}

//获取当前的角度（单位：PI）
float CGameEntryMoveable::GetDirectionTurnArc() const
{
	return PI * m_directionTurn / 180.0f;
}

//获取当前的弧度（单位：度）
float CGameEntryMoveable::GetDirectionTurn()const
{
	return m_direction;
}

//是否有效
bool CGameEntryMoveable::IsActive() const
{
	return m_bActive;
}

void CGameEntryMoveable::SetActive(bool bActive)
{
	m_bActive = bActive;
}

//占用范围
void CGameEntryMoveable::SetRect(const RectF rect)
{
	m_rect = rect;
}

RectF CGameEntryMoveable::GetRect() const
{
	return m_rect;
}

//移动速度
void CGameEntryMoveable::SetSpeed(float speed)
{
	m_fSpeed = speed;
}

float CGameEntryMoveable::GetSpeed() const
{
	return m_fSpeed;
}








