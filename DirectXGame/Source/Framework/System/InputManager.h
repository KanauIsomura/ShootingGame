//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
//[Template.h]
//�T�v	:
//		>
//------------------------------------------------------------------------------------------------
//�쐬��:�鑺 �t�F
//
//�쐬��:2015-00-0
//�X�V��:
//
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
#if (_MSC_VER > 1000)
#pragma once
#endif	//_MSC_VER > 1000

#ifndef ______INPUT_MANAGER_CLASS_H_INCLUDE_____
#define ______INPUT_MANAGER_CLASS_H_INCLUDE_____

//=== �C���N���[�h ===
#pragma	comment(lib, "dinput8.lib")
#pragma	comment(lib, "dxguid.lib")
#define	DIRECTINPUT_VERSION	( 0x0800 )	// �x���p
#include<d3d9.h>
#include<d3dx9.h>
#include<dinput.h>

#include"CSmartPointer.h"

//--- �萔��` ---
const int NUM_KEY_MAX	= 256;
const WORD MOUSE_LEFT_CLICK = 0;
const WORD MOUSE_RIGHT_CLICK = 1;
const WORD MOUSE_WHEEL_CLICK = 2;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//
class CInputManager{
private:
	LPDIRECTINPUT8		m_pDirectInput;					// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8	m_pDirectInputKeyboard;			// �L�[�{�[�h�f�o�C�X
	LPDIRECTINPUTDEVICE8	m_pDirectInputMouse;				// �}�E�X�f�o�C�X

	BYTE					m_aKeyboardData[NUM_KEY_MAX];		// �L�[�{�[�h�̏��(����)
	BYTE					m_aKeyboardLastData[NUM_KEY_MAX];	// �L�[�{�[�h�̏��(�P�t���[���O)
	DIMOUSESTATE2			m_MouseData;						// �}�E�X�̏��(����)
	DIMOUSESTATE2			m_MouseLastData;					// �}�E�X�̏��(����)

public:
	// �f�X�g���N�^
	~CInputManager();

	// �N���G�C�g�֐�
	static CSmartPointer<CInputManager> GetSingletonPointer();
	
	// �������֐�
	HRESULT InitInput( HWND WindowHandle);

	// �X�V�֐�
	HRESULT UpdateInput();

	// �I������
	HRESULT UninitInput();
	
	bool GetKeyboardHold( int KeyState );		//������Ă��邩
	bool GetKeyboardTrigger( int KeyState );		//�����ꂽ�u�Ԃ�
	bool GetKeyboardRelease( int KeyState );		//�����ꂽ�u�Ԃ�

	bool GetMouseHold( int MouseState );		//������Ă��邩
	bool GetMouseTrigger( int MouseState );		//�����ꂽ�u�Ԃ�
	bool GetMouseRelease( int MouseState );		//�����ꂽ�u�Ԃ�
	D3DXVECTOR3 GetMouseMove( void );			//�ړ���
	LONG GetMouseMoveX( void );				//�ړ���X
	LONG GetMouseMoveY( void );				//�ړ���Y
	LONG GetMouseMoveZ( void );				//�ړ���Z
	
private:
	//�R���X�g���N�^
	CInputManager();

	//�L�[�{�[�h�X�V�֐�
	HRESULT UpdataKeyboard();
	//�}�E�X�X�V�֐�
	HRESULT UpdataMouse();
};

inline CSmartPointer<CInputManager> GetInputManager(){
	return CInputManager::GetSingletonPointer();
}

#endif
