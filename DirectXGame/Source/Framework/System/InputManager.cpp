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
#include"InputManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//コンストラクタ
CInputManager::CInputManager()
	:m_pDirectInput(nullptr)
	,m_pDirectInputKeyboard(nullptr)
	,m_pDirectInputMouse(nullptr){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// デストラクタ
CInputManager::~CInputManager(){
	UninitInput();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// クリエイト関数
CSmartPointer<CInputManager> CInputManager::GetSingletonPointer(){
	static CSmartPointer<CInputManager> pDInput(new CInputManager);

	return pDInput;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 初期化関数
HRESULT CInputManager::InitInput( HWND WindowHandle){
	if( FAILED(DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL )) ){
		return E_FAIL;
	}

	//===== キーボードの初期化
	//キーボードデバイスの作成
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputKeyboard, NULL)))
		return E_FAIL;

	//データフォーマットをキーボード用にセット
	m_pDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);

	//協調レベルの設定
	m_pDirectInputKeyboard->SetCooperativeLevel(WindowHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// データ取得開始
	m_pDirectInputKeyboard->Acquire();

	//===== マウスの初期化
	//マウスデバイスの生成
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputMouse, NULL)))
		return E_FAIL;

	//データフォーマットをマウス用にセット
	m_pDirectInputMouse->SetDataFormat(&c_dfDIMouse2);

	//協調レベルの設定
	m_pDirectInputMouse->SetCooperativeLevel(WindowHandle, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
	m_pDirectInputMouse->Acquire();

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 更新関数
HRESULT CInputManager::UpdateInput(){
	if(FAILED(UpdataKeyboard()))
		return E_FAIL;

	if(FAILED(UpdataMouse()))
		return E_FAIL;

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// 終了処理
HRESULT CInputManager::UninitInput(){
	m_pDirectInput->Release();
	m_pDirectInputKeyboard->Release();
	m_pDirectInputMouse->Release();
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//キーボード更新関数
HRESULT CInputManager::UpdataKeyboard(){
	if( !m_pDirectInputKeyboard)
		return E_FAIL;
	
	// 習得＆更新
	m_pDirectInputKeyboard->Acquire();
	memcpy( m_aKeyboardLastData, m_aKeyboardData, NUM_KEY_MAX);
	m_pDirectInputKeyboard->GetDeviceState( NUM_KEY_MAX, m_aKeyboardData);

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//マウス更新関数
HRESULT CInputManager::UpdataMouse(){
	if( !m_pDirectInputMouse)
		return E_FAIL;

	// 習得＆更新
	m_pDirectInputMouse->Acquire();
	m_MouseLastData = m_MouseData;
	m_pDirectInputMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseData );
	
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//押されているか
bool CInputManager::GetKeyboardHold( int KeyState){
	return (m_aKeyboardData[KeyState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//押された瞬間か
bool CInputManager::GetKeyboardTrigger( int KeyState){
	return (m_aKeyboardData[KeyState] & 0x80) && !(m_aKeyboardLastData[KeyState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//放された瞬間か
bool CInputManager::GetKeyboardRelease( int KeyState){
	return !(m_aKeyboardData[KeyState] & 0x80) && (m_aKeyboardLastData[KeyState] & 0x80) ? true : false;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//押されているか
bool CInputManager::GetMouseHold( int MouseState){
	return (m_MouseData.rgbButtons[MouseState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//押された瞬間か
bool CInputManager::GetMouseTrigger( int MouseState){
	return (m_MouseData.rgbButtons[MouseState] & 0x80) && !(m_MouseLastData.rgbButtons[MouseState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//放された瞬間か
bool CInputManager::GetMouseRelease( int MouseState){
	return !(m_MouseData.rgbButtons[MouseState] & 0x80) && (m_MouseLastData.rgbButtons[MouseState] & 0x80) ? true : false;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//移動量
D3DXVECTOR3 CInputManager::GetMouseMove(){
	return D3DXVECTOR3( (FLOAT)GetMouseMoveX(), (FLOAT)GetMouseMoveY(), (FLOAT)GetMouseMoveZ() );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//移動量X
LONG CInputManager::GetMouseMoveX(){
	if( m_MouseData.lX < -1 || m_MouseData.lX > 1 ){
		return m_MouseData.lX;
	}
	return NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//移動量Y
LONG CInputManager::GetMouseMoveY(){
	if( m_MouseData.lY < -1 || m_MouseData.lY > 1 ){
		return m_MouseData.lY;
	}
	return NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//移動量Z
LONG CInputManager::GetMouseMoveZ(){
	if( m_MouseData.lZ < -1 || m_MouseData.lZ > 1 ){
		return m_MouseData.lZ;
	}
	return NULL;
}
