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

#ifndef ______INPUT_MANAGER_CLASS_H_INCLUDE_____
#define ______INPUT_MANAGER_CLASS_H_INCLUDE_____

//=== インクルード ===
#pragma	comment(lib, "dinput8.lib")
#pragma	comment(lib, "dxguid.lib")
#define	DIRECTINPUT_VERSION	( 0x0800 )	// 警告用
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>

#include"CSmartPointer.h"

//--- 定数定義 ---
const int NUM_KEY_MAX	= 256;
const WORD MOUSE_LEFT_CLICK = 0;
const WORD MOUSE_RIGHT_CLICK = 1;
const WORD MOUSE_WHEEL_CLICK = 2;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CInputManager{
private:
	LPDIRECTINPUT8		m_pDirectInput;					// DirectInputオブジェクト
	LPDIRECTINPUTDEVICE8	m_pDirectInputKeyboard;			// キーボードデバイス
	LPDIRECTINPUTDEVICE8	m_pDirectInputMouse;				// マウスデバイス

	BYTE					m_aKeyboardData[NUM_KEY_MAX];		// キーボードの状態(現在)
	BYTE					m_aKeyboardLastData[NUM_KEY_MAX];	// キーボードの状態(１フレーム前)
	DIMOUSESTATE2			m_MouseData;						// マウスの状態(現在)
	DIMOUSESTATE2			m_MouseLastData;					// マウスの状態(現在)

public:
	// デストラクタ
	~CInputManager();

	// クリエイト関数
	static CSmartPointer<CInputManager> GetSingletonPointer();
	
	// 初期化関数
	HRESULT InitInput( HWND WindowHandle);

	// 更新関数
	HRESULT UpdateInput();

	// 終了処理
	HRESULT UninitInput();
	
	bool GetKeyboardHold( int KeyState );		//押されているか
	bool GetKeyboardTrigger( int KeyState );		//押された瞬間か
	bool GetKeyboardRelease( int KeyState );		//放された瞬間か

	bool GetMouseHold( int MouseState );		//押されているか
	bool GetMouseTrigger( int MouseState );		//押された瞬間か
	bool GetMouseRelease( int MouseState );		//放された瞬間か
	D3DXVECTOR3 GetMouseMove( void );			//移動量
	LONG GetMouseMoveX( void );				//移動量X
	LONG GetMouseMoveY( void );				//移動量Y
	LONG GetMouseMoveZ( void );				//移動量Z
	
private:
	//コンストラクタ
	CInputManager();

	//キーボード更新関数
	HRESULT UpdataKeyboard();
	//マウス更新関数
	HRESULT UpdataMouse();
};

inline CSmartPointer<CInputManager> GetInputManager(){
	return CInputManager::GetSingletonPointer();
}

#endif
