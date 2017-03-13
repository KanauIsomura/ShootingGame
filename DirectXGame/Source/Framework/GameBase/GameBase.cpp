//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//概要	:
//		>
//------------------------------------------------------------------------------------------------
//作成者:磯村 奏宇
//
//作成日:2015-00-00
//更新日:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== インクルード ===
#include"GameBase.h"

#include"../API/Window.h"
#include"../System/Device.h"
#include"../System/FPSManager.h"

#include"../System/InputManager.h"

#include"../System/DebugPrint.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CGameBase::CGameBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//システム初期化
bool CGameBase::InitSystem(){
	//--	HINSTANCE取得
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//--	ウインドウ作成
	GetWindowClass()->MakeWindow( _T("Game_Base"), hInstance, 1024, 576);

	//--	ウインドウの表示
	GetWindowClass()->Show();

	//--	DirectX9生成初期化
		//--	DirectX9の初期化
	if( GetDeviceClass()->InitDirect3D(true) ){
		return false;
	}

	//FPSマネージャーの生成
	m_spFPSManager.SetPointer( new FPSManager );

	//インプットマネージャの初期化
	GetInputManager()->InitInput(GetWindowHandle());

	DebugPrint::SetFont();

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//起動
int CGameBase::Run(){
		//--	システム初期化
	if( ! InitSystem() ){
		return 0;			//エラー
	}

	//--	ゲーム初期化
	if( ! InitGame() ){
		UnInitAll();
		return 0;			//エラー
	}

	//--	メインループ
	while( GetWindowClass()->HandleMessage()){
		if( ! m_spFPSManager->CheckExec()){
			continue;
		}

		//--	更新
		//インプットマネージャの初期化
		GetInputManager()->UpdateInput();
		Update();

		//--	描画
		GetDeviceClass()->Clear();
		GetDeviceClass()->BeginScene();
		Draw();
		m_spFPSManager->Draw();
		GetDeviceClass()->EndScene();
		GetDeviceClass()->Present();
	}

	UnInitAll();

	return 0;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//DirectX9device取得
LPDIRECT3DDEVICE9	CGameBase::GetDirectXDevice(){
	return GetDeviceClass()->GetDirect3DDevice();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//WindowHandle取得
HWND	CGameBase::GetWindowHandle(){
	return GetWindowClass()->GetWindowHandle();
}
