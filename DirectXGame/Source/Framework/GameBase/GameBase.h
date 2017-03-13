//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-0
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______GAME_BASE_CALSS_H_INCLUDE_____
#define ______GAME_BASE_CALSS_H_INCLUDE_____

//=== インクルード ===
#pragma comment(lib, "d3d9.lib")
#include<d3d9.h>

#include"../System/CSmartPointer.h"
#include"../System/FPSManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//ゲームのベースプログラム
class CGameBase{
private:
	CSmartPointer<FPSManager>	m_spFPSManager;	//FPSマネージャ

public:
	//コンストラクタ
	CGameBase();
	//デストラクタ
	~CGameBase(){};
	
	//起動
	int Run();
	
	////--	ゲッター
	//DirectX9device取得
	LPDIRECT3DDEVICE9	GetDirectXDevice();
	//WindowHandle取得
	HWND	GetWindowHandle();

private:
	//--	システム初期化
	bool InitSystem();
	//--	ゲーム初期化
	virtual bool InitGame() = 0;
	//--	更新
	virtual void Update() = 0;
	//--	描画
	virtual void Draw() = 0;
	//--	終了
	virtual void UnInitAll() = 0;
};

#endif
