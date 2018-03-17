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
			m_player01=CPlayer(0, 0, _T("tank_palyer1.png"));//构造玩家1对象
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
	pt.X = rc.left + m_keymenu01.GetRect().Width / 2.0f + 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //设置该菜单项的位置
	m_keymenu01.SetStop(false);                    //设置不停止播放动画

	pt.X = rc.right - m_keymenu01.GetRect().Width / 2.0f - 100;
	pt.Y = rc.top + (rc.bottom - rc.top) / 2.0f;
	m_keymenu01.SetCenterPoint(pt);                //设置该菜单项的位置
	m_keymenu01.SetStop(false);                    //设置不停止播放动画

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

}















































