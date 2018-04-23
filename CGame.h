#pragma once
#include "wall.h"
#include "Bullet.h"
#include "Player.h"
#include "Bot.h"
#include "GameMap.h"
#include "GameMenuPanel.h"
#include "GameMenuPanel2.h"
#include "KeyMenuPlayer02.h"
#include "KeyMenuPlayer01.h"
#include "GameMenuBackground.h"
#include "stdafx.h"
class CGame
{
public:
	CGame();
	~CGame();

	//����������ڵľ��
	void SetHandle(HWND hWnd);

	//������Ϸ֡
	bool EnterFrame(DWORD dwTime);

	//��������ƶ��¼�
	void OnMouseMove(UINT nFlags,CPoint point);

	//�������̧���¼�
	void OnLButtonUp(UINT nFlags, CPoint point);

	//��ǰ��Ϸ�����Ľ׶�
	enum EGameType
	{
		EGameTypeMenu=0,         //ѡ��׶�
		EGameTypeOne2BotMenu,    //�˻���ս�˵��׶�
		EGameTypeOne2Bot,        //�˻���ս
		EGameTypeOne2BotEnd,     //�˻���ս����
		EGameTypeOne2OneMenu,    //˫�˴�ս�˵��׶�
		EGameTypeOne2One,        //˫�˴�ս
		EGameTypeOne2OneEnd,     //˫�˴�ս����
		EGameTypeCount,          //�߸��׶�
	};

	//���õ�ǰ��Ϸ�����Ľ׶Σ������ݲ����ʼ��
	void SetStep(CGame::EGameType step);

private:
	//����
	HWND m_hWnd;
	/*��Ϸ��ʼ����
	������Ϸ���󣬳�ʼ����ͼ������λ�õ�*/

	bool GameInit();

	bool GameInitMenu();          //��Ϸ��ʼ����ѡ��׶�

	bool GameInitOne2BotMenu();   //��Ϸ��ʼ�����˻���ս�˵��׶�

	bool GameInitOne2Bot();       //��Ϸ��ʼ�����˻���ս

	bool GameInitOne2BotEnd();    //��Ϸ��ʼ�����˻���ս����

	bool GameInitOne2OneMenu();   //��Ϸ��ʼ����˫�˴�ս�˵��׶�

	bool GameInitOne2One();       //��Ϸ��ʼ����˫�˴�ս

	bool GameInitOne2OneEnd();    //��Ϸ��ʼ����˫�˴�ս����

	//�����������������飬�������
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
	&CGame::GameInitMenu,        //ѡ��׶�
	&CGame::GameInitOne2BotMenu, //�˻���ս������ʾ
	&CGame::GameInitOne2Bot,     //�˻���ս
	&CGame::GameInitOne2BotEnd,  //�˻���ս����
	&CGame::GameInitOne2OneMenu, //˫�˴�ս������ʾ
	&CGame::GameInitOne2One,     //˫�˴�ս
	&CGame::GameInitOne2OneEnd   //˫�˴�ս����
	};
   

	/*��Ϸ�߼�����
	1.ά���ӵ�״̬
	2.ά��������
	3.ά�����̹��״̬*/
	void GameRunLogic();

	void GameRunLogicMenu();                 //��Ϸ�߼������˵��׶�

	void GameRunLogicOnOne2BotMenu();        //��Ϸ�߼������˻���ս�˵��׶�

	void GameRunLogicOnOne2Bot();            //��Ϸ�߼������˻���ս

	void GameRunLogicOnOne2BotEnd();         //��Ϸ�߼������˻���ս����

	void GameRunLogicOnOne2OneMenu();        //��Ϸ�߼�����˫�˴�ս�˵��׶�

	void GameRunLogicOnOne2One();            //��Ϸ�߼�����˫�˴�ս

	void GameRunLogicOnOne2OneEnd();         //��Ϸ�߼�����˫�˴�ս����
	//�����������������飬�������
	void (CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicMenu,            //ѡ��׶�
		&CGame::GameRunLogicOnOne2BotMenu,   //�˻���ս��ť��ʾ
		&CGame::GameRunLogicOnOne2Bot,       //�˻���ս
		&CGame::GameRunLogicOnOne2BotEnd,    //�˻���ս����
		&CGame::GameRunLogicOnOne2OneMenu,   //˫�˴�ս��ť��ʾ
		&CGame::GameRunLogicOnOne2One,       //˫�˴�ս
		&CGame::GameRunLogicOnOne2OneEnd,    //˫�˴�ս����
	};





	/*��Ϸ��ͼ�������������Ϸ�еĶ���*/

	void GameRunDraw();

	void GameRunDrawOnMenu(Graphics &gh);              //��Ϸ��ͼ����ѡ��׶�

	void GameRunDrawOnOne2BotMenu(Graphics &gh);       //��Ϸ��ͼ�����˻���ս�˵��׶�

	void GameRunDrawOnOne2Bot(Graphics &gh);           //��Ϸ��ͼ�����˻���ս

	void GameRunDrawOnOne2BotEnd(Graphics &gh);        //��Ϸ��ͼ�����˻���ս����

	void GameRunDrawOnOne2OneMenu(Graphics &gh);       //��Ϸ��ͼ����˫�˴�ս�˵��׶�

	void GameRunDrawOnOne2One(Graphics &gh);           //��Ϸ��ͼ����˫�˴�ս

	void GameRunDrawOnOne2OneEnd(Graphics &gh);        //��Ϸ��ͼ����˫�˴�ս����

	//��������ʽ�������飬�������
	void (CGame::*m_drawFunc[EGameTypeCount])() = {
		&CGame::GameRunDrawOnMenu,            //ѡ��׶�
		&CGame::GameRunDrawOnOne2BotMenu,   //�˻���ս��ť��ʾ
		&CGame::GameRunDrawOnOne2Bot,       //�˻���ս
		&CGame::GameRunDrawOnOne2BotEnd,    //�˻���ս����
		&CGame::GameRunDrawOnOne2OneMenu,   //˫�˴�ս��ť��ʾ
		&CGame::GameRunDrawOnOne2One,       //˫�˴�ս
		&CGame::GameRunDrawOnOne2OneEnd,    //˫�˴�ս����
	};

private:
	//��Ӹ�������
	void RemoveTimeoutBullets();           //�Ƴ���ʱ�ӵ�������Ӧ��̹��װ��

	void ProcessHitBullets();              //ά���ӵ������м�ײǽ����

	void AI();                             //ά�����Է����Զ�Ѱ·����

	void DrawFps(Graphics &gh);            //���fps

private:
	//��Ա����
	int m_fps{ 0 };                        //��¼��Ϸfps
	
	EGameType m_eStep{ EGameTypeMenu };    //��ǰ�׶Σ��˵�ѡ��׶�
	
	CPlayer m_player01;

	CPlayer m_player02;

	CBot m_bot;                            //������

	list<CBullet>m_lstBullets;             //�����ڵ�ͼ�����е��ӵ���������

	CGameMap m_map{ 10,10,780,580 };       //��ͼ����

	CGameMenuPanel m_menuSelect;           //��ʼ�˵�

	CGameMenuBackground m_menu;            //��ʼ�˵�����ͼ

	CGameMenuPanel2 m_menuBackup;          //���ز˵�

	CKeyMenuPlayer01 m_keymenu01;          //��ʾ��ť�Ĳ˵�
	CKeyMenuPlayer02 m_keymenu02;
};

