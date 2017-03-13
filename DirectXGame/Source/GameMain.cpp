
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[GameMain]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2017-03-01
//更新日:2017-03-01
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"GameMain.h"

#include"Framework\System\Device.h"

#include"Framework\GameObject\ObjectManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CGameMain::CGameMain(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//初期化関数
bool CGameMain::InitGame(){
	GetObjectManager()->Initialize();

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//更新関数
void CGameMain::Update(){
	m_spNowScene = GetSceneManagerThread()->GetScene();

	GetObjectManager()->ObjUpdate();

	if(!m_spNowScene.GetPointer())
		return;
	m_spNowScene->Update();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//描画関数
void CGameMain::Draw(){
	GetObjectManager()->ObjDraw();

	if(!m_spNowScene.GetPointer())
		return;
	m_spNowScene->Draw();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//後始末関数
void CGameMain::UnInitAll(){
	GetObjectManager()->Finalize();

	delete GetSceneManagerThread();
}
