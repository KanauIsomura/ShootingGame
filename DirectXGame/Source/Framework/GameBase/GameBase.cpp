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
#include"GameBase.h"

#include"../API/Window.h"
#include"../System/Device.h"
#include"../System/FPSManager.h"

#include"../System/InputManager.h"

#include"../System/DebugPrint.h"

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�R���X�g���N�^
CGameBase::CGameBase(){
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�V�X�e��������
bool CGameBase::InitSystem(){
	//--	HINSTANCE�擾
	HINSTANCE hInstance = GetModuleHandle(nullptr);

	//--	�E�C���h�E�쐬
	GetWindowClass()->MakeWindow( _T("Game_Base"), hInstance, 1024, 576);

	//--	�E�C���h�E�̕\��
	GetWindowClass()->Show();

	//--	DirectX9����������
		//--	DirectX9�̏�����
	if( GetDeviceClass()->InitDirect3D(true) ){
		return false;
	}

	//FPS�}�l�[�W���[�̐���
	m_spFPSManager.SetPointer( new FPSManager );

	//�C���v�b�g�}�l�[�W���̏�����
	GetInputManager()->InitInput(GetWindowHandle());

	DebugPrint::SetFont();

	return true;
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//�N��
int CGameBase::Run(){
		//--	�V�X�e��������
	if( ! InitSystem() ){
		return 0;			//�G���[
	}

	//--	�Q�[��������
	if( ! InitGame() ){
		UnInitAll();
		return 0;			//�G���[
	}

	//--	���C�����[�v
	while( GetWindowClass()->HandleMessage()){
		if( ! m_spFPSManager->CheckExec()){
			continue;
		}

		//--	�X�V
		//�C���v�b�g�}�l�[�W���̏�����
		GetInputManager()->UpdateInput();
		Update();

		//--	�`��
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
//DirectX9device�擾
LPDIRECT3DDEVICE9	CGameBase::GetDirectXDevice(){
	return GetDeviceClass()->GetDirect3DDevice();
}

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
//WindowHandle�擾
HWND	CGameBase::GetWindowHandle(){
	return GetWindowClass()->GetWindowHandle();
}
