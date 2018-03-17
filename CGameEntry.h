#pragma once
#ifndef entry_h
#define entry_h
#include "stdafx.h"
#include <math.h>
#include <cmath>
class CGameEntry
{
public:
	CGameEntry();
	virtual ~CGameEntry();
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;//基类绘图的纯虚函数
};

//可移动的物体
class CGameEntryMoveable :public CGameEntry
{
public:
	CGameEntryMoveable() {};
	virtual ~CGameEntryMoveable() {};
	//复制构造
	CGameEntryMoveable(const CGameEntryMoveable &gem);
	//重载赋值运算符
	CGameEntryMoveable &operator=(const CGameEntryMoveable &gem);
public:
	virtual void Draw(Gdiplus::Graphics &gh)const = 0;
	/*可移动的物体共有的属性*/
	//右转
	virtual void RotateRight();
	//左转
	virtual void RotateLeft();
	//前移
	virtual void Forward();
	//后移
	virtual void Backward();
	//获取水平方向的速度分量
	virtual float GetXSpeed()const;
	//获取竖直方向的速度分量
	virtual float GetYSpeed()const;
	//下一步(前进的)位置
	virtual RectF ForwardNextRect()const;
	//下一步(后退的)位置
	virtual RectF BackwardNextRect()const;

	//获取头部位置：图片为矩形，头部位置就是角度为0时
	//距离中心点的半径（对角线/2），重置自己在x轴上的位置
	virtual PointF GetHeadPos()const;
	//获取中心点
	virtual PointF GetCenterPoint()const;
	//设置中心点
	virtual void SetCenterPoint(const PointF &ptCenter);

	//属性存储器
#ifndef PI
#define PI (3.1415926f)
#endif

	//设置角度：单位为“a*PI”
	virtual void SetDirectionArc(float dir);

	//设置角度：单位为度
	virtual void SetDirection(float dir);

	//获取当前的角度：单位为”a*PI“
	virtual float GetDirectionArc()const;

	//获取当前的角度：单位为度
	virtual float GetDirection()const;

	//设置每次旋转的角度：单位为”a*PI“
	virtual void SetDirectionTurnArc(float dir);

	//设置每次旋转的角度：单位为度
	virtual void SetDirectionTurn(float dir);

	//获取当前的角度：单位为”a*PI“
	virtual float GetDirectionTurnArc()const;

	//获取当前的弧度
	virtual float GetDirectionTurn()const;


	//是否是有效的
	virtual bool IsActive()const;
	virtual void SetActive(bool bAction);

	//占用范围
	virtual void SetRect(const RectF rect);
	virtual RectF GetRect()const;

	//移动速度
	virtual void SetSpeed(float speed);
	virtual float GetSpeed()const;

private:
	bool m_bActive{ false };    //是否有效
	RectF m_rect{ 0,0,100,100 };  //占用区域
	float m_fSpeed{ 10 };       //移动速度
	float m_direction{ 0 };     //当前角度
	float m_directionTurn{ 5 };//每次转动的角度
};
#endif