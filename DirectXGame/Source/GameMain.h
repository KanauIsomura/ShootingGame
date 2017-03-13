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

#ifndef ______GAME_MAIN_CLASS_H_INCLUDE_____
#define ______GAME_MAIN_CLASS_H_INCLUDE_____

//=== インクルード ===
#include"Framework\GameBase\GameBase.h"

#include"Framework\System\CSmartPointer.h"
#include"Framework\GameBase\SceneManagerThread.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CGameMain : public CGameBase{
private:
	CSmartPointer<SceneBase> m_spNowScene;

public:
	//コンストラクタ
	CGameMain();
	
	// 初期化関数
	bool InitGame();

	// 更新関数
	void Update();

	// 描画関数
	void Draw();

	// 後始末関数
	void UnInitAll();
};

#endif
