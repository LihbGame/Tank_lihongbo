#include "stdafx.h"
#include "CGame.h"

#define KEYDOWN(VK) (GetAsyncKeyState(VK)&0x8000?1:0)    //使用正确的逻辑，避免出现按键的其他状态 

CGame::CGame()
{
	m_menuSelect.m_pParent = this;             //设置菜单选择项指向当前对象
	m_menuBackup.m_pParent = this;             //设置菜单背景项指向当前对象
}


CGame::~CGame()
{
}

//设置输出窗口的句柄
void CGame::SetHandle(HWND hWnd)
{
	m_hWnd = hWnd;
}

//进入游戏帧
bool CGame::EnterFrame(DWORD dwTime)
{
	GameRunLogic();                  //调用逻辑处理函数
	GameRunDraw();                   //调用绘图函数
	return false;
}

//处理鼠标移动事件
void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	//选择阶段
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnMouseMove(nFlags, point); //选择游戏类型
	}
	else
		m_menuBackup.OnMouseMove(nFlags, point); //返回主菜单
}

//设置当前游戏所处的阶段，并根据步骤初始化
void CGame::SetStep(CGame::EGameType step)
{
	m_eStep = step;
	GameInit();                       //调用初始化函数
}


//处理左键抬起事件
void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	//选择阶段
	if (m_eStep == EGameTypeMenu)
	{
		m_menuSelect.OnLButtonUp(nFlags, point);//选择游戏类型
	}
	else
		m_menuBackup.OnLButtonUp(nFlags, point);//返回主菜单
}





/*游戏初始化：
生成游戏对象
初始化地图
对象位置等*/
bool CGame::GameInit()
{
	srand(GetTickCount());                      //初始化随机数生成器
	return (this->*m_initFunc[m_eStep])();      //根据不同阶段调用不同的处理方法
}

//游戏初始化：选择阶段
bool CGame::GameInitMenu()
{
	return true;
}

//游戏初始化：人机大战菜单阶段
bool CGame::GameInitOne2BotMenu()
{
	RECT rc;
	GetWindowRect(m_hWnd, &rc);
	PointF pt;
	pt.X = rc.left + (rc.right - rc.left) / 2.0f;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                 //设置人机大战keymenu的位置为屏幕窗口正中间
	m_keymenu01.SetStop(false);                     //设置不停止播放动画
	return true;
}

//游戏初始化：人机大战
bool CGame::GameInitOne2Bot()
{
	while (true)
	{
		m_map.LoadMap();            //加载地图
		//玩家1
		{
			m_player01=CPlayer(0, 0, L"tank_player1.png");//构造玩家1对象
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))         //随机查找地图中的空地
			{
				AfxMessageBox(L"调整player01位置失败");      //提示调整位置失败
			}
			else
				m_player01.SetCenterPoint(ptCenter);         //放置玩家1到随机分配的空地中心

		}
		//机器人
		{
			m_bot = CBot(0,0,L"tank_bot.png");               //构造机器人
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))         //随机查找地图中的空地
			{
				AfxMessageBox(L"调整机器人坦克位置失败");    //提示调整位置失败
			}
			else
				m_bot.SetCenterPoint(ptCenter);              //放置机器人到随机分配的空地中心
		}
		m_lstBullets.clear();                                //清空子弹数组

		//判断是否合法
		{
			//获取机器人，玩家一的所在位置
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_bot, startx, starty) ||
				!m_map.FindObjPosition(m_player01, targetx, targety))
			{
				AfxMessageBox(L"获取坦克位置发生错误");
				break;
			}
			//判断玩家一和机器人是否连通，如果无法连通，需要重新设置
			VPath path;
			m_map.FindPath(startx,starty,targetx,targety,path);
			if (!path.empty())
			{
				break;     //可以连通，初始化结束
			}
		}
	}
	return true;
}


//游戏初始化：人机大战结束
bool CGame::GameInitOne2BotEnd()
{
	return true;
}

//游戏初始化：双人大战菜单阶段
bool CGame::GameInitOne2OneMenu()
{
	//设置两个玩家的keymenu,屏幕正中间
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	PointF pt;
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f +100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //设置该菜单项的位置
	m_keymenu01.SetStop(false);                    //设置不停止播放动画

	pt.X = rc.right - m_keymenu02.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu02.SetCenterPoint(pt);                //设置该菜单项的位置
	m_keymenu02.SetStop(false);                    //设置不停止播放动画

	return  true;
}

//游戏初始化：双人大战
bool CGame::GameInitOne2One()
{
	while (true)
	{
		m_map.LoadMap();                          //加载地图
		//放置玩家1坦克
		{
			m_player01 = CPlayer(0, 0, L"tank_player1.png");     //构造玩家一对象
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(L"调整player1位置失败");
			}
			else
				m_player01.SetCenterPoint(ptCenter);
		}
		//放置玩家2坦克
		{
			m_player02=CPlayer(0, 0, L"tank_player2.png");          //构造玩家2对象
			PointF ptCenter;
			if (!m_map.FindRandomPosition(ptCenter))
			{
				AfxMessageBox(L"调整player2位置失败");
			}
			else
				m_player02.SetCenterPoint(ptCenter);
		}
		m_lstBullets.clear();                                       //清空子弹数组
																	//判断是否合法
		{
			//获取机器人，玩家一的所在位置
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_player01, startx, starty) ||
				!m_map.FindObjPosition(m_player02, targetx, targety))
			{
				AfxMessageBox(L"获取坦克位置发生错误");
				break;
			}
			//判断玩家一和机器人是否连通，如果无法连通，需要重新设置
			VPath path;
			m_map.FindPath(startx, starty, targetx, targety, path);
			if (!path.empty())
			{
				break;     //可以连通，初始化结束
			}
		}
	}
	return true;
}

//游戏初始化：双人大战结束
bool CGame::GameInitOne2OneEnd()
{
	return true;              //不需要初始化
}


/*游戏逻辑处理：
维护子弹状态
维护机器人（自动移动，自动发射子弹）
维护玩家坦克的状态*/

void CGame::GameRunLogic()
{
	//根据不同阶段调用不同的处理方式
	(this->*m_logicFunc[m_eStep])();
}

//函数逻辑处理：选择阶段
void CGame::GameRunLogicMenu()
{
	//还没开始游戏，无游戏逻辑
}

//游戏逻辑处理：人机大战菜单阶段
void CGame::GameRunLogicOnOne2BotMenu()
{
	if (KEYDOWN('M'))        //如果按下m键，停止动画状态
	{
		m_keymenu01.SetStop();
	}
	if (m_keymenu01.GetStop())
	{
		SetStep(EGameTypeOne2Bot);  //设置状态为开始游戏
	}
}

//游戏逻辑处理：人机大战
void CGame::GameRunLogicOnOne2Bot()
{
	//状态维护
	//移除列表中无效的子弹，并给相应的坦克添加子弹
	RemoveTimeoutBullets();

	//检查子弹是否击中坦克：如果击中，坦克爆炸，游戏结束，子弹可不移除
	for (auto &blt : m_lstBullets)
	{
		if (m_bot.IsHitted(blt))                  //击中机器人
		{
			m_bot.Bomb();                         //机器人爆炸
			m_eStep = EGameTypeOne2BotEnd;        //游戏结束
			blt.SetActive(false);                 //使子弹无效
		}
		if (m_player01.IsHitted(blt))             //击中玩家一
		{
			m_player01.Bomb();                    //玩家一爆炸
			m_eStep = EGameTypeOne2BotEnd;        //游戏结束
			blt.SetActive(false);                 //使子弹无效
		}
		break;
	}			
	ProcessHitBullets();                          //子弹运动维护

	AI();                                         //机器人自动寻路并攻击玩家

	//按键处理
	{
		if (KEYDOWN(VK_LEFT))                     //按下左方向键
		{
			m_player01.RotateLeft();              //玩家一向左旋转
		}
		if (KEYDOWN(VK_RIGHT))					  //按下右方向键
		{ 
			m_player01.RotateRight();             //玩家一向右旋转
		}
		if (KEYDOWN(VK_UP))
		{
		    //坦克撞墙检测
			if (m_map.IsHitTheWall(m_player01,true))    //如果撞墙
			{
				m_player01.ChangeDirection(true);         //改变坦克方向
			}
			else
				m_player01.Forward();                     //没有撞墙继续向前进
		}
		if (KEYDOWN(VK_DOWN))
		{
			//坦克撞墙检测
			if (m_map.IsHitTheWall(m_player01, false))    //如果撞墙
			{
				m_player01.ChangeDirection(true);         //改变坦克方向
			}
			else
				m_player01.Backward();                     //没有撞墙继续向后
		}
		if (KEYDOWN('M'))                                 //如果按下m键
		{
			CBullet blt;
			if (m_player01.Fire(blt))                     //发射子弹
			{
				m_lstBullets.push_back(blt);              //子弹加入地图列表中显示
			}
		}
		if (KEYDOWN('I'))                                 //机器人步进
		{
			//机器人，玩家的位置
			int startx, starty, targetx, targety;
			if (!m_map.FindObjPosition(m_bot, startx, starty) ||
				!m_map.FindObjPosition(m_player01, targetx, targety))
			{
				return;
			}
			float fDirNext = 0;             //机器人下一步的方向
			if (!m_map.FindNextDirection(&fDirNext, startx, starty, targetx, targety))
			{
				return;
			}
			//获取机器人坦克的中心点
			PointF ptTankCenter = m_bot.GetCenterPoint();
			PointF ptAreaCenter = m_map.GetElementAreaCenter(startx, starty);
			RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

			if (!rc.Contains(ptTankCenter))        //判断坦克是否走到中心点位置
			{
				m_bot.Forward();                   //未到达则继续前进
				return;
			}
			else
			{
				m_bot.SetDirection(fDirNext);     //设置电脑玩家的方向
				m_bot.Forward();                  //机器人前进
			}
		}
	}
}

//游戏逻辑处理：人机大战结束
void CGame::GameRunLogicOnOne2BotEnd()
{
	//没有按键处理

	//状态维护
    //移除列表中无效的子弹，并给相应的坦克添加子弹
	RemoveTimeoutBullets();

	//子弹是否撞上墙
	ProcessHitBullets();
}

//游戏逻辑处理：双人大战菜单阶段
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
	if (m_keymenu01.GetStop() && m_keymenu02.GetStop())  //如果都按下就开始游戏
	{
		SetStep(EGameTypeOne2One);
	}
}

//游戏逻辑处理：双人大战
void CGame::GameRunLogicOnOne2One()
{
	//按键处理
	//玩家一的操作键
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
		//坦克撞墙检测
		if (m_map.IsHitTheWall(m_player01, true))    //如果撞墙
		{
			m_player01.ChangeDirection(true);         //改变坦克方向
		}
		else
			m_player01.Forward();                     //没有撞墙继续向前进
	}
	if (KEYDOWN(VK_DOWN))
	{
		//坦克撞墙检测
		if (m_map.IsHitTheWall(m_player01, false))    //如果撞墙
		{
			m_player01.ChangeDirection(true);         //改变坦克方向
		}
		else
			m_player01.Backward();                     //没有撞墙继续向前进
	}
	if (KEYDOWN('M'))
	{
		CBullet blt;
		if (m_player01.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}
	//玩家二的操作键
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
		//坦克撞墙检测
		if (m_map.IsHitTheWall(m_player02, true))    //如果撞墙
		{
			m_player02.ChangeDirection(true);         //改变坦克方向
		}
		else
			m_player02.Forward();                     //没有撞墙继续向前进
	}
	if (KEYDOWN('S'))
	{
		//坦克撞墙检测
		if (m_map.IsHitTheWall(m_player02, false))    //如果撞墙
		{
			m_player02.ChangeDirection(true);         //改变坦克方向
		}
		else
			m_player02.Backward();                     //没有撞墙继续向前进
	}
	if (KEYDOWN('Q'))
	{
		CBullet blt;
		if (m_player02.Fire(blt))
		{
			m_lstBullets.push_back(blt);
		}
	}
	//先判断状态
	//移除列表中无效的子弹，并给相应的坦克添加子弹
	RemoveTimeoutBullets();

	//检查子弹是否击中坦克
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
	ProcessHitBullets();              //子弹撞墙处理
}

//游戏逻辑处理：双人大战结束
void CGame::GameRunLogicOnOne2OneEnd()
{
	RemoveTimeoutBullets();           //移除列表中无效的子弹，并给相应的坦克添加子弹

	ProcessHitBullets();              //子弹撞墙处理
}




//游戏绘图
void CGame::GameRunDraw()
{
	HDC hdc = ::GetDC(m_hWnd);       //获取游戏窗口的dc
	CRect rc;
	GetClientRect(m_hWnd, &rc);      //获取客户区的大小

	CDC *dc = CClientDC::FromHandle(hdc);

	CDC m_dcMemory;                  //双缓冲绘图用
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc, rc.Width(), rc.Height());
	m_dcMemory.CreateCompatibleDC(dc);
	CBitmap *pOldBitmap = m_dcMemory.SelectObject(&bmp);

	Graphics gh(m_dcMemory.GetSafeHdc());         //构造对象
	gh.Clear(Color::White);                       //清除背景
	gh.ResetClip();
	
	(this->*m_drawFunc[m_eStep])(gh);            //调用相应阶段的绘图函数，画入内存

	::BitBlt(hdc,0,0,rc.Width(),rc.Height(),m_dcMemory.GetSafeHdc(),0,0,SRCCOPY);//从内存复制到屏幕
	::ReleaseDC(m_hWnd, hdc);                  //释放dc
	return;
}

//选择阶段
void CGame::GameRunDrawOnMenu(Graphics &gh)
{
	m_menuSelect.Draw(gh);
}

//人机大战:菜单阶段
void CGame::GameRunDrawOnOne2BotMenu(Graphics &gh)
{
	m_menu.Draw(gh);                    //画背景
	m_keymenu01.Draw(gh);               //画菜单
}

//人机大战
void CGame::GameRunDrawOnOne2Bot(Graphics &gh)
{
	m_menuBackup.Draw(gh);               //画背景
	m_map.Draw(gh);                      //画墙

	m_player01.Draw(gh);                 //画玩家

	m_bot.Draw(gh);                      //画机器人
	for (auto b : m_lstBullets)          //画已经发射的子弹
	{
		b.Draw(gh);
	}

	DrawFps(gh);                          //输出fps
}

//人机大战结束
void CGame::GameRunDrawOnOne2BotEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);               //背景
	m_map.Draw(gh);                      //墙
	m_player01.Draw(gh);                 //玩家
	m_bot.Draw(gh);                      //机器人


	DrawFps(gh);                         //fps
	if (m_player01.IsBombEnd() || m_bot.IsBombEnd())   //判断游戏是否结束，并改变游戏状态
	{
		m_eStep = EGameTypeMenu;
	}

}


//双人大战：菜单阶段
void CGame::GameRunDrawOnOne2OneMenu(Graphics &gh)
{
	m_menu.Draw(gh);                        //画菜单背景
	m_keymenu01.Draw(gh);                   //画菜单项一
	m_keymenu02.Draw(gh);                   //画菜单项二
}

//双人大战
void CGame::GameRunDrawOnOne2One(Graphics &gh)
{
	m_menuBackup.Draw(gh);                  //画游戏背景
	m_map.Draw(gh);                         //画墙
	m_player01.Draw(gh);                    //画玩家一
	m_player02.Draw(gh);                    //画玩家二
	for (auto b : m_lstBullets)             //画子弹
	{
		b.Draw(gh);
	}

	DrawFps(gh);                            //fps
}

//双人大战结束
void CGame::GameRunDrawOnOne2OneEnd(Graphics &gh)
{
	m_menuBackup.Draw(gh);                  //画游戏背景
	m_map.Draw(gh);                         //画墙
	m_player01.Draw(gh);                    //画玩家一
	m_player02.Draw(gh);                    //画玩家二
	for (auto b : m_lstBullets)             //画子弹
	{
		b.Draw(gh);
	}

	DrawFps(gh);                            //fps
	if (m_player01.IsBombEnd ()|| m_player02.IsBombEnd())
	{
		m_eStep = EGameTypeMenu;
	}
}

//画fps
void CGame::DrawFps(Graphics &gh)
{
	static int fps = 0;                             //定义局部静态变量
	m_fps++;                                        //进入一次该函数数值加一
	static DWORD dwLast = GetTickCount();           //记录上次运行时间，静态局部变量只初始化一次
	if (GetTickCount() - dwLast >= 1000)            //如果时间达到1秒
	{
		fps = m_fps;                               //记录这一秒累计的fps值
		m_fps = 0;                                  //清空全局静态变量，方便下次记录
		dwLast = GetTickCount();                    //记录本次时间
	}

	//显示fps
	CString s;
	s.Format(L"FPS:%d", fps);                        //将整形格式化为字符串
	SolidBrush brush(Color(0, 0, 255));              //定义画刷颜色
	Gdiplus::Font font(L"宋体", 10);                 //定义字体的字体和大小
	CRect rc;
	::GetClientRect(m_hWnd, &rc);                    //获取客户端的大小
	 PointF origin(static_cast<float>(rc.right - 50),
		static_cast<float>(rc.top + 2));             //定义显示位置，在窗口右上角
	gh.DrawString(s.GetString(), -1, &font, origin, &brush);    //输出文字
}

//移除超时子弹，并给相应坦克添加子弹
void CGame::RemoveTimeoutBullets()
{
	//定义查找方法
	auto itRemove = remove_if(m_lstBullets.begin(), m_lstBullets.end(),
		[](CBullet& blt)->bool {return blt.IsTimeout(); });

	//把子弹移除
	for (auto it = itRemove; it != m_lstBullets.end(); ++it)
	{
		//设置为无效
		it->SetActive(false);
		//给对应的坦克添加子弹
		it->GetOwner()->AddBullet(*it);
	}
	//从本地删除子弹
	m_lstBullets.erase(itRemove, m_lstBullets.end());
}

//子弹运动维护，撞墙后改变方向
void CGame::ProcessHitBullets()
{
	//子弹是否撞上墙：如果撞上了，改变方向
	for (auto& blt : m_lstBullets)
	{
		m_map.HitWallProcess(blt);        //如果撞上墙，该函数会改变子弹方向
		blt.Move();                       //子弹继续前进
	}
}

//维护电脑的自动寻路攻击
void CGame::AI()
{
	//电脑运动状态维护
	static CGameTimer acTimer(-1, 150);
	if (acTimer.IsTimeval())
	{
		//获取机器人和玩家所在位置
		int startx, starty, targetx, targety;
		if (!m_map.FindObjPosition(m_bot, startx, starty) ||
			!m_map.FindObjPosition(m_player01, targetx, targety))
		{
			return;
		}
		float fDirNext = 0;           //机器人下一步的方向
		if (!m_map.FindNextDirection(&fDirNext, startx, starty, targetx, targety))
		{
			return;
		}
		//获取机器人，地图中空格地中心点
		//获取机器人坦克的中心点
		PointF ptTankCenter = m_bot.GetCenterPoint();
		PointF ptAreaCenter = m_map.GetElementAreaCenter(startx, starty);
		RectF rc(ptAreaCenter.X - 5, ptAreaCenter.Y - 5, 10, 10);

		if (!rc.Contains(ptTankCenter))        //判断坦克是否走到中心点位置
		{
			m_bot.Forward();                   //未到达则继续前进
			return;
		}
		else
		{
			m_bot.SetDirection(fDirNext);     //设置电脑玩家的方向
			float dir;
			//判断是否可以打到玩家一
			if (m_map.IsCanKillTarget(m_bot, m_player01, &dir))
			{
				CBullet blt;
				if (m_bot.Fire(blt))      //机器人开火，进行主动攻击
				{
					m_lstBullets.push_back(blt);
				}
				return;
			}
			m_bot.Forward();             //机器人前进
		}
	}
}


































