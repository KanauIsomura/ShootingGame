//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.cpp]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-00
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

//=== �C���N���[�h ===
#include"InputManager.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CInputManager::CInputManager()
	:m_pDirectInput(nullptr)
	,m_pDirectInputKeyboard(nullptr)
	,m_pDirectInputMouse(nullptr){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �f�X�g���N�^
CInputManager::~CInputManager(){
	UninitInput();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �N���G�C�g�֐�
CSmartPointer<CInputManager> CInputManager::GetSingletonPointer(){
	static CSmartPointer<CInputManager> pDInput(new CInputManager);

	return pDInput;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �������֐�
HRESULT CInputManager::InitInput( HWND WindowHandle){
	if( FAILED(DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL )) ){
		return E_FAIL;
	}

	//===== �L�[�{�[�h�̏�����
	//�L�[�{�[�h�f�o�C�X�̍쐬
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pDirectInputKeyboard, NULL)))
		return E_FAIL;

	//�f�[�^�t�H�[�}�b�g���L�[�{�[�h�p�ɃZ�b�g
	m_pDirectInputKeyboard->SetDataFormat(&c_dfDIKeyboard);

	//�������x���̐ݒ�
	m_pDirectInputKeyboard->SetCooperativeLevel(WindowHandle, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// �f�[�^�擾�J�n
	m_pDirectInputKeyboard->Acquire();

	//===== �}�E�X�̏�����
	//�}�E�X�f�o�C�X�̐���
	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pDirectInputMouse, NULL)))
		return E_FAIL;

	//�f�[�^�t�H�[�}�b�g���}�E�X�p�ɃZ�b�g
	m_pDirectInputMouse->SetDataFormat(&c_dfDIMouse2);

	//�������x���̐ݒ�
	m_pDirectInputMouse->SetCooperativeLevel(WindowHandle, DISCL_EXCLUSIVE | DISCL_BACKGROUND);
	m_pDirectInputMouse->Acquire();

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �X�V�֐�
HRESULT CInputManager::UpdateInput(){
	if(FAILED(UpdataKeyboard()))
		return E_FAIL;

	if(FAILED(UpdataMouse()))
		return E_FAIL;

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
// �I������
HRESULT CInputManager::UninitInput(){
	m_pDirectInput->Release();
	m_pDirectInputKeyboard->Release();
	m_pDirectInputMouse->Release();
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�L�[�{�[�h�X�V�֐�
HRESULT CInputManager::UpdataKeyboard(){
	if( !m_pDirectInputKeyboard)
		return E_FAIL;
	
	// �K�����X�V
	m_pDirectInputKeyboard->Acquire();
	memcpy( m_aKeyboardLastData, m_aKeyboardData, NUM_KEY_MAX);
	m_pDirectInputKeyboard->GetDeviceState( NUM_KEY_MAX, m_aKeyboardData);

	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�}�E�X�X�V�֐�
HRESULT CInputManager::UpdataMouse(){
	if( !m_pDirectInputMouse)
		return E_FAIL;

	// �K�����X�V
	m_pDirectInputMouse->Acquire();
	m_MouseLastData = m_MouseData;
	m_pDirectInputMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &m_MouseData );
	
	return S_OK;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������Ă��邩
bool CInputManager::GetKeyboardHold( int KeyState){
	return (m_aKeyboardData[KeyState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����ꂽ�u�Ԃ�
bool CInputManager::GetKeyboardTrigger( int KeyState){
	return (m_aKeyboardData[KeyState] & 0x80) && !(m_aKeyboardLastData[KeyState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����ꂽ�u�Ԃ�
bool CInputManager::GetKeyboardRelease( int KeyState){
	return !(m_aKeyboardData[KeyState] & 0x80) && (m_aKeyboardLastData[KeyState] & 0x80) ? true : false;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//������Ă��邩
bool CInputManager::GetMouseHold( int MouseState){
	return (m_MouseData.rgbButtons[MouseState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����ꂽ�u�Ԃ�
bool CInputManager::GetMouseTrigger( int MouseState){
	return (m_MouseData.rgbButtons[MouseState] & 0x80) && !(m_MouseLastData.rgbButtons[MouseState] & 0x80) ? true : false;
}
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�����ꂽ�u�Ԃ�
bool CInputManager::GetMouseRelease( int MouseState){
	return !(m_MouseData.rgbButtons[MouseState] & 0x80) && (m_MouseLastData.rgbButtons[MouseState] & 0x80) ? true : false;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�ړ���
D3DXVECTOR3 CInputManager::GetMouseMove(){
	return D3DXVECTOR3( (FLOAT)GetMouseMoveX(), (FLOAT)GetMouseMoveY(), (FLOAT)GetMouseMoveZ() );
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�ړ���X
LONG CInputManager::GetMouseMoveX(){
	if( m_MouseData.lX < -1 || m_MouseData.lX > 1 ){
		return m_MouseData.lX;
	}
	return NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�ړ���Y
LONG CInputManager::GetMouseMoveY(){
	if( m_MouseData.lY < -1 || m_MouseData.lY > 1 ){
		return m_MouseData.lY;
	}
	return NULL;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�ړ���Z
LONG CInputManager::GetMouseMoveZ(){
	if( m_MouseData.lZ < -1 || m_MouseData.lZ > 1 ){
		return m_MouseData.lZ;
	}
	return NULL;
}
