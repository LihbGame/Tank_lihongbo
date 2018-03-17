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

	//设置输出窗口的句柄
	void SetHandle(HWND hWnd);

	//进入游戏帧
	bool EnterFrame(DWORD dwTime);

	//处理鼠标移动事件
	void OnMouseMove(UINT nFlags,CPoint point);

	//处理鼠标抬起事件
	void OnLButtonUp(UINT nFlags, CPoint point);

	//当前游戏所处的阶段
	enum EGameType
	{
		EGameTypeMenu=0,         //选择阶段
		EGameTypeOne2BotMenu,    //人机大战菜单阶段
		EGameTypeOne2Bot,        //人机大战
		EGameTypeOne2BotEnd,     //人机大战结束
		EGameTypeOne2OneMenu,    //双人大战菜单阶段
		EGameTypeOne2One,        //双人大战
		EGameTypeOne2OneEnd,     //双人大战结束
		EGameTypeCount,          //七个阶段
	};

	//设置当前游戏所处的阶段，并根据步骤初始化
	void SetStep(CGame::EGameType step);

private:
	//窗口
	HWND m_hWnd;
	/*游戏初始化：
	生成游戏对象，初始化地图，对象位置等*/

	bool GameInit();

	bool GameInitMenu();          //游戏初始化：选择阶段

	bool GameInitOne2BotMenu();   //游戏初始化：人机大战菜单阶段

	bool GameInitOne2Bot();       //游戏初始化：人机大战

	bool GameInitOne2BotEnd();    //游戏初始化：人机大战结束

	bool GameInitOne2OneMenu();   //游戏初始化：双人大战菜单阶段

	bool GameInitOne2One();       //游戏初始化：双人大战

	bool GameInitOne2OneEnd();    //游戏初始化：双人大战结束

	//把上述方法放入数组，方便调用
	bool (CGame::*m_initFunc[EGameTypeCount])() = {
	&CGame::GameInitMenu,        //选择阶段
	&CGame::GameInitOne2BotMenu, //人机大战键盘提示
	&CGame::GameInitOne2Bot,     //人机大战
	&CGame::GameInitOne2BotEnd,  //人机大战结束
	&CGame::GameInitOne2OneMenu, //双人大战键盘提示
	&CGame::GameInitOne2One,     //双人大战
	&CGame::GameInitOne2OneEnd   //双人大战结束
	};
   

	/*游戏逻辑处理：
	1.维护子弹状态
	2.维护机器人
	3.维护玩家坦克状态*/
	void GameRunLogic();

	void GameRunLogicMenu();                 //游戏逻辑处理：菜单阶段

	void GameRunLogicOnOne2BotMenu();        //游戏逻辑处理：人机大战菜单阶段

	void GameRunLogicOnOne2Bot();            //游戏逻辑处理：人机大战

	void GameRunLogicOnOne2BotEnd();         //游戏逻辑处理：人机大战结束

	void GameRunLogicOnOne2OneMenu();        //游戏逻辑处理：双人大战菜单阶段

	void GameRunLogicOnOne2One();            //游戏逻辑处理：双人大战

	void GameRunLogicOnOne2OneEnd();         //游戏逻辑处理：双人大战结束
	//把上述方法放入数组，方便调用
	void (CGame::*m_logicFunc[EGameTypeCount])() = {
		&CGame::GameRunLogicMenu,            //选择阶段
		&CGame::GameRunLogicOnOne2BotMenu,   //人机大战按钮提示
		&CGame::GameRunLogicOnOne2Bot,       //人机大战
		&CGame::GameRunLogicOnOne2BotEnd,    //人机大战结束
		&CGame::GameRunLogicOnOne2OneMenu,   //双人大战按钮提示
		&CGame::GameRunLogicOnOne2One,       //双人大战
		&CGame::GameRunLogicOnOne2OneEnd,    //双人大战结束
	};





	/*游戏绘图处理：负责绘制游戏中的对象*/

	void GameRunDraw();

	void GameRunDrawOnMenu(Graphics &gh);              //游戏绘图处理：选择阶段

	void GameRunDrawOnOne2BotMenu(Graphics &gh);       //游戏绘图处理：人机大战菜单阶段

	void GameRunDrawOnOne2Bot(Graphics &gh);           //游戏绘图处理：人机大战

	void GameRunDrawOnOne2BotEnd(Graphics &gh);        //游戏绘图处理：人机大战结束

	void GameRunDrawOnOne2OneMenu(Graphics &gh);       //游戏绘图处理：双人大战菜单阶段

	void GameRunDrawOnOne2One(Graphics &gh);           //游戏绘图处理：双人大战

	void GameRunDrawOnOne2OneEnd(Graphics &gh);        //游戏绘图处理：双人大战结束

	//把上述方式放入数组，方便调用
	void (CGame::*m_drawFunc[EGameTypeCount])() = {
		&CGame::GameRunDrawOnMenu,            //选择阶段
		&CGame::GameRunDrawOnOne2BotMenu,   //人机大战按钮提示
		&CGame::GameRunDrawOnOne2Bot,       //人机大战
		&CGame::GameRunDrawOnOne2BotEnd,    //人机大战结束
		&CGame::GameRunDrawOnOne2OneMenu,   //双人大战按钮提示
		&CGame::GameRunDrawOnOne2One,       //双人大战
		&CGame::GameRunDrawOnOne2OneEnd,    //双人大战结束
	};

private:
	//添加辅助方法
	void RemoveTimeoutBullets();           //移除超时子弹并给对应的坦克装弹

	void ProcessHitBullets();              //维护子弹的运行及撞墙处理

	void AI();                             //维护电脑夫人自动寻路攻击

	void DrawFps(Graphics &gh);            //输出fps

private:
	//成员变量
	int m_fps{ 0 };                        //记录游戏fps
	
	EGameType m_eStep{ EGameTypeMenu };    //当前阶段：菜单选择阶段
	
	CPlayer m_player01;

	CPlayer m_player02;

	CBot m_bot;                            //机器人

	list<CBullet>m_lstBullets;             //存在于地图场景中的子弹对象数组

	CGameMap m_map{ 10,10,780,580 };       //地图对象

	CGameMenuPanel m_menuSelect;           //开始菜单

	CGameMenuBackground m_menu;            //开始菜单背景图

	CGameMenuPanel2 m_menuBackup;          //返回菜单

	CKeyMenuPlayer01 m_keymenu01;          //提示按钮的菜单
	CKeyMenuPlayer02 m_keymenu02;
};

