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
			m_player01=CPlayer(0, 0, L"tank_player1.png");//�������1����
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
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f +100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //���øò˵����λ��
	m_keymenu01.SetStop(false);                    //���ò�ֹͣ���Ŷ���

	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);                //���øò˵����λ��
	m_keymenu02.SetStop(false);                    //���ò�ֹͣ���Ŷ���

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
	ProcessHitBullets();                          //�ӵ��˶�ά��

	AI();                                         //�������Զ�Ѱ·���������

	//��������
	{
		if (KEYDOWN(VK_LEFT))                     //���������
		{
			m_player01.RotateLeft();              //���һ������ת
		}
		if (KEYDOWN(VK_RIGHT))					  //�����ҷ����
		{ 
			m_player01.RotateRight();             //���һ������ת
		}
		if (KEYDOWN(VK_UP))
		{
		    //̹��ײǽ���
			if (m_map.IsHitTheWall(m_player01,true))    //���ײǽ
			{
				m_player01.ChangeDirection(true);         //�ı�̹�˷���
			}
			else
				m_player01.Forward();                     //û��ײǽ������ǰ��
		}
		if (KEYDOWN(VK_DOWN))
		{
			//̹��ײǽ���
			if (m_map.IsHitTheWall(m_player01, false))    //���ײǽ
			{
				m_player01.ChangeDirection(true);         //�ı�̹�˷���
			}
			else
				m_player01.Backward();                     //û��ײǽ�������
		}
		if (KEYDOWN('M'))                                 //�������m��
		{
			CBullet blt;
			if (m_player01.Fire(blt))                     //�����ӵ�
			{
				m_lstBullets.push_back(blt);              //�ӵ������ͼ�б�����ʾ
			}
		}
		if (KEYDOWN('I'))                                 //�����˲���
		{
			//�����ˣ���ҵ�λ��
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_bot, startx, starty) ||
				!m_map.FindObjPosition(m_player01, targetx, targety))
			{
				return;
			}
			float fDirNext = 0;             //��������һ���ķ���
			if (!m_map.FindNextDirection(&fDirNext, startx, starty, targetx, targety))
			{
				return;
			}
			//��ȡ������̹�˵����ĵ�
			PointF ptTankCenter = m_bot.GetCenterPoint();
			PointF ptAreaCenter = m_map.GetElementAreaCenter(startx, starty);
			RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

			if (!rc.Contains(ptTankCenter))        //�ж�̹���Ƿ��ߵ����ĵ�λ��
			{
				m_bot.Forward();                   //δ���������ǰ��
				return;
			}
			else
			{
				m_bot.SetDirection(fDirNext);     //���õ�����ҵķ���
				m_bot.Forward();                  //������ǰ��
			}
		}
	}
}

//��Ϸ�߼������˻���ս����
void CGame::GameRunLogicOnOne2BotEnd()
{
	//û�а�������

	//״̬ά��
    //�Ƴ��б�����Ч���ӵ���������Ӧ��̹������ӵ�
	RemoveTimeoutBullets();

	//�ӵ��Ƿ�ײ��ǽ
	ProcessHitBullets();
}

//��Ϸ�߼�����˫�˴�ս�˵��׶�
void CGame::GameRunLogicOnOne2OneMenu()
{
	if (KEYDOWN('M'))
	{
		m_keymenu01.SetStop();
	}
	if (KEYDOWN('Q'))
	{
		m_keymenu02.SetStop();
	}
	if (m_keymenu01.GetStop() && m_keymenu02.GetStop())  //��������¾Ϳ�ʼ��Ϸ
	{
		SetStep(EGameTypeOne2One);
	}
}

//��Ϸ�߼�����˫�˴�ս
void CGame::GameRunLogicOnOne2One()
{
	//��������
	//���һ�Ĳ�����
	if (KEYDOWN(VK_LEFT))
	{
		m_player01.RotateLeft();
	}
	if (KEYDOWN(VK_RIGHT))
	{
		m_player01.RotateRight();
	}
	if (KEYDOWN(VK_UP))
	{
		//̹��ײǽ���
		if (m_map.IsHitTheWall(m_player01, true))    //���ײǽ
		{
			m_player01.ChangeDirection(true);         //�ı�̹�˷���
		}
		else
			m_player01.Forward();                     //û��ײǽ������ǰ��
	}
	if (KEYDOWN(VK_DOWN))
	{
		//̹��ײǽ���
		if (m_map.IsHitTheWall(m_player01, false))    //���ײǽ
		{
			m_player01.ChangeDirection(true);         //�ı�̹�˷���
		}
		else
			m_player01.Backward();                     //û��ײǽ������ǰ��
	}
	if (KEYDOWN('M'))
	{
		CBullet blt;
		if (m_player01.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}
	//��Ҷ��Ĳ�����
	if (KEYDOWN('A'))
	{
		m_player02.RotateLeft();
	}
	if (KEYDOWN('D'))
	{
		m_player02.RotateRight();
	}
	if (KEYDOWN('W'))
	{
		//̹��ײǽ���
		if (m_map.IsHitTheWall(m_player02, true))    //���ײǽ
		{
			m_player02.ChangeDirection(true);         //�ı�̹�˷���
		}
		else
			m_player02.Forward();                     //û��ײǽ������ǰ��
	}
	if (KEYDOWN('S'))
	{
		//̹��ײǽ���
		if (m_map.IsHitTheWall(m_player02, false))    //���ײǽ
		{
			m_player02.ChangeDirection(true);         //�ı�̹�˷���
		}
		else
			m_player02.Backward();                     //û��ײǽ������ǰ��
	}
	if (KEYDOWN('Q'))
	{
		CBullet blt;
		if (m_player02.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}
	//���ж�״̬
	//�Ƴ��б�����Ч���ӵ���������Ӧ��̹������ӵ�
	RemoveTimeoutBullets();

	//����ӵ��Ƿ����̹��
	for (auto &blt : m_lstBullets)
	{
		if (!blt.IsActive())
		{
			continue;
		}
		if (m_player01.IsHitted(blt))
		{
			m_player01.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
		if (m_player02.IsHitted(blt))
		{
			m_player02.Bomb();
			m_eStep = EGameTypeOne2OneEnd;
			blt.SetActive(false);
		}
	}
	ProcessHitBullets();              //�ӵ�ײǽ����
}

//��Ϸ�߼�����˫�˴�ս����
void CGame::GameRunLogicOnOne2OneEnd()
{
	RemoveTimeoutBullets();           //�Ƴ��б�����Ч���ӵ���������Ӧ��̹������ӵ�

	ProcessHitBullets();              //�ӵ�ײǽ����
}




//��Ϸ��ͼ
void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);       //��ȡ��Ϸ���ڵ�dc
	CRect rc;
	GetClientRect(m_hWnd, &rc);      //��ȡ�ͻ����Ĵ�С

	CDC *dc = CClientDC::FromHandle(hdc);

	CDC m_dcMemory;                  //˫�����ͼ��
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());         //�������
	gh.Clear(Color::White);                       //�������
	gh.ResetClip();
	
	(this->*m_drawFunc[m_eStep])(gh);            //������Ӧ�׶εĻ�ͼ�����������ڴ�

	::BitBlt(hdc,0,0,rc.Width(),rc.Height(),m_dcMemory.GetSafeHdc(),0,0,SRCCOPY);//���ڴ渴�Ƶ���Ļ
	::ReleaseDC(m_hWnd, hdc);                  //�ͷ�dc
	return;
}

//ѡ��׶�
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.Draw(gh);
}

//�˻���ս:�˵��׶�
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.Draw(gh);                    //������
	m_keymenu01.Draw(gh);               //���˵�
}

//�˻���ս
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.Draw(gh);               //������
	m_map.Draw(gh);                      //��ǽ

	m_player01.Draw(gh);                 //�����

	m_bot.Draw(gh);                      //��������
	for (auto b : m_lstBullets)          //���Ѿ�������ӵ�
	{
		b.Draw(gh);
	}

	DrawFps(gh);                          //���fps
}

//�˻���ս����
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);               //����
	m_map.Draw(gh);                      //ǽ
	m_player01.Draw(gh);                 //���
	m_bot.Draw(gh);                      //������


	DrawFps(gh);                         //fps
	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())   //�ж���Ϸ�Ƿ���������ı���Ϸ״̬
	{
		m_eStep = EGameTypeMenu;
	}

}


//˫�˴�ս���˵��׶�
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.Draw(gh);                        //���˵�����
	m_keymenu01.Draw(gh);                   //���˵���һ
	m_keymenu02.Draw(gh);                   //���˵����
}

//˫�˴�ս
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.Draw(gh);                  //����Ϸ����
	m_map.Draw(gh);                         //��ǽ
	m_player01.Draw(gh);                    //�����һ
	m_player02.Draw(gh);                    //����Ҷ�
	for (auto b : m_lstBullets)             //���ӵ�
	{
		b.Draw(gh);
	}

	DrawFps(gh);                            //fps
}

//˫�˴�ս����
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);                  //����Ϸ����
	m_map.Draw(gh);                         //��ǽ
	m_player01.Draw(gh);                    //�����һ
	m_player02.Draw(gh);                    //����Ҷ�
	for (auto b : m_lstBullets)             //���ӵ�
	{
		b.Draw(gh);
	}

	DrawFps(gh);                            //fps
	if (m_player01.IsBombEnd ()|| m_player02.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

//��fps
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;                             //����ֲ���̬����
	m_fps++;                                        //����һ�θú�����ֵ��һ
	static DWORD dwLast = GetTickCount();           //��¼�ϴ�����ʱ�䣬��̬�ֲ�����ֻ��ʼ��һ��
	if (GetTickCount() - dwLast >= 1000)            //���ʱ��ﵽ1��
	{
		fps = m_fps;                               //��¼��һ���ۼƵ�fpsֵ
		m_fps = 0;                                  //���ȫ�־�̬�����������´μ�¼
		dwLast = GetTickCount();                    //��¼����ʱ��
	}

	//��ʾfps
	CString s;
	s.Format(L"FPS:%d", fps);                        //�����θ�ʽ��Ϊ�ַ���
	SolidBrush brush(Color(0, 0, 255));              //���廭ˢ��ɫ
	Gdiplus::Font font(L"����", 10);                 //�������������ʹ�С
	CRect rc;
	::GetClientRect(m_hWnd, &rc);                    //��ȡ�ͻ��˵Ĵ�С
	 PointF origin(static_cast<float>(rc.right - 50),
		static_cast<float>(rc.top + 2));             //������ʾλ�ã��ڴ������Ͻ�
	gh.DrawString(s.GetString(), -1, &font, origin, &brush);    //�������
}

//�Ƴ���ʱ�ӵ���������Ӧ̹������ӵ�
void CGame::RemoveTimeoutBullets()
{
	//������ҷ���
	auto itRemove = remove_if(m_lstBullets.begin(), m_lstBullets.end(),
		[](CBullet& blt)->bool {return blt.IsTimeout(); });

	//���ӵ��Ƴ�
	for (auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		//����Ϊ��Ч
		it->SetActive(false);
		//����Ӧ��̹������ӵ�
		it->GetOwner()->AddBullet(*it);
	}
	//�ӱ���ɾ���ӵ�
	m_lstBullets.erase(itRemove, m_lstBullets.end());
}

//�ӵ��˶�ά����ײǽ��ı䷽��
void CGame::ProcessHitBullets()
{
	//�ӵ��Ƿ�ײ��ǽ�����ײ���ˣ��ı䷽��
	for (auto& blt : m_lstBullets)
	{
		m_map.HitWallProcess(blt);        //���ײ��ǽ���ú�����ı��ӵ�����
		blt.Move();                       //�ӵ�����ǰ��
	}
}

//ά�����Ե��Զ�Ѱ·����
void CGame::AI()
{
	//�����˶�״̬ά��
	static CGameTimer acTimer(-1, 150);
	if (acTimer.IsTimeval())
	{
		//��ȡ�����˺��������λ��
		int startx, starty, targetx, targety;
		if (!m_map.FindObjPosition(m_bot, startx, starty) ||
			!m_map.FindObjPosition(m_player01, targetx, targety))
		{
			return;
		}
		float fDirNext = 0;           //��������һ���ķ���
		if (!m_map.FindNextDirection(&fDirNext, startx, starty, targetx, targety))
		{
			return;
		}
		//��ȡ�����ˣ���ͼ�пո�����ĵ�
		//��ȡ������̹�˵����ĵ�
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startx, starty);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		if (!rc.Contains(ptTankCenter))        //�ж�̹���Ƿ��ߵ����ĵ�λ��
		{
			m_bot.Forward();                   //δ���������ǰ��
			return;
		}
		else
		{
			m_bot.SetDirection(fDirNext);     //���õ�����ҵķ���
			float dir;
			//�ж��Ƿ���Դ����һ
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))      //�����˿��𣬽�����������
				{
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();             //������ǰ��
		}
	}
}


































