#include "stdafx.h"
#include "CGame.h"

#define KEYDOWN(VK) (GetAsyncKeyState(VK)&0x8000?1:0)    //ʹ����ȷ���߼���������ְ���������״̬ 

CGame::CGame()
{
	m_menuSelect.m_pParent = this;             //���ò˵�ѡ����ָ��ǰ����
	m_menuBackup.m_pParent = this;             //���ò˵�������ָ��ǰ����
}


CGame::~CGame()
{
}

//����������ڵľ��
void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

//������Ϸ֡
bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunLogic();                  //�����߼�������
	GameRunDraw();                   //���û�ͼ����
	return false;
}

//��������ƶ��¼�
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	//ѡ��׶�
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point); //ѡ����Ϸ����
	}
	else
		m_menuBackup.OnMouseMove(nFlags, point); //�������˵�
}

//���õ�ǰ��Ϸ�����Ľ׶Σ������ݲ����ʼ��
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();                       //���ó�ʼ������
}


//�������̧���¼�
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	//ѡ��׶�
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnLButtonUp(nFlags, point);//ѡ����Ϸ����
	}
	else
		m_menuBackup.OnLButtonUp(nFlags, point);//�������˵�
}





/*��Ϸ��ʼ����
������Ϸ����
��ʼ����ͼ
����λ�õ�*/
bool CGame::GameInit()
{
	srand(GetTickCount());                      //��ʼ�������������
	return (this->*m_initFunc[m_eStep])();      //���ݲ�ͬ�׶ε��ò�ͬ�Ĵ�����
}

//��Ϸ��ʼ����ѡ��׶�
bool CGame::GameInitMenu()
{
	return true;
}

//��Ϸ��ʼ�����˻���ս�˵��׶�
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                 //�����˻���սkeymenu��λ��Ϊ��Ļ�������м�
	m_keymenu01.SetStop(false);                     //���ò�ֹͣ���Ŷ���
	return true;
}

//��Ϸ��ʼ�����˻���ս
bool CGame::GameInitOne2Bot()
{
	while (true)
	{
		m_map.LoadMap();            //���ص�ͼ
		//���1
		{
			m_player01=CPlayer(0, 0, _T("tank_palyer1.png"));//�������1����
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))         //������ҵ�ͼ�еĿյ�
			{
				AfxMessageBox(L"����player01λ��ʧ��");      //��ʾ����λ��ʧ��
			}
			else
				m_player01.SetCenterPoint(ptCenter);         //�������1���������Ŀյ�����

		}
		//������
		{
			m_bot = CBot(0,0,L"tank_bot.png");               //���������
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))         //������ҵ�ͼ�еĿյ�
			{
				AfxMessageBox(L"����������̹��λ��ʧ��");    //��ʾ����λ��ʧ��
			}
			else
				m_bot.SetCenterPoint(ptCenter);              //���û����˵��������Ŀյ�����
		}
		m_lstBullets.clear();                                //����ӵ�����

		//�ж��Ƿ�Ϸ�
		{
			//��ȡ�����ˣ����һ������λ��
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_bot, startx, starty) ||
				!m_map.FindObjPosition(m_player01, targetx, targety))
			{
				AfxMessageBox(L"��ȡ̹��λ�÷�������");
				break;
			}
			//�ж����һ�ͻ������Ƿ���ͨ������޷���ͨ����Ҫ��������
			VPath path;
			m_map.FindPath(startx,starty,targetx,targety,path);
			if (!path.empty())
			{
				break;     //������ͨ����ʼ������
			}
		}
	}
	return true;
}


//��Ϸ��ʼ�����˻���ս����
bool CGame::GameInitOne2BotEnd()
{
	return true;
}

//��Ϸ��ʼ����˫�˴�ս�˵��׶�
bool CGame::GameInitOne2OneMenu()
{
	//����������ҵ�keymenu,��Ļ���м�
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //���øò˵����λ��
	m_keymenu01.SetStop(false);                    //���ò�ֹͣ���Ŷ���

	pt.X = rc.right - m_keymenu01.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //���øò˵����λ��
	m_keymenu01.SetStop(false);                    //���ò�ֹͣ���Ŷ���

	return  true;
}

//��Ϸ��ʼ����˫�˴�ս
bool CGame::GameInitOne2One()
{
	while (true)
	{
		m_map.LoadMap();                          //���ص�ͼ
		//�������1̹��
		{
			m_player01 = CPlayer(0, 0, L"tank_player1.png");     //�������һ����
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(L"����player1λ��ʧ��");
			}
			else
				m_player01.SetCenterPoint(ptCenter);
		}
		//�������2̹��
		{
			m_player02=CPlayer(0, 0, L"tank_player2.png");          //�������2����
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(L"����player2λ��ʧ��");
			}
			else
				m_player02.SetCenterPoint(ptCenter);
		}
		m_lstBullets.clear();                                       //����ӵ�����
																	//�ж��Ƿ�Ϸ�
		{
			//��ȡ�����ˣ����һ������λ��
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_player01, startx, starty) ||
				!m_map.FindObjPosition(m_player02, targetx, targety))
			{
				AfxMessageBox(L"��ȡ̹��λ�÷�������");
				break;
			}
			//�ж����һ�ͻ������Ƿ���ͨ������޷���ͨ����Ҫ��������
			VPath path;
			m_map.FindPath(startx, starty, targetx, targety, path);
			if (!path.empty())
			{
				break;     //������ͨ����ʼ������
			}
		}
	}
	return true;
}

//��Ϸ��ʼ����˫�˴�ս����
bool CGame::GameInitOne2OneEnd()
{
	return true;              //����Ҫ��ʼ��
}


/*��Ϸ�߼�����
ά���ӵ�״̬
ά�������ˣ��Զ��ƶ����Զ������ӵ���
ά�����̹�˵�״̬*/

void CGame::GameRunLogic()
{
	//���ݲ�ͬ�׶ε��ò�ͬ�Ĵ���ʽ
	(this->*m_logicFunc[m_eStep])();
}

//�����߼�����ѡ��׶�
void CGame::GameRunLogicMenu()
{
	//��û��ʼ��Ϸ������Ϸ�߼�
}

//��Ϸ�߼������˻���ս�˵��׶�
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))        //�������m����ֹͣ����״̬
	{
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);  //����״̬Ϊ��ʼ��Ϸ
	}
}

//��Ϸ�߼������˻���ս
void CGame::GameRunLogicOnOne2Bot()
{
	//״̬ά��
	//�Ƴ��б�����Ч���ӵ���������Ӧ��̹������ӵ�
	RemoveTimeoutBullets();

	//����ӵ��Ƿ����̹�ˣ�������У�̹�˱�ը����Ϸ�������ӵ��ɲ��Ƴ�
	for (auto &blt : m_lstBullets)
	{
		if (m_bot.IsHitted(blt))                  //���л�����
		{
			m_bot.Bomb();                         //�����˱�ը
			m_eStep = EGameTypeOne2BotEnd;        //��Ϸ����
			blt.SetActive(false);                 //ʹ�ӵ���Ч
		}
		if (m_player01.IsHitted(blt))             //�������һ
		{
			m_player01.Bomb();                    //���һ��ը
			m_eStep = EGameTypeOne2BotEnd;        //��Ϸ����
			blt.SetActive(false);                 //ʹ�ӵ���Ч
		}
		break;
	}			

}















































