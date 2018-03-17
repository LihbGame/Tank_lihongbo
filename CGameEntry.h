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
	virtual void Draw(Gdiplus::Graphics &gh) const = 0;//�����ͼ�Ĵ��麯��
};

//���ƶ�������
class CGameEntryMoveable :public CGameEntry
{
public:
	CGameEntryMoveable() {};
	virtual ~CGameEntryMoveable() {};
	//���ƹ���
	CGameEntryMoveable(const CGameEntryMoveable &gem);
	//���ظ�ֵ�����
	CGameEntryMoveable &operator=(const CGameEntryMoveable &gem);
public:
	virtual void Draw(Gdiplus::Graphics &gh)const = 0;
	/*���ƶ������干�е�����*/
	//��ת
	virtual void RotateRight();
	//��ת
	virtual void RotateLeft();
	//ǰ��
	virtual void Forward();
	//����
	virtual void Backward();
	//��ȡˮƽ������ٶȷ���
	virtual float GetXSpeed()const;
	//��ȡ��ֱ������ٶȷ���
	virtual float GetYSpeed()const;
	//��һ��(ǰ����)λ��
	virtual RectF ForwardNextRect()const;
	//��һ��(���˵�)λ��
	virtual RectF BackwardNextRect()const;

	//��ȡͷ��λ�ã�ͼƬΪ���Σ�ͷ��λ�þ��ǽǶ�Ϊ0ʱ
	//�������ĵ�İ뾶���Խ���/2���������Լ���x���ϵ�λ��
	virtual PointF GetHeadPos()const;
	//��ȡ���ĵ�
	virtual PointF GetCenterPoint()const;
	//�������ĵ�
	virtual void SetCenterPoint(const PointF &ptCenter);

	//���Դ洢��
#ifndef PI
#define PI (3.1415926f)
#endif

	//���ýǶȣ���λΪ��a*PI��
	virtual void SetDirectionArc(float dir);

	//���ýǶȣ���λΪ��
	virtual void SetDirection(float dir);

	//��ȡ��ǰ�ĽǶȣ���λΪ��a*PI��
	virtual float GetDirectionArc()const;

	//��ȡ��ǰ�ĽǶȣ���λΪ��
	virtual float GetDirection()const;

	//����ÿ����ת�ĽǶȣ���λΪ��a*PI��
	virtual void SetDirectionTurnArc(float dir);

	//����ÿ����ת�ĽǶȣ���λΪ��
	virtual void SetDirectionTurn(float dir);

	//��ȡ��ǰ�ĽǶȣ���λΪ��a*PI��
	virtual float GetDirectionTurnArc()const;

	//��ȡ��ǰ�Ļ���
	virtual float GetDirectionTurn()const;


	//�Ƿ�����Ч��
	virtual bool IsActive()const;
	virtual void SetActive(bool bAction);

	//ռ�÷�Χ
	virtual void SetRect(const RectF rect);
	virtual RectF GetRect()const;

	//�ƶ��ٶ�
	virtual void SetSpeed(float speed);
	virtual float GetSpeed()const;

private:
	bool m_bActive{ false };    //�Ƿ���Ч
	RectF m_rect{ 0,0,100,100 };  //ռ������
	float m_fSpeed{ 10 };       //�ƶ��ٶ�
	float m_direction{ 0 };     //��ǰ�Ƕ�
	float m_directionTurn{ 5 };//ÿ��ת���ĽǶ�
};
#endif